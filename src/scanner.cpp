
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

std::vector<int> Scanner::scan(std::string& equation) {
  for (size_t i = 0; i < equation.length(); i++) {
    if (equation[i] == ' ')
      continue;
    switch (equation[i]) {
      case '-': {
        tokens.push_back(+Token::SUB);
        continue;
      }
      case '+': {
        tokens.push_back(+Token::ADD);
        continue;
      }
      case '/': {
        tokens.push_back(+Token::DIV);
        continue;
      }
      case '*': {
        tokens.push_back(+Token::MUL);
        continue;
      }
      case '^': {
        tokens.push_back(+Token::POW);
        continue;
      }
      case 'x': {
        tokens.push_back(+Token::VAR);
        continue;
      }
      case '(': {
        tokens.push_back(+Token::O_PAREN);
        continue;
      }
      case ')': {
        tokens.push_back(+Token::C_PAREN);
        continue;
      }
    }
    size_t p = i;
    while (p < equation.length() && isdigit(equation[p])) {
      p++;
    }
    if (p == i) {
      std::cerr << "Invalid token: " << equation[i] << "\n";
    } else {
      std::string num = equation.substr(i, p - i);
      tokens.push_back(+Token::CONST);
      tokens.push_back(std::stoi(num));
      i = p - 1;
    }
  }
  return tokens;
}