#include "beziercruve.h"

BezierCruve::BezierCruve(QObject *parent) : QObject(parent),
    maxDotQty(0),
    dotQty(0),
    dotInterval(1),
    mouseDotCnt(0),
    CurveKeepOneLineFlag(0)
{
    for(int cnt=0; cnt<DEF_MAX_DOTS_QTY_OF_BEZIER_CRUVE; cnt++){
        testPoint[cnt].x=-512;
        testPoint[cnt].y=-512;
    }

    cruveDots.clear();
    dotQty = 0;
    dotInterval = 1;
}

void BezierCruve::bezier_draw_cruve(MY_POINT *pInPoints)
{
    if (dotQty <= 0) return;//point qty invalide;

    if((pInPoints[dotQty].x < pInPoints[0].x+dotInterval) && (pInPoints[dotQty].x > pInPoints[0].x-dotInterval)
            && (pInPoints[dotQty].y < pInPoints[0].y+dotInterval) && (pInPoints[dotQty].y > pInPoints[0].y-dotInterval))//如果第一点的xy坐标和第n点的不重合，不是一个点
    {
        //pDC->SetPixel(pInPoints[0].x, pInPoints[0].y, RGB(255,0,00));//0x0000ff，这里的每一个点都是贝塞尔曲线，pInPoints[0]，每次递归之后，拍p[0]会接近最后一点p[dotQty]
        cruveDots.append(pInPoints[0]);
        return;
    }
    MY_POINT *p1;
    p1 = new MY_POINT[dotQty+1];
    int i, j;
    p1[0] = pInPoints[0];
    for(i=1; i<=dotQty; i++)//第1点到第n点，可以遍历n-1次
    {
        for(j=0; j<=dotQty-i;j++)//从第0点到n-i，意识是每画一点之后，j可以遍历少一点
        {
            //pDC->MoveTo(pInPoints[j].x, pInPoints[j].y);
            //pDC->LineTo(pInPoints[j+1].x, pInPoints[j+1].y);

            pInPoints[j].x = (pInPoints[j].x + pInPoints[j+1].x)/2.0f;//取得x之间的中点
            pInPoints[j].y = (pInPoints[j].y + pInPoints[j+1].y)/2.0f;//取得每相邻的y点之间的中点

            //pDC->SetPixel(pInPoints[i].x, pInPoints[i].y, RGB(255,0,0));
            //pDC->Rectangle(pInPoints[j].x - 2, pInPoints[j].y -2, pInPoints[j].x + 2, pInPoints[j].y + 2);
            //Sleep(2000);
        }//执行完毕，pInPoints[j]变成两个线之间的中点

        p1[i] = pInPoints[0];
    }//每次i和j一起，历遍到没点连线的最后一个中点

    bezier_draw_cruve(pInPoints);
    bezier_draw_cruve(p1);
    delete p1;
    //emit cruveDots;//发送信号和数据,不可以在递归里面发生，否则多次发送
}

void BezierCruve::bezier_set_point_interval(qint32 pix)
{
    dotInterval = pix;
}

void BezierCruve::bezier_set_dot_qty(qint32 dot)
{
    dotQty = dot;
}

void BezierCruve::bezier_set_max_dot_qty(qint32 maxDot)
{
    maxDotQty = maxDot;
}
