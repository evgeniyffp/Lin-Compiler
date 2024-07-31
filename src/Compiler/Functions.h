#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <sstream>
#include <cassert>

#include "DefineTypes.h"
#include "VectorHasher.h"
#include "Parser.h"

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
    const std::vector<std::string> PositionFnArgsInFnCall = { "rdi", "rsi", "rdx", "rcx", "r8", "r9" };

    // Key is function signature => funcname(arg1, arg2...). For example `printf(String)` and others.
    struct FunctionOverload {
        std::unordered_map<FunctionArgs, Function, std::hash<FunctionArgs>> Overloads;
    };
    
    struct Functions {
        std::unordered_map<std::string, FunctionOverload> _Functions;
        std::stringstream _Output;

        void Execute(const Node::StatementFunctionCall* StatementFunctionCall, const std::optional<std::string>& where, std::stringstream& out) {
            if (this->_Functions.find(StatementFunctionCall->FunctionName) == this->_Functions.end()) {
                std::cerr << "Function `" << StatementFunctionCall->FunctionName <<"` don\'t be found!\n";
                exit(EXIT_FAILURE);
            }

            FunctionArgs FunctionArgs;
            FunctionArgs.Parametr = {};
            FunctionArgs.Args.reserve(StatementFunctionCall->Arguments.size());

            for (size_t i = 0; i < StatementFunctionCall->Arguments.size(); ++i) {
                if (auto ArgType = Definer::DefineExpressionType(StatementFunctionCall->Arguments[i])) {
                    FunctionArgs.Args.push_back(ArgType.value());
                }
                else {
                    std::cerr << "Error in defining var\'s type! \n";
                    exit(EXIT_FAILURE);
                }
            }

            Function FnOverload;
            bool isFailed = true;

            for (const auto& FnOverloadIterator : this->_Functions[StatementFunctionCall->FunctionName].Overloads) {
                if (FnOverloadIterator.first == FunctionArgs) {
                    FnOverload = FnOverloadIterator.second;
                    isFailed = false;
                    break;
                }
            }

            if (isFailed) {
                std::cerr << "Function with this args don\'t be found!\n";
                exit(EXIT_FAILURE);
            }

            for (size_t i = 0; i < PositionFnArgsInFnCall.size() && i < StatementFunctionCall->Arguments.size(); ++i) {
                this->GenetateExpression(StatementFunctionCall->Arguments.at(i), PositionFnArgsInFnCall[i]);
            }

            this->_Output << "\tcall " << FnOverload.AsmName << "\n";
            if (FnOverload.ReturnValue.has_value() && where.has_value() && where.value() != "rax") {
                this->_Output << "\tmov " << where.value() << ", rax\n";
            }
        }
    };

}

#endif