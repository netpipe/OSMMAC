#include "gui/mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    DisplayWidget* display = new DisplayWidget(this);
    setCentralWidget(display);
    this->setStyleSheet("color: silver; "
                        "background-color: #302F2F; "
                        "selection-background-color:#3d8ec9;"
                        "selection-color: black;"
                        "background-clip: border;"
                        "border-image: none;"
                        "outline: 0;");
}

MainWindow::~MainWindow()
{
}

