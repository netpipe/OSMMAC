#include "gui/imageviewer.h"

void ImageViewer::set_image(const QImage &img)
{
    if (img.width() > width())
    {
        image_ = img.scaled(width(), height(), Qt::KeepAspectRatio);
    }
    else
    {
        image_ = img;
    }
    update();

}
