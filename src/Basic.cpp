#include <iostream>
#include <memory>
#include <string>

#include "Lexer.hpp"
#include "Parser.hpp"
#include "Program.hpp"
#include "Token.hpp"
#include "utils/Error.hpp"

void printHelp() {
  std::cout << "Interpreter Commands:" << '\n';
  std::cout << "  RUN     - Begin program execution starting from the smallest line number" << '\n';
  std::cout << "  LIST    - List all program lines in ascending order by line number" << '\n';
  std::cout << "  CLEAR   - Clear all program lines" << '\n';
  std::cout << "  QUIT    - Exit the interpreter" << '\n';
  std::cout << "  HELP    - Print this help message" << '\n';
}

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
      TokenStream tokens = lexer.tokenize(line);
      const Token* cur_t = tokens.peek();

      switch (cur_t->type) {
        case TokenType::HELP:
          printHelp();
          break;
        case TokenType::QUIT:
          return 0;
        case TokenType::CLEAR:
          program.clear();
          break;
        case TokenType::LIST:
          program.list();
          break;
        case TokenType::RUN:
          program.run();
          break;
        default:
          ParsedLine pline = parser.parseLine(tokens, line);
          if (pline.getLine().has_value()) {
            // has line number
            int lineNumber = pline.getLine().value();
            Statement* stmt = pline.fetchStatement();
            if (stmt != nullptr) {
              try {
                program.addStmt(lineNumber, stmt);
              }
              catch (...) {
                delete stmt;
                throw;
              }
            }
            else {
              // only has line number
              program.removeStmt(lineNumber);
            }
          }
          else {
            // do immediately
            Statement* stmt = pline.fetchStatement();
            if (stmt != nullptr) {
              try {
                program.execute(stmt);
              }
              catch (...) {
                delete stmt;
                throw;
              }
              delete stmt;
            }
          }
          break;
      }
    } catch (const BasicError& e) {
      std::cout << e.message() << std::endl;
    }

  }
  return 0;
}