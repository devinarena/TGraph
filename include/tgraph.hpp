
/**
 * @file tgraph.h
 * @author Devin Arena
 * @brief Header file for TGraph, stores state about the current program
 * instance.
 * @since 7/19/2022
 **/

#ifndef TGRAPH_TGRAPH_H
#define TGRAPH_TGRAPH_H

#include <vector>

#include "./builtins.hpp"
#include "parser.hpp"

// version info

#define VERSION_MAJOR 0
#define VERSION_MINOR 6

// compiling with debug on prints out opcodes as the program runs

// #define TG_DEBUG

// Helper macros for creating Operand unions.
#define OPCODE(op) ((Operand){.opcode = op})
#define VALUE(val) ((Operand){.value = val})
#define FUNC(ptr) ((Operand){.fnptr = ptr})

// Opcodes for the instruction set, opcodes are generated by the Pratt parser
// and interpreted by TGraph.
enum class OP { VAR, CONST, NEG, ADD, SUB, MUL, DIV, POW, MAGIC, BUILTIN, END };

// overload operator for +(enum class) to give its ordinum
template <typename T>
constexpr auto operator+(T e) noexcept
    -> std::enable_if_t<std::is_enum<T>::value, std::underlying_type_t<T>> {
  return static_cast<std::underlying_type_t<T>>(e);
}

// Operand union for the instruction set.
union Operand {
  OP opcode;
  double value;
  BuiltinFunc fnptr;
};

class TGraph {
 private:
  // screen width and height of the terminal
  int screenWidth;
  int screenHeight;
  double stepX{1.0};
  double stepY{1.0};
  std::vector<std::vector<char>> screen;
  std::vector<std::vector<Operand>> ops;
  Parser parser;
  Scanner scanner;
  void writeToScreen(std::string text, int x, int y);

 public:
  TGraph();
  void setupWindow();
  void computePoints(int equation);
  void cli();
  void draw();
  void rerender();
  void parseEquation(std::string& equation);
  double simulateEquation(double x, int equation);
  void parseInput(std::string input);
  int getGraphed() const;
};

#endif