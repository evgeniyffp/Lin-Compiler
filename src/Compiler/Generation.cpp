//
// Created by Евгений on 20.06.2024.
//

#include <iostream>
#include <cassert>
#include <cmath>
#include "Generation.h"

namespace Core::Compiler {
    Generator::Generator(Node::Programm programm) : _Programm(programm), _StackSize(0) {
    }

    auto Generator::DefineExpressionType(const Node::Expression *Expression) -> std::optional<VariableType> {
        struct ExpressionVisitors {
            Generator* generator;
            std::optional<VariableType>& Type;
            void operator()(const Node::Term* Term) const {
                Type = generator->DefineTermType(Term);
            }

            void operator()(const Node::BinaryExpression* BinaryExpression) const {
                Type = generator->DefineBinaryExpressionType(BinaryExpression);
            }
        };

        std::optional<VariableType> Type;

        ExpressionVisitors visitors{this, Type};

        std::visit(visitors, Expression->var);

        return Type;
    }

    auto Generator::DefineBinaryExpressionType(const Node::BinaryExpression* BinaryExpression) -> std::optional<VariableType> {
        struct BinaryExpressionVisitors {
            Generator* generator;
            std::optional<VariableType>& Type;
            void operator()(const Node::BinaryExpressionAdd* BinaryExpressionAdd) const {
                auto TypeLhs = generator->DefineExpressionType(BinaryExpressionAdd->LeftHandSide);
                auto TypeRhs = generator->DefineExpressionType(BinaryExpressionAdd->RightHandSide);

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
                auto TypeLhs = generator->DefineExpressionType(BinaryExpressionMultiplication->LeftHandSide);
                auto TypeRhs = generator->DefineExpressionType(BinaryExpressionMultiplication->RightHandSide);

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
                auto TypeLhs = generator->DefineExpressionType(BinaryExpressionDivision->LeftHandSide);
                auto TypeRhs = generator->DefineExpressionType(BinaryExpressionDivision->RightHandSide);

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
                auto TypeLhs = generator->DefineExpressionType(BinaryExpressionSubtraction->LeftHandSide);
                auto TypeRhs = generator->DefineExpressionType(BinaryExpressionSubtraction->RightHandSide);

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

        BinaryExpressionVisitors visitors{this, Type};

        std::visit(visitors, BinaryExpression->var);

        return Type;
    }

    auto Generator::DefineTermType(const Node::Term* Term) -> std::optional<VariableType> {
        struct TermVisitors {
            Generator* generator;
            std::optional<VariableType>& Type;
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
                Type = generator->_Vars.at(TermIdentifier->Identifier.value.value()).Type;
            }

            void operator()(const Node::TermParent* TermParent) const {
                Type = generator->DefineExpressionType(TermParent->Expression);
            }
        };

        std::optional<VariableType> Type;

        TermVisitors visitors{this, Type};

        std::visit(visitors, Term->var);

        return Type;
    }

    auto Generator::GenerateTerm(const Node::Term *Term) -> void {
        struct TermVisitors {
            Generator* generator;
            void operator()(const Node::TermIntegerLiteral* TermIntegerLiteral) const {
                generator->_Output << "\tmov rax, " << TermIntegerLiteral->IntegerLiteral.value.value() << "\n";
                generator->_StackPush("rax");
            }

            void operator()(const Node::TermBoolLiteral* TermBoolLiteral) const {
                generator->_Output << "\tmov rax, " << TermBoolLiteral->BoolLiteral.value.value() << "\n";
                generator->_StackPush("rax");
            }

            void operator()(const Node::TermStringLiteral* TermStringLiteral) const {
                auto label = "msg" + std::to_string(reinterpret_cast<size_t>(TermStringLiteral));

                std::string AsmStringValue;
                std::string buffer;
                std::string InputValue = TermStringLiteral->StringLiteral.value.value();

                for (char i : InputValue) {
                    if (isEscape(i)) {
                        AsmStringValue += "\"" + buffer + "\", " + std::to_string(static_cast<int32_t>(i)) + ", ";
                        buffer.clear();
                    }
                    else {
                        buffer.push_back(i);
                    }
                }
                AsmStringValue += "\"" + buffer + "\", ";

                generator->_DataSegment << "\t" << label << " db " << AsmStringValue << 0 << "\n";

                generator->_Output << "\tmov rax, " << label << "\n";
                generator->_StackPush("rax");
            }

            void operator()(const Node::TermIdentifier* TermIdentifier) const {
                if (!generator->_Vars.contains(TermIdentifier->Identifier.value.value())) {
                    std::cerr << "Undeclared identifier: " << TermIdentifier->Identifier.value.value() << "\n";
                    exit(EXIT_FAILURE);
                }

                const auto& var = generator->_Vars.at(TermIdentifier->Identifier.value.value());
                generator->_StackPush("qword [rsp + " + std::to_string((generator->_StackSize - var.StackLock - 1) * 8) + "]");
            }

            void operator()(const Node::TermParent* TermParent) const {
                generator->GenetateExpression(TermParent->Expression);
            }
        };

        TermVisitors visitors{.generator = this};
        std::visit(visitors, Term->var);
    }

    auto Generator::GenerateIntegerBinaryExpression(const Node::BinaryExpression* BinaryExpression) -> void {
        struct IntegerBinaryExpressionVisitors {
            Generator* generator;
            void operator()(const Node::BinaryExpressionAdd* BinaryExpressionAdd) const {
                generator->GenetateExpression(BinaryExpressionAdd->RightHandSide);
                generator->GenetateExpression(BinaryExpressionAdd->LeftHandSide);

                generator->_StackPop("rax");
                generator->_StackPop("rbx");

                generator->_Output << "\tadd rax, rbx\n";

                generator->_StackPush("rax");
            }

            void operator()(const Node::BinaryExpressionMultiplication* BinaryExpressionMultiplication) const {
                generator->GenetateExpression(BinaryExpressionMultiplication->RightHandSide);
                generator->GenetateExpression(BinaryExpressionMultiplication->LeftHandSide);

                generator->_StackPop("rax");
                generator->_StackPop("rbx");

                generator->_Output << "\timul rax, rbx\n";

                generator->_StackPush("rax");
            }

            void operator()(const Node::BinaryExpressionDivision* BinaryExpressionDivision) const {
                generator->GenetateExpression(BinaryExpressionDivision->RightHandSide);
                generator->GenetateExpression(BinaryExpressionDivision->LeftHandSide);

                generator->_StackPop("rax");
                generator->_StackPop("rbx");

                generator->_Output << "\tdiv rbx\n";

                generator->_StackPush("rax");
            }

            void operator()(const Node::BinaryExpressionSubtraction* BinaryExpressionSubtraction) const {
                generator->GenetateExpression(BinaryExpressionSubtraction->RightHandSide);
                generator->GenetateExpression(BinaryExpressionSubtraction->LeftHandSide);

                generator->_StackPop("rax");
                generator->_StackPop("rbx");

                generator->_Output << "\tsub rax, rbx\n";

                generator->_StackPush("rax");
            }

            void operator()(const Node::BinaryExpressionEqualTo* BinaryExpressionEqualTo) const {
                assert(false);
            }

            void operator()(const Node::BinaryExpressionNotEqualTo* BinaryExpressionNotEqualTo) const {
                assert(false);
            }

        };

        IntegerBinaryExpressionVisitors visitors{.generator = this};
        std::visit(visitors, BinaryExpression->var);
    }

    auto Generator::GenerateStringBinaryExpression(const Node::BinaryExpression *BinaryExpression) -> void {
        struct StringBinaryExpressionVisitors {
            Generator* generator;
            void operator()(const Node::BinaryExpressionAdd* BinaryExpressionAdd) const {
                assert(false);
            }

            void operator()(const Node::BinaryExpressionMultiplication* BinaryExpressionMultiplication) const {
                assert(false);
            }

            void operator()(const Node::BinaryExpressionDivision* BinaryExpressionDivision) const {
                assert(false);
            }

            void operator()(const Node::BinaryExpressionSubtraction* BinaryExpressionSubtraction) const {
                assert(false);
            }

            void operator()(const Node::BinaryExpressionEqualTo* BinaryExpressionEqualTo) const {
                assert(false);
            }

            void operator()(const Node::BinaryExpressionNotEqualTo* BinaryExpressionNotEqualTo) const {
                assert(false);
            }

        };

        StringBinaryExpressionVisitors visitors{.generator = this};
        std::visit(visitors, BinaryExpression->var);
    }

    auto Generator::GenerateBoolBinaryExpression(const Node::BinaryExpression* BinaryExpression) -> void {
        struct BoolBinaryExpressionVisitors {
            Generator* generator;
            void operator()(const Node::BinaryExpressionAdd* BinaryExpressionAdd) const {
                assert(false);
            }

            void operator()(const Node::BinaryExpressionMultiplication* BinaryExpressionMultiplication) const {
                assert(false);
            }

            void operator()(const Node::BinaryExpressionDivision* BinaryExpressionDivision) const {
                assert(false);
            }

            void operator()(const Node::BinaryExpressionSubtraction* BinaryExpressionSubtraction) const {
                assert(false);
            }

            void operator()(const Node::BinaryExpressionEqualTo* BinaryExpressionEqualTo) const {
                auto LhsType = generator->DefineExpressionType(BinaryExpressionEqualTo->LeftHandSide);
                auto RhsType = generator->DefineExpressionType(BinaryExpressionEqualTo->RightHandSide);

                if (!LhsType.has_value() || !RhsType.has_value()) {
                    std::cerr << "Error in defining var\'s type! \n";
                    exit(EXIT_FAILURE);
                }

                if (LhsType.value() != RhsType.value()) {
                    std::cerr << "Expression has diffrent var types!\n";
                    exit(EXIT_FAILURE);
                }

                switch (LhsType.value()) {
                    case VariableType::Bool:
                    case VariableType::Integer: {
                        generator->GenetateExpression(BinaryExpressionEqualTo->RightHandSide);
                        generator->GenetateExpression(BinaryExpressionEqualTo->LeftHandSide);

                        generator->_StackPop("rax");
                        generator->_StackPop("rbx");

                        generator->_Output << "\tcmp rax, rbx\n";
                        generator->_Output << "\tsete cl\n";
                        generator->_Output << "\tmovzx rax, cl\n";

                        generator->_StackPush("rax");

                        break;
                    }
                    default: {
                        std::cerr << "Don\'t be fount this operator with this var type!\n";
                        exit(EXIT_FAILURE);
                    }

                }
            }

            void operator()(const Node::BinaryExpressionNotEqualTo* BinaryExpressionNotEqualTo) const {
                generator->GenetateExpression(BinaryExpressionNotEqualTo->RightHandSide);
                generator->GenetateExpression(BinaryExpressionNotEqualTo->LeftHandSide);

                generator->_StackPop("rax");
                generator->_StackPop("rbx");

                generator->_Output << "\tcmp rax, rbx\n";
                generator->_Output << "\tsetne cl\n";
                generator->_Output << "\tmovzx rax, cl\n";

                generator->_StackPush("rax");
            }

        };

        BoolBinaryExpressionVisitors visitors{.generator = this};
        std::visit(visitors, BinaryExpression->var);
    }

    auto Generator::GenerateBinaryExpression(const Node::BinaryExpression* BinaryExpression) -> void {
        auto ExpressionType = this->DefineBinaryExpressionType(BinaryExpression);
        if (!ExpressionType.has_value()) {
            std::cerr << "Unable to define expression type\n";
            exit(EXIT_FAILURE);
        }

        switch (ExpressionType.value()) {
            case VariableType::Integer: {
                this->GenerateIntegerBinaryExpression(BinaryExpression);
                break;
            }
            case VariableType::String: {
                this->GenerateStringBinaryExpression(BinaryExpression);
                break;
            }
            case VariableType::Bool: {
                this->GenerateBoolBinaryExpression(BinaryExpression);
                break;
            }
            default: {
                std::cerr << "Unable to define expression type\n";
                exit(EXIT_FAILURE);
            }
        }
    }

    auto Generator::GenetateExpression(const Node::Expression* Expression) -> void {
        struct ExpressionVisitors {
            Generator* generator;
            void operator()(const Node::Term* Term) const {
                generator->GenerateTerm(Term);
            }

            void operator()(const Node::BinaryExpression* BinaryExpression) const {
                generator->GenerateBinaryExpression(BinaryExpression);
            }
        };

        ExpressionVisitors visitors{.generator = this};
        std::visit(visitors, Expression->var);
    }

    auto Generator::GenetateStatement(const Node::Statement* Statement) -> void {
        struct StatementVisitors {
            Generator* generator;

            void operator()(const Node::StatementLet* StatementLet) const {
                if (generator->_Vars.contains(StatementLet->Identifier.value.value())) {
                    std::cerr << "Identifier already used: " << StatementLet->Identifier.value.value() << "\n";
                    exit(EXIT_FAILURE);
                }
                auto Type = generator->DefineExpressionType(StatementLet->Expression);
                if (!Type.has_value()) {
                    std::cerr << "Error in defining var\'s type! \n";
                    exit(EXIT_FAILURE);
                }
                generator->_Vars.insert({StatementLet->Identifier.value.value(), Variable{generator->_StackSize, Type.value()}});
                generator->GenetateExpression(StatementLet->Expression);
            }

            void operator()(const Node::StatementFunctionCall* StatementFunction) const {
                if (generator->_Functions.find(StatementFunction->FunctionName) == generator->_Functions.end()) {
                    std::cerr << "Function don\'t be found!\n";
                    exit(EXIT_FAILURE);
                }

                FunctionArgs FunctionArgs;
                FunctionArgs.Parametr = {};
                FunctionArgs.Args.reserve(StatementFunction->Arguments.size());

                for (size_t i = 0; i < StatementFunction->Arguments.size(); ++i) {
                    if (auto ArgType = generator->DefineExpressionType(StatementFunction->Arguments[i])) {
                        FunctionArgs.Args.push_back(ArgType.value());
                    }
                    else {
                        std::cerr << "Error in defining var\'s type! \n";
                        exit(EXIT_FAILURE);
                    }
                }

                Function FnOverload;
                bool isFailed = true;

                for (const auto& FnOverloadIterator : generator->_Functions[StatementFunction->FunctionName].Overloads) {
                    if (FnOverloadIterator.first == FunctionArgs) {
                        FnOverload = FnOverloadIterator.second;
                        isFailed = false;
                        break;
                    }
                }

                if (isFailed) {
                    std::cerr << "Function with this args don\'t be found!\n";
                    exit(EXIT_FAILURE);
                }

                // if (generator->_Functions[StatementFunction->FunctionName].Overloads.find(FunctionArgs) == generator->_Functions[StatementFunction->FunctionName].Overloads.end()) {
                //     std::cerr << "Function with this args don\'t be found!\n";
                //     exit(EXIT_FAILURE);
                // }

                for (size_t i = 0; i < generator->_PositionFnArgsInFnCall.size() && i < StatementFunction->Arguments.size(); ++i) {
                    generator->GenetateExpression(StatementFunction->Arguments.at(i));
                    generator->_StackPop(generator->_PositionFnArgsInFnCall[i]);
                }

                generator->_Output << "\tcall " << FnOverload.AsmName << "\n";
            }
        };

        StatementVisitors visitors{.generator = this};
        std::visit(visitors, Statement->var);
    }

    auto Generator::GenetateProgramm() -> std::string {
        this->_PositionFnArgsInFnCall[0] = "rdi";
        this->_PositionFnArgsInFnCall[1] = "rsi";
        this->_PositionFnArgsInFnCall[2] = "rdx";
        this->_PositionFnArgsInFnCall[3] = "rcx";
        this->_PositionFnArgsInFnCall[4] = "r8";
        this->_PositionFnArgsInFnCall[5] = "r9";

        this->_TextSegment << "section .text\n";
        this->_TextSegment << "\textern __malloc\n";
        this->_TextSegment << "\textern __free\n";
        this->_TextSegment << "\textern __malloc_init\n";
        this->_TextSegment << "\textern __malloc_deinit\n";

        this->_Functions["exit"].Overloads = {{{{VariableType::Integer}, {}}, Function{{}, "__exit"}}};
        this->_Functions["printf"].Overloads = {{{{VariableType::String}, {FunctionArgsParametr::UnlimitedArguments}}, Function{{}, "__printf"}}};
        this->_Functions["strlen"].Overloads = {{{{VariableType::String}, {}}, Function{{VariableType::Integer}, "__strlen"}}};

        this->_DataSegment << "section .data\n";

        this->_Output << "global _start\n";
        this->_Output << "global main\n\n";
        this->_Output << "_start:\n";
        this->_Output << "\tcall __malloc_init\n";
        this->_Output << "\tcall main\n";
        this->_Output << "\tcall __malloc_deinit\n";
        this->_Output << "\tret\n\n";
        this->_Output << "main:\n";

        for (const auto& Statement : this->_Programm.Statements) {
            this->GenetateStatement(&Statement);
            this->_Output << "\n";
        }

        for (const auto& FuncOverload : this->_Functions) {
            for (const auto& Func : FuncOverload.second.Overloads) {
                this->_TextSegment << "\textern " << Func.second.AsmName << "\n";
            }
        }

        this->_DataSegment << "\n";
        this->_TextSegment << "\n";
        this->_Output << "\n";

        return this->_DataSegment.str() + this->_TextSegment.str() + this->_Output.str();
    }
}
