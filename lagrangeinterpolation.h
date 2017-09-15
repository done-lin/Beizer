#ifndef LAGRANGEINTERPOLATION_H
#define LAGRANGEINTERPOLATION_H

#include <QObject>
#include <QVector>
#include "commondata.h"

class LagrangeInterpolation : public QObject
{
    Q_OBJECT
public:
    explicit LagrangeInterpolation(QObject *parent = 0);

    void vlagrange_interpolation();
    void legrange_get_ln(qreal inDataArray[][2], qint32 dataQty, qreal x);
    qreal legrange_get_y(qreal inDataArray[][2], QVector<qreal> lnx);
    qreal testRealPoints[DEF_MAX_DOTS_QTY_OF_BEZIER_CRUVE][2];
    qint32 gDotCnt;

private:

    QVector<qreal> ln;
    QVector<MY_POINT> lagrangeDots;
    qreal res_Y;//final function value: y

signals:
    void signal_lagrange_send_points(QVector<MY_POINT> lagrangeDots);
public slots:
    void slot_get_lagrange_mouse_lbtn_pos(QPoint lbtnPpos);
};

#endif // LAGRANGEINTERPOLATION_H
