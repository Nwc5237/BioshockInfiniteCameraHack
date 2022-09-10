#pragma once
#include "pch.h"
#include <vector>
#include "geom.h"

uintptr_t mem_read(uintptr_t base_ptr, std::vector<unsigned int> offsets);

void overwrite_position(LPVOID ptr, vec3 next_pos, unsigned int size);
void nop(LPVOID ptr, unsigned int size);