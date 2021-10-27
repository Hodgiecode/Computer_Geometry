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

    int ls_flag;
    int cs_flag;
    int hs_flag;

    void LagrangeSpline();
    void CubicSpline();
    void HermiteSpline();

protected:
    bool m_bMovePointFlag;
    int last_moved_point_id;
signals:

};

#endif // DRAWWIDGET_H
