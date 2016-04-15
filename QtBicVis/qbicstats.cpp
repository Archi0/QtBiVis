#include "qbicstats.h"

qBicStats::qBicStats()
{
    resize(400,400);
    setWindowTitle(tr("Bic Stats"));
}

void qBicStats::setData()
{

}

void qBicStats::setData(double **values, QList<QString>* biclusters, int index)
{
   QVector<double> x;
   QVector<double> y;
    x.append(0);
    y.append(0);
   for(int i = 0; i< biclusters->size(); i++)
   {
      if(index!=i)
      {

       if(values[index][i]!=0)
       {
           if(x.contains(values[index][i]))
           {
            y[x.indexOf(values[index][i])]+=1;
           }
           else
           {
               x.append(values[index][i]);
               y.append(1);
           }
       }

      }
   }
   plot->xAxis->setLabel("Coverage value");
   plot->yAxis->setLabel("Occurance");
   plot->yAxis->setAutoTickStep(false);
   plot->yAxis->setTickStep(1.0);
   float max = *std::max_element(x.begin(), x.end());
   float max2 = *std::max_element(y.begin(), y.end());
   plot->xAxis->setRange(0, max+0.1);
   plot->yAxis->setRange(0, max2);

   QCPGraph* graph = new QCPGraph(plot->xAxis, plot->yAxis);
   graph->setPen(QPen(Qt::red));
   graph->setData(x,y);
   QCPBars* bars = new QCPBars(plot->xAxis,plot->yAxis);

   bars->setData(x, y);
   bars->setWidthType(QCPBars::wtAbsolute);
   bars->setWidth(10);
}

