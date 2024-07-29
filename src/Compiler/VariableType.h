#ifndef VARIABLETYPE_H
#define VARIABLETYPE_H

#include <stdint.h>

namespace Core::Compiler {
    enum class VariableType : int8_t {
        Integer,
        String,
        Bool
    };
}

#endif
