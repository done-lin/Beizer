#ifndef BEZIERCRUVE_H
#define BEZIERCRUVE_H

#include <QObject>
#include <QVector>
#include <commondata.h>


class BezierCruve : public QObject
{
    Q_OBJECT
public:
    explicit BezierCruve(QObject *parent = 0);

public:
     void bezier_draw_cruve(MY_POINT *pInPoints);
     void bezier_set_point_interval(qint32 pix);
     void bezier_set_dot_qty(qint32 dot);
     void bezier_set_max_dot_qty(qint32 maxDot);
     QVector<MY_POINT> cruveDots;
     MY_POINT testPoint[DEF_MAX_DOTS_QTY_OF_BEZIER_CRUVE];
     qint32 mouseDotCnt;//鼠标捕获的点数，从1开始，捕获一个点就是1
     qint32 CurveKeepOneLineFlag;
     void bezier_add_point(QPoint lbtnPpos);

signals:
    void signal_send_points(QVector<MY_POINT> cruveDots);
private:
    qint32 dotQty;
    qint32 dotInterval;
    qint32 maxDotQty;
public slots:
};

#endif // BEZIERCRUVE_H
