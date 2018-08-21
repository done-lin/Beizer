#include "renderarea.h"
#include <QPainter>
#include "qevent.h"
#include <QLabel>

//! [0]
RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent)
{
    antialiased = false;
    transformed = false;

    drawData.clear();
    mousePos.clear();
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}
//! [0]

//! [1]
QSize RenderArea::minimumSizeHint() const
{
    return QSize(100, 100);
}
//! [1]

//! [2]
QSize RenderArea::sizeHint() const
{
    return QSize(400, 200);
}
//! [2]

//! [3]
void RenderArea::setShape(Shape shape)
{
    this->shape = shape;
    update();
}
//! [3]

//! [4]
void RenderArea::setPen(const QPen &pen)
{
    this->pen = pen;
    update();
}
//! [4]

//! [5]
void RenderArea::setBrush(const QBrush &brush)
{
    this->brush = brush;
    update();
}
//! [5]

//! [6]
void RenderArea::setAntialiased(bool antialiased)
{
    this->antialiased = antialiased;
    update();
}
//! [6]

//! [7]
void RenderArea::setTransformed(bool transformed)
{
    this->transformed = transformed;
    update();
}

void RenderArea::getDotData(QVector<MY_POINT> cruveDots)
{
    //qDebug("IN curveDots szie:%d", cruveDots.size());
    drawData = cruveDots;
    update();
}

void RenderArea::get_lbtn_pos(QPoint pos)
{
    //qDebug("get mouse pos!");
    mousePos.append(pos);
    update();
}

void RenderArea::get_desktop_geometry(QRect rect)
{
    this->setGeometry(rect);
    update();
}

void RenderArea::clear_all_lines()
{
    mousePos.clear();
    drawData.clear();
    update();
}



void RenderArea::paintEvent(QPaintEvent * /* event */)
{
    QString tmpQstring;
    QPainter painter(this);
    painter.window();
    painter.setPen(pen);
    painter.setBrush(brush);
    if (antialiased)
        painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin));
    //painter.drawLine(0,0,200,180);
    if(mousePos.size()){
        qint32 cnt=0;
        foreach (QPoint tmpPoint, mousePos) {
            painter.drawRect(tmpPoint.x()-2, tmpPoint.y()-2, 4, 4);
            tmpQstring.sprintf("p%d:(%d,%d)", cnt++, tmpPoint.x(), tmpPoint.y());
            painter.drawText(tmpPoint.x()+3, tmpPoint.y()+3, 220, 50,Qt::AlignHCenter|Qt::AlignVCenter, tmpQstring);
            tmpQstring.clear();
        }
    }

    //painter.drawLine(258, 350, 600, 480);

    //qDebug("drawdata size: %d", drawData.size());
    if(drawData.size()){

        foreach (MY_POINT p, drawData) {
        painter.drawLine(p.x, p.y, p.x, p.y+1);

        }
    }


}

