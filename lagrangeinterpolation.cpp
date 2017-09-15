#include "lagrangeinterpolation.h"

LagrangeInterpolation::LagrangeInterpolation(QObject *parent) : QObject(parent),
    multiplicativeSum(0),
    res_Y(0),
    dotCnt(-1)
{
    ln.clear();
    for(int n=0; n<DEF_MAX_DOTS_QTY_OF_BEZIER_CRUVE; n++){
        testRealPoints[n][0] = -512;
        testRealPoints[n][1] = -512;
    }
}

qreal LagrangeInterpolation::legrange_get_ln(qreal inDataArray[][2], qint32 dataQty, qreal x)
{
    for(qint32 dotCnt=0; dotCnt<dataQty; dotCnt++){//how many multiplay times
        for(qint32 j=0; j<=dataQty; j++){
            if(j != dotCnt){
                qDebug("inDataArray[j][0]: %.02f, inDataArray[j][1]: %.02f", inDataArray[j][0],inDataArray[j][1]);
                multiplicativeSum *= (x-inDataArray[j][0])/(inDataArray[dotCnt][0] - inDataArray[j][0]);
                qDebug("[%s]%d: mulitSum:%.02f, dotQty: %d", __FUNCTION__, __LINE__,
                       multiplicativeSum, dataQty);
            }
        }
    }
    return multiplicativeSum;
}

qreal LagrangeInterpolation::legrange_get_y(qreal inDataArray[][2], qint32 dataQty, QVector<qreal> lnx)
{
     for(qint32 dotCnt=0; dotCnt<dataQty; dotCnt++){
         res_Y += inDataArray[dotCnt][1]*lnx[dotCnt];
         qDebug("[%s]%d: res_Y:%.02f", __FUNCTION__, __LINE__, res_Y);
     }
     return res_Y;
}

void LagrangeInterpolation::slot_get_lagrange_mouse_lbtn_pos(QPoint lbtnPpos)
{
    if(++dotCnt>=99){
        dotCnt=0;
    }
    testRealPoints[dotCnt][0] = static_cast<qreal>(lbtnPpos.x());
    testRealPoints[dotCnt][1] = static_cast<qreal>(lbtnPpos.y());
    qDebug("testRealPoints[%d][0]: %.02f, testRealPoints[%d][1]: %.02f,", dotCnt, testRealPoints[dotCnt][0],dotCnt, testRealPoints[dotCnt][1]);
    qDebug("in slot, ln0: %.2f.", legrange_get_ln(testRealPoints, dotCnt+1, 1));
}

