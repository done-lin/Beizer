#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "renderarea.h"
#include "beziercruve.h"
#include "lagrangeinterpolation.h"
#include <QPushButton>
#include <QTouchEvent>

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
    RenderArea *pMyLagrangeRenderArea;
    LagrangeInterpolation *pMyLagrangeInterpolation;


    QRect get_desktop_geometry(void);
    QPushButton *clearBtn;
    QPushButton *repaintBtn;
    QPushButton *LagrangeClearBtn;
protected:
    void mousePressEvent(QMouseEvent *event) override;
    bool event(QEvent *e) override;

signals:
    void signal_mouse_lbtn_pos(QPoint lbtnPpos);
    void signal_mouse_lbtn_pos_lagrange(QPoint lbtnPpos);
    void signal_set_desktop_geometry(QRect);
    void signal_clear_all_lines(void);


public slots:
    void slot_draw_bezier(QPoint);

private slots:
    void on_clearButton_clicked();
    void on_one_line_button_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
