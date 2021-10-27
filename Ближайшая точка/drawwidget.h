#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H
#include <QWidget>
#include "task_data.h"

class DrawWidget : public QWidget
{
    Q_OBJECT;

public :
    task_data * pTask;

public:
    explicit DrawWidget(QWidget *parent = nullptr);
    void SetDefaultPoints();
    void  Clear();
    void RunFirstAlgo();
    void RunSecondAlgo();

    void paintEvent(QPaintEvent *event);
    void SetTaskPtr(task_data * p) { pTask = p; }

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    std::string time_algo_one;
    std::string dist_algo_one;
    int first_algo_btn_clicked;
    int second_algo_btn_clicked;

    std::string time_algo_two;
    std::string dist_algo_two;


protected:
    bool m_bMovePointFlag;
    int last_moved_point_id;

signals:

};

#endif // DRAWWIDGET_H
