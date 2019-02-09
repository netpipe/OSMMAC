#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QDebug>

class ImageViewer : public QWidget
{
    Q_OBJECT
    QImage image_;

    void paintEvent(QPaintEvent *event)
    {
        QPainter p(this);
        p.eraseRect(0,0,width(),height());
        p.setRenderHint(QPainter::Antialiasing);
        p.drawImage((width()-image_.width())/2, (height()-image_.height())/2, image_);

    }

public:
    explicit ImageViewer(QWidget *parent = 0) : QWidget(parent)
    {
        setAttribute(Qt::WA_OpaquePaintEvent);
    }

signals:

public slots:
    void set_image(const QImage & img);
};

#endif // IMAGEVIEWER_H
