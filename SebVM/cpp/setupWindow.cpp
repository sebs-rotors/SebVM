//
//  setupWindow.cpp
//  FedoraVM
//
//  Created by Sebastian Sidor on 3/7/26.
//

#include "setupWindow.hpp"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <thread>

#define CPUCOUNT_DEFAULT 2 // max determined by max hardware concurrency value
#define MEMSIZE_DEFAULT 4
#define MEMSIZE_MAX 64

SetupWindow::SetupWindow(QWidget* parent) : QWidget(parent) {
    setWindowTitle("SebVM - Setup");
    setMinimumWidth(500);
    
    auto* mainLayout = new QVBoxLayout(this);
    auto* formLayout = new QFormLayout();
    
    // Disk image path input
    auto* pathRow = new QHBoxLayout();
    imagePathEdit = new QLineEdit();
    imagePathEdit->setPlaceholderText("Path to Fedora .raw image...");
    auto* browseButton = new QPushButton("Browse");
    pathRow->addWidget(imagePathEdit);
    pathRow->addWidget(browseButton);
    formLayout->addRow("Disk Image:", pathRow);
    
    // CPU core count selection
    cpuCountSpinBox = new QSpinBox();
    cpuCountSpinBox->setRange(1, (int)std::thread::hardware_concurrency());
    cpuCountSpinBox->setValue(CPUCOUNT_DEFAULT);
    formLayout->addRow("CPU Cores:", cpuCountSpinBox);
    
    // Memory size selection
    memorySpinBox = new QSpinBox();
    memorySpinBox->setRange(1, MEMSIZE_MAX);
    memorySpinBox->setValue(MEMSIZE_DEFAULT);
    memorySpinBox->setSuffix(" GB");
    formLayout->addRow("Memory:", memorySpinBox);
    
    mainLayout->addLayout(formLayout);
    
    // Launch button
    auto* launchButton = new QPushButton("Launch VM");
    mainLayout->addWidget(launchButton);
    
    // Completely ignoring MOC so I dont suffer that badly (for now)
    connect(browseButton, &QPushButton::clicked, this ,[this]() {
        QString path = QFileDialog::getOpenFileName(
            this,
            "Select Fedora Disk Image",
            QDir::homePath(),
            "Raw Disk Images (*.raw);;All Files (*)",
            nullptr,
            QFileDialog::DontUseNativeDialog
        );
        if (!path.isEmpty()) imagePathEdit->setText(path);
    });
    
    connect(launchButton, &QPushButton::clicked, this, [this]() {
        if (!validateInputs()) return;
        if (onLaunch) {
            onLaunch(
                imagePathEdit->text().trimmed().toStdString(),
                cpuCountSpinBox->value(),
                memorySpinBox->value()
            );
        }
    });
}

bool SetupWindow::validateInputs() {
    QString path = imagePathEdit->text().trimmed();
    
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
