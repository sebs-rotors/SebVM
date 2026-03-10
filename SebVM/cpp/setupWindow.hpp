#pragma once

#include <QtWidgets/QWidget>
#include <functional>
#include "vmConfigForm.hpp"

class SetupWindow : public QWidget {
public:
    explicit SetupWindow(QWidget* parent = nullptr);
    std::function<void(const std::string&, int, int)> onLaunch;
    
private:
    VMConfigForm* form;
    bool validateInputs();
};
