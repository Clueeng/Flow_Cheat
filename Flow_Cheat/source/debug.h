#pragma once

#include <windows.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>

template<typename T>
std::string to_string_custom(const T& value);
void debugLog(const std::string& message);