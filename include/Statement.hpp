#pragma once

#include <memory>
#include <string>

#include "Expression.hpp"

class Program;
class VarState;

class Statement {
 public:
  explicit Statement(std::string source);
  virtual ~Statement() = default;

  virtual void execute(VarState& state, Program& program) const = 0;

  const std::string& text() const noexcept;

 private:
  std::string source_;
};

// TODO: Other statement types derived from Statement, e.g., GOTOStatement,
// LetStatement, etc.

// LET
class LetStatement : public Statement {
private:
    std::string variable_;
    Expression* expression_;

public:
    LetStatement(const std::string& source, const std::string& var, Expression* expr);
    ~LetStatement();
    void execute(VarState& state, Program& program) const override;
};

// PRINT
class PrintStatement : public Statement {
private:
    Expression* expression_;

public:
    PrintStatement(const std::string& source, Expression* expr);
    ~PrintStatement();
    void execute(VarState& state, Program& program) const override;
};

// INPUT
class InputStatement : public Statement {
private:
    std::string variable_;

public:
    InputStatement(const std::string& source, const std::string& var);
    void execute(VarState& state, Program& program) const override;
};

// GOTO
class GotoStatement : public Statement {
private:
    int targetLine_;

public:
    GotoStatement(const std::string& source, int line);
    void execute(VarState& state, Program& program) const override;
};

// IF
class IfStatement : public Statement {
private:
    Expression* expr1_;
    Expression* expr2_;
    char op_;
    int targetLine_;

public:
    IfStatement(const std::string& source, Expression* e1, char op, Expression* e2, int line);
    ~IfStatement();
    void execute(VarState& state, Program& program) const override;
};

// END
class EndStatement : public Statement {
public:
    EndStatement(const std::string& source);
    void execute(VarState& state, Program& program) const override;
};

// REM
class RemStatement : public Statement {
public:
    RemStatement(const std::string& source);
    void execute(VarState& state, Program& program) const override;
};