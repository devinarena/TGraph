
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

// #define TG_DEBUG

#define OPCODE(op) ((Operand){.opcode = op})
#define VALUE(val) ((Operand){.value = val})

enum class OP { VAR, CONST, NEG, ADD, SUB, MUL, DIV, POW, END };
template <typename T>
constexpr auto operator+(T e) noexcept
    -> std::enable_if_t<std::is_enum<T>::value, std::underlying_type_t<T>> {
  return static_cast<std::underlying_type_t<T>>(e);
}

union Operand {
  OP opcode;
  double value;
};

class TGraph {
 private:
  int screenWidth;
  int screenHeight;
  int graphed{0};
  std::vector<std::vector<char>> screen;
  std::vector<Operand> ops;
  Parser parser;
  Scanner scanner;

 public:
  TGraph();
  void setupWindow();
  void draw();
  void parseEquation(std::string& equation);
  void computePoints(char symbol);
  int simulateEquation(double x);
  int getGraphed() const;
};

#endif