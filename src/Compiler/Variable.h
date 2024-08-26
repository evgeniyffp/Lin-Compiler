#ifndef VARIABLE_H
#define VARIABLE_H

#include <sstream>
#include <cassert>
#include <cstdlib>

#include "VariableType.h"
#include "Functions.h"

namespace Core::Compiler {
    // Now all types of var must be 8 bytes!!!
    struct Variable {
        size_t StackLock;
        VariableType Type;
        std::string Name;
        bool isConstruct = true;
    };

    inline size_t SizeVariable(const VariableType VarType) {
        switch (VarType) {
            case VariableType::Integer:
            case VariableType::String:
            case VariableType::Bool:
                return 8;
            default:
                assert(false);
        }
    }

    inline size_t SizeVariable(const Variable& Var) {
        return SizeVariable(Var.Type);
    }

    inline std::string VariableTypeToString(const Variable& Var) {
        switch (Var.Type) {
            case VariableType::Integer:
                return "i64";
            case VariableType::String:
                return "str";
            case VariableType::Bool:
                return "bool";
            default:
                assert(false);
        }
    }

    struct Generator;

    class VirtualStack {
    private:
        std::stringstream& out;
        std::vector<std::vector<Variable>> _Scopes;
        size_t CurrentLevelScope = 0;

        Generator& generator;
        void (Generator::* Callback)(const Node::StatementFunctionCall*, const std::optional<std::string>&);
        std::unordered_map<std::string, FunctionOverload>& Functions;

    public:
        VirtualStack(const VirtualStack&) = delete;
        VirtualStack(VirtualStack&&) = delete;

        VirtualStack& operator=(const VirtualStack&) = delete;
        VirtualStack&& operator=(VirtualStack&&) = delete;

        VirtualStack() = delete;

        explicit VirtualStack(
                    std::stringstream& out,
                    Generator& generator,
                    void (Generator::* Callback)(const Node::StatementFunctionCall*, const std::optional<std::string>&),
                    std::unordered_map<std::string, FunctionOverload>& Functions
        )
                : out(out)
                , _Scopes(100)
                , generator(generator)
                , Callback(Callback)
                , Functions(Functions) {}

        void Push(const Variable& Var, const std::string& what) {
            this->_Scopes[this->CurrentLevelScope].insert(this->_Scopes[this->CurrentLevelScope].end(), Var);
            out << "\tmov qword [rbp - " << this->CalculateLock(Var.Name) << "], " << what << "\n";
        }

        void Pop(const std::string& where = "") {
            std::string FunctionName = VariableTypeToString(this->_Scopes[this->CurrentLevelScope].back()) + "::__delete";
            if (this->Functions.contains(FunctionName) && this->_Scopes[this->CurrentLevelScope].back().isConstruct) {
                out << "\tmov " << PositionFnArgsInFnCall.at(0) << ", qword [rbp - " << CalculateLock(this->_Scopes[this->CurrentLevelScope].back().Name) << "]\n";
                out << "\tcall " << this->Functions[FunctionName].Overloads[{{this->_Scopes[this->CurrentLevelScope].back().Type}, {}}].AsmName << "\n";
                out << "\n";
            }

            this->_Scopes[this->CurrentLevelScope].pop_back();
        }

        size_t CalculateLock(const std::string& VariableName) {
            size_t lock = this->CalculateMaxLock();
            for (size_t i = this->CurrentLevelScope; i != std::string::npos; --i) {
                for (size_t j = this->_Scopes[i].size() - 1; j != std::string::npos; --j) {
                    if (this->_Scopes[i][j].Name == VariableName) {
                        return lock;
                    }
                    lock -= SizeVariable(this->_Scopes[i][j]);
                }
            }

            std::cerr << "Undeclared identifier: " << VariableName << "\n";
            exit(EXIT_FAILURE);
        }

        size_t CalculateMaxLock() {
            size_t lock = 0;
            for (size_t i = this->CurrentLevelScope; i != std::string::npos; --i) {
                for (size_t j = this->_Scopes[i].size() - 1; j != std::string::npos; --j) {
                    lock += SizeVariable(this->_Scopes[i][j]);
                }
            }
            return lock;
        }

        void VariableMove(const std::string& VariableName, const std::string& where) {
            size_t lock = this->CalculateMaxLock();
            for (size_t i = this->CurrentLevelScope; i != std::string::npos; --i) {
                for (size_t j = this->_Scopes[i].size() - 1; j != std::string::npos; --j) {
                    if (this->_Scopes[i][j].Name == VariableName) {
                        out << "\tmov " << where << ", qword [rbp - " << lock << "]\n";
                        return;
                    }
                    lock -= SizeVariable(this->_Scopes[i][j]);
                }
            }
            std::cerr << "Undeclared identifier: " << VariableName << "\n";
            exit(EXIT_FAILURE);
        }

        Variable Find(const std::string& VariableName) {
            size_t lock = this->CalculateMaxLock();
            for (size_t i = this->CurrentLevelScope; i != std::string::npos; --i) {
                for (size_t j = this->_Scopes[i].size() - 1; j != std::string::npos; --j) {
                    if (this->_Scopes[i][j].Name == VariableName) {
                        return this->_Scopes[i][j];
                    }
                    lock -= SizeVariable(this->_Scopes[i][j]);
                }
            }
            std::cerr << "Undeclared identifier: " << VariableName << "\n";
            exit(EXIT_FAILURE);
        }

        void ClearScope() {
            while (!this->_Scopes[this->CurrentLevelScope].empty()) {
                this->Pop();
            }

            this->DecCurrentLevelScope();
        }

        void IncCurrentLevelScope() { ++this->CurrentLevelScope; }
        void DecCurrentLevelScope() { --this->CurrentLevelScope; }
    };
}

namespace std {
    template <>
    struct hash<Core::Compiler::VariableType> {
        size_t operator()(const Core::Compiler::VariableType& Type) const noexcept {
            return static_cast<size_t>(Type);
        }
    };
}

#endif