#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QObject>
#include <QWidget>
#include <qvector.h>
#include <QPen>
#include "renderarea.h"
#include "commondata.h"
#include <QLabel>


class RenderArea : public QWidget
{
    Q_OBJECT

public:


    enum Shape { Line, Points, Polyline, Polygon, Rect, RoundedRect, Ellipse, Arc,
                 Chord, Pie, Path, Text, Pixmap };

    RenderArea(QWidget *parent = 0);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
    QVector <QPoint> mousePos;


public slots:
    void setShape(Shape shape);
    void setPen(const QPen &pen);
    void setBrush(const QBrush &brush);
    void setAntialiased(bool antialiased);
    void setTransformed(bool transformed);
    void getDotData(QVector<MY_POINT> cruveDots);
    void get_lbtn_pos(QPoint pos);
    void get_desktop_geometry(QRect rect);
    void clear_all_lines(void);


protected:
    void paintEvent(QPaintEvent *event) override;
private:

    Shape shape;
    QPen pen;
    QBrush brush;
    bool antialiased;
    bool transformed;
    QPixmap pixmap;
    QVector<MY_POINT> drawData;

};

#endif // RENDERAREA_H
