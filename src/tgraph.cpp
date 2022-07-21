
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

void TGraph::draw() {
  system("cls");
  for (int j = 0; j < screenHeight; j++) {
    for (int i = 0; i < screenWidth; i++) {
      if (screen[j][i]) {
        std::cout << "*";
      } else {
        int x = i - screenWidth / 2;
        int y = screenHeight / 2 - j;
        if (x == 0 && y == 0)
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
  std::vector<int> tokens = scanner.scan(equation);
  ops = parser.parse(tokens);
#ifdef TG_DEBUG
  parser.printOPs(ops);
#endif
}

int TGraph::simulateEquation(int x) {
#undef CONST
  std::stack<int> nums;
  int start = 0;
  for (size_t i = 0; i < ops.size(); i++) {
    start = i;
    switch (ops[i]) {
      case +OP::CONST: {
        nums.push(ops[++i]);
        break;
      }
      case +OP::VAR: {
        nums.push(x);
        break;
      }
      case +OP::NEG: {
        int num = nums.top();
        nums.pop();
        nums.push(-num);
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
#ifdef TG_DEBUG
    parser.printOP(ops, start);
    std::vector<int> temp;
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

void TGraph::computePoints() {
  for (int i = 0; i < screenWidth; i++) {
    for (int j = 0; j < screenHeight; j++) {
      screen[j][i] = false;
    }
    int x = i - screenWidth / 2;
    int y = screenHeight / 2 - simulateEquation(x);
    if (y > 0 && y < screenHeight) {
      screen[y][i] = true;
    }
  }
}