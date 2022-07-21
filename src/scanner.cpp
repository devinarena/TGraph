
/**
 * @file scanner.cpp
 * @author Devin Arena
 * @brief Implementation file for scanner class.
 * @since 7/19/2022
 **/

#include "../include/scanner.h"
#include "../include/tgraph.h"

#include <iostream>

Scanner::Scanner() {}

std::vector<Token> Scanner::scan(std::string& equation) {
  std::vector<Token> tokens;
  for (size_t i = 0; i < equation.length(); i++) {
    if (equation[i] == ' ')
      continue;
    switch (equation[i]) {
      case '-': {
        tokens.push_back(TOKEN(TType::SUB));
        continue;
      }
      case '+': {
        tokens.push_back(TOKEN(TType::ADD));
        continue;
      }
      case '/': {
        tokens.push_back(TOKEN(TType::DIV));
        continue;
      }
      case '*': {
        tokens.push_back(TOKEN(TType::MUL));
        continue;
      }
      case '^': {
        tokens.push_back(TOKEN(TType::POW));
        continue;
      }
      case 'x': {
        tokens.push_back(TOKEN(TType::VAR));
        continue;
      }
      case '(': {
        tokens.push_back(TOKEN(TType::O_PAREN));
        continue;
      }
      case ')': {
        tokens.push_back(TOKEN(TType::C_PAREN));
        continue;
      }
    }
    size_t p = i;
    while (p < equation.length()) {
      if (isdigit(equation[p])) {
        p++;
      } else {
        if (equation[p] == '.') {
          p++;
          while (p < equation.length() && isdigit(equation[p])) {
            p++;
          }
        }
        break;
      }
    }
    if (p == i) {
      std::cerr << "Invalid token: " << equation[i] << "\n";
    } else {
      std::string num = equation.substr(i, p - i);
      tokens.push_back(TOKEN(TType::CONST));
      tokens.push_back(VALUE_TOKEN(std::stod(num)));
      i = p - 1;
    }
  }
  return tokens;
}