
/**
 * @file scanner.h
 * @author Devin Arena
 * @brief Scans through the tokens of an equation.
 * @since 7/19/2022
 **/

#ifndef TGRAPH_SCANNER_H
#define TGRAPH_SCANNER_H

#include "builtins.hpp"

#include <string>
#include <vector>

#define TOKEN(t) ((Token){.type = t})
#define VALUE_TOKEN(v) ((Token){.value = v})
#define FNPTR_TOKEN(ptr) ((Token){.fnptr = ptr})

enum class OP;

// Token Types for the scanner
enum class TType {
  NONE, // None
  VAR, // e.g. x
  CONST, // e.g. 1.0, 5, -3.5
  ADD, // +
  SUB, // -
  MUL, // *
  DIV, // /
  POW, // ^
  MAGIC, // ~
  O_PAREN, // (
  C_PAREN, // )
  FUNC, // e.g. sin, cos, tan, etc.
  END
};

// Token union for the scanner
union Token {
  TType type;
  double value;
  BuiltinFunc fnptr;
};

class Scanner {
 private:
 public:
  Scanner();
  std::vector<Token> scan(std::string& equation);
};

#endif