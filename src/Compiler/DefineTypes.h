#ifndef DEFINETYPES_H
#define DEFINETYPES_H

#include <iostream>
#include <optional>
#include <variant>

#include "Parser.h"
#include "Variable.h"

namespace Core::Compiler {
    class Definer {
        static auto DefineExpressionType(const Node::Expression* Expression, VirtualStack& VStack) -> std::optional<VariableType> {
            struct ExpressionVisitors {
                std::optional<VariableType>& Type;
                VirtualStack& VStack;
                void operator()(const Node::Term* Term) const {
                    Type = DefineTermType(Term, VStack);
                }

                void operator()(const Node::BinaryExpression* BinaryExpression) const {
                    Type = DefineBinaryExpressionType(BinaryExpression, VStack);
                }
            };

            std::optional<VariableType> Type;

            ExpressionVisitors visitors{Type, VStack};

            std::visit(visitors, Expression->var);

            return Type;
        }

        static auto DefineBinaryExpressionType(const Node::BinaryExpression* BinaryExpression, VirtualStack& VStack) -> std::optional<VariableType> {
            struct BinaryExpressionVisitors {
                std::optional<VariableType>& Type;
                VirtualStack& VStack;
                void operator()(const Node::BinaryExpressionAdd* BinaryExpressionAdd) const {
                    auto TypeLhs = DefineExpressionType(BinaryExpressionAdd->LeftHandSide, VStack);
                    auto TypeRhs = DefineExpressionType(BinaryExpressionAdd->RightHandSide, VStack);

                    if (!TypeLhs.has_value()) {
                        std::cerr << "Error in defining var\'s type! \n";
                        exit(EXIT_FAILURE);
                    }

                    if (!TypeRhs.has_value()) {
                        std::cerr << "Error in defining var\'s type! \n";
                        exit(EXIT_FAILURE);
                    }

                    if (TypeLhs.value() != TypeRhs.value()) {
                        std::cerr << "Left expression and right expression have diffrent types! \n";
                        exit(EXIT_FAILURE);
                    }

                    Type = TypeLhs.value();
                }

                void operator()(const Node::BinaryExpressionMultiplication* BinaryExpressionMultiplication) const {
                    auto TypeLhs = DefineExpressionType(BinaryExpressionMultiplication->LeftHandSide, VStack);
                    auto TypeRhs = DefineExpressionType(BinaryExpressionMultiplication->RightHandSide, VStack);

                    if (!TypeLhs.has_value()) {
                        std::cerr << "Error in defining var\'s type! \n";
                        exit(EXIT_FAILURE);
                    }

                    if (!TypeRhs.has_value()) {
                        std::cerr << "Error in defining var\'s type! \n";
                        exit(EXIT_FAILURE);
                    }

                    if (TypeLhs.value() != TypeRhs.value()) {
                        std::cerr << "Left expression and right expression have diffrent types! \n";
                        exit(EXIT_FAILURE);
                    }

                    Type = TypeLhs.value();
                }

                void operator()(const Node::BinaryExpressionDivision* BinaryExpressionDivision) const {
                    auto TypeLhs = DefineExpressionType(BinaryExpressionDivision->LeftHandSide, VStack);
                    auto TypeRhs = DefineExpressionType(BinaryExpressionDivision->RightHandSide, VStack);

                    if (!TypeLhs.has_value()) {
                        std::cerr << "Error in defining var\'s type! \n";
                        exit(EXIT_FAILURE);
                    }

                    if (!TypeRhs.has_value()) {
                        std::cerr << "Error in defining var\'s type! \n";
                        exit(EXIT_FAILURE);
                    }

                    if (TypeLhs.value() != TypeRhs.value()) {
                        std::cerr << "Left expression and right expression have diffrent types! \n";
                        exit(EXIT_FAILURE);
                    }

                    Type = TypeLhs.value();
                }

                void operator()(const Node::BinaryExpressionSubtraction* BinaryExpressionSubtraction) const {
                    auto TypeLhs = DefineExpressionType(BinaryExpressionSubtraction->LeftHandSide, VStack);
                    auto TypeRhs = DefineExpressionType(BinaryExpressionSubtraction->RightHandSide, VStack);

                    if (!TypeLhs.has_value()) {
                        std::cerr << "Error in defining var\'s type! \n";
                        exit(EXIT_FAILURE);
                    }

                    if (!TypeRhs.has_value()) {
                        std::cerr << "Error in defining var\'s type! \n";
                        exit(EXIT_FAILURE);
                    }

                    if (TypeLhs.value() != TypeRhs.value()) {
                        std::cerr << "Left expression and right expression have diffrent types! \n";
                        exit(EXIT_FAILURE);
                    }

                    Type = TypeLhs.value();
                }

                void operator()(const Node::BinaryExpressionEqualTo* BinaryExpressionEqualTo) const {
                    Type = VariableType::Bool;
                }

                void operator()(const Node::BinaryExpressionNotEqualTo* BinaryExpressionNotEqualTo) const {
                    Type = VariableType::Bool;
                }

            };

            std::optional<VariableType> Type;

            BinaryExpressionVisitors visitors{Type, VStack};

            std::visit(visitors, BinaryExpression->var);

            return Type;
        }

        static auto DefineTermType(const Node::Term* Term, VirtualStack& VStack) -> std::optional<VariableType> {
            struct TermVisitors {
                std::optional<VariableType>& Type;
                VirtualStack& VStack;
                void operator()(const Node::TermIntegerLiteral* TermIntegerLiteral) const {
                    Type = VariableType::Integer;
                }

                void operator()(const Node::TermStringLiteral* TermStringLiteral) const {
                    Type = VariableType::String;
                }

                void operator()(const Node::TermBoolLiteral* TermBoolLiteral) const {
                    Type = VariableType::Bool;
                }

                void operator()(const Node::TermIdentifier* TermIdentifier) const {
                    Type = VStack.Find(TermIdentifier->Identifier.value.value()).Type;
                }

                void operator()(const Node::TermParent* TermParent) const {
                    Type = DefineExpressionType(TermParent->Expression,VStack);
                }
            };

            std::optional<VariableType> Type;

            TermVisitors visitors{Type, VStack};

            std::visit(visitors, Term->var);

            return Type;
        }
    };
}

#endif