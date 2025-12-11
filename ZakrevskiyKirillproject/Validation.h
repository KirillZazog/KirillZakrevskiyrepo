#pragma once
#include <string>
#include <iostream>
#include <limits>

float getValidFloat(const std::string& prompt,
    float min = -std::numeric_limits<float>::infinity(),
    float max = std::numeric_limits<float>::infinity());

int getValidInt(const std::string& prompt,
    int min = std::numeric_limits<int>::min(),
    int max = std::numeric_limits<int>::max());

std::string getValidString(const std::string& prompt, bool allowEmpty = false);

bool getValidBool(const std::string& prompt);