//=========================================================
// Name: Caleb Morse
// EECS 348 – Lab 10: More C++ Programming
// Date: November 04, 2025
// Description: Handles file I/O and driver logic. Prompts user for
// input filename and calls processFile to validate and add number pairs.
// Contributors: Code written and verified by Caleb Morse
//               with support from ChatGPT and Cursor.
//=========================================================

#include "functions.h"
#include <iostream>
#include <string>

int main() {
    std::string filename;
    
    // Prompt user for input filename
    std::cout << "Enter input filename: ";
    std::getline(std::cin, filename);
    
    // Process the file
    processFile(filename);
    
    return 0;
}

