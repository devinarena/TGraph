
/**
 * @file parser.cpp
 * @author Devin Arena
 * @brief Implementation file for parser class.
 * @since 7/19/2022
 **/

#include "../include/parser.hpp"
#include "../include/tgraph.hpp"

#include <iostream>
#include <stack>

/**
 * @brief Default constructor. Generates the parseRule table.
 *
 * @param tokens the list of tokens to parse.
 */
Parser::Parser() : tindex(0) {
  parseRules[+TType::VAR] =
      (ParseRule){.prefix = &variable, .precedence = Precedence::NONE};
  parseRules[+TType::CONST] =
      (ParseRule){.prefix = &literal, .precedence = Precedence::NONE};
  parseRules[+TType::ADD] =
      (ParseRule){.infix = &binary, .precedence = Precedence::TERM};
  parseRules[+TType::SUB] = (ParseRule){
      .prefix = &unary, .infix = &binary, .precedence = Precedence::TERM};
  parseRules[+TType::MUL] =
      (ParseRule){.infix = &binary, .precedence = Precedence::FACTOR};
  parseRules[+TType::DIV] =
      (ParseRule){.infix = &binary, .precedence = Precedence::FACTOR};
  parseRules[+TType::POW] =
      (ParseRule){.infix = &binary, .precedence = Precedence::POWER};
  parseRules[+TType::P_O_M] =
      (ParseRule){.prefix = &unary, .precedence = Precedence::UNARY};
  parseRules[+TType::MAGIC] =
      (ParseRule){.prefix = &unary, .precedence = Precedence::UNARY};
  parseRules[+TType::FUNC] =
      (ParseRule){.prefix = &func, .precedence = Precedence::NONE};
  parseRules[+TType::C_PAREN] = (ParseRule){.precedence = Precedence::NONE};
  parseRules[+TType::O_PAREN] =
      (ParseRule){.prefix = &grouping, .precedence = Precedence::NONE};
}

/**
 * @brief Gets the current token or NONE if we are finished parsing.
 *
 * @return Token the current token.
 */
Token Parser::currentToken() {
  if (tindex >= tokens.size()) {
    return TOKEN(TType::NONE);
  }
  return tokens[tindex];
}

/**
 * @brief Gets the previous token or NONE if there is no previous token.
 *
 * @return Token the previous token.
 */
Token Parser::prevToken() {
  if (tindex == 0)
    return TOKEN(TType::NONE);
  return tokens[tindex - 1];
}

/**
 * @brief Base case for the Pratt Parser, all equations are expressions.
 */
void Parser::expression() {
  parsePrecedence(Precedence::TERM);
}

/**
 * @brief Descent case for Handles groupings (parentheses).
 */
void Parser::grouping() {
  expression();
  if (currentToken().type != TType::C_PAREN) {
    std::cerr << "Expected closing parenthesis.";
  }
  tindex++;
}

/**
 * @brief Descent case for built-in functions (e.g. sin, cos)
 */
void Parser::func() {
  Token funptr = currentToken();
  if (!funptr.fnptr) {
    std::cerr << "Unexpected function call.";
  }
  tindex += 2;  // skip this token and the opening parenthesis
  expression();
  if (currentToken().type != TType::C_PAREN) {
    std::cerr << "Expected closing parenthesis.";
    return;
  }
  tindex++;
  ops.push_back(OPCODE(OP::BUILTIN));
  ops.push_back(FUNC(funptr.fnptr));
}

/**
 * @brief Descent case for binary operators.
 */
void Parser::binary() {
  Token op = prevToken();

  parsePrecedence((Precedence)(+parseRules[+op.type].precedence + 1));

  switch (op.type) {
    case TType::ADD:
      ops.push_back(OPCODE(OP::ADD));
      break;
    case TType::SUB:
      ops.push_back(OPCODE(OP::SUB));
      break;
    case TType::MUL:
      ops.push_back(OPCODE(OP::MUL));
      break;
    case TType::DIV:
      ops.push_back(OPCODE(OP::DIV));
      break;
    case TType::POW:
      ops.push_back(OPCODE(OP::POW));
      break;
    default:
      std::cerr << "Error: Invalid binary operator\n";
      exit(1);
  }
}

