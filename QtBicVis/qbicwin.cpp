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
    valGb->setMinimumWidth(400);
    m_ptbPercView = new QTableView();
    percGb->setMaximumWidth(400);
    m_pValModel = new QStandardItemModel();
    m_pPercModel = new QStandardItemModel();
    valGrid->addWidget(m_ptbValView,0,0);
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
void qBicWin::init(double **perc, QList<QStringList> *vals, QString Bic, QStringList *rowN, QStringList *colN, QStringList* bics)
{
    m_plValues = vals;
    percVals = perc;
    m_bic = Bic;
    rowNames = rowN;
    colNames = colN;
    m_biclusters = bics;
    QStringList list = m_bic.split("|");
    QStringList rows = list[0].split(QRegExp("\\s+"));
    rows.removeAll("");
    QStringList cols = list[1].split(QRegExp("\\s+"));
    cols.removeAll("");
    m_pValModel->clear();
    m_pValModel->setRowCount(rows.size()+1);
    m_pValModel->setColumnCount(cols.size()+1);

    for(int i=0; i<rows.size(); i++)
    {
        for(int j=0; j<cols.size();j++)
        {
            if(i==0 && j==0)
                continue;
            else if(i>0 && j==0 && rowNames->size()>0)
            {
                QStandardItem *item = new QStandardItem((*rowNames)[rows[i].toInt()]);
                m_pValModel->setItem(i, j, item);
            }
            else if(i==0 && j>0 && colNames->size()>0)
            {
                QStandardItem *item = new QStandardItem((*colNames)[cols[j].toInt()]);
                m_pValModel->setItem(i, j, item);
            }
            else
            {
                QStandardItem *item = new QStandardItem((*m_plValues)[i][j]);
                m_pValModel->setItem(i, j, item);
            }
        }
    }
    m_ptbValView->setModel(m_pValModel);
    m_pPercModel->clear();
    if(percVals!=NULL)
    {
        QString bicluster = rows.join(" ") + "|" + cols.join(" ");
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
        m_ptbPercView->setModel(m_pPercModel);
    }
}

