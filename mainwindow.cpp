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


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mouseDotCnt(0)
{

    QGridLayout *mainLayout = new QGridLayout(this);//把renderarea画图区域加入gridlayout
    pMyRenderArea = new RenderArea(this);
    //pMyRenderArea->setFixedSize(800,600);//固定大小

    pMyBezierCruve = new BezierCruve(this);
    mainLayout->addWidget(pMyRenderArea);

    QRect deskRect = get_desktop_geometry();
    qDebug("desktop info, w:%d, h:%d", deskRect.width(), deskRect.height());
    this->setFixedSize(deskRect.width(), deskRect.height());
    pMyRenderArea->setFixedSize(deskRect.width(), deskRect.height());
    ui->setupUi(this);
    ui->pushButton->setGeometry(deskRect.width()/7*6, deskRect.height()/16,
                                deskRect.width()/6-10, deskRect.height()/16);

    this->setAttribute(Qt::WA_AcceptTouchEvents, true);//允许qt接受触屏事件，可操作触屏。


    connect(pMyBezierCruve, SIGNAL(signal_send_points(QVector<MY_POINT>)), pMyRenderArea, SLOT(getDotData(QVector<MY_POINT>)));
    connect(this, SIGNAL(signal_mouse_lbtn_pos(QPoint)), pMyRenderArea, SLOT(get_lbtn_pos(QPoint)));
    connect(this, SIGNAL(signal_mouse_lbtn_pos(QPoint)), this, SLOT(slot_draw_bezier(QPoint)));
    connect(this, SIGNAL(signal_set_desktop_geometry(QRect)), pMyRenderArea, SLOT(get_desktop_geometry(QRect)));
    connect(this, SIGNAL(signal_clear_all_lines()), pMyRenderArea, SLOT(clear_all_lines()));
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

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() ==Qt::LeftButton){
        qDebug("鼠标 左 键按下");
        QPoint lbtnPpos = event->pos();
        qDebug("left button posX: %d, pos Y:%d", lbtnPpos.x(), lbtnPpos.y());
        mouseDotCnt++;
        if(mouseDotCnt>DEF_MAX_DOTS_QTY_OF_BEZIER_CRUVE){
            mouseDotCnt = 0;
        }
        testPoint[mouseDotCnt-1].x = lbtnPpos.x();
        testPoint[mouseDotCnt-1].y = lbtnPpos.y();

        emit signal_mouse_lbtn_pos(lbtnPpos);

        qDebug("mouseDotCnt: %d", mouseDotCnt);


    }else if(event->button()==Qt::RightButton){
        qDebug("鼠标 右 键按下");
    }else if(event->button()==Qt::MidButton){
        qDebug("鼠标 中 键按下");
    }
}

bool MainWindow::event(QEvent *e)
{
    switch (e->type()){
    case QEvent::TouchBegin:
        return true;
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd:
    {
        QTouchEvent *te = static_cast<QTouchEvent *>(e);
        qDebug("touch button posX: %f, pos Y:%f", te->touchPoints().last().pos().x(), te->touchPoints().last().pos().y());
        mouseDotCnt++;
        if(mouseDotCnt>DEF_MAX_DOTS_QTY_OF_BEZIER_CRUVE){
            mouseDotCnt = 0;
        }
        testPoint[mouseDotCnt-1].x = static_cast<double>(te->touchPoints().last().pos().x());
        testPoint[mouseDotCnt-1].y = static_cast<double>(te->touchPoints().last().pos().y());
        QPoint tmpPoint(static_cast<int>(te->touchPoints().last().pos().x()),
                        static_cast<int>(te->touchPoints().last().pos().y()));
        emit signal_mouse_lbtn_pos(tmpPoint);
        qDebug("mouseDotCnt: %d", mouseDotCnt);

        return true;
    }
    default:
        break;

    }
    return QMainWindow::event(e);
}

//bug here
void MainWindow::slot_draw_bezier(QPoint)
{
    if(mouseDotCnt>2){//鼠标捕获3点才开始画线
        MY_POINT* tmpTestPoint = new MY_POINT[mouseDotCnt];//新分配n个点的测试点给贝塞尔曲线
        memcpy(tmpTestPoint, testPoint, sizeof(MY_POINT)*mouseDotCnt);
        pMyBezierCruve->bezier_set_dot_qty(mouseDotCnt-1);//一定要实际点数减1
        pMyBezierCruve->bezier_draw_cruve(tmpTestPoint);//核心函数，正式递归画点，画好的点存放在cruveDots
        emit pMyBezierCruve->signal_send_points(pMyBezierCruve->cruveDots);//当画图完毕，pMyBezierCruve->cruveDots已经产生了
        delete []tmpTestPoint;
    }
}

void MainWindow::on_pushButton_clicked()
{
    mouseDotCnt = 0;
    pMyBezierCruve->cruveDots.clear();
    emit signal_clear_all_lines();
}
