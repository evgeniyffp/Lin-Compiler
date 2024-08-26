#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <variant>

#include "Tokenization.h"
#include "VariableType.h"
#include "Statement.h"
#include "../Allocator/ArenaAllocator.h"

namespace Core::Compiler {
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
