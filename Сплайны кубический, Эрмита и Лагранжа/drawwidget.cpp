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

        for (int i=0;i<pTask->vector_of_points.size();i++){
            if (i < pTask->vector_of_points.size() - 1){
                p.drawLine(pTask->vector_of_points[i].x, pTask->vector_of_points[i].y,
                           pTask->vector_of_points[i+1].x, pTask->vector_of_points[i+1].y);
            }
        }

        QPen penHLines(QColor("#ffcc00"), 5);
        p.setPen(penHLines);

        for (int i=0;i<pTask->curve.size();i++){
            if (i < pTask->curve.size() - 1){
                p.drawLine(pTask->curve[i].x, pTask->curve[i].y,
                           pTask->curve[i+1].x, pTask->curve[i+1].y);
            }
        }

        if (ls_flag == 1){
            pTask->lagrange_spline();
        }

        if (ls_flag == 2){
            pTask->cubic_spline();
        }

        if (ls_flag == 3){
            pTask->hermite_spline();
        }


        pTask->curve.clear();

    }
}

void DrawWidget::SetDefaultPoints()
{
    int amount_of_points = 12;

    if(pTask){
        int w = width();
        int h = height();

        for (int i=0;i<amount_of_points;i++){
            pTask->vector_of_points.push_back( point(100+i*20,300+i*20,i));
        }
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


void DrawWidget::LagrangeSpline(){
    ls_flag = 1;
    cs_flag = 0;
    hs_flag = 0;

    pTask->lagrange_spline();
    repaint();
}

void DrawWidget::CubicSpline(){
    ls_flag = 0;
    cs_flag = 1;
    hs_flag = 0;
    pTask->cubic_spline();
    repaint();
}

void DrawWidget::HermiteSpline(){
    ls_flag = 0;
    cs_flag = 0;
    hs_flag = 1;
    pTask->hermite_spline();
    repaint();
}
