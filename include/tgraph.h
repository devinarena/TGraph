
/**
 * @file tgraph.h
 * @author Devin Arena
 * @brief Header for main terminal program.
 * @since 7/19/2022
 **/

#ifndef TGRAPH_TGRAPH_H
#define TGRAPH_TGRAPH_H

#include <vector>

#include "parser.h"

enum class OP { VAR, CVAR, CONST, ADD, SUB, MUL, DIV };
template <typename T>
constexpr auto operator+(T e) noexcept
    -> std::enable_if_t<std::is_enum<T>::value, std::underlying_type_t<T>> {
  return static_cast<std::underlying_type_t<T>>(e);
}

class TGraph {
 private:
  int screenWidth;
  int screenHeight;
  std::vector<std::vector<bool>> screen;
  std::vector<int> ops;
  Parser parser;

 public:
  TGraph();
  void setup();
  void draw();
  void parseEquation(std::string equation);
  int simulateEquation(std::vector<int> ops, int x);
};

#endif