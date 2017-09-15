#ifndef LAGRANGEINTERPOLATION_H
#define LAGRANGEINTERPOLATION_H

#include <QObject>
#include <QVector>

class LagrangeInterpolation : public QObject
{
    Q_OBJECT
public:
    explicit LagrangeInterpolation(QObject *parent = 0);

    void vlagrange_interpolation();
    qreal legrange_get_ln(qreal inDataArray[][2], qint32 dataQty, qreal x);
    qreal legrange_get_y(qreal inDataArray[][2], qint32 dataQty, QVector<qreal> lnx);

private:
    qreal multiplicativeSum;
    QVector<qreal> ln;
    qreal res_Y;//final function value: y

signals:

public slots:
};

#endif // LAGRANGEINTERPOLATION_H
