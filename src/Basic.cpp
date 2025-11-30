#include <iostream>
#include <memory>
#include <string>

#include "Lexer.hpp"
#include "Parser.hpp"
#include "Program.hpp"
#include "Token.hpp"
#include "utils/Error.hpp"

int main() {
  Lexer lexer;
  Parser parser;
  Program program;

  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }
    try {
      // TODO: The main function.
      // lexer
      TokenStream tokens = lexer.tokenize(line);

      // parse
      ParsedLine parsedLine = parser.parseLine(tokens, line);

      if (parsedLine.getLine().has_value()) {
        // has line number
        int lineNumber = parsedLine.getLine().value();
        if (parsedLine.getStatement() == nullptr) {
          // only has line number, delete
          program.removeStmt(lineNumber);
        }
        else {
          program.addStmt(lineNumber, parsedLine.fetchStatement());
        }
      } else {
        // do immediately
        Statement* stmt = parsedLine.fetchStatement();
        if (stmt) {
          program.execute(stmt);
          delete stmt; // delete
        }
      }
    } catch (const BasicError& e) {
      std::cout << e.message() << "\n";
    }
  }
  return 0;
}