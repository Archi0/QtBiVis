#include "qbicwin.h"
#include "qsigmawindow.h"
#include "qbicstats.h"

qBicWin::qBicWin(QWidget *parent) : QWidget(parent)
{
    resize(800,800);
    QGridLayout* mainLayout = new QGridLayout(this);
    QGroupBox* valGb = new QGroupBox(tr("Bicluster Values"));
    QGroupBox* percGb = new QGroupBox(tr("Neighbourhood"));
    QGridLayout* valGrid = new QGridLayout();
    QGridLayout* percGrid = new QGridLayout();
    m_pbtnOkButton = new QPushButton(tr("Ok"));
    m_pbtnOkButton->setFixedHeight(50);
    m_pbtnParallel = new QPushButton(tr("Parallel Coords"));
    m_pbtnParallel->setFixedHeight(50);
    m_pbtnStats = new QPushButton(tr("Stats"));
    m_pbtnStats->setFixedHeight(50);
    m_pbtnPerc = new QPushButton(tr("Coverage"));
    m_pbtnPerc->setFixedHeight(50);
    m_ptbValView = new QTableView();
    m_ptbValView->horizontalHeader()->setStretchLastSection(true);
    valGb->setMinimumWidth(400);
    m_ptbPercView = new QTableView();
    m_ptbPercView->horizontalHeader()->setStretchLastSection(true);
    percGb->setMaximumWidth(400);
    m_ptbGoView = new QTableView();
    m_ptbGoView->horizontalHeader()->setStretchLastSection(true);
    m_ptbGoView->setMaximumHeight(200);
    m_pValModel = new QStandardItemModel();
    m_pPercModel = new QStandardItemModel();
    m_pGoModel = new QStandardItemModel();
    valGrid->addWidget(m_ptbValView,0,0,2,2);
    valGrid->addWidget(m_ptbGoView, 2,0,1,2);
    percGrid->addWidget(m_ptbPercView,0,0);
    valGb->setLayout(valGrid);
    percGb->setLayout(percGrid);
    mainLayout->addWidget(valGb, 0,0,1,3);
    mainLayout->addWidget(percGb,0,3,1,1);
    mainLayout->addWidget(m_pbtnOkButton, 1, 0,1,1);
    mainLayout->addWidget(m_pbtnParallel,1,1,1,1);
    mainLayout->addWidget(m_pbtnPerc, 1,2,1,1);
    mainLayout->addWidget(m_pbtnStats, 1,3,1,1);
    setLayout(mainLayout);
    connect(m_pbtnOkButton, SIGNAL(clicked()),this, SLOT(goClose()));
    connect(m_pbtnParallel, SIGNAL(clicked(bool)), this, SLOT(showParallelCords()));
    connect(m_pbtnPerc, SIGNAL(clicked()),this,SLOT(showPerc()));
    connect(m_pbtnStats, SIGNAL(clicked()),this, SLOT(showStats()));
}

void qBicWin::goClose()
{
    this->close();
    emit closed();
}

void qBicWin::showParallelCords()
{
    qParallelPlot* win = new qParallelPlot();
    win->setData(m_plValues,m_bic);
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
void qBicWin::init(double **perc, QList<QStringList> *vals, QString Bic, QStringList *rowN, QStringList *colN, QStringList* bics, QStringList *goStats)
{
    m_plValues = vals;
    percVals = perc;
    m_bic = Bic;
    rowNames = rowN;
    colNames = colN;
    m_plGoStats = goStats;
    m_biclusters = bics;
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
    for(int i=0; i<rows.size(); i++)
    {
        //QStandardItem *item = new QStandardItem((*rowNames)[rows[i].toInt()]);
       // m_pValModel->setItem(i+1, 0, item);
        rowLabels.append((*rowNames)[rows[i].toInt()]);
    }
    for(int i=0;i <cols.size(); i++)
    {
        //QStandardItem *item = new QStandardItem((*colNames)[cols[i].toInt()]);
       // m_pValModel->setItem(0, i+1, item);
        colLabels.append((*colNames)[cols[i].toInt()]);
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

            QStringList temp = (*m_plGoStats)[i].split(",");
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
