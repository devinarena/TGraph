
/**
 * @file scanner.cpp
 * @author Devin Arena
 * @brief Implementation file for scanner class.
 * @since 7/19/2022
 **/

#include "../include/scanner.hpp"
#include "../include/builtins.hpp"
#include "../include/tgraph.hpp"

#include <iostream>

/**
 * @brief Default constructor.
 */
Scanner::Scanner() {}

/**
 * @brief Scans an equation for different token types.
 *
 * @param equation the string to scan.
 * @return std::vector<Token> the tokens found in the string.
 */
std::vector<Token> Scanner::scan(std::string& equation) {
  std::vector<Token> tokens;
  for (size_t i = 0; i < equation.length(); i++) {
    if (equation[i] == ' ')
      continue;
    // single token types
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
      case '~': {
        tokens.push_back(TOKEN(TType::MAGIC));
        continue;
      }
      case 'x': {
        // ensure this is actually x and not part of a function or constant
        if (i + 1 < equation.length()) {
          if (!isalnum(equation[i + 1]))
            tokens.push_back(TOKEN(TType::VAR));
          continue;
        } else
          tokens.push_back(TOKEN(TType::VAR));
        break;
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
    // for parsing substrings
    size_t p = i;
    bool num = true;
    while (p < equation.length()) {
      if (isdigit(equation[p])) {
        // keep parsing until we hit a non-digit
        p++;
      } else {
        // if we hit a decimal, this could still be a number
        if (equation[p] == '.') {
          p++;
          while (p < equation.length() && isdigit(equation[p])) {
            p++;
          }
        } else if (isalpha(equation[p])) {
          // otherwise its a word
          num = false;
          p++;
          // keep checking for valid characters
          while (p < equation.length() && isalnum(equation[p])) {
            p++;
          }
        }
        break;
      }
    }
    if (p == i) {
      // if p = i, we didn't parse anything
      std::cerr << "Invalid token: " << equation[i] << "\n";
    } else if (num) {
      // generate number tokens
      std::string num = equation.substr(i, p - i);
      tokens.push_back(TOKEN(TType::CONST));
      tokens.push_back(VALUE_TOKEN(std::stod(num)));
      i = p - 1;
    } else {
      // generate function tokens
      std::string name = equation.substr(i, p - i);
      BuiltinFunc fn = resolveFunction(name);
      if (fn != nullptr) {
        tokens.push_back(TOKEN(TType::FUNC));
        tokens.push_back(FNPTR_TOKEN(fn));
      } else {
        tokens.push_back(TOKEN(TType::CONST));
        tokens.push_back(VALUE_TOKEN(resolveConstant(name)));
      }
      i = p - 1;
    }
  }
  return tokens;
}