/**
 * @brief Descent case for unary operators.
 */
void Parser::unary() {
  Token op = prevToken();

  parsePrecedence(Precedence::UNARY);

  switch (op.type) {
    case TType::P_O_M:
      ops.push_back(OPCODE(OP::PLUS_OR_MINUS));
      break;
    case TType::SUB:
      ops.push_back(OPCODE(OP::NEG));
      break;
    case TType::MAGIC:
      ops.push_back(OPCODE(OP::MAGIC));
      break;
    default:
      std::cerr << "Invalid unary operator: " << +op.type << "\n";
      exit(1);
  }
}

/**
 * @brief Descent case for literals (constants).
 */
void Parser::literal() {
  ops.push_back(OPCODE(OP::CONST));
  ops.push_back(VALUE(currentToken().value));
  tindex++;
}

/**
 * @brief Descent case for variables.
 */
void Parser::variable() {
  ops.push_back(OPCODE(OP::VAR));
}

/**
 * @brief Pratt parser implementation. Parses the prefix and post-fix rules of
 * given tokens.
 *
 * @param precedence
 */
void Parser::parsePrecedence(Precedence precedence) {
  ParseRule rule = parseRules[+currentToken().type];
  tindex++;
  if (rule.prefix == NULL) {
    std::cerr << "Expected expression.\n";
    exit(1);
  }

  (this->*rule.prefix)();

  while (tindex < tokens.size() &&
         precedence <= parseRules[+currentToken().type].precedence) {
    rule = parseRules[+currentToken().type];
    tindex++;
    if (rule.infix != NULL)
      (this->*rule.infix)();
  }
}

// PUBLIC FUNCTIONS

/**
 * @brief Parses a list of tokens, returning a list of opcodes.
 *
 * @param tokens std::vector<Token>& of tokens to parse.
 * @return std::vector<Operand> the parsed opcodes.
 */
std::vector<Operand> Parser::parse(std::vector<Token>& tokens) {
  this->tokens = tokens;
  ops = std::vector<Operand>();
  tindex = 0;
  expression();
  return ops;
}

/**
 * @brief Debug function to print the opcodes.
 *
 * @param ops std::vector<Operand>& the opcodes to print.
 */
void Parser::printOPs(std::vector<Operand>& ops) {
  for (size_t i = 0; i < ops.size();) {
    i = printOP(ops, i);
  }
}

/**
 * @brief Debug function to print an opcode.
 *
 * @param ops std::vector<Operand>& the opcodes to print.
 * @param idx size_t the index of the opcode to print.
 * @return int the index of the next opcode.
 */
int Parser::printOP(std::vector<Operand>& ops, int idx) {
  OP op = ops[idx].opcode;
  switch (op) {
    case OP::VAR: {
      std::cout << "VAR\n";
      return idx + 1;
    }
    case OP::NEG: {
      std::cout << "NEG\n";
      return idx + 1;
    }
    case OP::ADD: {
      std::cout << "ADD\n";
      return idx + 1;
    }
    case OP::SUB: {
      std::cout << "SUB\n";
      return idx + 1;
    }
    case OP::MUL: {
      std::cout << "MUL\n";
      return idx + 1;
    }
    case OP::DIV: {
      std::cout << "DIV\n";
      return idx + 1;
    }
    case OP::POW: {
      std::cout << "POW\n";
      return idx + 1;
    }
    case OP::PLUS_OR_MINUS: {
      std::cout << "PLUS_OR_MINUS\n";
      return idx + 1;
    }
    case OP::MAGIC: {
      std::cout << "MAGIC\n";
      return idx + 1;
    }
    case OP::CONST: {
      std::cout << "CONST (" << ops[idx + 1].value << ")\n";
      return idx + 2;
    }
    case OP::BUILTIN: {
      std::cout << "BUILTIN (" << ops[idx + 1].fnptr << ")\n";
      return idx + 2;
    }
    default:
      std::cerr << "Unknown operation: " << +op << "\n";
      exit(1);
  }
}