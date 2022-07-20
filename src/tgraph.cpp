
/**
 * @file tgraph.cpp
 * @author Devin Arena
 * @brief Implementation file for tgraph class.
 * @since 7/19/2022
 **/

#include "../include/tgraph.h"

#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <stack>
#include <vector>

TGraph::TGraph() {
  setup();
}

void TGraph::setup() {
  CONSOLE_SCREEN_BUFFER_INFO csbi;

  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  screenWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
  screenHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

  screen = std::vector(screenHeight, std::vector(screenWidth, false));
}

void TGraph::parseEquation(std::string equation) {
  ops = parser.parse(equation);
  parser.printOPs(ops);
  std::cout << simulateEquation(ops, 1);
}

int TGraph::simulateEquation(std::vector<int> ops, int x) {
#undef CONST
  std::stack<int> nums;
  for (size_t i = 0; i < ops.size(); i++) {
    switch(ops[i]) {
        case +OP::CONST: {
            nums.push(ops[++i]);
            break;
        }
        case +OP::VAR: {
            nums.push(x);
            break;
        }
        case +OP::CVAR: {
            break;
        }
        case +OP::ADD: {
            int a = nums.top();
            nums.pop();
            int b = nums.top();
            nums.pop();
            nums.push(a + b);
            break;
        }
        case +OP::SUB: {
            int a = nums.top();
            nums.pop();
            int b = nums.top();
            nums.pop();
            nums.push(b - a);
            break;
        }
        case +OP::MUL: {
            int a = nums.top();
            nums.pop();
            int b = nums.top();
            nums.pop();
            nums.push(a * b);
            break;
        }
        case +OP::DIV: {
            int a = nums.top();
            nums.pop();
            int b = nums.top();
            nums.pop();
            nums.push(b / a);
            break;
        }
    }
#define CONST const
  }
  return nums.top();
}