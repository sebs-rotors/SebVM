//
//  main.cpp
//  FedoraVM
//
//  Created by Sebastian Sidor on 3/7/26.
//

#include <QtWidgets/QApplication>
#include "setupWindow.hpp"
#include "vmconfig.hpp"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    
    if (configExists()) {
        VMConfig config = loadConfig();
    } else {
        SetupWindow* window = new SetupWindow();
        window->onLaunch = [window](const std::string& path, int cpuCount, int memoryGB) {
            VMConfig config;
            config.diskPath = path;
            config.cpuCount = cpuCount;
            config.memoryGB = memoryGB;
            saveConfig(config);
            window->close();
            // TODO: start VM via bridging
        };
        window->show();
    }
    
    return app.exec();
}
