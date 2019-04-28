#ifndef DETECTTHREAD_H
#define DETECTTHREAD_H
#include <QThread>
#include <QProcess>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utility.hpp>
#include <vector>
#include <iostream>

class DetectThread : public QThread
{
Q_OBJECT

public:

    DetectThread(){viewResult = false; dollars = 0;}
    DetectThread(std::string samplePath,std::string detectPath,cv::Mat frame);
    int stopDetectThread();
    void setViewResult(bool flag){viewResult = flag;}
    void setTrainPath(std::string path){samplePath = path;}
    void setTestPath(std::string path){detectPath = path;}
    void setCameraData(cv::Mat frame){cameraData = frame;}
    int getResult(){return dollars;}

private:

    std::string samplePath;
    std::string detectPath;
    cv::Mat cameraData;
    bool viewResult;
    int dollars;
signals:
    void result(int);
    void serial(QString);
protected:
    void run();
};

#endif // DETECTTHREAD_H
