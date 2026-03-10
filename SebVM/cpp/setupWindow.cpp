#include "setupWindow.hpp"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QMessageBox>
#include <QtCore/QFileInfo>

#define CPUCOUNT_DEFAULT 2 // max determined by max hardware concurrency value
#define MEMSIZE_DEFAULT 4

SetupWindow::SetupWindow(QWidget* parent) : QWidget(parent) {
    setWindowTitle("SebVM - Setup");
    setMinimumWidth(500);
    
    auto* mainLayout = new QVBoxLayout(this);
    
    VMConfig defaults;
    defaults.cpuCount = CPUCOUNT_DEFAULT;
    defaults.memoryGB = MEMSIZE_DEFAULT;
    
    form = new VMConfigForm(defaults, this);
    mainLayout->addWidget(form);
    
    // Launch button
    auto* launchButton = new QPushButton("Launch VM");
    mainLayout->addWidget(launchButton);
    
    connect(launchButton, &QPushButton::clicked, this, [this]() {
        if (!validateInputs()) return;
        if (onLaunch) {
            VMConfig config = form->values();
            onLaunch(config.diskPath, config.cpuCount, config.memoryGB);
        }
    });
}

bool SetupWindow::validateInputs() {
    QString path = QString::fromStdString(form->values().diskPath);
    
    if (path.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please select a disk image.");
        return false;
    }
    
    QFileInfo fileInfo(path);
    if (!fileInfo.exists()) {
        QMessageBox::warning(this, "Validation Error", "File does not exist.");
        return false;
    }
    
    if (fileInfo.suffix().toLower() != "raw") {
        QMessageBox::warning(this, "Validation Error", "Please select a .raw disk image.");
        return false;
    }
    
    return true;
}
