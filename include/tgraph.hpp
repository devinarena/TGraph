
/**
 * @file tgraph.h
 * @author Devin Arena
 * @brief Header for main terminal program.
 * @since 7/19/2022
 **/

#ifndef TGRAPH_TGRAPH_H
#define TGRAPH_TGRAPH_H

#include <vector>

#include "./builtins.hpp"
#include "parser.hpp"

#define VERSION_MAJOR 1
#define VERSION_MINOR 0

#define TG_DEBUG

#define OPCODE(op) ((Operand){.opcode = op})
#define VALUE(val) ((Operand){.value = val})
#define FUNC(ptr) ((Operand){.fnptr = ptr})

enum class OP { VAR, CONST, NEG, ADD, SUB, MUL, DIV, POW, MAGIC, BUILTIN, END };
template <typename T>
constexpr auto operator+(T e) noexcept
    -> std::enable_if_t<std::is_enum<T>::value, std::underlying_type_t<T>> {
  return static_cast<std::underlying_type_t<T>>(e);
}

union Operand {
  OP opcode;
  double value;
  BuiltinFunc fnptr;
};

class TGraph {
 private:
  int screenWidth;
  int screenHeight;
  int graphed{0};
  double stepX{1.0};
  double stepY{1.0};
  std::vector<std::vector<char>> screen;
  std::vector<Operand> ops;
  Parser parser;
  Scanner scanner;
  void writeToScreen(std::string text, int x, int y);

 public:
  TGraph();
  void setupWindow();
  void cli();
  void draw();
  void parseEquation(std::string& equation);
  void computePoints(char symbol);
  double simulateEquation(double x);
  int getGraphed() const;
};

#endif