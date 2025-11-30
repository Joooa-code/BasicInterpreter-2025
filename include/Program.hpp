#pragma once

#include <memory>

#include "Recorder.hpp"
#include "VarState.hpp"

class Statement;

class Program {
 public:
  Program();

  void addStmt(int line, Statement* stmt);
  void removeStmt(int line);

  void run();
  void list() const;
  void clear();

  void execute(Statement* stmt);

  int getPC() const noexcept;  // PC:store the address of the next program to be executed.
  void changePC(int line);
  void programEnd();

 private:
  Recorder recorder_;
  VarState vars_;
  int programCounter_;
  bool programEnd_;

  void resetAfterRun() noexcept;
};
