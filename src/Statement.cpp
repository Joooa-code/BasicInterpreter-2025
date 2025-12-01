#include "Statement.hpp"

#include <iostream>
#include <climits>
#include <sstream>
#include <utility>

#include "Program.hpp"
#include "VarState.hpp"
#include "utils/Error.hpp"

Statement::Statement(std::string source) : source_(std::move(source)) {}

const std::string& Statement::text() const noexcept { return source_; }

// TODO: Imply interfaces declared in the Statement.hpp.
// LetStatement

LetStatement::LetStatement(const std::string& source, const std::string& var, Expression* expr)
    : Statement(source), variable_(var), expression_(expr) {}

LetStatement::~LetStatement() {
    delete expression_;
}

void LetStatement::execute(VarState& state, Program& program) const {
    int value = expression_->evaluate(state);
    state.setValue(variable_, value);  // name-value
}



PrintStatement::PrintStatement(const std::string& source, Expression* expr)
    : Statement(source), expression_(expr) {}

PrintStatement::~PrintStatement() {
    delete expression_;
}

void PrintStatement::execute(VarState& state, Program& program) const {
    int value = expression_->evaluate(state);
    std::cout << value << '\n';
}

InputStatement::InputStatement(const std::string& source, const std::string& var)
    : Statement(source), variable_(var) {}

void InputStatement::execute(VarState& state, Program& program) const {
  while (true) {
    std::cout << " ? ";
    std::string input;
    std::getline(std::cin, input);

    size_t pos;
    long value = std::stol(input, &pos);

    // check if all converted
    if (pos != input.length()) {
      throw BasicError("INVALID NUMBER");
    }
    // check overflow
    if (value < INT_MIN || value > INT_MAX) {
      throw BasicError("INVALID NUMBER");
    }

    state.setValue(variable_, static_cast<int>(value));
    break;

  }
}

GotoStatement::GotoStatement(const std::string& source, int line)
    : Statement(source), targetLine_(line) {}

void GotoStatement::execute(VarState& state, Program& program) const {
    program.changePC(targetLine_);
}

IfStatement::IfStatement(const std::string& source, Expression* e1, char op, Expression* e2, int line)
    : Statement(source), expr1_(e1), expr2_(e2), op_(op), targetLine_(line) {}

IfStatement::~IfStatement() {
    delete expr1_;
    delete expr2_;
}

void IfStatement::execute(VarState& state, Program& program) const {
    int left = expr1_->evaluate(state);
    int right = expr2_->evaluate(state);

    bool condition = false;

    switch (op_) {
     case '=':
        condition = (left == right);
        break;
     case '<':
        condition = (left < right);
        break;
     case '>':
        condition = (left > right);
        break;
        default:
        throw BasicError("SYNTAX ERROR");
   }

    if (condition) {
      program.changePC(targetLine_);
    }

}

EndStatement::EndStatement(const std::string& source) : Statement(source) {}

void EndStatement::execute(VarState& state, Program& program) const {
  program.programEnd();
}

RemStatement::RemStatement(const std::string& source) : Statement(source) {}

void RemStatement::execute(VarState& state, Program& program) const {}