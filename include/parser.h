
/**
 * @file parser.h
 * @author Devin Arena
 * @brief Parses equations, converting into a list of operations.
 * @since 7/19/2022
 **/

#ifndef TGRAPH_PARSER_H
#define TGRAPH_PARSER_H

#include <string>
#include <vector>

enum class OP;

class Parser {
 private:
 public:
  Parser();
  std::vector<int> parse(std::string& equation);
  void printOPs(std::vector<int>& ops);
  int printOP(std::vector<int>& ops, int op);
};

#endif