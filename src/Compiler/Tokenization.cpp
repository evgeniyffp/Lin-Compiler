//
// Created by Евгений on 20.06.2024.
//

#include <iostream> // for std::cerr
#include <utility> // for std::move
#include <fstream>

#include "Tokenization.h"

namespace Core::Compiler {
    std::optional<char> Tokenizer::_peek(const int8_t& offset) const {
        if (this->_current_index + offset >= this->_src.size())
            return {};
        else
            return this->_src.at(this->_current_index + offset);
    }

    auto Tokenizer::_consume() -> char {
        return this->_src.at(this->_current_index++);
    }

    Tokenizer::Tokenizer(std::string src): _src(std::move(src)), _current_index(0) {
    }

    auto Tokenizer::Tokenize() -> std::vector<Token> {
        std::vector<Token> tokens;
        std::string buffer;
        while (this->_peek().has_value()) {
            if (std::isalpha(this->_peek().value())) {
                buffer.push_back(this->_consume());
                while (std::isalnum(this->_peek().value()) && this->_peek().has_value()) {
                    buffer.push_back(this->_consume());
                }
                if (buffer == "let") {
                    tokens.push_back({TokenType::Let});
                    buffer.clear();
                    continue;
                }
                else if (buffer == "true") {
                    tokens.push_back({TokenType::BoolLiteral, "1"});
                    buffer.clear();
                    continue;
                }
                else if (buffer == "false") {
                    tokens.push_back({TokenType::BoolLiteral, "0"});
                    buffer.clear();
                    continue;
                }
                else {
                    tokens.push_back({.type = TokenType::Word, .value = buffer});
                    buffer.clear();
                    continue;
                }
            }
            else if (std::isdigit(this->_peek().value())) {
                buffer.push_back(this->_consume());
                while (std::isdigit(this->_peek().value()) && this->_peek().has_value()) {
                    buffer.push_back(this->_consume());
                }
                tokens.push_back({TokenType::IntegerLiteral, buffer});
                buffer.clear();
                continue;
            }
            else {
                switch (this->_peek().value()) {
                    case '\"': {
                        this->_consume();
                        while (this->_peek().has_value() && this->_peek().value() != '\"') {
                            if (this->_peek().value() == '\\') {
                                this->_consume();
                                switch (this->_peek().value()) {
                                    case 'n':
                                        this->_consume();
                                        buffer.push_back('\n');
                                        break;
                                    case 't':
                                        this->_consume();
                                        buffer.push_back('\n');
                                        break;
                                    default:
                                        std::cerr << "Unknown value: `\\" << this->_peek().value() << "` !\n";
                                        exit(EXIT_FAILURE);
                                    break;
                                }
                            }
                            else {
                                buffer.push_back(this->_consume());
                            }
                        }
                        tokens.push_back({TokenType::StringLiteral, buffer});
                        buffer.clear();
                        break;
                    }
                    case '(': {
                        tokens.push_back({TokenType::LeftParent});
                        break;
                    }
                    case ')': {
                        tokens.push_back({TokenType::RightParent});
                        break;
                    }
                    case ',': {
                        tokens.push_back({TokenType::Comma});
                        break;
                    }
                    case ';': {
                        tokens.push_back({TokenType::CommandEnd});
                        break;
                    }
                    case '!': {
                        if (this->_peek(1).has_value() && this->_peek(1).value() == '=') {
                            tokens.push_back({TokenType::NotEqualTo});
                            this->_consume();
                        }
                        break;
                    }
                    case '=': {
                        if (this->_peek(1).has_value() && this->_peek(1).value() == '=') {
                            tokens.push_back({TokenType::EqualTo});
                            this->_consume();
                        }
                        else {
                            tokens.push_back({TokenType::Equal});
                        }
                        break;
                    }
                    case '+': {
                        tokens.push_back({TokenType::Plus});
                        break;
                    }
                    case '-': {
                        tokens.push_back({TokenType::Subtraction});
                        break;
                    }
                    case '*': {
                        tokens.push_back({TokenType::Multiplication});
                        break;
                    }
                    case '/': {
                        tokens.push_back({TokenType::Division});
                        break;
                    }

                    default: {
                        if (std::isspace(this->_peek().value())) {
                            this->_consume();
                            continue;
                        }
                        else {
                            std::cerr << "Unknown token: `" << this->_peek().value() << "` !\n";
                            exit(EXIT_FAILURE);
                        }
                    }
                }
                this->_consume();
            }
        }

        this->_current_index = 0;
        return tokens;
    }
}
