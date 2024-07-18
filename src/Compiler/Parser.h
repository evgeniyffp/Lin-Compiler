#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <variant>

#include "Tokenization.h"
#include "../Allocator/ArenaAllocator.h"

namespace Core::Compiler {
    namespace Node {
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
            Node::Expression* Expression;
        };

        struct BinaryExpressionAdd {
            Node::Expression* LeftHandSide;
            Node::Expression* RightHandSide;
        };

        struct BinaryExpressionSubtraction {
            Node::Expression* LeftHandSide;
            Node::Expression* RightHandSide;
        };

        struct BinaryExpressionMultiplication {
            Node::Expression* LeftHandSide;
            Node::Expression* RightHandSide;
        };

        struct BinaryExpressionDivision {
            Node::Expression* LeftHandSide;
            Node::Expression* RightHandSide;
        };

        struct BinaryExpressionEqualTo {
            Node::Expression* LeftHandSide;
            Node::Expression* RightHandSide;
        };

        struct BinaryExpressionNotEqualTo {
            Node::Expression* LeftHandSide;
            Node::Expression* RightHandSide;
        };

        struct BinaryExpression {
            std::variant<
                Node::BinaryExpressionAdd*,
                Node::BinaryExpressionSubtraction*,
                Node::BinaryExpressionMultiplication*,
                Node::BinaryExpressionDivision*,
                Node::BinaryExpressionNotEqualTo*,
                Node::BinaryExpressionEqualTo*
            >var;
        };

        struct Term {
            std::variant<
                Node::TermIntegerLiteral*,
                Node::TermStringLiteral*,
                Node::TermBoolLiteral*,
                Node::TermIdentifier*,
                Node::TermParent*
            >var;
        };

        struct Expression {
            std::variant<Node::Term*, Node::BinaryExpression*> var;
        };

        // STATEMENTS ---------------------------------------------------------------------
        struct StatementFunctionCall {
            std::string FunctionName;
            std::vector<Node::Expression*> Arguments;

        };

        struct StatementExit {
            Node::Expression* Expression;
        };

        struct StatementLet {
            Token Identifier;
            Node::Expression* Expression;
        };

        struct Statement;

        struct StatementScope {
            std::vector<Node::Statement*> Statements;
        };

        struct StatementIf {
            Node::Expression* Condition;
            Node::Statement* IfStatement;
            std::optional<Node::Statement*> ElseStatement;
        };

        struct Statement {
            std::variant<
                Node::StatementFunctionCall*,
                Node::StatementLet*,
                Node::StatementScope*,
                Node::StatementIf*
            >var;
        };

        // PROGRAMM ---------------------------------------------------------------------
        struct Programm {
            std::vector<Node::Statement*> Statements;
        };
    }

    class Parser {
    private:
        size_t _CurrentIndex;
        const std::vector<Core::Compiler::Token> _Tokens;

        ArenaAllocator _Allocator;

        [[nodiscard]] auto _Peek(const int64_t &offset = 0) const -> std::optional<Token>;

        auto _Consume() -> Token;
        auto _TryConsume(const TokenType& Type, const std::string& ErrorMessage) -> Token;

    public:
        explicit Parser(std::vector<Core::Compiler::Token> tokens);

        auto ParseBinaryExpression() -> std::optional<Node::BinaryExpression*>;
        auto ParseTerm() ->std::optional<Node::Term*>;
        auto ParseExpression(uint16_t MinPriority = MaxBinaryPriority) -> std::optional<Node::Expression*>;

        auto ParseStatementScope() -> std::optional<Node::StatementScope*>;

        auto ParseStatement() -> std::optional<Node::Statement*>;

        auto ParseProgramm() -> std::optional<Node::Programm>;
    };
}

#endif //PARSER_H
