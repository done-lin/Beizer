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
    qreal legrange_get_ln(qreal inDataArray[][2], qint32 dataQty, qreal x);
    qreal legrange_get_y(qreal inDataArray[][2], qint32 dataQty, QVector<qreal> lnx);
    qreal testRealPoints[DEF_MAX_DOTS_QTY_OF_BEZIER_CRUVE][2];
    qint32 dotCnt;

private:
    qreal multiplicativeSum;
    QVector<qreal> ln;
    qreal res_Y;//final function value: y

signals:

public slots:
    void slot_get_lagrange_mouse_lbtn_pos(QPoint lbtnPpos);
};

#endif // LAGRANGEINTERPOLATION_H
