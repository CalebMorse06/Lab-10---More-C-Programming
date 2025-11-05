//=========================================================
// Name: Caleb Morse
// EECS 348 – Lab 10: More C++ Programming
// Date: November 04, 2025
// Description: Header file containing function declarations for
// validating double format strings and performing manual string-based
// arithmetic operations.
// Contributors: Code written and verified by Caleb Morse
//               with support from ChatGPT and Cursor.
//=========================================================

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>

// Validates if a string represents a valid double number
// Format: optional +/- sign, digits, optional decimal point and digits
bool isValidDouble(const std::string& str);

// Adds two valid string doubles manually (string arithmetic only)
// Returns the sum as a string
std::string addStrings(const std::string& num1, const std::string& num2);

// Removes leading zeros from a number string (normalizes input/output)
std::string trimLeadingZeros(const std::string& str);

// Opens file, reads pairs of numbers, validates and adds them
void processFile(const std::string& filename);

#endif // FUNCTIONS_H

