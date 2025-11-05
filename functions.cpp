//=========================================================
// Name: Caleb Morse
// EECS 348 – Lab 10: More C++ Programming
// Date: November 04, 2025
// Description: Implementation of helper and arithmetic functions including
// manual validation of double format strings, string-based addition/subtraction,
// leading zero trimming, and file processing for number pairs.
// Contributors: Code written and verified by Caleb Morse
//               with support from ChatGPT and Cursor.
//=========================================================

#include "functions.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>

// Validates if a string represents a valid double number
bool isValidDouble(const std::string& str) {
    if (str.empty()) {
        return false;
    }
    
    std::string s = str;
    size_t start = 0;
    
    // Check for optional sign
    if (s[0] == '+' || s[0] == '-') {
        start = 1;
        if (s.length() == 1) {
            return false; // Only sign, no digits
        }
    }
    
    // Find decimal point
    size_t dotPos = s.find('.');
    
    // Check if there's at least one digit before decimal (if present)
    if (dotPos != std::string::npos) {
        if (dotPos == start) {
            return false; // Decimal immediately after sign or at start
        }
        // Check digits before decimal
        bool hasDigitBefore = false;
        for (size_t i = start; i < dotPos; i++) {
            if (!std::isdigit(s[i])) {
                return false;
            }
            hasDigitBefore = true;
        }
        if (!hasDigitBefore) {
            return false;
        }
        
        // Check digits after decimal
        bool hasDigitAfter = false;
        for (size_t i = dotPos + 1; i < s.length(); i++) {
            if (!std::isdigit(s[i])) {
                return false;
            }
            hasDigitAfter = true;
        }
        // Require at least one digit after decimal point
        if (!hasDigitAfter) {
            return false;
        }
    } else {
        // No decimal point - all characters after sign must be digits
        if (start >= s.length()) {
            return false;
        }
        for (size_t i = start; i < s.length(); i++) {
            if (!std::isdigit(s[i])) {
                return false;
            }
        }
    }
    
    return true;
}

// Removes leading zeros from a number string
std::string trimLeadingZeros(const std::string& str) {
    if (str.empty()) {
        return str;
    }
    
    std::string result = str;
    size_t start = 0;
    bool isNegative = false;
    
    // Preserve sign
    if (result[0] == '+' || result[0] == '-') {
        if (result[0] == '-') {
            isNegative = true;
        }
        start = 1;
        if (result.length() == 1) {
            return result;
        }
    }
    
    // Find first non-zero digit or decimal point
    size_t firstNonZero = start;
    while (firstNonZero < result.length() && 
           result[firstNonZero] == '0' && 
           (firstNonZero + 1 < result.length() && result[firstNonZero + 1] != '.')) {
        firstNonZero++;
    }
    
    // If we removed all digits before decimal, keep at least one zero
    if (firstNonZero < result.length() && result[firstNonZero] == '.') {
        if (firstNonZero > start) {
            firstNonZero--; // Keep one zero before decimal
        }
    }
    
    // If all digits were zeros, keep one zero
    if (firstNonZero >= result.length() || 
        (result[firstNonZero] == '.' && firstNonZero == start)) {
        return "0";
    }
    
    std::string trimmed = result.substr(firstNonZero);
    if (isNegative && trimmed != "0") {
        return "-" + trimmed;
    } else if (!isNegative && result[0] == '+' && trimmed != "0") {
        return "+" + trimmed;
    }
    
    return trimmed;
}

// Helper: Compare two absolute value strings (without signs)
// Returns: -1 if abs1 < abs2, 0 if equal, 1 if abs1 > abs2
int compareAbsolute(const std::string& abs1, const std::string& abs2) {
    size_t dot1 = abs1.find('.');
    size_t dot2 = abs2.find('.');
    
    std::string int1 = (dot1 != std::string::npos) ? abs1.substr(0, dot1) : abs1;
    std::string frac1 = (dot1 != std::string::npos) ? abs1.substr(dot1 + 1) : "";
    std::string int2 = (dot2 != std::string::npos) ? abs2.substr(0, dot2) : abs2;
    std::string frac2 = (dot2 != std::string::npos) ? abs2.substr(dot2 + 1) : "";
    
    // Compare integer parts
    if (int1.length() < int2.length()) {
        return -1;
    } else if (int1.length() > int2.length()) {
        return 1;
    } else {
        // Same length, compare digit by digit
        if (int1 < int2) {
            return -1;
        } else if (int1 > int2) {
            return 1;
        }
    }
    
    // Integer parts equal, compare fractional parts
    size_t maxFrac = std::max(frac1.length(), frac2.length());
    std::string paddedFrac1 = frac1;
    std::string paddedFrac2 = frac2;
    while (paddedFrac1.length() < maxFrac) {
        paddedFrac1 += "0";
    }
    while (paddedFrac2.length() < maxFrac) {
        paddedFrac2 += "0";
    }
    
    if (paddedFrac1 < paddedFrac2) {
        return -1;
    } else if (paddedFrac1 > paddedFrac2) {
        return 1;
    }
    
    return 0;
}

