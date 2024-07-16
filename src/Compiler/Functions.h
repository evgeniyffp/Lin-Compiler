#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <assert.h>

#include "VectorHasher.h"

namespace Core::Compiler {
    struct Function {
        std::optional<VariableType> ReturnValue;
        std::string AsmName;
    };

    enum class FunctionArgsParametr : uint8_t {
        UnlimitedArguments
    };

    struct FunctionArgs {
        std::vector<VariableType> Args;
        std::optional<FunctionArgsParametr> Parametr;

        bool operator==(const Core::Compiler::FunctionArgs& other) const {
            if (!this->Parametr.has_value() && !other.Parametr.has_value()) {
                if (this->Args.size() != other.Args.size()) return false;
                for (size_t i = 0; i < this->Args.size(); ++i) {
                    if (this->Args[i] != other.Args[i])
                        return false;
                }
                return true;
            }
            else {
                for (size_t i = 0; i < this->Args.size(); ++i) {
                    if (i >= other.Args.size() - 1) {
                        return true;
                    }

                    if (this->Parametr.value() == FunctionArgsParametr::UnlimitedArguments) {
                        return true;
                    }
                    else if (other.Parametr.value() == FunctionArgsParametr::UnlimitedArguments) {
                        return true;
                    }
                    if (this->Args[i] != other.Args[i])
                        return false;
                }

                assert(false);
            }
        }
    };
}

namespace std {
    template <>
    struct hash<Core::Compiler::FunctionArgs> {
        size_t operator()(const Core::Compiler::FunctionArgs& FunctionArgs) const noexcept {
            return ::Core::VectorHasher<Core::Compiler::VariableType>{}(FunctionArgs.Args);
        }
    };
}

namespace Core::Compiler {
    // Key is function signature => funcname(arg1, arg2...). For example `printf(String)` and others.
    struct FunctionOverload {
        std::unordered_map<FunctionArgs, Function, std::hash<FunctionArgs>> Overloads;
    };

}

#endif