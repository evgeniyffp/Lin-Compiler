//
// Created by Евгений on 20.06.2024.
//

#include <iostream>

#include "Parser.h"

#include <assert.h>

namespace Core::Compiler {
    auto Parser::_Peek(const int64_t& offset) const -> std::optional<Token> {
        if (this->_CurrentIndex + offset >= this->_Tokens.size())
            return {};
        else
            return this->_Tokens.at(this->_CurrentIndex + offset);
    }

    auto Parser::_Consume() -> Token {
        return this->_Tokens.at(this->_CurrentIndex++);
    }

    auto Parser::_TryConsume(const TokenType& Type, const std::string& ErrorMessage) -> Token {
        if (this->_Peek().has_value() && this->_Peek().value().type == Type) {
            return this->_Consume();
        }
        else {
            std::cerr << ErrorMessage << "\n";
            exit(EXIT_FAILURE);
        }
    }

    Parser::Parser(std::vector<Token> tokens)
    : _Tokens(std::move(tokens)), _CurrentIndex(0), _Allocator(1024 * 1024 * 8)
    {}

    auto Parser::ParseBinaryExpression() -> std::optional<Node::BinaryExpression*> {
        if (auto lhs = this->ParseExpression()) {
            auto BinaryExpression = this->_Allocator.allocate<Node::BinaryExpression>();

            this->_TryConsume(TokenType::Plus, "Unsupported binary operator: " + this->_Peek().value().value.value());

            auto BinaryExpressionAdd = this->_Allocator.allocate<Node::BinaryExpressionAdd>();
            BinaryExpressionAdd->LeftHandSide = lhs.value();

            this->_Consume();

            if (auto rhs = this->ParseExpression()) {
                BinaryExpressionAdd->RightHandSide = rhs.value();
                BinaryExpression->var = BinaryExpressionAdd;
            }
            else {
                std::cerr << "Expented expression! \n";
                exit(EXIT_FAILURE);
            }

            return BinaryExpression;
        }
        else {
            return {};
        }
    }

    auto Parser::ParseTerm() -> std::optional<Node::Term*> {
        if (!this->_Peek().has_value()) {
            return {};
        }

        auto ReturnExpression = this->_Allocator.allocate<Node::Term>();

        if (this->_Peek().value().type == TokenType::IntegerLiteral) {
            auto temp = this->_Allocator.allocate<Node::TermIntegerLiteral>();
            temp->IntegerLiteral = this->_Consume();

            ReturnExpression->var = temp;
        }
        else if (this->_Peek().value().type == TokenType::StringLiteral) {
            auto temp = this->_Allocator.allocate<Node::TermStringLiteral>();
            temp->StringLiteral = this->_Consume();

            ReturnExpression->var = temp;
        }
        else if (this->_Peek().value().type == TokenType::BoolLiteral) {
            auto temp = this->_Allocator.allocate<Node::TermBoolLiteral>();
            temp->BoolLiteral = this->_Consume();

            ReturnExpression->var = temp;
        }
        else if (this->_Peek().value().type == TokenType::Word) {
            auto temp = this->_Allocator.allocate<Node::TermIdentifier>();
            temp->Identifier = this->_Consume();

            ReturnExpression->var = temp;
        }
        else if (this->_Peek().value().type == TokenType::LeftParent) {
            auto temp = this->_Allocator.allocate<Node::TermParent>();

            this->_Consume();

            auto Expression = this->ParseExpression();
            if (!Expression.has_value()) {
                std::cerr << "Unable to parse expression\n";
                exit(EXIT_FAILURE);
            }

            this->_TryConsume(TokenType::RightParent, "Expented `)`!");

            temp->Expression = Expression.value();

            ReturnExpression->var = temp;
        }
        else {
            return {};
        }

        return ReturnExpression;
    }


