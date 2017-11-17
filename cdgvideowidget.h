#ifndef CDGVIDEOWIDGET_H
#define CDGVIDEOWIDGET_H

#include "cdgvideosurface.h"
#ifdef USE_GL
    #include <QGLWidget>
#else
    #include <QWidget>
#endif
#include "settings.h"


#ifdef USE_GL
class CdgVideoWidget : public QGLWidget
#else
class CdgVideoWidget : public QWidget
#endif
{
    Q_OBJECT
public:
    explicit CdgVideoWidget(QWidget *parent = 0);
    ~CdgVideoWidget();
    CdgVideoSurface *videoSurface() const { return surface; }
    QSize sizeHint() const;
    void clear();
private:
    CdgVideoSurface *surface;

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

signals:
    void resized(QSize size);

};

#endif // CDGVIDEOWIDGET_H
