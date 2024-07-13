//
// Created by Евгений on 24.06.2024.
//

#ifndef ARENAALLOCATOR_H
#define ARENAALLOCATOR_H

#include <cstdint>
#include <stdlib.h>

namespace Core::Compiler {
    using byte = uint8_t;

    class ArenaAllocator {
    private:
        size_t _size;
        byte* _buffer;
        byte* _offset;

    public:
        explicit ArenaAllocator(size_t bytes);

        template <typename T>
        auto allocate() -> T* {
            auto offset = this->_offset;
            this->_offset += sizeof(T);
            return reinterpret_cast<T*>(offset);
        }

        ArenaAllocator(const ArenaAllocator& other) = delete;
        ArenaAllocator operator=(const ArenaAllocator& other) = delete;

        ~ArenaAllocator();
    };
}

#endif
