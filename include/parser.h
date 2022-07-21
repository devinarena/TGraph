
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

#include "./scanner.h"

enum class OP;

enum class Precedence {
  NONE,
  TERM,
  FACTOR,
  POWER,
  UNARY,
};

class Parser;

struct ParseRule {
  void (Parser::*prefix)(void);
  void (Parser::*infix)(void);
  Precedence precedence;
};

class Parser {
  typedef Parser* (*ParserFunc)(void);

 private:
  std::vector<int> ops;
  std::vector<int> tokens;
  size_t tindex;
  struct ParseRule parseRules[(int)Token::END];
  Precedence getPrecedence(Token token);
  void parsePrecedence(Precedence precedence);
  void expression();
  void grouping();
  void binary();
  void unary();
  void variable();
  void literal();
  Token currentToken();
  Token prevToken();

 public:
  Parser();
  std::vector<int> parse(std::vector<int>& tokens);
  void printOPs(std::vector<int>& ops);
  int printOP(std::vector<int>& ops, int op);
};

#endif