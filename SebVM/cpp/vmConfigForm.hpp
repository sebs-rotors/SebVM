#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>
#include "vmconfig.hpp"

class VMConfigForm : public QWidget {
public:
    explicit VMConfigForm(const VMConfig& initial, QWidget* parent = nullptr);
    VMConfig values() const;
    
private:
    QLineEdit* imagePathEdit;
    QSpinBox* cpuCountSpinBox;
    QSpinBox* memorySpinBox;
};
