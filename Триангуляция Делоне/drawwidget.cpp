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
        QPen penHLines(QColor("#8B0000"),3);
        p.setPen(penHLines);

        for (int i=0;i<pTask->vector_of_points.size();i++){
            p.drawArc(pTask->vector_of_points[i].x - circ_size / 2, pTask->vector_of_points[i].y - circ_size / 2, circ_size, circ_size, 0, 360 * 16);
        }

        QPen penHLines2(QColor("#000000"),5);
        p.setPen(penHLines2);

        for (int i=0; i<pTask->triangle_vector.size(); i++){

             p.drawLine(pTask->triangle_vector[i].p1.x, pTask->triangle_vector[i].p1.y,
                        pTask->triangle_vector[i].p2.x, pTask->triangle_vector[i].p2.y);

             p.drawLine(pTask->triangle_vector[i].p1.x, pTask->triangle_vector[i].p1.y,
                        pTask->triangle_vector[i].p3.x, pTask->triangle_vector[i].p3.y);

             p.drawLine(pTask->triangle_vector[i].p2.x, pTask->triangle_vector[i].p2.y,
                        pTask->triangle_vector[i].p3.x, pTask->triangle_vector[i].p3.y);
        }

        QPen penHLines3(QColor("#000000"));
        p.setPen(penHLines3);

        for (int i=0; i<pTask->triangle_vector.size(); i++){
             p.drawEllipse(QPointF(pTask->triangle_vector[i].center.x,
                       pTask->triangle_vector[i].center.y),
                       pTask->triangle_vector[i].radius,
                       pTask->triangle_vector[i].radius);
        }

        pTask->run();
        pTask->triangle_vector.clear();
    }

}

void DrawWidget::SetDefaultPoints()
{
    int amount_of_points = 10;

    if(pTask){
        int w = width();
        int h = height();

        for (int i=0;i<amount_of_points;i++){
            pTask->vector_of_points.push_back( point(100+i*20,300+i*20,i));
        }


        pTask->run();
        repaint();
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