    auto Parser::ParseExpression(uint16_t MinPriority) -> std::optional<Node::Expression*> {
        std::optional<Node::Term*> TermLhs = this->ParseTerm();

        if (!TermLhs.has_value()) {
            return {};
        }

        auto ExpressionrLhs = this->_Allocator.allocate<Node::Expression>();
        ExpressionrLhs->var = TermLhs.value();

        while (true) {
            std::optional<Token> CurrentToken = this->_Peek();

            std::optional<int> Priority;

            if (CurrentToken.has_value()) {
                Priority = BinaryPriority(CurrentToken->type);
                if (!Priority.has_value() || Priority > MinPriority) {
                    break;
                }
            }
            else {
                break;
            }

            auto TempIndex = this->_CurrentIndex;
            Token Operator = this->_Consume();

            /*if (this->_Peek(-2).value().type != this->_Peek().value().type) {
                std::cerr << "Operations between these types are not supported\n";
                exit(EXIT_FAILURE);
            }*/

            auto ExpressionrRhs = this->ParseExpression(Priority.value() - 1);

            if (!ExpressionrRhs.has_value()) {
                std::cerr << "Unable to parse expression!\n";
                exit(EXIT_FAILURE);
            }

            auto Expression = this->_Allocator.allocate<Node::BinaryExpression>();

            auto ExpressionLhs2 = this->_Allocator.allocate<Node::Expression>();
            ExpressionLhs2->var = ExpressionrLhs->var;

            switch (Operator.type) {
                case TokenType::Plus: {
                    auto Add = this->_Allocator.allocate<Node::BinaryExpressionAdd>();
                    ExpressionLhs2->var = ExpressionrLhs->var;
                    Add->LeftHandSide = ExpressionLhs2;
                    Add->RightHandSide = ExpressionrRhs.value();
                    Expression->var = Add;
                }
                break;

                case TokenType::Subtraction: {
                    auto Subtraction = this->_Allocator.allocate<Node::BinaryExpressionSubtraction>();
                    Subtraction->LeftHandSide = ExpressionLhs2;
                    Subtraction->RightHandSide = ExpressionrRhs.value();
                    Expression->var = Subtraction;
                }
                break;

                case TokenType::Multiplication: {
                    auto Multiplication = this->_Allocator.allocate<Node::BinaryExpressionMultiplication>();
                    Multiplication->LeftHandSide = ExpressionLhs2;
                    Multiplication->RightHandSide = ExpressionrRhs.value();
                    Expression->var = Multiplication;
                }
                break;

                case TokenType::Division: {
                    auto Division = this->_Allocator.allocate<Node::BinaryExpressionDivision>();
                    Division->LeftHandSide = ExpressionLhs2;
                    Division->RightHandSide = ExpressionrRhs.value();
                    Expression->var = Division;
                }
                break;

                case TokenType::EqualTo: {
                    auto Division = this->_Allocator.allocate<Node::BinaryExpressionEqualTo>();
                    Division->LeftHandSide = ExpressionLhs2;
                    Division->RightHandSide = ExpressionrRhs.value();
                    Expression->var = Division;
                }
                break;

                case TokenType::NotEqualTo: {
                    auto Division = this->_Allocator.allocate<Node::BinaryExpressionNotEqualTo>();
                    Division->LeftHandSide = ExpressionLhs2;
                    Division->RightHandSide = ExpressionrRhs.value();
                    Expression->var = Division;
                }
                break;


                default:
                    assert(false);

                break;
            }

            ExpressionrLhs->var = Expression;
        }
        return ExpressionrLhs;
    }

    std::optional<Node::Statement> Parser::ParseStatement() {
        if (!this->_Peek().has_value()) {
            return {};
        }

        if (this->_Peek().value().type == TokenType::Let) {
            this->_Consume();

            auto StatementLet = this->_Allocator.allocate<Node::StatementLet>();

            if (this->_Peek().has_value() && this->_Peek().value().type == TokenType::Word) {
                StatementLet->Identifier = this->_Consume();
            }
            else {
                std::cerr << "Expected identifier! \n";
                exit(EXIT_FAILURE);
            }

            this->_TryConsume(TokenType::Equal, "Expected `=`! ");

            if (!this->_Peek().has_value()){
                std::cerr << "Expected integer literal or identifier! \n";
                exit(EXIT_FAILURE);
            }

            if (auto Expression = this->ParseExpression()) {
                StatementLet->Expression = Expression.value();
            }
            else {
                std::cerr << "Invalid expression! \n";
                exit(EXIT_FAILURE);
            }

            this->_TryConsume(TokenType::CommandEnd, "Expected `;`! ");

            return Node::Statement{.var = StatementLet};
        }
        else if (this->_Peek().value().type == TokenType::Word) {
            auto StatementFunction = this->_Allocator.allocate<Node::StatementFunctionCall>();

            auto FunctionName = this->_Consume().value.value();
            StatementFunction->FunctionName = FunctionName;

            this->_TryConsume(TokenType::LeftParent, "Expected `(`!");

            if (auto Expression = this->ParseExpression()) {
                StatementFunction->Arguments.push_back(Expression.value());
            }
            else {
                std::cerr << "Invalid expression!\n";
                exit(EXIT_FAILURE);
            }

            this->_TryConsume(TokenType::RightParent, "Expected `)`!");

            this->_TryConsume(TokenType::CommandEnd, "Expected `;`!");

            return Node::Statement{.var = StatementFunction};

        }
        else {
            return {};
        }
    }

    auto Parser::ParseProgramm() -> std::optional<Node::Programm> {
        Node::Programm Programm;
        while (this->_Peek().has_value()) {
            if (auto Statement = this->ParseStatement()) {
                Programm.Statements.push_back(Statement.value());
            }
            else {
                std::cerr << "Invalid statement! \n";
                exit(EXIT_FAILURE);
            }
        }
        return Programm;
    }
}
