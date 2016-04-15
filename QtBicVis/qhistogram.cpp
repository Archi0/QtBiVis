#include "qhistogram.h"

qHistogram::qHistogram(QWidget *parent) : qPlotWindow(parent)
{
    resize(400,400);
    setWindowTitle(tr("Histogram"));
}

void qHistogram::setData()
{

}

void qHistogram::setData(double **values, int size)
{
    QVector<double> uniqueVals;
    QVector<double> occuVals;

    for(int i=0; i < size; i++)
    {
        for(int j=0; j< size; j++)
        {
            if(i!=j)
            {
                if(values[i][j] >0.25)
                {
                    if(uniqueVals.contains(values[i][j]))
                    {
                        occuVals[uniqueVals.indexOf(values[i][j])] +=1;
                    }
                    else
                    {
                        uniqueVals.append(values[i][j]);
                        occuVals.append(1);
                    }
                }
            }

        }
    }
    plot->xAxis->setLabel("Coverage");
    plot->yAxis->setLabel("Occurance");
    plot->xAxis->setNumberFormat("f");
    plot->yAxis->setNumberFormat("f");
    plot->xAxis->setNumberPrecision(3);
    plot->yAxis->setNumberPrecision(0);
    plot->axisRect()->setRangeZoom(Qt::Horizontal);
    //plot->axisRect()->setRangeDrag(Qt::Horizontal);
    float max = *std::max_element(uniqueVals.begin(), uniqueVals.end());
    float maxO = *std::max_element(occuVals.begin(), occuVals.end());
    plot->xAxis->setRange(0.24,max+0.1);
    plot->yAxis->setRange(0,maxO);
    bars = new QCPBars(plot->xAxis,plot->yAxis);

    bars->setData(uniqueVals, occuVals);
    bars->setWidthType(QCPBars::wtPlotCoords);
    bars->setWidth(max/uniqueVals.size());
}

