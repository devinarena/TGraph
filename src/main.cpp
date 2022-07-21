
/**
 * @file tgraph.cpp
 * @author Devin Arena
 * @brief Contains implementation logic for Terminal Graphing program.
 * @since 7/19/2022
 **/

#include <string.h>
#include <iostream>
#include <string>

#include "../include/tgraph.h"

#define VERSION_MAJOR 1
#define VERSION_MINOR 0

int main(int argc, char** argv) {
  if (argc == 1) {
    TGraph tG;
  } else {
    std::string equation{""};
    for (int i = 1; i < argc; i++) {
      if (strcmp(argv[i], "-info") == 0) {
        std::cout << "TGraph v" << VERSION_MAJOR << "." << VERSION_MINOR
                  << "\n";
        std::cout << "Created by Devin Arena\n";
        std::cout
            << "Simply run the program with no arguments to get started.\n";
        continue;
      }
      equation.append(argv[i]);
      equation.append(" ");
    }
    TGraph tG;
    tG.parseEquation(equation);
    tG.computePoints();
    tG.draw();
  }
  return 0;
}