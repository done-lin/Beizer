#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "renderarea.h"
#include "beziercruve.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    RenderArea *pMyRenderArea;
    BezierCruve *pMyBezierCruve;

    MY_POINT testPoint[DEF_MAX_DOTS_QTY_OF_BEZIER_CRUVE];
    qint32 mouseDotCnt;//鼠标捕获的点数，从1开始，捕获一个点就是1
    QRect get_desktop_geometry(void);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    bool event(QEvent *e);

signals:
    void signal_mouse_lbtn_pos(QPoint);
    void signal_set_desktop_geometry(QRect);
    void signal_clear_all_lines(void);

public slots:
    void slot_draw_bezier(QPoint);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
