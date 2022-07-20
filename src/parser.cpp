
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

Parser::Parser() {}

std::vector<int> Parser::parse(std::string equation) {
  // for possibly adding words I need to consider strings not just characters.
  std::stack<std::string> operators;
  std::stack<std::string> operands;
  std::vector<int> ops;

  size_t start = 0;

  for (size_t i = 0; i < equation.length(); i++) {
    if (equation[i] == ' ') {
      if ((i - start) == 0) {
        start = i + 1;
        continue;
      }
      std::string token = equation.substr(start, i - start);
      if (token.length() == 1) {
        if (token == "+" || token == "-" || token == "*" || token == "/") {
          operators.push(token);
        } else {
          operands.push(token);
        }
      }
      start = i + 1;
    }
  }

  // 1 + x + 2
  // 1 x 2
  // + +

  while (!operators.empty()) {
    std::string op = operators.top();
    if (op == "+" || op == "-" || op == "*" || op == "/") {
      std::string a = operands.top();
      operands.pop();
      std::string b = operands.top();
      operands.pop();
      if (b == "x" || b == "cx") {
        ops.push_back(b == "x" ? +OP::VAR : +OP::CVAR);
        if (a != "x" && a != "cx") {
          ops.push_back(+OP::CONST);
          ops.push_back(std::stoi(a));
          operands.push("cx");
        } else {
          ops.push_back(a == "x" ? +OP::VAR : +OP::CVAR);
          if (b == "x" && a == "x")
            operands.push("x");
        }
      } else {
        ops.push_back(+OP::CONST);
        ops.push_back(std::stoi(b));
        if (a != "x" && a != "cx") {
          if (op == "+")
            ops[ops.size() - 1] += std::stoi(a);
          else if (op == "-")
            ops[ops.size() - 1] -= std::stoi(a);
          else if (op == "*")
            ops[ops.size() - 1] *= std::stoi(a);
          else if (op == "/")
            ops[ops.size() - 1] /= std::stoi(a);
          operands.push("" + ops[ops.size() - 1]);
        } else {
          ops.push_back(a == "x" ? +OP::VAR : +OP::CVAR);
          operands.push("cx");
        }
      }
      if (op == "+")
        ops.push_back(+OP::ADD);
      else if (op == "-")
        ops.push_back(+OP::SUB);
      else if (op == "*")
        ops.push_back(+OP::MUL);
      else if (op == "/")
        ops.push_back(+OP::DIV);
    }
    operators.pop();
  }

  while (!operands.empty()) {
    std::string op = operands.top();
    if (op == "x" || op == "cx") {
      ops.push_back(op == "x" ? +OP::VAR : +OP::CVAR);
    } else {
      ops.push_back(+OP::CONST);
      ops.push_back(std::stoi(op));
    }
    operands.pop();
  }

  return ops;
}

void Parser::printOPs(std::vector<int> ops) {
  for (size_t i = 0; i < ops.size(); i++) {
    std::cout << i << " : ";
    if (ops[i] == +OP::VAR)
      std::cout << "VAR";
    else if (ops[i] == +OP::CVAR)
      std::cout << "CVAR";
    else if (ops[i] == +OP::CONST)
      std::cout << "CONST = " << ops[++i];
    else if (ops[i] == +OP::ADD)
      std::cout << "ADD";
    else if (ops[i] == +OP::SUB)
      std::cout << "SUB";
    else if (ops[i] == +OP::MUL)
      std::cout << "MUL";
    else if (ops[i] == +OP::DIV)
      std::cout << "DIV";

    std::cout << "\n";
  }
}