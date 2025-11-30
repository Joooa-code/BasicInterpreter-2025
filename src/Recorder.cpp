// TO DO
#include "Recorder.hpp"
#include <iostream>
#include <algorithm>

// delete
Recorder::~Recorder() {
    clear();
}

// Insert or cover specified lines
void Recorder::add(int line, Statement* stmt) {
  // if exists
  auto it = programs_.find(line);
  if (it != programs_.end()) {
    // cover
    delete it->second;
    it->second = stmt;  // change
  }
  else {
    // insert
    programs_[line] = stmt;
  }
}

// remove
void Recorder::remove(int line) {
  auto it = programs_.find(line);
  if (it != programs_.end()) {
    delete it->second;
    programs_.erase(it);
  }
}

// find statement
const Statement* Recorder::get(int line) const noexcept {
  auto it = programs_.find(line);
  if (it != programs_.end()) {
    return it->second;
  }
  // do not exist
  return nullptr;
}

// if has line
bool Recorder::hasLine(int line) const noexcept {
  return programs_.find(line) != programs_.end();
}

// clear
void Recorder::clear() noexcept {
  auto it = programs_.begin();
  while (it != programs_.end()) {
    delete it->second;
    ++it;                   // move to the next
  }
  programs_.clear();
}

// print lines
void Recorder::printLines() const {
  auto it = programs_.begin();
  while (it != programs_.end()) {
    std::cout << it->first << " " << it->second->text() << '\n';
    ++it;
  }
} 

// return next line
int Recorder::nextLine(int line) const noexcept {
    // use upper_bound(in <map>) to find
    auto it = programs_.upper_bound(line);
    if (it != programs_.end()) {
        return it->first;
    }
    return -1;
}

// get first line
int Recorder::firstline() const noexcept {
  if (programs_.empty()) {
    return -1;
  }
  return programs_.begin()->first;
}
