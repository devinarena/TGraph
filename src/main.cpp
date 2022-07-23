
/**
 * @file tgraph.cpp
 * @author Devin Arena
 * @brief Handles starting up TGraph (command line argument parsing, etc.)
 * @since 7/19/2022
 **/

#include <string.h>
#include <iostream>
#include <string>

#include "../include/tgraph.hpp"

/**
 * @brief Parses command line arguments. If given equations, graphs them and
 * shows the resulting graph. Finally opens the interactive terminal program.
 *
 * @param argc int the argument count
 * @param argv char** the argument list
 * @return int exit code for the program
 */
int main(int argc, char** argv) {
  TGraph tG;
  // Parse command line arguments before starting cli if necessary
  if (argc > 1) {
    std::string equation{""};
    for (int i = 1; i < argc; i++) {
      // equations separated by 'and'
      if (strcmp(argv[i], "and") == 0) {
        tG.parseInput(equation);
        equation = "";
        continue;
      }
      // special for info command
      if (strcmp(argv[i], "-info") == 0) {
        std::cout << "TGraph v" << VERSION_MAJOR << "." << VERSION_MINOR
                  << "\n";
        std::cout << "Created by Devin Arena\n";
        std::cout
            << "Simply run the program with no arguments to get started.\n";
        continue;
      }
      // add to the current equation
      equation.append(argv[i]);
      equation.append(" ");
    }
    // if there is an equation left, parse it
    if (equation.length() > 0) {
      tG.parseInput(equation);
    }
  }
  // start the cli
  tG.cli();
  return 0;
}