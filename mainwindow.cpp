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
    ui(new Ui::MainWindow)
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

    ui->label->setGeometry(deskRect.width()/2-ui->label->width()/2, (deskRect.height()/2-ui->label->height()/2)/2, ui->label->width(), ui->label->height());
    ui->label_2->setGeometry(deskRect.width()/2-ui->label_2->width()/2, (deskRect.height()/2-ui->label_2->height()/2)*1.2, ui->label_2->width(), ui->label_2->height());


    ui->tabWidget->addTab(pMyRenderArea, tr("BezierRender"));
    ui->tabWidget->addTab(pMyLagrangeRenderArea, tr("legrange"));

    ui->tabWidget->setGeometry(0,0,deskRect.width(), deskRect.height());
    ui->tabWidget->setWindowState(Qt::WindowMaximized);


    ui->tabWidget->setCurrentIndex(0);
    //ui->tabWidget->currentWidget()->setAttribute(Qt::WA_TransparentForMouseEvents);

    ui->tabWidget->setCurrentIndex(1);//qt tab widget用法，qt widget切换和widget添加按钮，tabwidget添加控件
    ui->tabWidget->setAttribute(Qt::WA_AcceptTouchEvents, true);

    clearBtn = new QPushButton(ui->tabWidget->currentWidget());
    clearBtn->setGeometry(deskRect.width()/7*6, deskRect.height()/16-5, deskRect.width()/7-10, deskRect.height()/16);
    clearBtn->setText("clear");


    repaintBtn = new QPushButton(ui->tabWidget->currentWidget());
    repaintBtn->setGeometry(deskRect.width()/7*6, deskRect.height()/16*2.1-5, deskRect.width()/7-10, deskRect.height()/16);
    repaintBtn->setText("set oneline");
    repaintBtn->setStyleSheet("background-color:#00FF00");


    this->setAttribute(Qt::WA_AcceptTouchEvents, true);//允许qt接受触屏事件，可操作触屏。
    //acceptDrops();

    connect(pMyBezierCruve, SIGNAL(signal_send_points(QVector<MY_POINT>)), pMyRenderArea, SLOT(getDotData(QVector<MY_POINT>)));
    connect(pMyLagrangeInterpolation, SIGNAL(signal_lagrange_send_points(QVector<MY_POINT>)), pMyLagrangeRenderArea, SLOT(getDotData(QVector<MY_POINT>)));

    connect(this, SIGNAL(signal_mouse_lbtn_pos(QPoint)), pMyRenderArea, SLOT(get_lbtn_pos(QPoint)));
    connect(this, SIGNAL(signal_mouse_lbtn_pos_lagrange(QPoint)), pMyLagrangeRenderArea, SLOT(get_lbtn_pos(QPoint)));

    connect(this, SIGNAL(signal_mouse_lbtn_pos(QPoint)), this, SLOT(slot_draw_bezier(QPoint)));
    connect(this, SIGNAL(signal_set_desktop_geometry(QRect)), pMyRenderArea, SLOT(get_desktop_geometry(QRect)));
    connect(this, SIGNAL(signal_clear_all_lines()), pMyRenderArea, SLOT(clear_all_lines()));
    connect(clearBtn, SIGNAL(clicked(bool)), this, SLOT(on_clearButton_clicked()));
    connect(repaintBtn, SIGNAL(clicked(bool)), this, SLOT(on_one_line_button_clicked()));
    connect(this, SIGNAL(signal_mouse_lbtn_pos_lagrange(QPoint)), pMyLagrangeInterpolation, SLOT(slot_get_lagrange_mouse_lbtn_pos(QPoint)));

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
    //QString tmpDebugString;

    if(event->button() ==Qt::LeftButton){
        //qDebug("mouse left button down.");
        //tmpDebugString.clear();
        //tmpDebugString+="mouse left btn!";
        //clearBtn->setText(tmpDebugString);

        if(ui->tabWidget->currentIndex() == 0) return;

        QPoint lbtnPpos = event->pos();
        //qDebug("left button posX: %d, pos Y:%d", lbtnPpos.x(), lbtnPpos.y());

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
        if(ui->tabWidget->currentIndex() == 0) return false;
        QTouchEvent *te = static_cast<QTouchEvent *>(e);
        QList<QTouchEvent::TouchPoint> touchPointsList = te->touchPoints();
        qDebug("touch points cnt:%d p0PosX: %f, p0Pos Y:%f", touchPointsList.count(),
               touchPointsList.first().pos().x(), touchPointsList.first().pos().y());

        pMyBezierCruve->mouseDotCnt++;
        if(pMyBezierCruve->mouseDotCnt>DEF_MAX_DOTS_QTY_OF_BEZIER_CRUVE){
            pMyBezierCruve->mouseDotCnt = 0;
        }
        pMyBezierCruve->testPoint[pMyBezierCruve->mouseDotCnt-1].x = static_cast<double>(touchPointsList.last().pos().x());
        pMyBezierCruve->testPoint[pMyBezierCruve->mouseDotCnt-1].y = static_cast<double>(touchPointsList.last().pos().y());
        QPoint tmpPoint(static_cast<int>(touchPointsList.last().pos().x()),
                        static_cast<int>(touchPointsList.last().pos().y()));
        emit signal_mouse_lbtn_pos(tmpPoint);
        qDebug("touchPointDotCnt: %d", pMyBezierCruve->mouseDotCnt);

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

