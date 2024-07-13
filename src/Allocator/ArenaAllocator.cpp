//
// Created by Евгений on 24.06.2024.
//

#include "ArenaAllocator.h"

namespace Core::Compiler {
    ArenaAllocator::ArenaAllocator(size_t bytes) : _size(bytes) {
        this->_buffer = new byte[bytes];
        this->_offset = this->_buffer;
    }

    ArenaAllocator::~ArenaAllocator() {
        delete[] this->_buffer;
    }
}
