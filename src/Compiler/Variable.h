#ifndef VARIABLE_H
#define VARIABLE_H

#include <stdint.h>
#include <stdlib.h>
#include <stack>
#include <sstream>

namespace Core::Compiler {
    enum class VariableType : int8_t {
        Integer,
        String,
        Bool
    };

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

        explicit VirtualStack(std::stringstream& out) : out(out), Scopes(100) {}

        void Push(const Variable& Var, const std::string& what) {
            this->Scopes[this->CurrentLevelScope].push_back(Var);
            //out << "\tpush " << what << "\n";
            out << "\tmov qword [rbp - " << this->CalculateLock(Var.Name) << "], " << what << "\n";
        }

        void Pop(const std::string& where = "") {
            this->Scopes[this->CurrentLevelScope].pop_back();
            //out << "\tpop " << where << "\n";
        }

        size_t CalculateLock(const std::string& VariableName) {
            size_t lock = 0;
            for (size_t i = this->CurrentLevelScope; static_cast<int>(i + 1) > 0; --i) {
                for (size_t j = 0; j < this->Scopes[i].size(); ++j) {
                    if (this->Scopes[i][j].Name == VariableName) {
                        return lock;
                    }
                    lock += SizeVariable(this->Scopes[i][j]);
                }
            }
            std::cerr << "Undeclared identifier: " << VariableName << "\n";
            exit(EXIT_FAILURE);
        }

        void VariableMove(const std::string& VariableName, const std::string& where) {
            size_t lock = 0;
            for (size_t i = this->CurrentLevelScope; static_cast<int>(i + 1) > 0; --i) {
                for (size_t j = 0; j < this->Scopes[i].size(); ++j) {
                    if (this->Scopes[i][j].Name == VariableName) {
                        out << "\tmov rax, [rsp - " << lock << "]\n";
                        out << "\tmov " << where << ", rax\n";
                        return;
                    }
                    lock += SizeVariable(this->Scopes[i][j]);
                }
            }
            std::cerr << "Undeclared identifier: " << VariableName << "\n";
            exit(EXIT_FAILURE);
        }

        Variable Find(const std::string& VariableName) {
            size_t lock = 0;
            for (size_t i = this->CurrentLevelScope; static_cast<int>(i + 1) > 0; --i) {
                for (size_t j = 0; j < this->Scopes[i].size(); ++j) {
                    if (this->Scopes[i][j].Name == VariableName) {
                        return this->Scopes[i][j];
                    }
                    lock += SizeVariable(this->Scopes[i][j]);
                }
            }
            std::cerr << "Undeclared identifier: " << VariableName << "\n";
            exit(EXIT_FAILURE);
        }

        void ClearScope() {
            while (!this->Scopes[this->CurrentLevelScope].empty()) {
                this->Pop();
            }

            this->DecCurrentLevelScope();
        }

        std::vector<std::vector<Variable>> Scopes;

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