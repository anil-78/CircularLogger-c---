#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <filesystem>
#include <nlohmann/json.hpp> // For JSON handling

class CircularLogger {
public:
    CircularLogger(const std::string& configFile);
    void log(const std::string& message);
    void configure(const std::string& configFile);
    
private:
    void createLogFile();
    void deleteOldestLogFile();
    std::string getCurrentTimestamp();
    void loadConfig(const std::string& configFile);

    int maxLogFiles;
    int logFrequency; // in days  
    std::string logDirectory;
    std::vector<std::string> logFiles;
};
void CircularLogger::loadConfig(const std::string& configFile) {
    std::ifstream file(configFile);
    nlohmann::json config;
    file >> config;

    maxLogFiles = config["maxLogFiles"];
    logFrequency = config["logFrequency"];
    logDirectory = config["logDirectory"];
}
void CircularLogger::log(const std::string& message) {
    createLogFile();
    std::string timestamp = getCurrentTimestamp();
    std::ofstream logFile(logDirectory + "/" + timestamp + ".log", std::ios_base::app);
    logFile << message << std::endl;
    logFile.close();
}
void CircularLogger::createLogFile() {
    // Check the number of log files and delete the oldest if necessary  
    if (logFiles.size() >= maxLogFiles) {
        deleteOldestLogFile();
    }
    
    // Create a new log file and add it to the list  
    std::string timestamp = getCurrentTimestamp();
    logFiles.push_back(logDirectory + "/" + timestamp + ".log");
}
std::string CircularLogger::getCurrentTimestamp() {
    std::time_t now = std::time(nullptr);
    std::tm* utcTime = std::gmtime(&now);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H-%M-%SZ", utcTime);
    return std::string(buffer);
}
void CircularLogger::deleteOldestLogFile() {
    std::filesystem::remove(logFiles.front());
    logFiles.erase(logFiles.begin());
}
int main() {
    CircularLogger logger("config.json");
    logger.log("This is a log message.");
    return ;
}
{
    "maxLogFiles": 7,
    "logFrequency": 1,
    "logDirectory": "./logs"
}