#include "lagrangeinterpolation.h"
#include <QPoint>

LagrangeInterpolation::LagrangeInterpolation(QObject *parent) : QObject(parent),
    res_Y(0),
    gDotCnt(0)
{
    ln.clear();
    lagrangeDots.clear();
    for(int n=0; n<DEF_MAX_DOTS_QTY_OF_BEZIER_CRUVE; n++){
        testRealPoints[n][0] = -512;
        testRealPoints[n][1] = -512;
    }
}

void LagrangeInterpolation::legrange_get_ln(qreal inDataArray[][2], qint32 dataQty, qreal x)
{
    qreal tmpLn=1;

    if(dataQty<3){
        qDebug("too less dots!");
        return;
    }

    ln.clear();
    for(qint32 dotCnt=0; dotCnt<dataQty; dotCnt++){//how many multiplay times
        for(qint32 j=0; j<dataQty; j++){
            if(j != dotCnt){
                tmpLn = tmpLn * (x-inDataArray[j][0])/(inDataArray[dotCnt][0] - inDataArray[j][0]);
                //qDebug("[%s]%d: dotQty: %d, inDataArray[%d][0]:%.2f, inDataArray[%d][0]:%.2f", __FUNCTION__, __LINE__,
                //       dataQty, j, inDataArray[j][0], j, inDataArray[j][1]);
            }

        }
        //qDebug("tmpLn:%.2f", tmpLn);
        ln.append(tmpLn);//store in the ln vector;
        tmpLn = 1;
    }
}

qreal LagrangeInterpolation::legrange_get_y(qreal inDataArray[][2], QVector<qreal> lnx)
{
    res_Y = 0;
    qint32 dotCnt=0;
    if(lnx.size()){
        foreach (qreal ln, lnx) {
            res_Y += inDataArray[dotCnt][1]*ln;
            //qDebug("[%s]%d: res_Y:%.02f, inDataArray[dotCnt][1]:%.02f, ln:%.2f", __FUNCTION__, __LINE__, res_Y, inDataArray[dotCnt][1], ln);
            dotCnt++;
        }
    }

     return res_Y;
}

void LagrangeInterpolation::slot_get_lagrange_mouse_lbtn_pos(QPoint lbtnPpos)
{
    MY_POINT tmpMYPOINT;
    testRealPoints[gDotCnt][0] = static_cast<qreal>(lbtnPpos.x());
    testRealPoints[gDotCnt][1] = static_cast<qreal>(lbtnPpos.y());

    if(++gDotCnt>=DEF_MAX_DOTS_QTY_OF_BEZIER_CRUVE){
        gDotCnt=0;
    }

    lagrangeDots.clear();
    if(gDotCnt>2){
        for(qint32 x_cnt=0; x_cnt<lbtnPpos.x(); x_cnt++)
        {
            for(qint32 n=0; n<gDotCnt; n++){
                if(testRealPoints[n][0] != x_cnt){
                    legrange_get_ln(testRealPoints, gDotCnt, x_cnt);
                    tmpMYPOINT.y = legrange_get_y(testRealPoints, ln);
                    tmpMYPOINT.x = x_cnt;
                    //qDebug("tmpMYPOINT.x:%.2f, tmpMYPOINT.y:%.2f",tmpMYPOINT.x, tmpMYPOINT.y);
                    lagrangeDots.append(tmpMYPOINT);
                    break;
                }

            }
        }

        qDebug("lagrange dot size:%d", lagrangeDots.size());
        emit signal_lagrange_send_points(lagrangeDots);
    }



}

void LagrangeInterpolation::slot_clear_all_dots()
{
    gDotCnt = 0;
    lagrangeDots.clear();
}

