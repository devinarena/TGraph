
/**
 * @file parser.h
 * @author Devin Arena
 * @brief Parses equations, converting into a list of operations.
 * @since 7/19/2022
 **/

#ifndef TGRAPH_PARSER_H
#define TGRAPH_PARSER_H

#include <string>
#include <vector>

#include "./scanner.hpp"

enum class OP;

enum class Precedence {
  NONE,
  TERM,
  FACTOR,
  POWER,
  UNARY,
};

class Parser;
union Operand;

struct ParseRule {
  void (Parser::*prefix)(void);
  void (Parser::*infix)(void);
  Precedence precedence;
};

class Parser {
  typedef Parser* (*ParserFunc)(void);

 private:
  std::vector<Operand> ops;
  std::vector<Token> tokens;
  size_t tindex;
  struct ParseRule parseRules[(int)TType::END];
  void parsePrecedence(Precedence precedence);
  void call();
  void expression();
  void func();
  void grouping();
  void binary();
  void unary();
  void variable();
  void literal();
  Token currentToken();
  Token prevToken();

 public:
  Parser();
  std::vector<Operand> parse(std::vector<Token>& tokens);
  void printOPs(std::vector<Operand>& ops);
  int printOP(std::vector<Operand>& ops, int op);
};

#endif