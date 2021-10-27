#include "drawwidget.h"
#include <QPainter>
#include <QMouseEvent>

const int circ_size = 20;

DrawWidget::DrawWidget(QWidget *parent) : QWidget(parent)
{
    pTask = 0;
    m_bMovePointFlag = false;
    intersection_flag = 0;
    union_flag = 0;
    diff_flag1 = 0;
    diff_flag2 = 0;
}


void DrawWidget::paintEvent(QPaintEvent *event){
    QPainter p(this);

    if(pTask){
        std::vector<std::string> color = {"#ff0000","#ffcc00","#000080"};

        QPen penHLines(QColor("#8B0000"),4);
        p.setPen(penHLines);

        for (int i=0;i<pTask->polygons.size();i++){

            QPen penHLines2(QColor(color[i].c_str()),3);
            p.setPen(penHLines2);

            for (int j=0;j<pTask->polygons[i].size();j++){
                p.drawArc(pTask->polygons[i][j].x - circ_size / 2,
                          pTask->polygons[i][j].y - circ_size / 2, circ_size, circ_size, 0, 360 * 16);

                if (j<pTask->polygons[i].size()-1){
                    p.drawLine(pTask->polygons[i][j].x, pTask->polygons[i][j].y,
                               pTask->polygons[i][j+1].x, pTask->polygons[i][j+1].y);
                } else {
                    p.drawLine(pTask->polygons[i][j].x, pTask->polygons[i][j].y,
                               pTask->polygons[i][0].x, pTask->polygons[i][0].y);
                }
            }
        }


        if (diff_flag1 == 1 || diff_flag2 == 1 || intersection_flag == 1 || union_flag == 1){
            QPen penHLines2(QColor("#000000"),4);
            p.setPen(penHLines2);

            for (int i=0;i<pTask->result.size();i++){
                p.drawLine(pTask->result[i].s.x, pTask->result[i].s.y,
                           pTask->result[i].e.x, pTask->result[i].e.y);
            }


            pTask->result.clear();

        }
    }

}

void DrawWidget::SetDefaultPoints()
{
    int amount_of_points = 10;
    int amount_of_polygon = 2;
    std::vector<point> temp;

    if(pTask){
        int w = width();
        int h = height();

        for (int i=0;i<amount_of_polygon;i++){
            for (int j=0;j<amount_of_points;j++){
                temp.push_back(point((i+1)*100+j*50,300+j*20,j));
            }

            pTask->polygons.push_back(temp);
            temp.clear();
        }

        repaint();
      }
}


void DrawWidget::Intersection(){
    intersection_flag = 1;
    diff_flag1 = 0;
    diff_flag2 = 0;
    union_flag = 0;
    pTask->intersection();
    repaint();
}


void DrawWidget::Union(){
    intersection_flag = 0;
    diff_flag1 = 0;
    diff_flag2 = 0;
    union_flag = 1;
    pTask->union_();
    repaint();
}

void DrawWidget::Difference1(){
    diff_flag1 = 1;
    diff_flag2 = 0;
    union_flag = 0;
    intersection_flag = 0;

    pTask->difference1();
    repaint();
}



void DrawWidget::Difference2(){
    diff_flag1 = 0;
    diff_flag2 = 1;
    union_flag = 0;
    intersection_flag = 0;

    pTask->difference2();
    repaint();
}

void DrawWidget::mouseMoveEvent(QMouseEvent *event){
  if(m_bMovePointFlag)
  {
    for (int i=0;i<pTask->polygons.size();i++){
       for (int j=0;j<pTask->polygons[i].size();j++){
            if(last_moved_point_id == j && last_moved_polygon == i){
                pTask->polygons[i][j].x = event->x();
                pTask->polygons[i][j].y = event->y();
                repaint();
                break;
                }
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
       for (int i=0;i<pTask->polygons.size();i++){
          for (int j=0;j<pTask->polygons[i].size();j++){
              if((v - pTask->polygons[i][j]).length() < circ_size / 2){
                    m_bMovePointFlag = true;
                    last_moved_point_id = pTask->polygons[i][j].id;
                    last_moved_polygon = i;
                    break;
          }
       }
     }
   }
}

void DrawWidget::mouseReleaseEvent(QMouseEvent *event)
{
  m_bMovePointFlag = false;
}