// Helper: Add two absolute value strings (both positive)
std::string addAbsolute(const std::string& abs1, const std::string& abs2) {
    size_t dot1 = abs1.find('.');
    size_t dot2 = abs2.find('.');
    
    std::string int1 = (dot1 != std::string::npos) ? abs1.substr(0, dot1) : abs1;
    std::string frac1 = (dot1 != std::string::npos) ? abs1.substr(dot1 + 1) : "";
    std::string int2 = (dot2 != std::string::npos) ? abs2.substr(0, dot2) : abs2;
    std::string frac2 = (dot2 != std::string::npos) ? abs2.substr(dot2 + 1) : "";
    
    // Make fractional parts same length
    size_t maxFracLen = std::max(frac1.length(), frac2.length());
    while (frac1.length() < maxFracLen) {
        frac1 += "0";
    }
    while (frac2.length() < maxFracLen) {
        frac2 += "0";
    }
    
    std::string resultFrac;
    int carry = 0;
    
    // Add fractional parts (right to left)
    for (int i = maxFracLen - 1; i >= 0; i--) {
        int digit1 = frac1[i] - '0';
        int digit2 = frac2[i] - '0';
        int sum = digit1 + digit2 + carry;
        carry = sum / 10;
        resultFrac = static_cast<char>('0' + (sum % 10)) + resultFrac;
    }
    
    // Add integer parts (right to left)
    size_t maxIntLen = std::max(int1.length(), int2.length());
    std::string paddedInt1 = int1;
    std::string paddedInt2 = int2;
    while (paddedInt1.length() < maxIntLen) {
        paddedInt1 = "0" + paddedInt1;
    }
    while (paddedInt2.length() < maxIntLen) {
        paddedInt2 = "0" + paddedInt2;
    }
    
    std::string resultInt;
    for (int i = maxIntLen - 1; i >= 0; i--) {
        int digit1 = paddedInt1[i] - '0';
        int digit2 = paddedInt2[i] - '0';
        int sum = digit1 + digit2 + carry;
        carry = sum / 10;
        resultInt = static_cast<char>('0' + (sum % 10)) + resultInt;
    }
    
    if (carry > 0) {
        resultInt = "1" + resultInt;
    }
    
    // Combine result
    std::string result = resultInt;
    if (!resultFrac.empty()) {
        // Remove trailing zeros, but preserve max precision if all zeros
        size_t lastNonZero = resultFrac.length();
        while (lastNonZero > 0 && resultFrac[lastNonZero - 1] == '0') {
            lastNonZero--;
        }
        if (lastNonZero == 0) {
            // All zeros - preserve max precision (maxFracLen)
            resultFrac = resultFrac.substr(0, maxFracLen);
        } else {
            resultFrac = resultFrac.substr(0, lastNonZero);
        }
        result += "." + resultFrac;
    }
    
    return result;
}

