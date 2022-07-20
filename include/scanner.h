
/**
 * @file scanner.h
 * @author Devin Arena
 * @brief Scans through the tokens of an equation.
 * @since 7/19/2022
 **/

#ifndef TGRAPH_SCANNER_H
#define TGRAPH_SCANNER_H

#include <string>
#include <vector>

enum class OP;

enum class Token {
  NONE,
  VAR,
  CONST,
  ADD,
  SUB,
  MUL,
  DIV,
  END
};

class Scanner {
 private:
  std::vector<int> tokens;

 public:
  Scanner();
  std::vector<int> scan(std::string& equation);
};

#endif