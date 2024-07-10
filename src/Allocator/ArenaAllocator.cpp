//
// Created by Евгений on 24.06.2024.
//

#include <stdlib.h>

#include "ArenaAllocator.h"

namespace Core::Compiler {
    ArenaAllocator::ArenaAllocator(size_t bytes) : _size(bytes) {
        this->_buffer = reinterpret_cast<byte*>(malloc(bytes));
        this->_offset = this->_buffer;
    }

    ArenaAllocator::~ArenaAllocator() {
        free(this->_buffer);
    }
}
