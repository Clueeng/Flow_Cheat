#include <windows.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>

// Set to true if you also want to log to a file
constexpr bool ENABLE_FILE_LOGGING = true;
template<typename T>
std::string to_string_custom(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

void debugLog(const std::string& message) {
    // Timestamped prefix
    std::ostringstream fullMessage;
    std::time_t now = std::time(nullptr);
    std::tm localTime;
    localtime_s(&localTime, &now);
    fullMessage << "[" << std::put_time(&localTime, "%H:%M:%S") << "] " << message << "\n";

    // Output to debugger
    OutputDebugStringA(fullMessage.str().c_str());

    // Optional: also log to file
    if (ENABLE_FILE_LOGGING) {
        std::ofstream logFile("C:\\Users\\flaily\\AppData\\Roaming\\Flow\\debug.log", std::ios::app);
        if (logFile.is_open()) {
            logFile << fullMessage.str();
            logFile.close();
        }
    }
}