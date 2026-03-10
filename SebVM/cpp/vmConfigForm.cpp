#include "vmConfigForm.hpp"

#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QFileDialog>
#include <QtCore/QDir>
#include <thread>

#define MEMSIZE_MAX 64

VMConfigForm::VMConfigForm(const VMConfig& initial, QWidget* parent) : QWidget(parent) {
    auto* formLayout = new QFormLayout(this);
    
    // Image path input and browser button
    auto* pathRow = new QHBoxLayout();
    imagePathEdit = new QLineEdit();
    imagePathEdit->setText(QString::fromStdString(initial.diskPath));
    imagePathEdit->setPlaceholderText("Path to Linux .raw image...");
    auto* browseButton = new QPushButton("Browse");
    pathRow->addWidget(imagePathEdit);
    pathRow->addWidget(browseButton);
    formLayout->addRow("Disk Image:", pathRow);
    
    // CPU count selector
    cpuCountSpinBox = new QSpinBox();
    cpuCountSpinBox->setRange(1, (int)std::thread::hardware_concurrency());
    cpuCountSpinBox->setValue(initial.cpuCount);
    formLayout->addRow("CPU Cores:", cpuCountSpinBox);
    
    // Memory size selection
    memorySpinBox = new QSpinBox();
    memorySpinBox->setRange(1, MEMSIZE_MAX);
    memorySpinBox->setValue(initial.memoryGB);
    memorySpinBox->setSuffix(" GB");
    formLayout->addRow("Memory:", memorySpinBox);
    
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
}

VMConfig VMConfigForm::values() const {
    VMConfig config;
    config.diskPath = imagePathEdit->text().trimmed().toStdString();
    config.cpuCount = cpuCountSpinBox->value();
    config.memoryGB = memorySpinBox->value();
    return config;
}
