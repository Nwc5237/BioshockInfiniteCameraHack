// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "processthreadsapi.h"
#include "stdio.h"
#include <iostream>
#include <Windows.h>
#include "mem.h"
#include <vector>
#include "geom.h"

void dll_main(HMODULE hModule) {
    AllocConsole();
    FILE* f = new FILE;

    freopen_s(&f, "CONOUT$", "w", stdout);
    printf("I really hope this works\n");

    bool FlyHack = false;
    uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"BioShockInfinite.exe"); //maybe try the current process that this is in?

    std::vector<unsigned int> offsets = {0x18, 0x44};
    std::vector<unsigned int> offsets_pitch = { 0x8C, 0x50 };
    std::vector<unsigned int> offsets_yaw = { 0x8C, 0x54 };

    vec3 *vec;
    vec3 next_pos = {63900.0f, -200.0f, -500.0f}; //make this whatever the current position is
    MSG msg = { 0 };
    bool fly_enabled = false;
   // float height_speed = .005f; //for z vals
    float speed = .08f * 4.f;
    unsigned position_base_offset = 0x00B9F058;
    unsigned cam_rotation_base_offset = 0x00F679A0;

    
    float pitch;
    float yaw;

    short pitch_unreal;
    short yaw_unreal;

    vec3 forward = {1.f, 0.f, 0.f};
    float toRadians = 3.14159f / 180.f;

    //puts nop over the instruction that writes the z value
    nop((LPVOID)(moduleBase + 0x3C0940), 3);

    //overwrites the instructions that write x and y coords
    nop((LPVOID)(moduleBase + 0x3C0938), 5);

    /*
    Add a delta time thing
    */
    while (true) {

        //break all of these memory reads out into their pointers so we don't do all of these function calls
        vec = (vec3*)(mem_read((uintptr_t)(moduleBase + 0x00B9F058), offsets));
        
        pitch_unreal    = *(short *)mem_read((uintptr_t)(moduleBase + 0x00F679A0), offsets_pitch);
        yaw_unreal      = *(short *)mem_read((uintptr_t)(moduleBase + 0x00F679A0), offsets_yaw);

        pitch   = ((float)pitch_unreal / (float)65536.f) * 360.f;
        yaw     = ((float)yaw_unreal / (float)65536.f) * 360.f;

        pitch *= toRadians;
        yaw *= toRadians;

        

        printf("pitch and yaw: (%f, %f)\n", pitch, yaw);

        if (GetKeyState('W') & 0x8000)
        {
            fly_enabled = false;

            forward.x = cos(pitch);
            forward.y = 0.f;
            forward.z = sin(pitch);

            forward.x *= cos(yaw);
            forward.y = sin(yaw);
            //forward.z = 0; //don't overwrite this value
            normalize(&forward);

            scale_vec(speed, &forward);
            next_pos = *vec;

            next_pos.x += speed * forward.x;
            next_pos.y += speed * forward.y;
            next_pos.z += speed * forward.z;
        }

        if (GetKeyState('S') & 0x8000)
        {
            fly_enabled = false;

            forward.x = cos(pitch);
            forward.y = 0.f;
            forward.z = sin(pitch);

            forward.x *= cos(yaw);
            forward.y = sin(yaw);
            //forward.z = 0; //don't overwrite this value
            normalize(&forward);

            scale_vec(speed, &forward);
            next_pos = *vec;

            next_pos.x -= speed * forward.x;
            next_pos.y -= speed * forward.y;
            next_pos.z -= speed * forward.z;
        }
        /*Next two fuck things up*/
        /*if (GetKeyState('D') & 0x8000)
        {
            fly_enabled = false;

            //these need to move to the forward key W check but right now it's crashing (probably because of missing nops, so have those run when the dll injects, not in the loop)

            forward.x = cos(pitch * toRadians);
            forward.y = 0.f;
            forward.z = sin(pitch * toRadians);

            forward.x *= cos(yaw * toRadians);
            forward.y = sin(yaw * toRadians);
            //forward.z = 0; //don't overwrite this value
            normalize(&forward);

            vec3 right = cross(forward, vec3{ 0.f, 0.f, 1.f}); //probavly need a safeguard for when forward is straight up because then we have zero are cross

            next_pos = *vec;

            //note to self: make a vector addition function
            next_pos.x += speed * 4 * right.x;
            next_pos.y += speed * 4 * right.y;
            next_pos.z += speed * 4 * right.z;
        }

        if (GetKeyState('A') & 0x8000)
        {
            fly_enabled = false;

            //these need to move to the forward key W check but right now it's crashing (probably because of missing nops, so have those run when the dll injects, not in the loop)

            forward.x = cos(pitch * toRadians);
            forward.y = 0.f;
            forward.z = sin(pitch * toRadians);

            forward.x *= cos(yaw * toRadians);
            forward.y = sin(yaw * toRadians);
            //forward.z = 0; //don't overwrite this value
            normalize(&forward);

            vec3 right = cross(forward, vec3{ 0.f, 0.f, 1.f }); //probavly need a safeguard for when forward is straight up because then we have zero are cross

            next_pos = *vec;

            //note to self: make a vector addition function
            next_pos.x -= speed * 4 * right.x;
            next_pos.y -= speed * 4 * right.y;
            next_pos.z -= speed * 4 * right.z;
        }*/
       
        //overwrites position
        overwrite_position(vec, next_pos, sizeof(float) * 3);
    }
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)dll_main, hModule, 0, nullptr);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }

    return TRUE;
}

