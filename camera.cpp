#include "camera.h"

Camera::~Camera()
{
}

void Camera::runSlot()
{
    // TODO: clean up. Would be nice not to have nested `if` statements
 //   if (!videoCapture_ or !usingVideoCamera_)
 //   {
      //  if (usingVideoCamera_)
            usingVideoCamera_ = true;
            videoCapture_.reset(new cv::VideoCapture(cameraIndex_));
            timer_.start(0, this);

      //  else
      //      videoCapture_.reset(new cv::VideoCapture(videoFileName_));
  //  }
    if (videoCapture_->isOpened())
    {
        timer_.start(0, this);
     //   emit started();
    }
}

void Camera::stopped()
{
    timer_.stop();
}

void Camera::timerEvent(QTimerEvent *ev)
{
    cv::Mat frame;
    if (!videoCapture_->read(frame)) // Blocks until a new frame is ready
    {
        return;
    }
    if (usingVideoCamera_) // Blocks until a new frame is ready
    {
        cv::Mat grey_image;
        cv::cvtColor(frame, grey_image, CV_BGR2GRAY);
        cv::equalizeHist(grey_image, grey_image);
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
        const QImage image((const unsigned char*)frame.data, frame.cols, frame.rows, frame.step,
                           QImage::Format_RGB888, &matDeleter, new cv::Mat(frame));
        image.rgbSwapped();
        Q_ASSERT(image.constBits() == frame.data);
        emit image_signal(image);
    }
 }

cv::Mat Camera::getFrame()
{
    cv::Mat frame;
    videoCapture_->read(frame);
    return frame;
}

void Camera::matDeleter(void *mat)
{
    delete static_cast<cv::Mat*>(mat);
}

void Camera::usingVideoCameraSlot(bool value)
{
    usingVideoCamera_ = value;
}

void Camera::cameraIndexSlot(int index)
{
    cameraIndex_ = index;
}

void Camera::videoFileNameSlot(QString fileName)
{
    videoFileName_ = fileName.toStdString().c_str();
}
