
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
#include <stack>

#include "./builtins.hpp"
#include "parser.hpp"

// version infos

#define VERSION_MAJOR 0
#define VERSION_MINOR 6

// os info
#if (defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__))
#define TG_WINDOWS
#endif

#if (defined(__linux__) || defined(__unix__) || defined(__unix) || \
     defined(unix) || defined(__APPLE__) || defined(__MACH__))
#define TG_LINUX
#endif

// compiling with debug on prints out opcodes as the program runs

// #define TG_DEBUG

// Helper macros for creating Operand unions.
#define OPCODE(op) ((Operand){.opcode = op})
#define VALUE(val) ((Operand){.value = val})
#define FUNC(ptr) ((Operand){.fnptr = ptr})

// Opcodes for the instruction set, opcodes are generated by the Pratt parser
// and interpreted by TGraph.
enum class OP { VAR, CONST, NEG, ADD, SUB, MUL, DIV, POW, PLUS_OR_MINUS, MAGIC, BUILTIN, END };

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
  std::vector<std::string> equations;
  Parser parser;
  Scanner scanner;
  void writeToScreen(std::string text, int x, int y);

 public:
  TGraph();
  void setupWindow();
  void computePoints(int equation);
  void cli();
  void draw(std::ostream& stream);
  void rerender();
  void parseEquation(std::string& equation);
  std::vector<double> simulateEquation(double x, int equation);
  void parseInput(std::string input);
  int getGraphed() const;
};

#endif