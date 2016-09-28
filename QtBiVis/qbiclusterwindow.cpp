#include "qbiclusterwindow.h"

qQuantityWin::qQuantityWin(QWidget *parent) : qPlotWindow(parent)
{
    resize(400,400);
    setWindowTitle(tr("Quantity histogram"));
}

void qQuantityWin::setData()
{

}

void qQuantityWin::setData(QVector<int> size, QVector<double> result)
{
    qs2(&size, 0, size.size()-1,&result );

    QCPBarsGroup *group = new QCPBarsGroup(plot);
    QVector<double> tempSize(size.size());

    for(int i=0;i<size.size();i++)
        tempSize[i]=(double)size[i];

    plot->xAxis->setLabel("Size of bicluster");
    plot->yAxis->setLabel("Count");
    plot->xAxis->setNumberFormat("f");
    plot->yAxis->setNumberFormat("f");
    plot->xAxis->setNumberPrecision(0);
    plot->yAxis->setNumberPrecision(0);
    plot->axisRect()->setRangeZoom(Qt::Horizontal);
    //plot->axisRect()->setRangeDrag(Qt::Horizontal);
    double max = *std::max_element(result.begin(), result.end());

    plot->xAxis->setRange(0,tempSize.last());
    plot->yAxis->setRange(0,max);
    bars = new QCPBars(plot->xAxis,plot->yAxis);

    bars->setData(tempSize, result);
    bars->setBarsGroup(group);
}

