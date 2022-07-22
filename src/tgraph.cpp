
/**
 * @file tgraph.cpp
 * @author Devin Arena
 * @brief Implementation file for tgraph class.
 * @since 7/19/2022
 **/

#include "../include/tgraph.hpp"

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

  stepX = 1.0;
  stepY = 1.0;
  graphed = 0;

  writeToScreen("TGraph v" + std::to_string(VERSION_MAJOR) + "." +
                    std::to_string(VERSION_MINOR),
                1, 1);
  writeToScreen("x-step: 1.0", 1, 2);
  writeToScreen("y-step: 1.0", 1, 3);
  writeToScreen("Equations:", 1, 4);
}

void TGraph::writeToScreen(std::string text, int x, int y) {
  for (size_t i = 0; i < text.length(); i++) {
    screen[y][x + i] = text[i];
  }
}

void TGraph::cli() {
  draw();
  std::string command{""};
  do {
    char curr = 'a' + (23 + getGraphed()) % 26;
    std::cout << "Equation or command > (y = ) ";
    std::getline(std::cin, command);
    std::vector<std::string> tokens;
    size_t start = 0;
    for (size_t i = 0; i < command.length(); i++) {
      if (command[i] == ' ' && start != i) {
        tokens.push_back(command.substr(start, i - start));
        start = i + 1;
      }
    }
    if (start != command.length()) {
      tokens.push_back(command.substr(start, command.length() - start));
    }
    if (tokens.size() == 0)
      break;
    if (tokens[0].compare("help") == 0) {
      system("cls");
      std::cout << "TGraph v" << VERSION_MAJOR << "." << VERSION_MINOR
                << " by Devin Arena:\n";
      std::cout << "help - displays this help message menu\n";
      std::cout << "graph - re-renders the graph\n";
      std::cout << "clear - clears all equations\n";
      std::cout << "exit - exits the program\n";
      std::cout << "xstep [step_size, default=1] - sets the x step size\n";
      std::cout << "ystep [step_size, default=1] - sets the y step size\n";
      std::cout << "Simply enter an equation to graph it.\n";
      std::cout << "\nExamples:\n";
      std::cout << "y = x^2\n";
      std::cout << "y = sin(x)\n";
      std::cout << "y = x*cos(x / 5)\n";
      std::cout << "y = e ^ sqrt(x)\n\n";
    } else if (tokens[0].compare("graph") == 0) {
      system("cls");
      draw();
    } else if (tokens[0].compare("clear") == 0) {
      system("cls");
      setupWindow();
      draw();
    } else if (tokens[0].compare("exit") == 0) {
      exit(0);
    } else if (tokens[0].compare("xstep") == 0) {
      if (tokens.size() == 1) {
        std::cout << "x-step: " << stepX << "\n";
      } else if (tokens.size() == 2) {
        stepX = std::stod(tokens[1]);
        writeToScreen("x-step: " + std::to_string(stepX), 1, 2);
      } else {
        std::cout << "Invalid command syntax.\n";
      }
    } else if (tokens[0].compare("ystep") == 0) {
      if (tokens.size() == 1) {
        std::cout << "y-step: " << stepY << "\n";
      } else if (tokens.size() == 2) {
        stepY = std::stod(tokens[1]);
        writeToScreen("y-step: " + std::to_string(stepY), 1, 3);
      } else {
        std::cout << "Invalid command syntax.\n";
      }
    } else {
      parseEquation(command);
      computePoints(curr);
      draw();
    }
  } while (command.length() > 0);
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
  writeToScreen("y = " + equation, 1, 4 + graphed);
#ifdef TG_DEBUG
  parser.printOPs(ops);
#endif
}

double TGraph::simulateEquation(double x) {
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
      case OP::MAGIC: {
        double a = nums.top();
        nums.pop();
        if (a > 0) {
          nums.push(INT_MAX);
        } else {
          // INTEGRAL(e^-a^2t) = -1/a^2
          nums.push(1 / std::pow(a, 2));
        }
        break;
      }
      case OP::BUILTIN: {
        double a = nums.top();
        nums.pop();
        nums.push((*ops[++i].fnptr)(a));
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
    int x = (i - screenWidth / 2) * stepX;
    int y = round(screenHeight / 2 - simulateEquation(x) / stepY);
    if (y > 0 && y < screenHeight) {
      screen[y][i] = symbol;
    }
  }
}

// Getters and Setters

int TGraph::getGraphed() const {
  return graphed;
}