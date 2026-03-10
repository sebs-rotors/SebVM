#include "settingsWindow.hpp"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

SettingsWindow::SettingsWindow(const VMConfig& config, QWidget* parent) : QWidget(parent) {
    setWindowTitle("SebVM - Settings");
    setMinimumWidth(450);
    setAttribute(Qt::WA_DeleteOnClose);
    
    auto* mainLayout = new QVBoxLayout(this);
    
    form = new VMConfigForm(config, this);
    mainLayout->addWidget(form);
    
    auto* note = new QLabel("Changes apply on next boot.");
    note->setStyleSheet("color: gray; font-size: 11px;");
    mainLayout->addWidget(note);
    
    auto* saveButton = new QPushButton("Save");
    mainLayout->addWidget(saveButton);
    
    connect(saveButton, &QPushButton::clicked, this, [this]() {
        if (onSave) onSave(form->values());
        close();
    });
}
