#include "mem.h"
#include "pch.h"
#include <vector>
#include "geom.h"


//returns an address
uintptr_t mem_read(uintptr_t base_ptr, std::vector<unsigned int> offsets) {
	uintptr_t addr = base_ptr;
	for (int i = 0; i < offsets.size(); i++) {
		addr = *(uintptr_t*)addr;
		addr += offsets[i];
	}
	return addr; //*(float *)addr;
}

void overwrite_position(LPVOID ptr, vec3 next_pos, unsigned int size) {
	DWORD oldProtect;
	VirtualProtect(ptr, size, PAGE_EXECUTE_READWRITE, &oldProtect);

	memcpy(ptr, &next_pos, sizeof(float)*3); //can last arg just be size of vec3 struct?
}

void nop(LPVOID ptr, unsigned int size) {
	DWORD oldProtect;
	VirtualProtect(ptr, size, PAGE_EXECUTE_READWRITE, &oldProtect);

	//four nops
	unsigned char nop_byte = 0x90;

	memset(ptr, nop_byte, size);
}