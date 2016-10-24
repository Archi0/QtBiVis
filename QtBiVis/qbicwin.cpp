#include "qbicwin.h"
#include "qsigmawindow.h"
#include "qbicstats.h"
#include "mainwindow.h"

qBicWin::qBicWin(QWidget *parent) : QWidget(parent)
{
    resize(800,800);
    QGridLayout* mainLayout = new QGridLayout(this);
    QGroupBox* valGb = new QGroupBox(tr("Bicluster Values"));
    QGroupBox* percGb = new QGroupBox(tr("Neighbourhood"));
    QGridLayout* valGrid = new QGridLayout();
    QGridLayout* percGrid = new QGridLayout();
    QSplitter* splitter = new QSplitter();

    m_pbtnOkButton = new QPushButton(tr("Ok"));
    m_pbtnOkButton->setFixedHeight(50);
    m_pbtnParallel = new QPushButton(tr("Parallel Coords"));
    m_pbtnParallel->setFixedHeight(50);
    m_pbtnStats = new QPushButton(tr("Stats"));
    m_pbtnStats->setFixedHeight(50);
    m_pbtnPerc = new QPushButton(tr("Coverage"));
    m_pbtnPerc->setFixedHeight(50);
    m_pbtnSort = new QPushButton(tr("Sort"));
    m_pbtnSort->setFixedHeight(50);

    m_ptbValView = new QTableView();
    m_ptbValView->horizontalHeader()->setStretchLastSection(true);
    valGb->setMinimumWidth(400);
    m_ptbPercView = new QTableView();
    m_ptbPercView->horizontalHeader()->setStretchLastSection(true);
  //  percGb->setMaximumWidth(400);
    m_ptbGoView = new QTableView();
    m_ptbGoView->horizontalHeader()->setStretchLastSection(true);
    m_ptbGoView->setMaximumHeight(200);
    m_pValModel = new QStandardItemModel();
    m_pPercModel = new QStandardItemModel();
    m_pGoModel = new QStandardItemModel();

    plot = new QCustomPlot(this);
    plot->setMinimumWidth(400);
    plot->setMinimumHeight(500);
    plot->setInteractions(QCP::iRangeZoom|QCP::iRangeDrag);
    colorMap = new QCPColorMap(plot->xAxis,plot->yAxis);
    colorMap->data()->fill(0);
   curve = new QCPCurve(plot->xAxis,plot->yAxis);

    valGrid->addWidget(m_ptbValView,0,0,2,2);
    valGrid->addWidget(m_ptbGoView, 2,0,1,2);
    percGrid->addWidget(plot,0,0);
    percGrid->addWidget(m_ptbPercView,1,0);
    valGb->setLayout(valGrid);
    percGb->setLayout(percGrid);
    splitter->addWidget(valGb);
    splitter->addWidget(percGb);
   // mainLayout->addWidget(valGb, 0,0,2,3);
    //mainLayout->addWidget(percGb,0,3,2,1);
    mainLayout->addWidget(splitter,0,0,2,4);
    mainLayout->addWidget(m_pbtnOkButton, 2, 0,1,1);
    mainLayout->addWidget(m_pbtnParallel,2,1,1,1);
    mainLayout->addWidget(m_pbtnPerc, 2,2,1,1);
    mainLayout->addWidget(m_pbtnStats, 2,3,1,1);
    mainLayout->addWidget(m_pbtnSort,3,0,1,4);
    setLayout(mainLayout);
    plot->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(plot, SIGNAL(customContextMenuRequested(QPoint)),this, SLOT(contextPlot(QPoint)));
    connect(m_pbtnOkButton, SIGNAL(clicked()),this, SLOT(goClose()));
    connect(m_pbtnParallel, SIGNAL(clicked(bool)), this, SLOT(showParallelCords()));
    connect(m_pbtnPerc, SIGNAL(clicked()),this,SLOT(showPerc()));
    connect(m_pbtnStats, SIGNAL(clicked()),this, SLOT(showStats()));
    connect(m_pbtnSort,SIGNAL(clicked()),this,SLOT(sort()));
}

void qBicWin::goClose()
{
    this->close();
    emit closed();
}

void qBicWin::showParallelCords()
{
    qParallelPlot* win = new qParallelPlot();
    win->setData(m_plValues,m_bic,colorMap->data()->valueRange());
    win->show();
}

void qBicWin::showStats()
{
    qSigmaWindow* win = new qSigmaWindow();
    win->setData(m_plValues,m_bic);
    win->show();
}

void qBicWin::showPerc()
{
    qBicStats* win = new qBicStats();
    win->setData(percVals,m_biclusters,m_biclusters->indexOf(m_bic));
    win->show();
}

