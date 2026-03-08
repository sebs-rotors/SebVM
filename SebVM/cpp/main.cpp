//
//  main.cpp
//  FedoraVM
//
//  Created by Sebastian Sidor on 3/7/26.
//

#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    
    QWidget window;
    window.setWindowTitle("SebVM");
    window.resize(400, 300);
    
    QLabel label("Qt is working!", &window);
    label.move(150, 150);
    
    window.show();
    return app.exec();
}
