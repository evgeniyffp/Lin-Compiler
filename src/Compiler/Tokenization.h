//
// Created by Евгений on 19.06.2024.
//

#ifndef TOCKENIZATION_H
#define TOCKENIZATION_H

#include <string>
#include <optional>
#include <vector>

namespace Core::Compiler {
    enum class TokenType {
        Let,

        Equal,

        Plus,
        Subtraction,
        Multiplication,
        Division,

        EqualTo,
        NotEqualTo,

        IntegerLiteral,
        StringLiteral,
        BoolLiteral,

        Comma,
        CommandEnd,

        Word,

        LeftParent,
        RightParent
    };

    inline bool IsBinaryOperator(const TokenType& Type) {
        switch (Type) {
            case TokenType::Plus:
            case TokenType::Subtraction:
            case TokenType::Multiplication:
            case TokenType::Division:
            case TokenType::EqualTo:
            case TokenType::NotEqualTo:
                return true;
            default:
                return false;
        }
    }

    constexpr int MaxBinaryPriority = 3;

    inline std::optional<uint16_t> BinaryPriority(const TokenType& Type) {
        switch (Type) {
            case TokenType::EqualTo:
            case TokenType::NotEqualTo:
                return 3;
            case TokenType::Plus:
            case TokenType::Subtraction:
                return 2;
            case TokenType::Multiplication:
            case TokenType::Division:
                return 1;
            default:
                return {};
        }
    }

    inline bool isEscape(const char symbol) {
        switch (symbol) {
            case '\n':
            case '\t':
                return true;
            default:
                return false;
        }
    }

    struct Token {
        TokenType type;
        std::optional <std::string> value;
    };

    class Tokenizer {
    private:
        const std::string _src;
        size_t _current_index;

        [[nodiscard]] std::optional<char> _peek(const int8_t& offset = 0) const;

        char _consume();

    public:
        explicit Tokenizer(std::string src);

        auto Tokenize() -> std::vector<Token>;

    };
}

#endif //TOCKENIZATION_H