void qBicWin::sort()
{
    QList<QStringList>* sortedVals = new QList<QStringList>();
    QStringList list = m_bic.split("|");
    QStringList rows = list[0].split(QRegExp("\\s+"));
    rows.removeAll("");
    QStringList cols = list[1].split(QRegExp("\\s+"));
    cols.removeAll("");
    QVector<int> iRows;
    QVector<int> iCols;
    for(int i=rows.size()-1;i>=0;i--)
    {
        sortedVals->push_front((*m_plValues)[rows.at(i).toInt()]);
        iRows.push_back(rows[i].toInt());
    }
    for(int i=0; i< cols.size(); i++)
    {
        iCols.push_back(cols[i].toInt());
    }
    for(int i=0;i<rowC;i++)
    {
        if(!iRows.contains(i))
            sortedVals->push_back((*m_plValues)[i]);
        QStringList temp;
        for(int j=iCols.size()-1; j>=0; j--)
        {
            temp.push_front((*sortedVals)[i].at(iCols[j]));
        }
        for(int j=0;j<colC;j++)
        {
            if(!iCols.contains(j))
                temp.push_back((*sortedVals)[i].at(j));
        }
        (*sortedVals)[i] = temp;
    }
    for(int nV =0;nV<sortedVals->size();nV++)
    {
        QStringList row = (*sortedVals)[nV];
        for(int nC = 0; nC<row.size();nC++)
        {
            QString value = row.at(nC);
            double val = value.toDouble();
           // qDebug() << val;
            colorMap->data()->setCell(nC, nV, value.toDouble());
        }
    }
    colorMap->rescaleDataRange(true);

    QVector<double> x_1;
    QVector<double> x_2;
    x_1.append(0);
    x_1.append(iRows.size());
    x_1.append(iRows.size());
    x_1.append(0);
    x_1.append(0);

    x_2.append(0);
    x_2.append(0);
    x_2.append(iCols.size());
    x_2.append(iCols.size());
    x_2.append(0);

    curve->setPen(QPen(Qt::white));
  //  curve->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));

    curve->setData(x_2,x_1);


    plot->replot();
}

void qBicWin::contextPlot(QPoint pos)
{
    QMenu *menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    menu->addAction("Save heatmap", this, SLOT(saveHeat()));

    menu->popup(plot->mapToGlobal(pos));
}


void qBicWin::saveHeat()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save graph", "", "*.pdf");
    fileName = fileName + ".pdf";
    plot->savePdf(fileName, 0, 0);
}

