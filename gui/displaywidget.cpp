#include "gui/displaywidget.h"

DisplayWidget::DisplayWidget(QWidget *parent) : QWidget(parent)
{
    //---------------------------------------UI Design-------------------------------
    QHBoxLayout* mainLayout = new QHBoxLayout();
    QVBoxLayout* subLayout = new QVBoxLayout();

    image_viewer_ = new ImageViewer(this);
    image_viewer_->setFixedSize(640,480);

    QLabel* label = new QLabel("Train Folder:        ");
    trainPathEdit = new QLineEdit();
    btn_train = new QPushButton("...");
    QHBoxLayout* layout1 = new QHBoxLayout();
    layout1->addWidget(label);
    layout1->addWidget(trainPathEdit);
    layout1->addWidget(btn_train);
    subLayout->addLayout(layout1);

    QStringList cameraOptions;
    cameraOptions << "0" << "1" << "2" << "3" << "4" << "5" << "6";
    QComboBox* cameraComboBox = new QComboBox;
    cameraComboBox->addItems(cameraOptions);
    radio_camera = new QRadioButton("From Camera:");
    QHBoxLayout* layout2 = new QHBoxLayout();
    layout2->addWidget(radio_camera);
    layout2->addWidget(cameraComboBox);
    subLayout->addLayout(layout2);

    radio_folder = new QRadioButton("From Folder:");
    folderPathEdit = new QLineEdit();
    btn_folder = new QPushButton("...");
    QHBoxLayout* layout3 = new QHBoxLayout();
    layout3->addWidget(radio_folder);
    layout3->addWidget(folderPathEdit);
    layout3->addWidget(btn_folder);
    subLayout->addLayout(layout3);

    radio_file = new QRadioButton("From File:    ");
    filePathEdit = new QLineEdit();
    btn_file = new QPushButton("...");
    QHBoxLayout* layout4 = new QHBoxLayout();
    layout4->addWidget(radio_file);
    layout4->addWidget(filePathEdit);
    layout4->addWidget(btn_file);
    subLayout->addLayout(layout4);

    btn_detect = new QPushButton("Check");
    QFont font = btn_detect->font();
    font.setPointSize(20);
    btn_detect->setFont(font);
    QHBoxLayout* layout7 = new QHBoxLayout();
    layout7->addWidget(btn_detect);
    subLayout->addLayout(layout7);

    QLabel* label1 = new QLabel("Serial Number: ");
    serialNum = new QLineEdit();
    QHBoxLayout* layout6 = new QHBoxLayout();
    layout6->addWidget(label1);
    layout6->addWidget(serialNum);
    subLayout->addLayout(layout6);

    QLabel* label2 = new QLabel("Dollars:              ");
    resultView = new QLineEdit("0");
    QHBoxLayout* layout5 = new QHBoxLayout();
    layout5->addWidget(label2);
    layout5->addWidget(resultView);
    subLayout->addLayout(layout5);

    mainLayout->addWidget(image_viewer_);
    mainLayout->addLayout(subLayout);
    this->setLayout(mainLayout);

    QPushButton *runButton = new QPushButton("run", this);
    runButton->setVisible(false);

//---------------------------------------UI Design-------------------------------
    camera_ = new Camera();
    cameraThread_.start();
    camera_->moveToThread(&cameraThread_);

    cashDetector_ = new DetectThread();
    std::string currentPath;
    currentPath=QDir::currentPath().toLocal8Bit().constData();
    cashDetector_->setTrainPath(currentPath + "/images/train");
    cashDetector_->setTestPath( currentPath + "/images/test");
    trainPathEdit->setText(QDir::currentPath() + "/images/train");
    folderPathEdit->setText(QDir::currentPath() + "/images/test");

    // TODO: Add in slot to turn off camera_, or something
    QObject::connect(cashDetector_,SIGNAL(result(int)),
                     this,SLOT(result(int)));

    QObject::connect(cashDetector_,SIGNAL(serial(QString)),
                     this,SLOT(serial(QString)));

    image_viewer_->connect(camera_,
                           SIGNAL(image_signal(QImage)),
                           SLOT(set_image(QImage)));

    QObject::connect(btn_detect, SIGNAL(clicked()),
                     this, SLOT(detectSlot()));

    QObject::connect(runButton, SIGNAL(clicked()),
                     camera_, SLOT(runSlot()));

    QObject::connect(cameraComboBox, SIGNAL(currentIndexChanged(int)),
                     camera_, SLOT(cameraIndexSlot(int)));

    QObject::connect(btn_file, SIGNAL(clicked()),
                     this,	SLOT(filePathSelect()));

    QObject::connect(btn_folder, SIGNAL(clicked()),
                     this,	SLOT(folderPathSelect()));

    QObject::connect(btn_train, SIGNAL(clicked()),
                     this,	SLOT(trainPathSelect()));

    QObject::connect(radio_camera, SIGNAL(toggled(bool)),
                     camera_, SLOT(usingVideoCameraSlot(bool)));

    QObject::connect(this, SIGNAL(videoFileNameSignal(QString)),
                     camera_, SLOT(videoFileNameSlot(QString)));

    emit runButton->clicked();

    radio_camera->setChecked(true);

}

DisplayWidget::~DisplayWidget()
{
    camera_->~Camera();
    cameraThread_.quit();
    cameraThread_.wait();
}
void DisplayWidget::serial(QString serial)
{
    serialNum->setText(serial);
}
void DisplayWidget::result(int result)
{
    resultView->setText(QString::number(result));
}
void DisplayWidget::filePathSelect()
{
   QString filename = QFileDialog::getOpenFileName(this, tr("Select image file to test."), "", tr("Images (*.png *.jfif *.jpg)"));
   radio_file->setChecked(true);

   if(filename!="")
   {
       emit camera_->usingVideoCameraSlot(false);
       emit videoFileNameSignal(filename);
       QImage img;
       img.load(filename);
       image_viewer_->set_image(img);
       cashDetector_->setTestPath(filename.toStdString());
       filePathEdit->setText(filename);
   }
   qDebug() << "test file"<<filename;

}
void DisplayWidget::trainPathSelect()
{
    QString selectPath = QFileDialog::getExistingDirectory(this, tr("Select train data directory."));

    if (selectPath !="") {
        cashDetector_->setTrainPath(selectPath.toStdString());
        trainPathEdit->setText(selectPath);
    }
   qDebug() << "train path:"<<selectPath;
}
void DisplayWidget::folderPathSelect()
{
    radio_folder->setChecked(true);
    QString selectPath = QFileDialog::getExistingDirectory(this, tr("Select test data directory."));

    if (selectPath !="") {
        emit camera_->usingVideoCameraSlot(false);
        cashDetector_->setTestPath(selectPath.toStdString());
        folderPathEdit->setText(selectPath);
    }
   qDebug() << "folder path:"<<selectPath;
}

void DisplayWidget::detectSlot()
{
    resultView->setText("");
    serialNum->setText("");

    if (radio_camera->isChecked()) cashDetector_->setTestPath("camera.jpg");
    if (radio_file->isChecked()) {
        if (filePathEdit->text()=="") return;
        cashDetector_->setTestPath(filePathEdit->text().toStdString());
    }
    if (radio_folder->isChecked()) {
        if (folderPathEdit->text()=="") return;
        cashDetector_->setTestPath(folderPathEdit->text().toStdString());
    }

    cashDetector_->start();

}
