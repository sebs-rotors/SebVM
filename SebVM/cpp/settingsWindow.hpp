#pragma once

#include <QtWidgets/QWidget>
#include <functional>
#include "vmConfigForm.hpp"

class SettingsWindow : public QWidget {
public:
    explicit SettingsWindow(const VMConfig& config, QWidget* parent = nullptr);
    std::function<void(const VMConfig&)> onSave;
    
private:
    VMConfigForm* form;
};
