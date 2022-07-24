
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
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>

TGraph::TGraph() {
  setupWindow();
}

/**
 * @brief Sets up the window. Grabs the window size, initializes the screen, and
 * draws a blank graph.
 */
void TGraph::setupWindow() {
// get the window size based on platform
// windows
#ifdef TG_WINDOWS
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  screenWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
  screenHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#endif
// macOS and Linux
#ifdef TG_LINUX
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  screenWidth = w.ws_col;
  screenHeight = w.ws_row;
#endif

  screen = std::vector(screenHeight, std::vector(screenWidth, ' '));

  stepX = 1.0;
  stepY = 1.0;
  ops.clear();
  equations.clear();

  rerender();
}

/**
 * @brief Helper for writing text to the screen 2d array.
 *
 * @param text std::string The text to write.
 * @param x int The x coordinate to write the text to.
 * @param y int The y coordinate to write the text to.
 */
void TGraph::writeToScreen(std::string text, int x, int y) {
  for (size_t i = 0; i < text.length(); i++) {
    screen[y][x + i] = text[i];
  }
}

/**
 * @brief CLI interface.
 */
void TGraph::cli() {
  std::string command{""};
  do {
    std::cout << "Equation or command > (f(x) = ) ";
    std::getline(std::cin, command);
    parseInput(command);
  } while (command.length() > 0);
}

/**
 * @brief Compute the points of a specified equation.
 *
 * @param equation int The index of the equation to compute.
 */
void TGraph::computePoints(int equation) {
  char symbol = 'a' + (23 + equation) % 26;
  for (int i = 0; i < screenWidth; i++) {
    double x = (i - screenWidth / 2) * stepX;
    int y = round(screenHeight / 2 - simulateEquation(x, equation) / stepY);
    if (y > 0 && y < screenHeight) {
      screen[y][i] = symbol;
    }
  }
  writeToScreen("f(x) = " + equations[equation], 1, 5 + equation);
}

/**
 * @brief Draws the graph to the screen.
 */
void TGraph::draw(std::ostream& stream) {
  stream << std::endl;
  for (int j = 0; j < screenHeight; j++) {
    for (int i = 0; i < screenWidth; i++) {
      if (screen[j][i] != ' ') {
        stream << screen[j][i];
      } else {
        int x = i - screenWidth / 2;
        int y = screenHeight / 2 - j;
        if (x == 0 && y == 0)
          stream << "+";
        else if (x == -1 && y == -1)
          stream << "O";
        else if (x == 0)
          stream << "|";
        else if (y == 0)
          stream << "-";
        else
          stream << " ";
      }
    }
    stream << "\n";
  }
}

/**
 * @brief Re-renders the 2d screen array (for when zooming happens, etc.)
 */
void TGraph::rerender() {
#ifdef TG_WINDOWS
  system("cls");
#endif
#ifdef TG_LINUX
  system("clear");
#endif
  screen = std::vector(screenHeight, std::vector(screenWidth, ' '));

  writeToScreen("TGraph v" + std::to_string(VERSION_MAJOR) + "." +
                    std::to_string(VERSION_MINOR),
                1, 1);
  writeToScreen("x-step: " + std::to_string(stepX), 1, 2);
  writeToScreen("y-step: " + std::to_string(stepY), 1, 3);
  writeToScreen("Equations:", 1, 4);

  for (size_t i = 0; i < ops.size(); i++) {
    computePoints(i);
  }

  draw(std::cout);
}

/**
 * @brief Parses an equation string and adds it to the list of equations.
 *
 * @param equation std::string The equation to add.
 */
void TGraph::parseEquation(std::string& equation) {
  std::vector<Token> tokens = scanner.scan(equation);
  ops.push_back(parser.parse(tokens));
  equations.push_back(equation);
#ifdef TG_DEBUG
  parser.printOPs(ops);
#endif
}

/**
 * @brief Simulates the specified equation.
 *
 * @param x double The x value to simulate the equation with.
 * @param equation int the index of the equation to simulate.
 * @return double The y value of the equation at x.
 */
