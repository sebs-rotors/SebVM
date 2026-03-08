//
//  main.cpp
//  FedoraVM
//
//  Created by Sebastian Sidor on 3/7/26.
//

#include <QtWidgets/QApplication>
#include "setupWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    
    SetupWindow window;
    window.onLaunch = [](const std::string& path, int cpuCount, int memoryGB) {
        // TODO: save config and start VM
    };
    window.show();
    
    return app.exec();
}
