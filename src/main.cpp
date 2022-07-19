
/**
* @file main.cpp
* @author Devin Arena
* @brief Contains implementation logic for Terminal Graphing program.
* @since 7/19/2022
**/

#include<iostream>

int main(int argc, char** argv) {
    for (int i = 0; i < argc; i++) {
        std::cout << argv[i] << "\n";
    }
    return 0;
}