#include "drawwidget.h"
#include <QPainter>
#include <QMouseEvent>

const int circ_size = 20;

DrawWidget::DrawWidget(QWidget *parent) : QWidget(parent)
{
    pTask = 0;
    m_bMovePointFlag = false;
}


void DrawWidget::paintEvent(QPaintEvent *event){
    QPainter p(this);

    if(pTask){
        for (int i=0;i<pTask->vector_of_points.size();i++){
            p.drawArc(pTask->vector_of_points[i].x - circ_size / 2, pTask->vector_of_points[i].y - circ_size / 2, circ_size, circ_size, 0, 360 * 16);
        }

        int result_size = pTask->result_vector.size();
        for (int i=0;i < result_size;i++){
            if (i < result_size - 1){
                p.drawLine(pTask->result_vector[i].x, pTask->result_vector[i].y, pTask->result_vector[i+1].x, pTask->result_vector[i+1].y);
            } else {
                p.drawLine(pTask->result_vector[i].x, pTask->result_vector[i].y, pTask->result_vector[0].x, pTask->result_vector[0].y);
            }
        }

        pTask->result_vector.clear();
    }

}

void DrawWidget::SetDefaultPoints()
{
    int amount_of_points = 10;

    if(pTask){
        int w = width();
        int h = height();

        for (int i=0; i<amount_of_points;i++){
            point p((i+1)*25 + 100,(i+1)*25, i); //у точки есть координаты и ее номер
            pTask->vector_of_points.push_back(p);
        }

        pTask->run();
      }
}

void DrawWidget::mouseMoveEvent(QMouseEvent *event)
{
  if(m_bMovePointFlag)
  {
    for (int i=0;i<pTask->vector_of_points.size();i++){
       int id = pTask->vector_of_points[i].id;

       if(last_moved_point_id == id)
        {
          pTask->vector_of_points[i].x = event->x();
          pTask->vector_of_points[i].y = event->y();

          pTask->run();
          repaint();

          break;
        }
    }
  }
}

void DrawWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
      int XX = event->x();
      int YY = event->y();

       point v(XX, YY, -1);
       for (int i=0;i<pTask->vector_of_points.size();i++){

          if((v - pTask->vector_of_points[i]).length() < circ_size / 2){
              m_bMovePointFlag = true;
              last_moved_point_id = i;
              break;
          }
       }

    }
}

void DrawWidget::mouseReleaseEvent(QMouseEvent *event)
{
  m_bMovePointFlag = false;
}
