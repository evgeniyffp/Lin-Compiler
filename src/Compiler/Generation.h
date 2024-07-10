//
// Created by Евгений on 20.06.2024.
//

#ifndef GENERATION_H
#define GENERATION_H

#include <string>
#include <sstream>
#include <unordered_map>

#include "Parser.h"

namespace Core::Compiler {
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

        enum class VarType {
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
            std::vector<VarType> Args;
            std::string AsmName;
        };

        std::unordered_map<std::string, Var> _Vars;
        std::unordered_map<std::string, Function> _Functions;

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
