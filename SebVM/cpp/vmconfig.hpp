#pragma once
#include <string>

struct VMConfig {
    std::string diskPath;
    int cpuCount;
    int memoryGB;
};

bool configExists();
VMConfig loadConfig();
void saveConfig(const VMConfig& config);
