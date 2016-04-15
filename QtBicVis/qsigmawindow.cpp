#include "qsigmawindow.h"

qSigmaWindow::qSigmaWindow(QWidget *parent) : qPlotWindow(parent)
{
    resize(400,400);
    setWindowTitle(tr("Sigma Values"));
}

void qSigmaWindow::setData()
{

}


void qSigmaWindow::setData(QList<QStringList> *values, QString Bicluster)
{
    QStringList split = Bicluster.split("|");
    QString tempRows = split[0];
    QString tempCols = split[1];
    QStringList rows = tempRows.split(QRegExp("\\s+"));
    rows.removeAll("");
    QStringList cols = tempCols.split(QRegExp("\\s+"));
    cols.removeAll("");
    QVector<double> avg(cols.size()), varr(cols.size()), sig(cols.size()), x(cols.size()),y1(cols.size()),y2(cols.size());
    avg.fill(0);
    varr.fill(0);
    sig.fill(0);
    for(int i=0; i<cols.size(); i++)
    {
        for(int j=0; j<rows.size(); j++)
        {
            avg[i]+=(*values)[rows.at(j).toInt()].at(i).toDouble();
        }
        avg[i]=avg[i]/(double)rows.size();
        for(int j=0; j<rows.size(); j++)
        {
            varr[i]+=(((*values)[rows.at(j).toInt()].at(i).toDouble()-avg[i])*((*values)[rows.at(j).toInt()].at(i).toDouble()-avg[i]));
        }
        varr[i]=varr[i]/(double)rows.size();
        sig[i]=sqrt(varr[i]);
        y1[i]=avg[i]+sig[i];
        y2[i]=avg[i]-sig[i];
        x[i]=cols[i].toDouble();
    }
    double val=0;
    for(int i=0;i<sig.size();i++)
    {
        val+=sig[i];
    }
    val = val/(double)sig.size();
    plot->xAxis->setLabel("Cols");
    plot->yAxis->setLabel("Values");
    plot->xAxis->setNumberFormat("f");
    plot->yAxis->setNumberFormat("f");
    plot->xAxis->setNumberPrecision(0);
    plot->yAxis->setNumberPrecision(0);
    plot->axisRect()->setRangeZoom(Qt::Horizontal);
    //plot->axisRect()->setRangeDrag(Qt::Horizontal);
    double max = *std::max_element(x.begin(), x.end());
    double max0 = *std::max_element(y1.begin(), y1.end());
    double min = *std::min_element(y2.begin(), y2.end());

    plot->xAxis->setRange(0.24,max+0.1);
    plot->yAxis->setRange(min-10,max0+10);
    QCPGraph* graph = new QCPGraph(plot->xAxis,plot->yAxis);
    graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc,6));
    graph->setLineStyle(QCPGraph::lsNone);
    graph->setData(x,avg);
    QCPGraph* graph2 = new QCPGraph(plot->xAxis,plot->yAxis);
    graph2->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus,6));
    graph2->setLineStyle(QCPGraph::lsNone);
    graph2->setData(x,y1);
    QCPGraph* graph3 = new QCPGraph(plot->xAxis,plot->yAxis);
    graph3->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus,6));
    graph3->setLineStyle(QCPGraph::lsNone);
    graph3->setData(x,y2);
    QCPItemStraightLine* line = new QCPItemStraightLine(plot);
    line->point1->setCoords(0,val);
    line->point2->setCoords(max, val);
    plot->addItem(line);
    plot->replot();

}
