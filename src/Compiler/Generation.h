//
// Created by Евгений on 20.06.2024.
//

#ifndef GENERATION_H
#define GENERATION_H

#include <string>
#include <sstream>
#include <unordered_map>
#include <assert.h>

#include "Parser.h"

namespace Core::Compiler {
    enum class VarType : int8_t {
        Integer,
        String,
        Bool
    };

    // Now all types of var must be 8 bytes!!!
    struct Var {
        size_t StackLock;
        VarType Type;
    };

    struct Function {
        std::optional<VarType> ReturnValue;
        std::string AsmName;
    };

    template <typename T>
    struct VectorHasher {
        size_t operator()(const std::vector<T>& V) const noexcept {
            size_t hash = V.size();
            for(auto &i : V) {
                hash ^= static_cast<size_t>(i) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            }
            return hash;
        }
    };

    enum class FunctionArgsParametr : uint8_t {
        UnlimitedArguments
    };

    struct FunctionArgs {
        std::vector<VarType> Args;
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
    struct hash<Core::Compiler::VarType> {
        size_t operator()(const Core::Compiler::VarType& Type) const noexcept {
            return static_cast<size_t>(Type);
        }
    };

    template <>
    struct hash<Core::Compiler::FunctionArgs> {
        size_t operator()(const Core::Compiler::FunctionArgs& FunctionArgs) const noexcept {
            size_t hash = FunctionArgs.Args.size();
            for(auto &i : FunctionArgs.Args) {
                hash ^= static_cast<size_t>(i) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            }
            return hash;
        }
    };
}

namespace Core::Compiler {
    // Key is function signature => funcname(arg1, arg2...). For example `printf(String)` and others.
    struct FunctionOverload {
        std::unordered_map<FunctionArgs, Function, std::hash<FunctionArgs>> Overloads;
    };

    class Generator {
    private:
        const Node::Programm _Programm;

        std::stringstream _DataSegment;
        std::stringstream _TextSegment;
        std::stringstream _Output;

        size_t _StackSize;

        void _StackPush(const std::string& value) {
            this->_Output << "\tpush " << value << "\n";
            ++this->_StackSize;
        }

        void _StackPop(const std::string& value = "") {
            this->_Output << "\tpop " << value << "\n";
            --this->_StackSize;
        }

        std::unordered_map<std::string, Var> _Vars;
        std::unordered_map<std::string, FunctionOverload> _Functions;
        std::unordered_map<size_t, std::string> _PositionFnArgsInFnCall;

    public:
        explicit Generator(Node::Programm programm);

        auto DefineExpressionType(const Node::Expression* Expression) -> std::optional<VarType>;
        auto DefineBinaryExpressionType(const Node::BinaryExpression* BinaryExpression) -> std::optional<VarType>;
        auto DefineTermType(const Node::Term* Term) -> std::optional<VarType>;

        auto GenerateTerm(const Node::Term* Term) -> void;

        auto GenerateIntegerBinaryExpression(const Node::BinaryExpression* BinaryExpression) -> void;
        auto GenerateStringBinaryExpression(const Node::BinaryExpression* BinaryExpression) -> void;
        auto GenerateBoolBinaryExpression(const Node::BinaryExpression* BinaryExpression) -> void;

        auto GenerateBinaryExpression(const Node::BinaryExpression* BinaryExpression) -> void;

        auto GenetateExpression(const Node::Expression* Expression) -> void;

        auto GenetateStatement(const Node::Statement* Statement) -> void;

        [[nodiscard]] auto GenetateProgramm() -> std::string;
    };
}

#endif //GENERATION_H
