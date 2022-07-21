
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

#define TOKEN(t) ((Token){.type = t})
#define VALUE_TOKEN(v) ((Token){.value = v})

enum class OP;

enum class TType {
  NONE,
  VAR,
  CONST,
  ADD,
  SUB,
  MUL,
  DIV,
  POW,
  MAGIC,
  O_PAREN,
  C_PAREN,
  END
};

union Token {
  TType type;
  double value;
};

class Scanner {
 private:
 public:
  Scanner();
  std::vector<Token> scan(std::string& equation);
};

#endif