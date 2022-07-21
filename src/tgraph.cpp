
/**
 * @file tgraph.cpp
 * @author Devin Arena
 * @brief Implementation file for tgraph class.
 * @since 7/19/2022
 **/

#include "../include/tgraph.h"

#include <stdlib.h>
#include <windows.h>
#include <cmath>
#include <iostream>
#include <stack>
#include <vector>

TGraph::TGraph() {
  setupWindow();
}

void TGraph::setupWindow() {
  CONSOLE_SCREEN_BUFFER_INFO csbi;

  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  screenWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
  screenHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

  screen = std::vector(screenHeight, std::vector(screenWidth, ' '));
}

void TGraph::draw() {
  system("cls");
  std::cout << std::endl;
  for (int j = 0; j < screenHeight; j++) {
    for (int i = 0; i < screenWidth; i++) {
      if (screen[j][i] != ' ') {
        std::cout << screen[j][i];
      } else {
        int x = i - screenWidth / 2;
        int y = screenHeight / 2 - j;
        if (x == 0 && y == 0)
          std::cout << "+";
        else if (x == -1 && y == -1)
          std::cout << "O";
        else if (x == 0)
          std::cout << "|";
        else if (y == 0)
          std::cout << "-";
        else
          std::cout << " ";
      }
    }
    std::cout << "\n";
  }
}

void TGraph::parseEquation(std::string& equation) {
  std::vector<Token> tokens = scanner.scan(equation);
  ops.erase(ops.begin(), ops.end());
  ops = parser.parse(tokens);
  graphed++;
#ifdef TG_DEBUG
  parser.printOPs(ops);
#endif
}

int TGraph::simulateEquation(double x) {
#undef CONST
  std::stack<double> nums;
  int start = 0;
  for (size_t i = 0; i < ops.size(); i++) {
    start = i;
    switch (ops[i].opcode) {
      case OP::CONST: {
        nums.push(ops[++i].value);
        break;
      }
      case OP::VAR: {
        nums.push(x);
        break;
      }
      case OP::NEG: {
        double num = nums.top();
        nums.pop();
        nums.push(-num);
        break;
      }
      case OP::ADD: {
        double a = nums.top();
        nums.pop();
        double b = nums.top();
        nums.pop();
        nums.push(a + b);
        break;
      }
      case OP::SUB: {
        double a = nums.top();
        nums.pop();
        double b = nums.top();
        nums.pop();
        nums.push(b - a);
        break;
      }
      case OP::MUL: {
        double a = nums.top();
        nums.pop();
        double b = nums.top();
        nums.pop();
        nums.push(a * b);
        break;
      }
      case OP::DIV: {
        double a = nums.top();
        nums.pop();
        double b = nums.top();
        nums.pop();
        if (a == 0)
          nums.push(INT_MIN);
        else
          nums.push(b / a);
        break;
      }
      case OP::POW: {
        double a = nums.top();
        nums.pop();
        double b = nums.top();
        nums.pop();
        nums.push(std::pow(b, a));
        break;
      }
    }
#ifdef TG_DEBUG
    parser.printOP(ops, start);
    std::vector<double> temp;
    while (!nums.empty()) {
      temp.push_back(nums.top());
      nums.pop();
    }
    for (int i = temp.size() - 1; i >= 0; i--) {
      std::cout << "[" << temp[i] << "]";
      nums.push(temp[i]);
    }
    std::cout << "\n";
#endif
  }
  return nums.top();
#define CONST const
}

void TGraph::computePoints(char symbol) {
  for (int i = 0; i < screenWidth; i++) {
    int x = i - screenWidth / 2;
    int y = screenHeight / 2 - simulateEquation(x);
    if (y > 0 && y < screenHeight) {
      screen[y][i] = symbol;
    }
  }
}

// Getters and Setters

int TGraph::getGraphed() const {
  return graphed;
}