double TGraph::simulateEquation(double x, int equation) {
#undef CONST
  std::stack<double> nums;
  int start = 0;
  std::vector<Operand>& eqOps = ops[equation];
  for (size_t i = 0; i < eqOps.size(); i++) {
    start = i;
    switch (eqOps[i].opcode) {
      case OP::CONST: {
        nums.push(eqOps[++i].value);
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
        nums.push((*eqOps[++i].fnptr)(a));
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

/**
 * @brief Input for the CLI and command line arguments. Either runs the command
 * or parses an equation.
 *
 * @param input std::string The input to parse.
 */
void TGraph::parseInput(std::string input) {
  std::vector<std::string> tokens;
  size_t start = 0;
  for (size_t i = 0; i < input.length(); i++) {
    if (input[i] == ' ') {
      if (start == i) {
        start = i + 1;
        continue;
      }
      tokens.push_back(input.substr(start, i - start));
      start = i + 1;
    }
  }
  if (start != input.length()) {
    tokens.push_back(input.substr(start, input.length() - start));
  }
  if (tokens.size() == 0)
    return;
  if (tokens[0].compare("help") == 0) {
#ifdef TG_WINDOWS
    system("cls");
#endif
#ifdef TG_LINUX
    system("clear");
#endif
    std::cout << "TGraph v" << VERSION_MAJOR << "." << VERSION_MINOR
              << " by Devin Arena:\n";
    std::cout << "help - displays this help message menu\n";
    std::cout << "graph - re-renders the graph\n";
    std::cout << "clear - clears all equations\n";
    std::cout << "exit - exits the program\n";
    std::cout << "save [file] - save the current output to a file\n";
    std::cout << "xstep [step_size, default=1] - sets the x step size\n";
    std::cout << "ystep [step_size, default=1] - sets the y step size\n";
    std::cout << "'+' - zoom in (xstep /= 2, ystep /= 2)\n";
    std::cout << "'-' - zoom out (xstep *= 2, ystep *= 2)\n";
    std::cout << "Simply enter an equation to graph it.\n";
    std::cout << "\nExamples:\n";
    std::cout << "f(x) = x^2\n";
    std::cout << "f(x) = sin(x)\n";
    std::cout << "f(x) = x*cos(x / 5)\n";
    std::cout << "f(x) = e ^ sqrt(x)\n\n";
  } else if (tokens[0].compare("graph") == 0) {
    rerender();
  } else if (tokens[0].compare("clear") == 0) {
    setupWindow();
  } else if (tokens[0].compare("exit") == 0) {
    exit(0);
  } else if (tokens[0].compare("xstep") == 0) {
    if (tokens.size() == 1) {
      std::cout << "x-step: " << stepX << "\n";
    } else if (tokens.size() == 2) {
      stepX = std::stod(tokens[1]);
      rerender();
    } else {
      std::cout << "Invalid command syntax.\n";
    }
  } else if (tokens[0].compare("ystep") == 0) {
    if (tokens.size() == 1) {
      std::cout << "y-step: " << stepY << "\n";
    } else if (tokens.size() == 2) {
      stepY = std::stod(tokens[1]);
      rerender();
    } else {
      std::cout << "Invalid command syntax.\n";
    }
  } else if (tokens[0].compare("-") == 0) {
    // zoom out
    stepY *= 2;
    stepX *= 2;
    rerender();
  } else if (tokens[0].compare("+") == 0) {
    stepY *= 0.5;
    stepX *= 0.5;
    rerender();
  } else if (tokens[0].compare("save") == 0) {
    if (tokens.size() != 2) {
      std::cout << "Invalid command syntax.\n";
      return;
    }
    std::string filename = tokens[1];
    std::ofstream outfile;
    outfile.open(filename);
    if (!outfile.is_open()) {
      std::cout << "Error opening file.\n";
      return;
    }
    draw(outfile);
    outfile.close();
  } else {
    parseEquation(input);
    computePoints(ops.size() - 1);
#ifdef TG_WINDOWS
    system("cls");
#endif
#ifdef TG_LINUX
    system("clear");
#endif
    draw(std::cout);
  }
}

// Getters and Setters