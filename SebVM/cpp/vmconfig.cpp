#include "vmconfig.hpp"
#include "json.hpp"

#include <QtCore/QStandardPaths>
#include <QtCore/QDir>
#include <fstream>

using json = nlohmann::json;

static std::string configFilePath() {
    QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dir);
    return (dir + "/config.json").toStdString();
}

bool configExists() {
    std::ifstream f(configFilePath());
    return f.good();
}

VMConfig loadConfig() {
    std::ifstream f(configFilePath());
    json j = json::parse(f);
    
    VMConfig config;
    config.diskPath = j["diskPath"].get<std::string>();
    config.cpuCount = j["cpuCount"].get<int>();
    config.memoryGB = j["memoryGB"].get<int>();
    return config;
}

void saveConfig(const VMConfig& config) {
    json j;
    j["diskPath"] = config.diskPath;
    j["cpuCount"] = config.cpuCount;
    j["memoryGB"] = config.memoryGB;
    
    std::ofstream f(configFilePath());
    f << j.dump(4);
}
