#ifndef VARIABLE_H
#define VARIABLE_H

#include <stdint.h>
#include <stdlib.h>

namespace Core::Compiler {
    enum class VariableType : int8_t {
        Integer,
        String,
        Bool
    };

    // Now all types of var must be 8 bytes!!!
    struct Variable {
        size_t StackLock;
        VariableType Type;
    };
}

namespace std {
    template <>
    struct hash<Core::Compiler::VariableType> {
        size_t operator()(const Core::Compiler::VariableType& Type) const noexcept {
            return static_cast<size_t>(Type);
        }
    };
}

#endif