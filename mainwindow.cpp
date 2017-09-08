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
    mouseDotCnt(0)
{

    QGridLayout *mainLayout = new QGridLayout(this);//把renderarea画图区域加入gridlayout
    pMyRenderArea = new RenderArea(this);
    pMylegrange = new RenderArea(this);
    pMyBezierCruve = new BezierCruve(this);

    QRect deskRect = get_desktop_geometry();
    qDebug("desktop info, w:%d, h:%d", deskRect.width(), deskRect.height());
    this->setFixedSize(deskRect.width(), deskRect.height());
    pMyRenderArea->setFixedSize(deskRect.width(), deskRect.height());
    ui->setupUi(this);


    ui->tabWidget->addTab(pMyRenderArea, tr("BezierRender"));
    ui->tabWidget->addTab(pMylegrange, tr("legrange"));

    ui->tabWidget->setGeometry(0,0,deskRect.width(), deskRect.height());
    ui->tabWidget->setWindowState(Qt::WindowMaximized);


    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->currentWidget()->setAttribute(Qt::WA_TransparentForMouseEvents);

    ui->tabWidget->setCurrentIndex(1);//qt tab widget用法，qt widget切换和widget添加按钮，tabwidget添加控件
    clearBtn = new QPushButton(ui->tabWidget->currentWidget());
    clearBtn->setGeometry(deskRect.width()/7*6, deskRect.height()/16, deskRect.width()/7-10, deskRect.height()/16);
    clearBtn->setText("clear");

    //mainLayout->addWidget(ui->pushButton, 5, 5);
    //mainLayout->setColumnStretch(5,1);
    //mainLayout->setColumnMinimumWidth(5, 100);
    //ui->tabWidget->currentWidget()->setLayout(mainLayout);
    //ui->tabWidget->setLayout(mainLayout);

    //ui->pushButton->setGeometry(deskRect.width()/7*6, deskRect.height()/16,
    //                            deskRect.width()/7-10, deskRect.height()/16);

    this->setAttribute(Qt::WA_AcceptTouchEvents, true);//允许qt接受触屏事件，可操作触屏。
    acceptDrops();

    connect(pMyBezierCruve, SIGNAL(signal_send_points(QVector<MY_POINT>)), pMyRenderArea, SLOT(getDotData(QVector<MY_POINT>)));
    connect(this, SIGNAL(signal_mouse_lbtn_pos(QPoint)), pMyRenderArea, SLOT(get_lbtn_pos(QPoint)));
    connect(this, SIGNAL(signal_mouse_lbtn_pos(QPoint)), this, SLOT(slot_draw_bezier(QPoint)));
    connect(this, SIGNAL(signal_set_desktop_geometry(QRect)), pMyRenderArea, SLOT(get_desktop_geometry(QRect)));
    connect(this, SIGNAL(signal_clear_all_lines()), pMyRenderArea, SLOT(clear_all_lines()));
    connect(clearBtn, SIGNAL(clicked(bool)), this, SLOT(on_clearButton_clicked()));

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
        qDebug("鼠标 左 键按下");
        //tmpDebugString.clear();
        //tmpDebugString+="mouse left btn!";
        //clearBtn->setText(tmpDebugString);

        if(ui->tabWidget->currentIndex() == 0) return;

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
    //QString tmpDebugString("no touch!");

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
        //tmpDebugString.clear();
        //tmpDebugString+="touch points!";
        //clearBtn->setText(tmpDebugString);
        mouseDotCnt++;
        if(mouseDotCnt>DEF_MAX_DOTS_QTY_OF_BEZIER_CRUVE){
            mouseDotCnt = 0;
        }
        testPoint[mouseDotCnt-1].x = static_cast<double>(touchPointsList.last().pos().x());
        testPoint[mouseDotCnt-1].y = static_cast<double>(touchPointsList.last().pos().y());
        QPoint tmpPoint(static_cast<int>(touchPointsList.last().pos().x()),
                        static_cast<int>(touchPointsList.last().pos().y()));
        emit signal_mouse_lbtn_pos(tmpPoint);
        qDebug("touchPointDotCnt: %d", mouseDotCnt);

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

void MainWindow::on_clearButton_clicked()
{
    mouseDotCnt = 0;
    pMyBezierCruve->cruveDots.clear();
    emit signal_clear_all_lines();
}