// Helper: Subtract two absolute value strings (abs1 - abs2, assumes abs1 >= abs2)
std::string subtractAbsolute(const std::string& abs1, const std::string& abs2) {
    size_t dot1 = abs1.find('.');
    size_t dot2 = abs2.find('.');
    
    std::string int1 = (dot1 != std::string::npos) ? abs1.substr(0, dot1) : abs1;
    std::string frac1 = (dot1 != std::string::npos) ? abs1.substr(dot1 + 1) : "";
    std::string int2 = (dot2 != std::string::npos) ? abs2.substr(0, dot2) : abs2;
    std::string frac2 = (dot2 != std::string::npos) ? abs2.substr(dot2 + 1) : "";
    
    // Make fractional parts same length
    size_t maxFracLen = std::max(frac1.length(), frac2.length());
    while (frac1.length() < maxFracLen) {
        frac1 += "0";
    }
    while (frac2.length() < maxFracLen) {
        frac2 += "0";
    }
    
    std::string resultFrac;
    int borrow = 0;
    
    // Subtract fractional parts (right to left)
    for (int i = maxFracLen - 1; i >= 0; i--) {
        int digit1 = frac1[i] - '0';
        int digit2 = frac2[i] - '0';
        int diff = digit1 - digit2 - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        resultFrac = static_cast<char>('0' + diff) + resultFrac;
    }
    
    // Subtract integer parts (right to left)
    size_t maxIntLen = std::max(int1.length(), int2.length());
    std::string paddedInt1 = int1;
    std::string paddedInt2 = int2;
    while (paddedInt1.length() < maxIntLen) {
        paddedInt1 = "0" + paddedInt1;
    }
    while (paddedInt2.length() < maxIntLen) {
        paddedInt2 = "0" + paddedInt2;
    }
    
    std::string resultInt;
    for (int i = maxIntLen - 1; i >= 0; i--) {
        int digit1 = paddedInt1[i] - '0';
        int digit2 = paddedInt2[i] - '0';
        int diff = digit1 - digit2 - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        resultInt = static_cast<char>('0' + diff) + resultInt;
    }
    
    // Combine result
    std::string result = resultInt;
    if (!resultFrac.empty()) {
        // Remove trailing zeros, but preserve max precision if all zeros
        size_t lastNonZero = resultFrac.length();
        while (lastNonZero > 0 && resultFrac[lastNonZero - 1] == '0') {
            lastNonZero--;
        }
        if (lastNonZero == 0) {
            // All zeros - preserve max precision (maxFracLen)
            resultFrac = resultFrac.substr(0, maxFracLen);
        } else {
            resultFrac = resultFrac.substr(0, lastNonZero);
        }
        result += "." + resultFrac;
    }
    
    return result;
}

// Adds two valid string doubles manually
std::string addStrings(const std::string& num1, const std::string& num2) {
    // Extract signs and absolute values
    bool neg1 = (num1[0] == '-');
    bool neg2 = (num2[0] == '-');
    bool pos1 = (num1[0] == '+');
    
    std::string abs1 = num1;
    std::string abs2 = num2;
    
    if (neg1 || pos1) {
        abs1 = abs1.substr(1);
    }
    if (num2[0] == '-' || num2[0] == '+') {
        abs2 = abs2.substr(1);
    }
    
    std::string result;
    bool resultNeg = false;
    bool hasPositiveSign = (num1[0] == '+' || num2[0] == '+');
    
    // Determine operation based on signs
    if (neg1 == neg2) {
        // Same signs: add absolute values
        result = addAbsolute(abs1, abs2);
        resultNeg = neg1;
    } else {
        // Different signs: subtract
        int cmp = compareAbsolute(abs1, abs2);
        if (cmp == 0) {
            result = "0";
            resultNeg = false;
        } else if (cmp > 0) {
            // abs1 > abs2
            result = subtractAbsolute(abs1, abs2);
            resultNeg = neg1;
        } else {
            // abs2 > abs1
            result = subtractAbsolute(abs2, abs1);
            resultNeg = neg2;
        }
    }
    
    // Trim leading zeros
    result = trimLeadingZeros(result);
    
    // Add sign
    if (result == "0") {
        return "0";
    } else if (resultNeg) {
        result = "-" + result;
    } else if (hasPositiveSign && !resultNeg) {
        result = "+" + result;
    }
    
    return result;
}

// Opens file, reads pairs of numbers, validates and adds them
void processFile(const std::string& filename) {
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        // Skip empty lines
        if (line.empty()) {
            continue;
        }
        
        // Find first space or tab separator
        size_t spacePos = line.find(' ');
        if (spacePos == std::string::npos) {
            spacePos = line.find('\t');
        }
        
        if (spacePos == std::string::npos) {
            // Only one number or invalid format
            std::cout << "Invalid number(s): " << line << std::endl;
            continue;
        }
        
        std::string num1 = line.substr(0, spacePos);
        std::string num2 = line.substr(spacePos + 1);
        
        // Trim whitespace
        while (!num1.empty() && std::isspace(num1.back())) {
            num1.pop_back();
        }
        while (!num2.empty() && std::isspace(num2.front())) {
            num2 = num2.substr(1);
        }
        
        // Validate both numbers
        bool valid1 = isValidDouble(num1);
        bool valid2 = isValidDouble(num2);
        
        if (!valid1 || !valid2) {
            std::cout << "Invalid number(s): " << num1 << " " << num2 << std::endl;
        } else {
            // Perform addition
            std::string sum = addStrings(num1, num2);
            std::cout << num1 << " + " << num2 << " = " << sum << std::endl;
        }
    }
    
    file.close();
}

