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

    void paintEvent(QPaintEvent *event);
    void SetTaskPtr(task_data * p) { pTask = p; }

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void Difference1();
    void Difference2();
    void Intersection();
    void Union();

    int diff_flag1;
    int diff_flag2;
    int intersection_flag;
    int union_flag;

protected:
    bool m_bMovePointFlag;
    int last_moved_point_id;
    int last_moved_polygon;
signals:

};

#endif // DRAWWIDGET_H
