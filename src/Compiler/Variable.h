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
    };

    inline size_t SizeVariable(VariableType VarType) {
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

    struct VirtualStack {
        VirtualStack(const VirtualStack&) = delete;
        VirtualStack(VirtualStack&&) = delete;

        VirtualStack& operator=(const VirtualStack&) = delete;
        VirtualStack&& operator=(VirtualStack&&) = delete;

        VirtualStack() = delete;

        std::stringstream& out;

        explicit VirtualStack(std::stringstream& out, std::unordered_map<std::string, FunctionOverload>& Functions) : out(out), _Scopes(100), _Functions(Functions) {}

        void Push(const Variable& Var, const std::string& what) {
            this->_Scopes[this->CurrentLevelScope].insert(this->_Scopes[this->CurrentLevelScope].begin(), Var);
            out << "\tmov qword [rbp - " << this->CalculateLock(Var.Name) << "], " << what << "\n";
        }

        void Pop(const std::string& where = "") {
            this->_Scopes[this->CurrentLevelScope].pop_back();
        }

        size_t CalculateLock(const std::string& VariableName) {
            size_t lock = this->CalculateMaxLock();
            for (size_t i = this->CurrentLevelScope; static_cast<int>(i + 1) > 0; --i) {
                for (size_t j = 0; j < this->_Scopes[i].size(); ++j) {
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
            for (size_t i = this->CurrentLevelScope; static_cast<int>(i + 1) > 0; --i) {
                for (size_t j = 0; j < this->_Scopes[i].size(); ++j) {
                    lock += SizeVariable(this->_Scopes[i][j]);
                }
            }
            return lock;
        }

        void VariableMove(const std::string& VariableName, const std::string& where) {
            size_t lock = this->CalculateMaxLock();
            for (size_t i = this->CurrentLevelScope; static_cast<int>(i + 1) > 0; --i) {
                for (size_t j = 0; j < this->_Scopes[i].size(); ++j) {
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
            for (size_t i = this->CurrentLevelScope; static_cast<int>(i + 1) > 0; --i) {
                for (size_t j = 0; j < this->_Scopes[i].size(); ++j) {
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

        std::vector<std::vector<Variable>> _Scopes;
        std::unordered_map<std::string, FunctionOverload>& _Functions;

        size_t CurrentLevelScope = 0;
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