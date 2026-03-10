//
//  settingsWindow.h
//  FedoraVM
//
//  Created by Sebastian Sidor on 3/7/26.
//

#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QPushButton>
#include <functional>

class SetupWindow : public QWidget {
public:
    explicit SetupWindow(QWidget* parent = nullptr);
    std::function<void(const std::string&, int, int)> onLaunch;
    
private:
    QLineEdit* imagePathEdit;
    QSpinBox* cpuCountSpinBox;
    QSpinBox* memorySpinBox;
    
    bool validateInputs();
};
