#include "lagrangeinterpolation.h"

LagrangeInterpolation::LagrangeInterpolation(QObject *parent) : QObject(parent),
    multiplicativeSum(0),
    res_Y(0)
{
    ln.clear();
}

qreal LagrangeInterpolation::legrange_get_ln(qreal inDataArray[][2], qint32 dataQty, qreal x)
{
    for(qint32 dotCnt=0; dotCnt<dataQty; dotCnt++){//how many multiplay times
        for(qint32 j=0; j<=dataQty; j++){
            if(j != dotCnt){
                multiplicativeSum *= (x-inDataArray[j][0])/(inDataArray[dotCnt][0] - inDataArray[j][0]);
            }
        }
    }
    return multiplicativeSum;
}

qreal LagrangeInterpolation::legrange_get_y(qreal inDataArray[][2], qint32 dataQty, QVector<qreal> lnx)
{
     for(qint32 dotCnt=0; dotCnt<dataQty; dotCnt++){
         res_Y += inDataArray[dotCnt][1]*lnx[dotCnt];
     }
     return res_Y;
}

