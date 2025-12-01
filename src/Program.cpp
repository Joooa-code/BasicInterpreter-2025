// TODO: Imply interfaces declared in the Program.hpp.
#include "Program.hpp"

#include <iostream>
#include <algorithm>
#include <memory>

#include "Statement.hpp"
#include "utils/Error.hpp"

Program::Program() : programCounter_(-1), programEnd_(false) {}

void Program::addStmt(int line, Statement* stmt) {
    recorder_.add(line, stmt);  // use recorder
}

void Program::removeStmt(int line) {
    recorder_.remove(line);  // use recorder
}

void Program::run() {
    // reset
    resetAfterRun();
    programCounter_ = recorder_.firstline();  // check if program is empty
    if (programCounter_ < 0) {
      return;  // no need to run
    }

    // run till the end
    while (!programEnd_ && programCounter_ != -1) {
      const Statement* stmt = recorder_.get(programCounter_);
      if (stmt == nullptr) {
        throw BasicError("LINE NUMBER ERROR");
      }
      // find nextline
      programCounter_ = recorder_.nextLine(programCounter_);
      stmt->execute(vars_, *this);
    }
    resetAfterRun();  // CHECK:about goto and if?
}

void Program::list() const {
    // CHECK:if no lines?
    recorder_.printLines();
}

void Program::clear() {
    recorder_.clear();
    vars_.clear();
    programCounter_ = -1;
    programEnd_ = false;
}

void Program::execute(Statement* stmt) {
    stmt->execute(vars_, *this);
}


int Program::getPC() const noexcept {
    return programCounter_;
}

void Program::changePC(int line) {
    // if has line
    if (recorder_.hasLine(line)) {
      programCounter_ = line;
    }
    else {
      // error
      throw BasicError("LINE NUMBER ERROR");
    }
}

void Program::programEnd() {
    programEnd_ = true;
    programCounter_ = -1;
}

void Program::resetAfterRun() noexcept {
    programCounter_ = -1;
    programEnd_ = false;
}
