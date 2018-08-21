#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "renderarea.h"
#include "qgridlayout.h"
#include "qrect.h"
#include "beziercruve.h"
#include "qvector.h"
#include "qdebug.h"
#include "qevent.h"
#include "qdesktopwidget.h"
#include "qtabwidget.h"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    fixPosY(0)
    //mouseDotCnt(0)
{

    //QGridLayout *mainLayout = new QGridLayout(this);//把renderarea画图区域加入gridlayout
    pMyRenderArea = new RenderArea(this);
    pMyLagrangeRenderArea = new RenderArea(this);
    pMyBezierCruve = new BezierCruve(this);
    pMyLagrangeInterpolation = new LagrangeInterpolation(this);

    QRect deskRect = get_desktop_geometry();
    qDebug("desktop info, w:%d, h:%d", deskRect.width(), deskRect.height());
    this->setFixedSize(deskRect.width(), deskRect.height());
    pMyRenderArea->setFixedSize(deskRect.width(), deskRect.height());
    ui->setupUi(this);

    tabbarRects.clear();

    ui->label->setGeometry(deskRect.width()/2-ui->label->width()/2, (deskRect.height()/2-ui->label->height()/2)/2, ui->label->width()*1.5, ui->label->height()*1.5);
    ui->label_2->setGeometry(deskRect.width()/2-ui->label_2->width()/2, (deskRect.height()/2-ui->label_2->height()/2)*1.2, ui->label_2->width()*1.5, ui->label_2->height()*1.5);
    ui->label->setWordWrap(true);
    ui->label_2->setWordWrap(true);

    ui->tabWidget->addTab(pMyRenderArea, tr("BezierRender"));
    ui->tabWidget->addTab(pMyLagrangeRenderArea, tr("legrange"));

    ui->tabWidget->setGeometry(0,0,deskRect.width(), deskRect.height());
    ui->tabWidget->setWindowState(Qt::WindowMaximized);

////////////////////////////////////////////////
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->currentWidget()->setAttribute(Qt::WA_TransparentForMouseEvents);
///////////////////////////////////////////////

    ui->tabWidget->setCurrentIndex(1);//qt tab widget用法，qt widget切换和widget添加按钮，tabwidget添加控件
    ui->tabWidget->currentWidget()->setAttribute(Qt::WA_AcceptTouchEvents, true);
////////////////////////////////////////////////

    clearBtn = new QPushButton(ui->tabWidget->currentWidget());
    clearBtn->setGeometry(deskRect.width()/7*6, deskRect.height()/16-5, deskRect.width()/7-10, deskRect.height()/16);
    clearBtn->setText("clear");
    clearBtn->setAttribute(Qt::WA_AcceptTouchEvents, true);

    repaintBtn = new QPushButton(ui->tabWidget->currentWidget());
    repaintBtn->setGeometry(deskRect.width()/7*6, deskRect.height()/16*2.1-5, deskRect.width()/7-10, deskRect.height()/16);
    repaintBtn->setText("set oneline");
    repaintBtn->setStyleSheet("background-color:#00FF00");
    repaintBtn->setAttribute(Qt::WA_AcceptTouchEvents, true);
////////////////////////////////////////////////////
    ui->tabWidget->setCurrentIndex(2);
    ui->tabWidget->currentWidget()->setAttribute(Qt::WA_AcceptTouchEvents, true);
    //ui->tabWidget->currentWidget()->setAttribute(Qt::WA_TransparentForMouseEvents);
///////////////////////////////////////////////////
    LagrangeClearBtn = new QPushButton(ui->tabWidget->currentWidget());
    LagrangeClearBtn->setGeometry(deskRect.width()/7*6, deskRect.height()/16-5, deskRect.width()/7-10, deskRect.height()/16);
    LagrangeClearBtn->setText("Clear");
    LagrangeClearBtn->setAttribute(Qt::WA_AcceptTouchEvents, true);


////////////////////////////////////////////
    ui->tabWidget->setAttribute(Qt::WA_AcceptTouchEvents, true);
    ui->tabWidget->tabBar()->setAttribute(Qt::WA_AcceptTouchEvents, true);

///////////////////////////////////////////

    this->setAttribute(Qt::WA_AcceptTouchEvents, true);//允许qt接受触屏事件，可操作触屏。

//==================connect signals to slots=======================//
    connect(pMyBezierCruve, SIGNAL(signal_send_points(QVector<MY_POINT>)), pMyRenderArea, SLOT(getDotData(QVector<MY_POINT>)));
    connect(pMyLagrangeInterpolation, SIGNAL(signal_lagrange_send_points(QVector<MY_POINT>)), pMyLagrangeRenderArea, SLOT(getDotData(QVector<MY_POINT>)));

    connect(this, SIGNAL(signal_mouse_lbtn_pos(QPoint)), pMyRenderArea, SLOT(get_lbtn_pos(QPoint)));
    connect(this, SIGNAL(signal_mouse_lbtn_pos_lagrange(QPoint)), pMyLagrangeRenderArea, SLOT(get_lbtn_pos(QPoint)));

    connect(this, SIGNAL(signal_mouse_lbtn_pos(QPoint)), this, SLOT(slot_draw_bezier(QPoint)));
    connect(this, SIGNAL(signal_set_desktop_geometry(QRect)), pMyRenderArea, SLOT(get_desktop_geometry(QRect)));
    connect(this, SIGNAL(signal_clear_all_lines()), pMyRenderArea, SLOT(clear_all_lines()));
    connect(clearBtn, SIGNAL(released()), this, SLOT(on_clearButton_clicked()));
    connect(repaintBtn, SIGNAL(released()), this, SLOT(on_one_line_button_clicked()));
    connect(this, SIGNAL(signal_mouse_lbtn_pos_lagrange(QPoint)), pMyLagrangeInterpolation, SLOT(slot_get_lagrange_mouse_lbtn_pos(QPoint)));
    connect(LagrangeClearBtn, SIGNAL(released()), pMyLagrangeInterpolation, SLOT(slot_clear_all_dots()));
    connect(LagrangeClearBtn, SIGNAL(released()), pMyLagrangeRenderArea, SLOT(clear_all_lines()));
//==============================================================//
    tabbarRects.append(get_tabbar_geometry(0));
    tabbarRects.append(get_tabbar_geometry(1));
    tabbarRects.append(get_tabbar_geometry(2));
    fixPosY = tabbarRects[0].height();//fix the coordinate y!
    //qDebug("fixPosY:%d", ui->tabWidget->tabBar()->height());
    foreach (QRect tmpDebugRect, tabbarRects) {
       qDebug("tabbar Rects geometry: x:%d, y:%d, w:%d, h:%d", tmpDebugRect.x(),tmpDebugRect.x(),
              tmpDebugRect.width(),tmpDebugRect.height());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

QRect MainWindow::get_desktop_geometry()
{
    QDesktopWidget *dwsktopwidget = QApplication::desktop();
    QRect deskrect = dwsktopwidget->availableGeometry();
    return deskrect;
}

QRect MainWindow::get_tabbar_geometry(int tabbarIndex)
{
    int x,y,w,h;
    QRect tmpRect;

    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->tabBar()->setEnabled(true);
    ui->tabWidget->tabBar()->tabRect(tabbarIndex).getRect(&x, &y, &w, &h);
    qDebug("Tabbar:%d, Rect:(x:%d, y:%d, w:%d, h:%d)",x, y, w, h);

    tmpRect.setX(x); tmpRect.setY(y); tmpRect.setWidth(w); tmpRect.setHeight(h);
    return tmpRect;
}

bool MainWindow::judge_pos_in_button_or_not(int x, int y, QPushButton *btn)
{
    if(   ((x > btn->pos().x()) && x < (btn->pos().x() + btn->width())) &&
            ((y > btn->pos().y()) && y < (btn->pos().y() + btn->height()))   ){
        return true;
    }else{
        return false;
    }
    return false;
}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
    //QString tmpDebugString;

    if(event->button() ==Qt::LeftButton){
        //qDebug("mouse left button down.");
        //tmpDebugString.clear();
        //tmpDebugString+="mouse left btn!";
        //clearBtn->setText(tmpDebugString);

        if(ui->tabWidget->currentIndex() == 0) return;

        //QPoint point05(0, 10);
        QPoint lbtnPpos = event->pos();
        lbtnPpos.setY(lbtnPpos.y() - fixPosY);
        //qDebug("y:%d", );
        //lbtnPpos = mapToGlobal(ui->tabWidget->pos());
        //lbtnPpos = mapFrom(ui->tabWidget->currentWidget(), point05);
        qDebug("left button posX: %d, pos Y:%d", lbtnPpos.x(), lbtnPpos.y());

        switch (ui->tabWidget->currentIndex()) {

        case 1:
            emit signal_mouse_lbtn_pos(lbtnPpos);
            //qDebug("mouseDotCnt: %d", pMyBezierCruve->mouseDotCnt);
            break;

        case 2:
            emit signal_mouse_lbtn_pos_lagrange(lbtnPpos);
            break;

        case 3:

            break;

        default:
            break;
        }

    }else if(event->button()==Qt::RightButton){
        qDebug("mouse right button down.");
    }else if(event->button()==Qt::MidButton){
        qDebug("mouse mid button down.");
    }
}

bool MainWindow::event(QEvent *e)
{
    switch (e->type()){
    case QEvent::TouchBegin:
        return true;
    case QEvent::TouchUpdate:
        return true;
    case QEvent::TouchEnd:
    {

        QTouchEvent *te = static_cast<QTouchEvent *>(e);
        QList<QTouchEvent::TouchPoint> touchPointsList = te->touchPoints();
        qDebug("touch points cnt:%d p0PosX: %f, p0Pos Y:%f", touchPointsList.count(),
               touchPointsList.first().pos().x(), touchPointsList.first().pos().y());

        if(touchPointsList.last().pos().x() < (tabbarRects[0].x()+tabbarRects[0].width()) &&
                touchPointsList.last().pos().y() < tabbarRects[0].height()){
            ui->tabWidget->setCurrentIndex(0);
            return true;
        }else if(touchPointsList.last().pos().x() < (tabbarRects[1].x()+tabbarRects[1].width()) &&
                 touchPointsList.last().pos().y() < tabbarRects[1].height()){
            ui->tabWidget->setCurrentIndex(1);
            return true;
        }else if(touchPointsList.last().pos().x() < (tabbarRects[2].x()+tabbarRects[2].width()) &&
                 touchPointsList.last().pos().y() < tabbarRects[2].height()){
            ui->tabWidget->setCurrentIndex(2);
            return true;
        }

        if(ui->tabWidget->currentIndex() == 0) return false;

        QPoint lbtnPpos;
        lbtnPpos.setX(static_cast<int>(touchPointsList.last().pos().x()));
        lbtnPpos.setY(static_cast<int>(touchPointsList.last().pos().y()-fixPosY));
        switch (ui->tabWidget->currentIndex()) {

        case 1:
            if(judge_pos_in_button_or_not(lbtnPpos.x(), lbtnPpos.y(), clearBtn)){
                emit clearBtn->released();
                return true;
            }
            if(judge_pos_in_button_or_not(lbtnPpos.x(), lbtnPpos.y(), repaintBtn)){
                emit repaintBtn->released();
                return true;
            }
            emit signal_mouse_lbtn_pos(lbtnPpos);
            //qDebug("mouseDotCnt: %d", pMyBezierCruve->mouseDotCnt);
            break;

        case 2:
            if(judge_pos_in_button_or_not(lbtnPpos.x(), lbtnPpos.y(), LagrangeClearBtn)){
                emit LagrangeClearBtn->released();
                return true;
            }

            emit signal_mouse_lbtn_pos_lagrange(lbtnPpos);
            break;

        case 3:

            break;

        default:
            break;
        }

        //pMyBezierCruve->mouseDotCnt++;
        //if(pMyBezierCruve->mouseDotCnt>DEF_MAX_DOTS_QTY_OF_BEZIER_CRUVE){
        //    pMyBezierCruve->mouseDotCnt = 0;
        //}
        //pMyBezierCruve->testPoint[pMyBezierCruve->mouseDotCnt-1].x = static_cast<double>(touchPointsList.last().pos().x());
        //pMyBezierCruve->testPoint[pMyBezierCruve->mouseDotCnt-1].y = static_cast<double>(touchPointsList.last().pos().y());
        //QPoint tmpPoint(static_cast<int>(touchPointsList.last().pos().x()),
        //                static_cast<int>(touchPointsList.last().pos().y()));
        //emit signal_mouse_lbtn_pos(tmpPoint);
        //qDebug("touchPointDotCnt: %d", pMyBezierCruve->mouseDotCnt);

        return true;
    }
    default:
        break;

    }
    return QMainWindow::event(e);
}


//bug here
void MainWindow::slot_draw_bezier(QPoint lbtnPpos)
{
    pMyBezierCruve->bezier_add_point(lbtnPpos);//增加贝塞尔曲线的点数
    if(pMyBezierCruve->mouseDotCnt>2){//鼠标捕获3点才开始画线
        MY_POINT* tmpTestPoint = new MY_POINT[pMyBezierCruve->mouseDotCnt];//新分配n个点的测试点给贝塞尔曲线
        memcpy(tmpTestPoint, pMyBezierCruve->testPoint, sizeof(MY_POINT)*pMyBezierCruve->mouseDotCnt);//必须复制，否则永远画的是最后两点的线
        pMyBezierCruve->bezier_set_dot_qty(pMyBezierCruve->mouseDotCnt-1);//一定要实际点数减1
        pMyBezierCruve->bezier_draw_cruve(tmpTestPoint);//核心函数，正式递归画点，画好的点存放在cruveDots
        emit pMyBezierCruve->signal_send_points(pMyBezierCruve->cruveDots);//当画图完毕，pMyBezierCruve->cruveDots已经产生了
        if(pMyBezierCruve->CurveKeepOneLineFlag){
            pMyBezierCruve->cruveDots.clear();
        }
        delete []tmpTestPoint;
    }

}

void MainWindow::on_clearButton_clicked()
{
    pMyBezierCruve->mouseDotCnt = 0;
    pMyBezierCruve->cruveDots.clear();
    emit signal_clear_all_lines();
}

void MainWindow::on_one_line_button_clicked()
{
    pMyBezierCruve->CurveKeepOneLineFlag = !pMyBezierCruve->CurveKeepOneLineFlag;
    if(pMyBezierCruve->CurveKeepOneLineFlag)
        repaintBtn->setStyleSheet("background-color:#FF0000");
    else
        repaintBtn->setStyleSheet("background-color:#00FF00");
}

