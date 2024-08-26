#ifndef COMPILER_STATEMENT_H
#define COMPILER_STATEMENT_H

#include <variant>
#include <optional>
#include <vector>
#include <string>

#include "Tokenization.h"

namespace Core::Compiler::Node {
    // Expression ---------------------------------------------------------------------
    struct TermIntegerLiteral {
        Token IntegerLiteral;
    };

    struct TermStringLiteral {
        Token StringLiteral;
    };

    struct TermBoolLiteral {
        Token BoolLiteral;
    };

    struct TermIdentifier {
        Token Identifier;
    };

    struct Expression;

    struct TermParent {
        Node::Expression *Expression;
    };

    struct BinaryExpressionAdd {
        Node::Expression *LeftHandSide;
        Node::Expression *RightHandSide;
    };

    struct BinaryExpressionSubtraction {
        Node::Expression *LeftHandSide;
        Node::Expression *RightHandSide;
    };

    struct BinaryExpressionMultiplication {
        Node::Expression *LeftHandSide;
        Node::Expression *RightHandSide;
    };

    struct BinaryExpressionDivision {
        Node::Expression *LeftHandSide;
        Node::Expression *RightHandSide;
    };

    struct BinaryExpressionEqualTo {
        Node::Expression *LeftHandSide;
        Node::Expression *RightHandSide;
    };

    struct BinaryExpressionNotEqualTo {
        Node::Expression *LeftHandSide;
        Node::Expression *RightHandSide;
    };

    struct BinaryExpression {
        std::variant<
                Node::BinaryExpressionAdd *,
                Node::BinaryExpressionSubtraction *,
                Node::BinaryExpressionMultiplication *,
                Node::BinaryExpressionDivision *,
                Node::BinaryExpressionNotEqualTo *,
                Node::BinaryExpressionEqualTo *
        > var;
    };

    struct Term {
        std::variant<
                Node::TermIntegerLiteral *,
                Node::TermStringLiteral *,
                Node::TermBoolLiteral *,
                Node::TermIdentifier *,
                Node::TermParent *
        > var;
    };

    struct Expression {
        std::variant<Node::Term *, Node::BinaryExpression *> var;
    };

    // STATEMENTS ---------------------------------------------------------------------
    struct StatementFunctionCall {
        std::string FunctionName;
        std::vector<Node::Expression *> Arguments;

    };

    struct StatementExit {
        Node::Expression *Expression;
    };

    struct StatementLet {
        Token Identifier;
        Node::Expression *Expression;
        std::optional<VariableType> VarType;
    };

    struct Statement;

    struct StatementScope {
        std::vector<Node::Statement *> Statements;
    };

    struct StatementIf {
        Node::Expression *Condition;
        Node::Statement *IfStatement;
        std::optional<Node::Statement *> ElseStatement;
    };

    struct StatementAssignment {
        std::string LetName;
        Node::Expression *Expression;
    };

    struct StatementWhile {
        Node::Expression *Condition;
        Node::Statement *Statement;
    };

    struct Statement {
        std::variant<
                Node::StatementFunctionCall *,
                Node::StatementLet *,
                Node::StatementScope *,
                Node::StatementIf *,
                Node::StatementAssignment *,
                Node::StatementWhile *
        > var;
    };

    // PROGRAMM ---------------------------------------------------------------------
    struct Programm {
        std::vector<Node::Statement *> Statements;
    };
}

#endif
