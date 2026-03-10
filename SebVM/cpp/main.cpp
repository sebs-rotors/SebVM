//
//  main.cpp
//  FedoraVM
//
//  Created by Sebastian Sidor on 3/7/26.
//

#include <QtWidgets/QApplication>
#include <QtWidgets/QMenu>
#include <QtWidgets/QSystemTrayIcon>
#include <QtWidgets/QStyle>
#include "setupWindow.hpp"
#include "settingsWindow.hpp"
#include "vmconfig.hpp"
#include "vmbridge_api.h"

static void setupTrayIcon() {
    auto* tray = new QSystemTrayIcon();
    tray->setIcon(QApplication::style()->standardIcon(QStyle::SP_ComputerIcon));
    tray->setToolTip("SebVM");
    
    auto* menu = new QMenu();
    auto* settingsAction = menu->addAction("Settings");
    menu->addSeparator();
    auto* quitAction = menu->addAction("Quit SebVM");
    
    QObject::connect(settingsAction, &QAction::triggered, []() {
        VMConfig config = loadConfig();
        auto* sw = new SettingsWindow(config);
        sw->onSave = [](const VMConfig& updated) {
            saveConfig(updated);
        };
        sw->show();
    });
    
    QObject::connect(quitAction, &QAction::triggered, []() {
        QApplication::quit();
    });
    
    tray->setContextMenu(menu);
    tray->show();
}


int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("SebVM");
    app.setOrganizationName("SebVM");
    app.setQuitOnLastWindowClosed(false);
    
    if (configExists()) {
        VMConfig config = loadConfig();
        startVM(config.diskPath.c_str(), config.cpuCount, config.memoryGB);
        setupTrayIcon();
    } else {
        SetupWindow* window = new SetupWindow();
        window->onLaunch = [window](const std::string& path, int cpuCount, int memoryGB) {
            VMConfig config;
            config.diskPath = path;
            config.cpuCount = cpuCount;
            config.memoryGB = memoryGB;
            saveConfig(config);
            window->close();
            startVM(config.diskPath.c_str(), config.cpuCount, config.memoryGB);
            setupTrayIcon();
        };
        window->show();
    }
    
    return app.exec();
}
