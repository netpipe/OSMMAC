#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QWidget>

#include <QObject>
#include <QVBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QLineEdit>
#include <QComboBox>
#include <QFileDialog>
#include <QThread>
#include <QMessageBox>
#include <QGroupBox>
#include <QLabel>
#include <iostream>
#include <stdio.h>

#include "gui/imageviewer.h"
#include "camera.h"
#include "DetectThread.h"

class DisplayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DisplayWidget(QWidget *parent = 0);
    ~DisplayWidget();

signals:
    void videoFileNameSignal(QString videoFileName);
    void facecascade_name_signal(QString filename);

public slots:
    void filePathSelect();
    void trainPathSelect();
    void folderPathSelect();
    void detectSlot();
    void result(int);

private:
    Camera* camera_;
    DetectThread* cashDetector_;
    QThread cameraThread_;

    ImageViewer* image_viewer_;
    QLineEdit* trainPathEdit, *folderPathEdit, *filePathEdit, *resultView;
    QPushButton* btn_train, *btn_folder, *btn_file, *btn_detect;
    QRadioButton* radio_camera, *radio_folder, *radio_file;

};

#endif // DISPLAYWIDGET_H
