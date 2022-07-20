
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

static int getPrecedence(std::string& op) {
  if (op.compare("+") == 0) {
    return 1;
  } else if (op.compare("-") == 0) {
    return 1;
  } else if (op.compare("*") == 0) {
    return 2;
  } else if (op.compare("/") == 0) {
    return 2;
  } else if (op.compare("^") == 0) {
    return 3;
  } else {
    return 0;
  }
}

static void parsePrecedence(int precedence) {
    
}

std::vector<int> Parser::parse(std::string& equation) {
//   // for possibly adding words I need to consider strings not just characters.
//   std::stack<std::string> operators;
//   std::stack<std::string> operands;
//   std::vector<int> ops;

//   size_t start = 0;

//   for (size_t i = 0; i < equation.length(); i++) {
//     if (equation[i] == ' ') {
//       if ((i - start) == 0) {
//         start = i + 1;
//         continue;
//       }
//       std::string token = equation.substr(start, i - start);
//       if (token.length() == 1) {
//         if (token[0] == '+' || token[0] == '-' || token[0] == '*' ||
//             token[0] == '/') {
//           if (operators.empty()) {
//             operators.push(token);
//           } else {
//             std::string& top = operators.top();
//             if (getPrecedence(top) > getPrecedence(token)) {
//               operators.pop();
//               operators.push(token);
//               operators.push(top);
//               std::string& a = operands.top();
//               operands.pop();
//               std::string& b = operands.top();
//               operands.pop();
//               operands.push(a);
//               operands.push(b);
//             } else
//               operators.push(token);
//           }
//           start = i + 1;
//           continue;
//         }
//       }
//       operands.push(token);
//       start = i + 1;
//     }
//   }

//   // 1 + x + 2
//   // 1 x 2
//   // + +

//   while (!operators.empty()) {
//     std::string& op = operators.top();
//     if (op[0] == '+' || op[0] == '-' || op[0] == '*' || op[0] == '/') {
//       std::string& a = operands.top();
//       operands.pop();
//       std::string& b = operands.top();
//       operands.pop();
//       if (b[0] == 'x' || b.compare("cx") == 0) {
//         ops.push_back(b[0] == 'x' ? +OP::VAR : +OP::CVAR);
//         if (a[0] != 'x' && a.compare("cx") != 0) {
//           ops.push_back(+OP::CONST);
//           ops.push_back(std::stoi(a));
//           operands.push("cx");
//         } else {
//           ops.push_back(a[0] == 'x' ? +OP::VAR : +OP::CVAR);
//           if (b[0] == 'x' && a[0] == 'x')
//             operands.push("x");
//           else {
//             operands.push("cx");
//           }
//         }
//       } else {
//         ops.push_back(+OP::CONST);
//         ops.push_back(std::stoi(b));
//         if (a[0] != 'x' && a.compare("cx") != 0) {
//           ops.push_back(+OP::CONST);
//           ops.push_back(std::stoi(a));
//           operands.push("cx");
//         } else {
//           ops.push_back(a[0] == 'x' ? +OP::VAR : +OP::CVAR);
//           operands.push("cx");
//         }
//       }
//       if (op == "+")
//         ops.push_back(+OP::ADD);
//       else if (op == "-")
//         ops.push_back(+OP::SUB);
//       else if (op == "*")
//         ops.push_back(+OP::MUL);
//       else if (op == "/")
//         ops.push_back(+OP::DIV);
//     }
//     operators.pop();
//   }

//   while (!operands.empty()) {
//     std::string& op = operands.top();
//     if (op[0] == 'x') {
//       ops.push_back(+OP::VAR);
//     } else if (op.compare("cx") == 0) {
//       ops.push_back(+OP::CVAR);
//     } else {
//       ops.push_back(+OP::CONST);
//       ops.push_back(std::stoi(op));
//     }
//     operands.pop();
//   }

//   return ops;
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
  } else if (op == +OP::CVAR) {
    std::cout << "CVAR\n";
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