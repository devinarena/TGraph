
/**
 * @file parser.h
 * @author Devin Arena
 * @brief Parses tokens, converting into a list of opcodes.
 * @since 7/19/2022
 **/

#ifndef TGRAPH_PARSER_H
#define TGRAPH_PARSER_H

#include <string>
#include <vector>

#include "./scanner.hpp"

enum class OP;

// Pratt Parser precedence table
enum class Precedence {
  NONE,
  TERM,
  FACTOR,
  POWER,
  UNARY,
};

class Parser;
union Operand;

/**
 * @brief ParseRules contain pointers to prefix and infix functions for tokens.
 */
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
  ParseRule parseRules[(int)TType::END];

  Token currentToken();
  Token prevToken();
  void parsePrecedence(Precedence precedence);
  void expression();
  void func();
  void grouping();
  void binary();
  void unary();
  void variable();
  void literal();

 public:
  Parser();
  std::vector<Operand> parse(std::vector<Token>& tokens);
  void printOPs(std::vector<Operand>& ops);
  int printOP(std::vector<Operand>& ops, int op);
};

#endif