//
// Created by Евгений on 20.06.2024.
//

#ifndef GENERATION_H
#define GENERATION_H

#include <string>
#include <sstream>
#include <unordered_map>

#include "Parser.h"
#include "Variable.h"
#include "Functions.h"

namespace Core::Compiler {
    class Generator {
    private:
        const Node::Programm _Programm;

        std::stringstream _DataSegment;
        std::stringstream _TextSegment;
        std::stringstream _Output;

        VirtualStack _VStack;

        std::unordered_map<std::string, FunctionOverload> _Functions;
        std::vector<std::string> _PositionFnArgsInFnCall;

    public:
        explicit Generator(Node::Programm programm);

        auto DefineExpressionType(const Node::Expression* Expression) -> std::optional<VariableType>;
        auto DefineBinaryExpressionType(const Node::BinaryExpression* BinaryExpression) -> std::optional<VariableType>;
        auto DefineTermType(const Node::Term* Term) -> std::optional<VariableType>;

        auto GenerateTerm(const Node::Term* Term, const std::string& where) -> void;

        auto GenerateIntegerBinaryExpression(const Node::BinaryExpression* BinaryExpression, const std::string& where) -> void;
        auto GenerateStringBinaryExpression(const Node::BinaryExpression* BinaryExpression, const std::string& where) -> void;
        auto GenerateBoolBinaryExpression(const Node::BinaryExpression* BinaryExpression, const std::string& where) -> void;

        auto GenerateBinaryExpression(const Node::BinaryExpression* BinaryExpression, const std::string& where) -> void;

        auto GenetateExpression(const Node::Expression* Expression, const std::string& where) -> void;

        auto GenetateStatementScope(const std::vector<Node::Statement *> &Statements) -> void;

        auto GenetateStatement(const Node::Statement* Statement) -> void;

        [[nodiscard]] auto GenetateProgramm() -> std::string;
    };
}

#endif //GENERATION_H
