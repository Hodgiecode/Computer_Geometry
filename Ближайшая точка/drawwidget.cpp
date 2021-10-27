#include "drawwidget.h"
#include <QPainter>
#include <QMouseEvent>

const int circ_size = 20;

DrawWidget::DrawWidget(QWidget *parent) : QWidget(parent)
{
    pTask = 0;
    m_bMovePointFlag = false;
    first_algo_btn_clicked = 0;
    second_algo_btn_clicked = 0;

    time_algo_one = "";
    dist_algo_one = "";
}


void DrawWidget::paintEvent(QPaintEvent *event){
    QPainter p(this);

    if(pTask){

        QPen penHLines2(QColor("#000000"),5);
        p.setPen(penHLines2);

        for (int i=0;i<pTask->vector_of_points.size();i++){
            if (pTask->nearest_point_vector_index.size()>0 && i == pTask->nearest_point_vector_index[0]){
                QPen penHLines2(QColor("#ffcc00"), 5);
                p.setPen(penHLines2);
            } else {
                if (pTask->nearest_point_vector_index.size()>0 && i == pTask->nearest_point_vector_index[1]){
                    QPen penHLines(QColor("#ffcc00"), 5);
                    p.setPen(penHLines);
                } else {
                    QPen penHLines2(QColor("#000000"),5);
                    p.setPen(penHLines2);
                }
            }


            p.drawArc(pTask->vector_of_points[i].x - circ_size / 2, pTask->vector_of_points[i].y - circ_size / 2, circ_size, circ_size, 0, 360 * 16);
        }


        if (first_algo_btn_clicked == 1){
            pTask->run_task_one();
        }

        if (second_algo_btn_clicked == 2){
            pTask->run_task_two();
        }
    }

}

void DrawWidget::SetDefaultPoints()
{
    int amount_of_points = 18;
    if(pTask){
        int w = width();
        int h = height();

        for (int i=0;i<amount_of_points;i++){
            pTask->vector_of_points.push_back( point((i+1)*10,(i+1)*20,i));
        }
     }
}

void DrawWidget::Clear()
{
    QPainter p(this);
    pTask->nearest_point_vector_index.clear();
    pTask->vector_of_points.clear();
}

void DrawWidget::RunFirstAlgo(){
    first_algo_btn_clicked = 1;
    second_algo_btn_clicked = 0;

    pTask->run_task_one();
    time_algo_one = pTask->time_algo_one;
    dist_algo_one = pTask->dist_algo_one;

    repaint();
}


void DrawWidget::RunSecondAlgo(){
    first_algo_btn_clicked = 0;
    second_algo_btn_clicked = 1;

    pTask->run_task_two();
    time_algo_two = pTask->time_algo_two;
    dist_algo_two = pTask->dist_algo_two;

    repaint();
}


void DrawWidget::mouseMoveEvent(QMouseEvent *event)
{
  if(m_bMovePointFlag)
  {
    first_algo_btn_clicked = 0;
    second_algo_btn_clicked = 0;

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

