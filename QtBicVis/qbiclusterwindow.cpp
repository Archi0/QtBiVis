#include "qbiclusterwindow.h"

qQuantityWin::qQuantityWin(QWidget *parent) : qPlotWindow(parent)
{
    resize(400,400);
    setWindowTitle(tr("QTBiclustVisual"));
}

void qQuantityWin::setData()
{

}

void qQuantityWin::setData(QVector<int> size, QVector<double> result)
{
    qs2(&size, 0, size.size()-1,&result );
    QVector<double> avg;
    QVector<double> values;
    QVector<double> res;
    QVector<int> quan;
    QCPBarsGroup *group = new QCPBarsGroup(plot);
    QVector<double> size2(size.size());
    for(int i=0;i<size.size();i++)
        size2[i]=(double)size[i];
    int j=0;
    for(int i=0; i<size2.size();i++)
    {
        if(values.contains(size2[i]))
        {
            quan[quan.size()-1]+=1;
            avg[avg.size()-1]+=result[i];
        }
        else
        {
            values.append(size2[i]);
            avg.append(result[i]);
            quan.append(1);
        }
    }
    for(int i=0;i<avg.size();i++)
    {
        avg[i]=avg[i]/(double)quan[i];
    }

    plot->xAxis->setLabel("Size");
    plot->yAxis->setLabel("Value");
    plot->xAxis->setNumberFormat("f");
    plot->yAxis->setNumberFormat("f");
    plot->xAxis->setNumberPrecision(0);
    plot->yAxis->setNumberPrecision(0);
    plot->axisRect()->setRangeZoom(Qt::Horizontal);
    //plot->axisRect()->setRangeDrag(Qt::Horizontal);
    double max = *std::max_element(result.begin(), result.end());

    plot->xAxis->setRange(0,size2.last());
    plot->yAxis->setRange(0,max);
    bars = new QCPBars(plot->xAxis,plot->yAxis);

    bars->setData(size2, result);
    bars->setBarsGroup(group);
    /*QCPGraph* graph = new QCPGraph(plot->xAxis,plot->yAxis);
    graph->setPen(QPen(Qt::red));
    graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
    graph->setLineStyle(QCPGraph::lsLine);
    graph->setData(values, avg);*/
}