void qBicWin::draw()
{
    if(m_plValues->size()>0)
    {
        plot->xAxis->setRange(0, colC);
        plot->yAxis->setRange(0, rowC);
        plot->yAxis->setRangeReversed(true);
        plot->xAxis->setNumberFormat("f");
        plot->yAxis->setNumberFormat("f");
        plot->xAxis->setNumberPrecision(0);
        plot->yAxis->setNumberPrecision(0);

      //  plot->axisRect()->setRangeZoom(Qt::Vertical);
       //plot->axisRect()->setRangeDrag(Qt::Vertical);
        colorMap->data()->setSize(colC,rowC);
        colorMap->data()->setRange(QCPRange(0, colC),QCPRange(0,rowC));
        QCPColorGradient* grad;
        if(MainWindow::gradientType!=100)
        {
            grad = new QCPColorGradient((QCPColorGradient::GradientPreset)MainWindow::gradientType);
        }
        else
        {
            grad = new QCPColorGradient(QCPColorGradient::gpSpectrum);
            grad->clearColorStops();
            grad->setColorStopAt(0,Qt::red);
            grad->setColorStopAt(0.5,Qt::black);
            grad->setColorStopAt(1, Qt::green);
            grad->setColorInterpolation(QCPColorGradient::ciRGB);

        }

        colorMap->setGradient(*grad);
        colorMap->setTightBoundary(true);
        colorMap->setInterpolate(false);
        colorMap->data()->fill(0);

        if(MainWindow::dataScaleType==0)
            colorMap->setDataScaleType(QCPAxis::stLogarithmic);
        else
            colorMap->setDataScaleType(QCPAxis::stLinear);

        for(int nV =0;nV<m_plValues->size();nV++)
        {
            QStringList row = (*m_plValues)[nV];
            for(int nC = 0; nC<row.size();nC++)
            {
                QString value = row.at(nC);
                double val = value.toDouble();
               // qDebug() << val;
                colorMap->data()->setCell(nC, nV, value.toDouble());
            }
        }
        colorMap->rescaleDataRange(true);
        plot->xAxis->setRange(0, 100);
        plot->yAxis->setRange(0, 100);
        plot->replot();
    }
}
void qBicWin::init(double **perc, QList<QStringList> *vals, QString Bic, QStringList *rowN, QStringList *colN, QStringList* bics, QStringList *goStats, int rowsn, int colsn)
{
    m_plValues = vals;
    percVals = perc;
    m_bic = Bic;
    rowNames = rowN;
    colNames = colN;
    m_plGoStats = goStats;
    m_biclusters = bics;
    colC = colsn;
    rowC = rowsn;
    QStringList list = m_bic.split("|");
    QStringList rows = list[0].split(QRegExp("\\s+"));
    rows.removeAll("");
    QStringList cols = list[1].split(QRegExp("\\s+"));
    cols.removeAll("");
    m_pValModel->clear();
    m_pValModel->setRowCount(rows.size());
    m_pValModel->setColumnCount(cols.size());
    QStringList rowLabels;
    QStringList colLabels;
    //rowLabels.append("");
    //colLabels.append("");
    if(rowNames->size()> 0)
    {
        for(int i=0; i<rows.size(); i++)
        {
            //QStandardItem *item = new QStandardItem((*rowNames)[rows[i].toInt()]);
           // m_pValModel->setItem(i+1, 0, item);
            rowLabels.append((*rowNames)[rows[i].toInt()]);
        }
    }
    if(colNames->size()>0)
    {
        for(int i=0;i <cols.size(); i++)
        {
            //QStandardItem *item = new QStandardItem((*colNames)[cols[i].toInt()]);
           // m_pValModel->setItem(0, i+1, item);
            colLabels.append((*colNames)[cols[i].toInt()]);
        }
    }

    m_pValModel->setHorizontalHeaderLabels(colLabels);
    m_pValModel->setVerticalHeaderLabels(rowLabels);
    for(int i=0; i<rows.size(); i++)
    {
        for(int j=0; j<cols.size();j++)
        {
            QString value = "0";
            if(rows[i].toInt() < m_plValues->size())
            {
                if(cols[j].toInt() < (*m_plValues)[rows[i].toInt()].size())
                    value = (*m_plValues)[rows[i].toInt()][cols[j].toInt()];
            }
            QStandardItem *item = new QStandardItem(value);
            m_pValModel->setItem(i, j, item);
        }
    }

    m_ptbValView->setModel(m_pValModel);
    m_pPercModel->clear();
    if(percVals!=NULL)
    {
        QString bicluster = rows.join(" ") + "|" + cols.join(" ");
        QStringList perLabels;
        perLabels.append("Bicluster:");
        perLabels.append("Coverage value:");
        int index = m_biclusters->indexOf(bicluster);
        int j =0;
        for(int i=0; i<m_biclusters->size();i++)
        {
            if(index>=0 && i!=index)
            {
                if(percVals[index][i]>=0.25)
                {
                    QStandardItem *item = new QStandardItem((*m_biclusters)[i]);
                    m_pPercModel->setItem(j, 0, item);
                    QStandardItem *item2 = new QStandardItem(QString::number(percVals[index][i], 'g', 8));
                    m_pPercModel->setItem(j, 1, item2);
                    j++;
                }

            }
        }
        m_pPercModel->setHorizontalHeaderLabels(perLabels);
        m_ptbPercView->setModel(m_pPercModel);

    }
    m_pGoModel->clear();
    if(m_plGoStats->size()>0)
    {
        QStringList goLabels;
        goLabels.append("GO:");
        goLabels.append("pvalue:");
        goLabels.append("pvalue 2:");
        for(int i=0; i< m_plGoStats->size(); i++)
        {
            QString go, pval1,pval2;
            go=pval1=pval2="";

            QStringList temp = (*m_plGoStats)[i].split(" ");
            go=temp[0];
            pval1 = temp[1];
            pval2 = temp[2];

            QStandardItem *goItem = new QStandardItem(go);
            QStandardItem *p1Item = new QStandardItem(pval1);
            QStandardItem *p2Item = new QStandardItem(pval2);
            m_pGoModel->setItem(i,0,goItem);
            m_pGoModel->setItem(i, 1, p1Item);
            m_pGoModel->setItem(i,2,p2Item);
            m_pGoModel->setHorizontalHeaderLabels(goLabels);
           // QStandardItem *item = new QStandardItem((*m_plGoStats)[i]);
            //m_pGoModel->setItem(i,0,item);
        }
        m_ptbGoView->setModel(m_pGoModel);
    }
    draw();
}


void qGoFilter::setGo(QString go)
{
    m_go = go;
}

bool qGoFilter::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex index = sourceModel()->index(source_row, 0, source_parent);

    if(index.data().toString().contains(m_go))
        return true;
    if(m_go!="")
        return false;
    else
        return true;
}
