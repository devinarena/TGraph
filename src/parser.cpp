
/**
 * @file parser.cpp
 * @author Devin Arena
 * @brief Implementation file for parser class.
 * @since 7/19/2022
 **/

#include "../include/parser.h"
#include "../include/tgraph.h"

#include <iostream>
#include <stack>

Parser::Parser() : tindex(0) {
  parseRules[+Token::VAR] =
      (ParseRule){.prefix = &variable, .precedence = Precedence::NONE};
  parseRules[+Token::CONST] =
      (ParseRule){.prefix = &literal, .precedence = Precedence::NONE};
  parseRules[+Token::ADD] =
      (ParseRule){.infix = &binary, .precedence = Precedence::ADD_SUB};
  parseRules[+Token::SUB] = (ParseRule){
      .prefix = &unary, .infix = &binary, .precedence = Precedence::ADD_SUB};
  parseRules[+Token::MUL] =
      (ParseRule){.infix = &binary, .precedence = Precedence::MUL_DIV};
  parseRules[+Token::DIV] =
      (ParseRule){.infix = &binary, .precedence = Precedence::MUL_DIV};
}

Precedence Parser::getPrecedence(Token token) {
  if (token == Token::ADD || token == Token::SUB) {
    return Precedence::ADD_SUB;
  } else if (token == Token::MUL || token == Token::DIV) {
    return Precedence::MUL_DIV;
  }
  return Precedence::NONE;
}

Token Parser::currentToken() {
  if (tindex >= tokens.size()) {
    return Token::NONE;
  }
  return static_cast<Token>(tokens[tindex]);
}

Token Parser::prevToken() {
  if (tindex == 0)
    return Token::NONE;
  return static_cast<Token>(tokens[tindex - 1]);
}

void Parser::binary() {
  Token op = prevToken();

  parsePrecedence((Precedence)(+getPrecedence(op) + 1));

  switch (op) {
    case Token::ADD:
      ops.push_back(+OP::ADD);
      break;
    case Token::SUB:
      ops.push_back(+OP::SUB);
      break;
    case Token::MUL:
      ops.push_back(+OP::MUL);
      break;
    case Token::DIV:
      ops.push_back(+OP::DIV);
      break;
    default:
      std::cerr << "Error: Invalid binary operator\n";
      break;
  }
}

void Parser::unary() {
  Token op = prevToken();

  parsePrecedence(Precedence::UNARY);

  switch (op) {
    case Token::SUB:
      ops.push_back(+OP::NEG);
      break;
    default:
      std::cerr << "Invalid unary operator: " << +op << "\n";
      break;
  }
}

void Parser::literal() {
  ops.push_back(+OP::CONST);
  ops.push_back(+currentToken());
  tindex++;
}

void Parser::variable() {
  ops.push_back(+OP::VAR);
}

void Parser::parsePrecedence(Precedence precedence) {
  ParseRule rule = parseRules[+currentToken()];
  tindex++;
  if (rule.prefix == NULL) {
    std::cerr << "Expected expression.\n";
    exit(1);
  }

  (this->*rule.prefix)();

  while (tindex < tokens.size() &&
         precedence <= getPrecedence(currentToken())) {
    rule = parseRules[+currentToken()];
    tindex++;
    if (rule.infix != NULL)
      (this->*rule.infix)();
  }
}

void Parser::expression() {
  parsePrecedence(Precedence::NONE);
}

std::vector<int> Parser::parse(std::vector<int>& tokens) {
  this->tokens = tokens;
  expression();
  return ops;
}

void Parser::printOPs(std::vector<int>& ops) {
  for (size_t i = 0; i < ops.size();) {
    i = printOP(ops, i);
  }
}

int Parser::printOP(std::vector<int>& ops, int idx) {
  int op = ops[idx];
  if (op == +OP::VAR) {
    std::cout << "VAR\n";
    return idx + 1;
  } else if (op == +OP::NEG) {
    std::cout << "NEG\n";
    return idx + 1;
  } else if (op == +OP::ADD) {
    std::cout << "ADD\n";
    return idx + 1;
  } else if (op == +OP::SUB) {
    std::cout << "SUB\n";
    return idx + 1;
  } else if (op == +OP::MUL) {
    std::cout << "MUL\n";
    return idx + 1;
  } else if (op == +OP::DIV) {
    std::cout << "DIV\n";
    return idx + 1;
  } else if (op == +OP::CONST) {
    std::cout << "CONST (" << ops[idx + 1] << ")\n";
    return idx + 2;
  }
  std::cerr << "Unknown operation: " << op << "\n";
  exit(1);
}