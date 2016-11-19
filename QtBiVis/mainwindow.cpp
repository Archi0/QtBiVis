#include "mainwindow.h"
#include <boost/dynamic_bitset.hpp>

#include <iostream>

int MainWindow::gradientType = 100;
int MainWindow::dataScaleType = 0;


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent),
      rowC(0),
      colC(0),
      percVals(NULL)
{
    resize(1024,768);
    setWindowTitle(tr("QtBiVis"));
    m_plValues = new QList<QStringList>();
   // m_pvData = new QVector<CellData* >();
    m_pMainLayout = new QGridLayout(this);
    m_pMainLayout->addWidget(createPathGroup(),0,0,1,1);
    m_pMainLayout->addWidget(createPaintGroup(),1,0,1,2);
    m_plBiclusters = new QStringList();
    m_plRowNames = new QStringList();
    m_plColNames = new QStringList();
    m_pvData = new QVector<CellData*>(0);
    m_pmCountMap = new QMap<int, QVector<CellData*> >();
    m_pmCellMap = new QMap<QString, CellData*>();
    m_pmGOMap = new QMap<QString, QStringList>();
    m_bicWin = new QVector<qBicWin>();
    plot->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_pbtnBrowse, SIGNAL(clicked()), this, SLOT(browseFile()));
    connect(m_pbtnDraw, SIGNAL(clicked()),this,SLOT(draw()));
    connect(m_pbtnBrowseMain, SIGNAL(clicked()), this, SLOT(browseMainFile()));
    connect(m_pbtnMore,SIGNAL(clicked(bool)), this, SLOT(quantityStats()));
    connect(plot, SIGNAL(mouseDoubleClick(QMouseEvent*)),this,SLOT(select(QMouseEvent*)));
    connect(m_plvBicList,SIGNAL(clicked(QModelIndex)), this, SLOT(selectedList(QModelIndex)));
    connect(m_pbtnCalc, SIGNAL(clicked()), this, SLOT(percStats()));
    connect(m_pbtnStats, SIGNAL(clicked()),this, SLOT(showStats()));
    //connect(m_pbtnSigma, SIGNAL(clicked()), this, SLOT(calcSaveSigma()));
    connect(m_pbtnParser, SIGNAL(clicked()), this, SLOT(testStats()));
    connect(m_pleGoDef, SIGNAL(textChanged(QString)), this, SLOT(setGoText(QString)));
    connect(m_pbtnHeatSett, SIGNAL(clicked()),this, SLOT(setOptions()));
    connect(plot, SIGNAL(customContextMenuRequested(QPoint)),this, SLOT(contextPlot(QPoint)));
   // connect(plot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(setRange(QCPRange)));
   // connect(m_pBiclusterWin, SIGNAL(closed()), this, SLOT(closeBiclusterWin()));
    setLayout(m_pMainLayout);
}

MainWindow::~MainWindow()
{

}
void MainWindow::clearPerc()
{
    if(percVals!=NULL)
    {
        for(int i=0; i<m_plBiclusters->size();i++)
            delete [] percVals[i];
        delete percVals;
        percVals = NULL;
    }
}

void MainWindow::clearData()
{
    plot->replot();
}

void MainWindow::prepareTableView()
{
    m_psmBicListModel->setRowCount(m_plBiclusters->size());
    m_psmBicListModel->setColumnCount(4);
    QStringList labels;
    labels.append("Size");
    labels.append("GO:");
    labels.append("pvalue:");
    labels.append("pvalue 2:");
    labels.append("Bicluster");
    qDebug() << "Bics: " << QString::number(m_plBiclusters->size());
    qDebug() << "GO: " << QString::number(m_pmGOMap->size());
    for(int i =0; i< m_plBiclusters->size(); i++)
    {
        QStringList gostats = (*m_pmGOMap)[(*m_plBiclusters)[i]];
        QString go, pval1,pval2, bicluster;
        go=pval1=pval2="";
        bicluster=(*m_plBiclusters)[i];
        if(gostats.size()>0)
        {
            QStringList temp = gostats[0].split(" ");
            go=temp[0];
            pval1 = temp[1];
            pval2 = temp[2];
        }
        QStringList list =bicluster.split("|");
        QStringList rows = list[0].split(QRegExp("\\s+"));
        QStringList cols = list[1].split(QRegExp("\\s+"));
        int size = rows.size() * cols.size();
        QStandardItem *sizeItem = new QStandardItem(QString::number(size));
        QStandardItem *goItem = new QStandardItem(go);
        QStandardItem *p1Item = new QStandardItem(pval1);
        QStandardItem *p2Item = new QStandardItem(pval2);
        QStandardItem *bicItem = new QStandardItem(bicluster);
        m_psmBicListModel->setItem(i,0,sizeItem);
        m_psmBicListModel->setItem(i,1,goItem);
        m_psmBicListModel->setItem(i, 2, p1Item);
        m_psmBicListModel->setItem(i,3,p2Item);
        m_psmBicListModel->setItem(i,4,bicItem);
        m_psmBicListModel->setHorizontalHeaderLabels(labels);
    }

    m_plvBicList->setColumnWidth(0, 35);
    m_plvBicList->setColumnWidth(1, 75);
    m_plvBicList->setColumnWidth(2, 60);
    m_plvBicList->setColumnWidth(3, 60);
    m_plvBicList->setColumnWidth(4, 75);
}
bool MainWindow::find(CellData *input, QString Bic)
{
    if(m_pmCellMap->contains(input->sCoords))
    {
        (*m_pmCellMap)[input->sCoords]->nCounts++;
        (*m_pmCellMap)[input->sCoords]->sBicluster.append(Bic);
        return true;
    }
    else
    {
        return false;
    }
}

bool MainWindow::findBic(CellData *input)
{
    /*if(m_pmBiclusterMap->contains(input->sBicluster))
    {
        if(!(*m_pmBiclusterMap)[input->sBicluster].contains(input))
            return true;
    }
    else
    {
        return false;
    }*/
}

void MainWindow::generateCountMap()
{
    for(int nCell = 0; nCell < m_pvData->size(); ++nCell)
    {
        if(!(*m_pmCountMap)[(*m_pvData)[nCell]->nCounts].contains((*m_pvData)[nCell]))
            (*m_pmCountMap)[(*m_pvData)[nCell]->nCounts].append((*m_pvData)[nCell]);
    }
}

void MainWindow::browseFile()
{
    m_plBiclusters->clear();
    clearData();
    for(int i=0; i<m_pvData->size(); i++)
        delete (*m_pvData)[i];
    m_pvData->clear();
    m_pmCountMap->clear();
    m_pmCellMap->clear();
    m_pmGOMap->clear();

    QStringList fileNames = QFileDialog::getOpenFileNames(this,tr("Open Data"), "./");
    for(int nF = 0; nF < fileNames.size(); nF++)
    {
        QString fileName = fileNames[nF];
        QFile dataFile(fileName);

        if (!dataFile.open(QIODevice::ReadOnly | QIODevice::Text))
                return;

        QTextStream in(&dataFile);

        QString line = in.readLine();
        if(line.contains("GDS"))
            line = in.readLine();
        QString ColLine;
        QStringList list,rows,cols;
        QString sBicluster;
        int fileType=0;
        if(line.contains("ROWS:"))
            fileType=3;
        else if(line.contains("|"))
            fileType =1;
        else
            fileType = 2;
       /* if(line.contains("|"))
            fileType=1;
        else
            line = in.readLine();*/
        while (!line.isNull())
        {
            if(line.contains("GDS"))
            {
                line = in.readLine();
                continue;
            }
            if(line.contains("GO:"))
            {
                if(!(*m_pmGOMap)[m_plBiclusters->last()].contains(line))
                    (*m_pmGOMap)[m_plBiclusters->last()].append(line);
                line = in.readLine();
                continue;
            }
            if(fileType==1)
            {
               //if(!line.contains("|"))
               //{
               //    line = in.readLine();
               //    continue;
               //}
               list = line.split("|");
               if(list.size()==1)
               {
                   line = in.readLine();
                   continue;
               }
               rows = list[0].split(QRegExp("\\s+"));
               rows.removeAll("");
               cols = list[1].split(QRegExp("\\s+"));
               cols.removeAll("");
               sBicluster = rows.join(" ") + "|" + cols.join(" ");

            }
            else if(fileType == 2)
            {
             //  line = in.readLine();
                if(line.isNull())
                    break;
                rows = (line.split(QRegExp("\\s+")));
                ColLine = in.readLine();
                if(ColLine.isNull())
                    break;
                cols = (ColLine.split(QRegExp("\\s+")));
                sBicluster = line+ "|" + ColLine;
            }
            else if(fileType == 3)
            {
              //  line = in.readLine();
                line = line.mid(5);
                if(line.isNull())
                    break;
                rows = (line.split(QRegExp("\\s+")));
                rows.removeAll("");
                ColLine = in.readLine();
                ColLine = ColLine.mid(5);
                if(ColLine.isNull())
                    break;
                cols = (ColLine.split(QRegExp("\\s+")));
                cols.removeAll("");
                sBicluster = rows.join(" ")+ "|" + cols.join(" ");

            }
            CellData* input = new CellData();
            if(!m_plBiclusters->contains(sBicluster))
            {
               for( int nEle = 0; nEle < rows.size();nEle++)
                {
                    for( int nCol = 0; nCol< cols.size();nCol++)
                    {
                        input->sCoords = rows[nEle] + "," +cols[nCol];
                        if(!find(input,sBicluster))
                        {
                            m_pvData->append(new CellData());
                            (*m_pvData)[m_pvData->size()-1]->nCounts = 1;
                            (*m_pvData)[m_pvData->size()-1]->sBicluster.append(sBicluster);
                            (*m_pvData)[m_pvData->size()-1]->sCoords = input->sCoords;
                            (*m_pvData)[m_pvData->size()-1]->sValue = input->sValue;
                            m_pmCellMap->insert((*m_pvData)[m_pvData->size()-1]->sCoords,(*m_pvData)[m_pvData->size()-1]);
                        }
                        delete input;
                        input = new CellData();
                    }
                }
                m_plBiclusters->append(sBicluster);
            }
            delete input;
            line = in.readLine();
        }
        dataFile.close();

    }
    //m_psmBicListModel->setStringList(*m_plBiclusters);
    prepareTableView();
    m_pbtnDraw->setEnabled(true);
    m_pbtnCalc->setEnabled(true);
    m_pbtnMore->setEnabled(true);
}

void MainWindow::showStats()
{
    if(m_pleGoDef->text().size()>0)
    {
        qSigmaWindow* win = new qSigmaWindow();
        win->setData(m_plValues,m_pleGoDef->text());
        win->show();
        int index=0;
        for(int i=0; i< m_plBiclusters->size(); i++)
        {
            if(m_plBiclusters->at(i) == m_pleGoDef->text())
            {
                index=i;
                break;
            }
        }
        qBicStats* win2 = new qBicStats();
        win2->setData(percVals, m_plBiclusters, index);
        win2->show();
    }
}

void MainWindow::quantityStats()
{
    generateCountMap();
    QVector<int> size(m_plBiclusters->size()), value(m_plBiclusters->size()), quantity(m_plBiclusters->size());
    QVector<double> result(m_plBiclusters->size()), uniques;
    QStringList tempBic;
    quantity.fill(1);
    value.fill(0);
    for(int i = 0; i < m_plBiclusters->size(); i++)
    {
        QStringList list = m_plBiclusters->at(i).split("|");
        QStringList rows = list[0].split(QRegExp("\\s+"));
        QStringList cols = list[1].split(QRegExp("\\s+"));
        for(int nR = 0; nR < rows.size();nR++)
        {
            for(int nC = 0; nC < cols.size();nC++)
            {
                QString coord = rows.at(nR) + "," + cols.at(nC);
                value[i] += (*m_pmCellMap)[coord]->nCounts;
            }
        }
        size[i] = rows.size()*cols.size();
        tempBic.append(m_plBiclusters->at(i));
    }
    QString filename="Data.txt";
    QFile file( filename );

    for(int i=0;i < value.size(); i++)
    {
        result[i] = value[i]/(float)size[i];
    }
    qs(&result,0,result.size()-1,&tempBic);
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );
        stream << tempBic.size() << endl;
        for(int i = 0; i < tempBic.size(); i++)
        {

            stream << tempBic.at(i)<< " ; " << "W/S = " << result[i] << " ; " << endl;
            if(!uniques.contains(result[i]))
            {
                uniques.append(result[i]);
            }
            else
            {
                quantity[i]+=1;
            }
        }
        stream << "Stats: " << endl;

        stream << "Result Quantity: " << endl;
        for(int i = 0; i < m_plBiclusters->size(); i++)
        {
            stream << "Val: " << result[i] << " ; "<< "Quantity: " << quantity[i] << endl;
        }
        stream << "Unique result: " << uniques.size() << endl;
        for(int i = 0; i < uniques.size(); i++)
        {
            stream << "Unique: " << uniques[i] << endl;
        }
    }
    file.close();
    qQuantityWin* quantityWin = new qQuantityWin();
    quantityWin->setData(size, result);
    quantityWin->show();
}

void MainWindow::percStats()
{
    clearPerc();
    percVals = new double*[m_plBiclusters->size()];

    boost::dynamic_bitset<>** rowsB = new boost::dynamic_bitset<>*[m_plBiclusters->size()];
    boost::dynamic_bitset<>** colsB = new boost::dynamic_bitset<>*[m_plBiclusters->size()];
    QElapsedTimer timer;
    timer.start();
    for(int i = 0; i < m_plBiclusters->size(); i++)
    {
        QStringList list = m_plBiclusters->at(i).split("|");
        QStringList rows = list[0].split(QRegExp("\\s+"));
        QStringList cols = list[1].split(QRegExp("\\s+"));
        rowsB[i] = new boost::dynamic_bitset<>(rowC+1);
        colsB[i] = new boost::dynamic_bitset<>(colC+1);
        percVals[i] = new double[m_plBiclusters->size()];
        for(int j=0; j< m_plBiclusters->size();j++)
            percVals[i][j] = 0;
        for(int j = 0; j < rows.size(); j++)
        {
            rowsB[i]->set(rows.at(j).toInt());
        }
        for(int j = 0; j < cols.size(); j++)
        {
            colsB[i]->set(cols.at(j).toInt());
        }
    }
    QString filename="Data.txt";
     QFile file( filename );
     if ( file.open(QIODevice::ReadWrite) )
     {
        QTextStream stream( &file );
        for(int i=0;i<m_plBiclusters->size(); i++)
        {
             stream << i<<endl;
                for(int k=0; k<m_plBiclusters->size(); k++)
                {

                    if(i!=k)
                    {
                        boost::dynamic_bitset<> commonRows(rowC+1);
                        commonRows = (*rowsB[i]) & (*rowsB[k]);
                            if(commonRows.count() > 0)
                            {
                                 boost::dynamic_bitset<> commonCols(colC+1);
                                 commonCols = (*colsB[i]) & (*colsB[k]);
                                        if(commonCols.count() > 0)
                                        {
                                          unsigned int intersect = commonRows.count() * commonCols.count();
                                          boost::dynamic_bitset<> subRows = (*rowsB[i]) - (*rowsB[k]);
                                          boost::dynamic_bitset<> subCols = (*colsB[i]) - (*colsB[k]);
                                          boost::dynamic_bitset<> subColsRev = (*colsB[k]) - (*colsB[i]);
                                          boost::dynamic_bitset<> subRowsRev = (*colsB[k]) - (*colsB[i]);
                                          boost::dynamic_bitset<> sumRows = (*rowsB[i]) | (*rowsB[k]);
                                          boost::dynamic_bitset<> sumCols = (*colsB[i]) | (*colsB[k]);
                                          unsigned int bitUnion = (sumRows.count()*sumCols.count())-(subRows.count()*subColsRev.count()) - (subRowsRev.count()*subCols.count());
                                          if(bitUnion != 0)
                                            percVals[i][k] = (double)intersect/(double)bitUnion;
                                        }
                            }
                    }
                    if(percVals[i][k]!=0 && percVals[i][k]>0.25)
                        stream << k << "-> " << percVals[i][k] << ";";
                }
                stream<<endl;
        }
        stream << timer.elapsed()<<endl;
    }
    file.close();
    qHistogram *hist = new qHistogram();
    hist->setData(percVals, m_plBiclusters->size());
    hist->show();
    m_pbtnParser->setEnabled(true);
}

void MainWindow::calcSaveSigma()
{
    QVector<double>* avg = new QVector<double>(m_plBiclusters->size());
    QVector<double> sigmas  = CalcSigma(m_plValues, m_plBiclusters,avg);
    QVector<int> indexes(m_plBiclusters->size());
    QString fileName = QFileDialog::getSaveFileName(this, "Save sigma", "", ".txt");
    fileName.append(".txt");
    QFile file(fileName);
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );
        for(int i=0; i<sigmas.size();i++)
        {
            indexes[i]=i;
            stream << m_plBiclusters->at(i) << ";" << "W=" << sigmas[i] << ";" << endl;
            (*avg)[i] = (*avg)[i]/sigmas[i];
        }
        qs3(avg, 0, avg->size()-1, &indexes);
        for(int i=0; i<sigmas.size(); i++)
        {
            stream << m_plBiclusters->at(indexes[i]) << " sig/avg=" << avg->at(i)<<";"<<endl;
        }
        file.close();
    }

}

void MainWindow::testStats()
{
    /*QString fileName = QFileDialog::getOpenFileName(this, tr("Open Data"), "./", tr(""));
    QFile dataFile(fileName);

    if (!dataFile.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
    QTextStream in(&dataFile);
    QString line = in.readLine();
    QStringList bics;
    bics.clear();
    while(!line.isNull())
    {
        if(line.contains("|"))
        {
            int n = line.indexOf(":");
            QString line2 = line.mid(n+1);
            QStringList list = line2.split("|");
            QStringList rows = list[0].split(QRegExp("\\s+"));
            rows.removeAll("");
            QStringList cols = list[1].split(QRegExp("\\s+"));
            cols.removeAll("");
            QString sBicluster = rows.join(" ") + "|" + cols.join(" ");
            if(!bics.contains(sBicluster))
            {
                bics.append(sBicluster);
            }
        }
        line = in.readLine();
    }
    dataFile.close();*/
    QString fileName = QFileDialog::getSaveFileName(this, "Save stats", "", ".txt");
    fileName.append(".txt");
    QFile file(fileName);
    QVector<int> index;
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );
      /*  for(int i=0; i<bics.size(); i++)
        {
            int ind  = m_plBiclusters->indexOf(bics[i]);
            if(ind >= 0)
            {
                index.append(ind);
                int occ = 0;
                double perc =0;
                stream << bics[i] << endl;
                QStringList list = bics[i].split("|");
                QStringList rows = list[0].split(QRegExp("\\s+"));
                QStringList cols = list[1].split(QRegExp("\\s+"));
                for(int f=0;f < m_plBiclusters->size();f++)
                {
                    if(percVals[ind][f]>0.25)
                    {
                        stream << percVals[ind][f] << ",";
                        occ++;
                    }
                }
                stream << endl;
                stream << occ << endl;
            }
        }*/
       // stream << "---------------------------------------" << endl;
        for(int i=0;i<m_plBiclusters->size();i++)
        {
           // if(!index.contains(i))
            {

                 int occ = 0;
                for(int f=0;f < m_plBiclusters->size();f++)
                {
                    if(percVals[i][f]>0.25)
                    {
                        occ++;
                    }
                }
                if(occ == 0)
                {
                    stream << (*m_plBiclusters)[i] << endl;
                    stream << (*m_pmGOMap)[(*m_plBiclusters)[i]].at(0)<<endl;
                   // stream << percVals[i][f] << ",";
                    //stream << endl;
                    //stream << occ << endl;
                }

            }
        }
    }
    file.close();
}

void MainWindow::browseMainFile()
{

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Data"), "./", tr(""));
    QFile dataFile(fileName);

    if (!dataFile.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
    clearData();
    m_plValues->clear();
    QTextStream in(&dataFile);    
    QString line = in.readLine();
    rowC=colC=0;
    QRegExp re("[-+]?[0-9]*\\.?[0-9]+.");  // a digit (\d), zero or more times (*)
    line = line.simplified();
    if(line.count(" ")==1)
    {

        QStringList list = line.split(QRegExp("\\s+"));
        rowC = list[0].toInt();
        colC = list[1].toInt();
        line = in.readLine();
        list = line.split(QRegExp("\\s+"));
        if (re.exactMatch(list[0]))
        {
            while(!line.isNull())
            {
                m_plValues->append(line.split(QRegExp("\\s+")));
                line=in.readLine();
            }
        }
        else
        {
            m_plColNames->append(list);
            line = in.readLine();
            while(!line.isNull())
            {
               list = line.split(QRegExp("\\s+"));
               m_plRowNames->append(list[0]);
               m_plValues->append(list.mid(1));
               line=in.readLine();
            }

        }

    }
    else
    {
        QStringList list = line.split(QRegExp("\\s+"));
        colC=list.size();
        if (re.exactMatch(list[0]))
        {
            while(!line.isNull())
            {
                m_plValues->append(line.split(QRegExp("\\s+")));
                line=in.readLine();
                rowC++;
            }
        }
        else
        {
            m_plColNames->append(list);
            line = in.readLine();
            while(!line.isNull())
            {
               list = line.split(QRegExp("\\s+"));
               m_plRowNames->append(list[0]);
               m_plValues->append(list.mid(1));
               line=in.readLine();
               rowC++;
            }

        }
    }


   // QRegExp re("[-+]?[0-9]*\\.?[0-9]+.");  // a digit (\d), zero or more times (*)
    //if (re.exactMatch(list[0]))
    {


    }
   // else
   // {
    //        m_plColNames->append(list);
    //    line = in.readLine();
    //    while(!line.isNull())
     //   {
     //       list = line.split(QRegExp("\\s+"));
     //       m_plRowNames->append(list[0]);
     //       m_plValues->append(list.mid(1));
       //     line=in.readLine();

       // }
   // }
    dataFile.close();
    m_pbtnBrowse->setEnabled(true);
    drawHeatmap();
    m_pbtnHeatSett->setEnabled(true);
}

void MainWindow::openBiclusterWin()
{
    qQuantityWin* quantityWin = new qQuantityWin();
    //m_pBiclusterWin->setBicluster((*m_pmBiclusterMap)[(*m_pvData)[m_pselectedCell->getNIndex()]->sBicluster]);
    quantityWin->show();
}
void MainWindow::draw()
{
    plot->xAxis->scaleRange(0.5);
/*
    plot->xAxis->setLabel("cols");
    plot->yAxis->setLabel("rows");
    // set axes ranges, so we see all data:
    if(m_plValues->size()>0)
    {
        plot->xAxis->setRange(0, colC);
        plot->yAxis->setRange(0, rowC);
        plot->xAxis->setNumberFormat("f");
        plot->yAxis->setNumberFormat("f");
        plot->xAxis->setNumberPrecision(0);
        plot->yAxis->setNumberPrecision(0);
        plot->axisRect()->setRangeZoom(Qt::Vertical);
        plot->axisRect()->setRangeDrag(Qt::Vertical);
        colorMap->setKeyAxis(plot->xAxis);
        colorMap->setValueAxis(plot->yAxis);
        colorMap->data()->setSize((*m_plValues)[0].size(),m_plValues->size());
        colorMap->data()->setRange(QCPRange(0, (*m_plValues)[0].size()),QCPRange(0, m_plValues->size()));
        QCPColorGradient* grad = new QCPColorGradient(QCPColorGradient::gpCold);
        grad->clearColorStops();
        grad->setColorStopAt(0,Qt::white);
        grad->setColorStopAt(1, QColor(155,0,0,255));
        grad->setColorInterpolation(QCPColorGradient::ciRGB);
        colorMap->setGradient(*grad);
        colorMap->setTightBoundary(false);
        colorMap->setInterpolate(false);
        colorMap->data()->fill(0);
        colorMap->data()->recalculateDataBounds();

        for(int nV =0;nV<m_plValues->size();nV++)
        {
            QStringList row = (*m_plValues)[nV];
            for(int nC = 0; nC<row.size();nC++)
            {
                QString coord = QString::number(nV)+ ","+QString::number(nC);
                if(m_pmCellMap->contains(coord))
                {
                    colorMap->data()->setCell(nC, nV, (*m_pmCellMap)[coord]->nCounts);
                }
            }
        }

        colorMap->rescaleDataRange(true);

        QCPColorScale *colorScale = new QCPColorScale(plot);
        colorScale->setGradient(*grad);
        colorScale->setDataRange(colorMap->dataRange());
        colorScale->setDataScaleType(colorMap->dataScaleType());
        QCPMarginGroup *group = new QCPMarginGroup(plot);
        colorScale->setMarginGroup(QCP::msTop|QCP::msBottom, group);
        plot->axisRect()->setMarginGroup(QCP::msTop|QCP::msBottom, group);
        plot->plotLayout()->addElement(0,1,colorScale);
        plot->replot();
    }
    // m_ptxtConsole->append(stream.str().c_str());
    */
}

void MainWindow::selectedList(QModelIndex index)
{
   QModelIndexList list = m_plvBicList->selectionModel()->selectedIndexes();

   QStringList slist;
   if(index.column()>3)
   {
       foreach(const QModelIndex &index, list)
       {
          // QModelIndex index2 = index.model()->index(index.row(), 3, index.parent());
           slist.append( index.data(Qt::DisplayRole ).toString());
       }
       if(slist.size()==1)
       {
          // drawBicluster(slist[0]);
       }
        qBicWin* win = new qBicWin();
        win->init(percVals,m_plValues,slist.last(),m_plRowNames, m_plColNames, m_plBiclusters, &(*m_pmGOMap)[slist.last()], rowC, colC);
        win->show();
   }

}
void MainWindow::drawBicluster(QString Bicluster)
{
   /* plot->addLayer("graphLayer");
    plot->addGraph(plot->xAxis,plot->yAxis);
    plot->graph()->setPen(QPen(Qt::black));
    plot->graph()->setLineStyle(QCPGraph::lsNone);
    plot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 15));
    plot->graph()->setLayer("graphLayer");
    plot->moveLayer(plot->layer("graphLayer"),plot->layer("main"));*/
    plot->addLayer("select",plot->layer("grid"));
    QCPColorGradient *grad = new QCPColorGradient(QCPColorGradient::gpCold);
    grad->clearColorStops();
    grad->setColorStopAt(0, QColor(255,255,255,10));
    grad->setColorStopAt(1, QColor(0,0,0,255));
    grad->setColorInterpolation(QCPColorGradient::ciRGB);
    QCPColorMap* Map = new QCPColorMap(plot->xAxis,plot->yAxis);
    Map->data()->setSize((*m_plValues)[0].size(),m_plValues->size());
    Map->data()->setRange(QCPRange(0, (*m_plValues)[0].size()),QCPRange(0, m_plValues->size()));
    Map->setGradient(*grad);
    Map->setTightBoundary(false);
    Map->setInterpolate(false);
    Map->data()->fill(0);
    Map->setLayer("select");
    QStringList list = Bicluster.split("|");
    QStringList rows = list[0].split(QRegExp("\\s+"));
    QStringList cols = list[1].split(QRegExp("\\s+"));
    int size = rows.size()*cols.size();
    for(int nR = 0; nR < rows.size();nR++)
    {
        for(int nC = 0; nC < cols.size();nC++)
        {
            QString row(rows.at(nR));
            QString col(cols.at(nC));
            int nRow=row.toInt();
            int nCol=col.toInt();
            QString coord = row+ ","+col;
            //if(m_pmCellMap->contains(coord))
            {
              Map->data()->setCell(nCol, nRow,8);

            }

        }
    }
    Map->data()->recalculateDataBounds();
    Map->rescaleDataRange(true);
    plot->replot();
    //colorMap->setVisible(false);
}

void MainWindow::drawHeatmap()
{
    //plot->xAxis->setLabel("Columns");
    //plot->yAxis->setLabel("Rows");
    // set axes ranges, so we see all data:
    if(m_plValues->size()>0)
    {
        plot->xAxis->setRange(0, colC);
        plot->yAxis->setRange(0, rowC*colC);
        plot->yAxis->setRangeReversed(true);
        plot->xAxis->setNumberFormat("f");
        plot->yAxis->setNumberFormat("f");
        plot->xAxis->setNumberPrecision(0);
        plot->yAxis->setNumberPrecision(0);

      //  plot->axisRect()->setRangeZoom(Qt::Vertical);
       //plot->axisRect()->setRangeDrag(Qt::Vertical);
        colorMap->data()->setSize(colC,rowC);
        colorMap->data()->setRange(QCPRange(0, colC),QCPRange(0,rowC));
        QCPColorGradient* grad = new QCPColorGradient(QCPColorGradient::gpSpectrum);
        grad->clearColorStops();
        grad->setColorStopAt(0,Qt::red);
        grad->setColorStopAt(0.5,Qt::black);
        grad->setColorStopAt(1, Qt::green);
        grad->setColorInterpolation(QCPColorGradient::ciRGB);
        colorMap->setGradient(*grad);
        colorMap->setTightBoundary(true);
        colorMap->setInterpolate(false);
        colorMap->data()->fill(0);
        colorMap->setDataScaleType(QCPAxis::stLogarithmic);
        for(int nV =0;nV<m_plValues->size();nV++)
        {
            QStringList row = (*m_plValues)[nV];
            for(int nC = 0; nC<row.size();nC++)
            {
                QString value = row.at(nC);
                colorMap->data()->setCell(nC, nV, value.toDouble());
            }
        }
        colorMap->rescaleDataRange(true);

        colorScale->setGradient(*grad);
        colorScale->setDataRange(colorMap->dataRange());
        colorScale->setDataScaleType(colorMap->dataScaleType());

        plot->xAxis->setRange(0, 10);
        plot->yAxis->setRange(0, 10);
        plot->replot();
    }
}

void MainWindow::select(QMouseEvent* event)
{
    if(colorMap!=NULL)
    {
        if(colorMap->selectTest(event->pos(),true))
        {
            int x = qRound(plot->xAxis->pixelToCoord(event->pos().x()));
            int y = qRound(plot->yAxis->pixelToCoord(event->pos().y()));

            std::stringstream stream;
            stream << y << "," << x;
           // m_ptxtConsole->append(stream.str().c_str());
            CellData* selectedData = (*m_pmCellMap)[QString(stream.str().c_str())];
            if(selectedData!=NULL)
            {
                for(int i=0; i<selectedData->sBicluster.size();i++)
                {
                    m_pslBicStrList->append(selectedData->sBicluster[i]);
                }
                if(m_pslBicStrList->size()>0)
                    drawBicluster(selectedData->sBicluster[0]);
                //m_psmBicListModel->setStringList(*m_pslBicStrList);
                m_pslBicStrList->clear();
            }

        }
    }
}

void MainWindow::setRange(QCPRange newRange)
{
    plot->xAxis->setRange(0, m_plValues->size());
}

void MainWindow::setGoText(QString text)
{
    goFilter->setGo(text);
    goFilter->invalidate();
}

void MainWindow::setOptions()
{
    optDialog = new OptionsDialog();
    optDialog->setGradientType(gradientType);
    optDialog->setDataScaleType(dataScaleType);
    optDialog->exec();
    if(optDialog->getStatus())
    {
        QCPColorGradient* grad;
        if(optDialog->getGradientType()!=100)
        {
            grad = new QCPColorGradient((QCPColorGradient::GradientPreset)optDialog->getGradientType());
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
        gradientType=optDialog->getGradientType();
        dataScaleType=optDialog->getDataScaleType();
        colorMap->setGradient(*grad);


        if(optDialog->getDataScaleType()==0)
            colorMap->setDataScaleType(QCPAxis::stLogarithmic);
        else
            colorMap->setDataScaleType(QCPAxis::stLinear);
        colorMap->rescaleDataRange(true);
        colorScale->setGradient(*grad);
        colorScale->setDataRange(colorMap->dataRange());
        colorScale->setDataScaleType(colorMap->dataScaleType());

    }
    delete optDialog;
    plot->replot();
}

void MainWindow::contextPlot(QPoint pos)
{
    QMenu *menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    menu->addAction("Save heatmap", this, SLOT(saveHeat()));

    menu->popup(plot->mapToGlobal(pos));
}

void MainWindow::saveHeat()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save graph", "", "*.pdf");
    fileName = fileName + ".pdf";
    plot->savePdf(fileName, 0, 0);
}

QGroupBox* MainWindow::createPaintGroup()
{
    QGridLayout* grid = new QGridLayout;
    QGroupBox* gb = new QGroupBox();
    QGroupBox* valGb = new QGroupBox(tr("Bicluster Values"));
    QGroupBox* percGb = new QGroupBox(tr("Heatmap"));
    percGb->setMinimumWidth(550);
    QGridLayout* valGrid = new QGridLayout();
    QGridLayout* percGrid = new QGridLayout();
    QSplitter* splitter = new QSplitter();
    plot = new QCustomPlot(this);
    plot->setMinimumWidth(500);
    plot->setInteractions(QCP::iRangeZoom|QCP::iRangeDrag);
    colorMap = new QCPColorMap(plot->xAxis,plot->yAxis);
    colorMap->data()->fill(0);

    colorScale = new QCPColorScale(plot);
    QCPMarginGroup *group = new QCPMarginGroup(plot);
    colorScale->setMarginGroup(QCP::msTop|QCP::msBottom, group);
    plot->axisRect()->setMarginGroup(QCP::msTop|QCP::msBottom, group);
    plot->plotLayout()->addElement(0,1,colorScale);
    plot->replot();

    QPushButton* m_pbtnRowPlus;
    QPushButton* m_pbtnRowMinus;
    QPushButton* m_pbtnColPlus;
    QPushButton* m_pbtnColMinus;

    m_pleGoDef = new QLineEdit();
    m_pbtnStats = new QPushButton(tr("Stats"));
    m_plvBicList = new QTableView(this);
    m_plvBicList->setMinimumWidth(300);
    m_plvBicList->horizontalHeader()->setStretchLastSection(true);
    m_plvBicList->setSortingEnabled(true);
    m_pslBicStrList = new QStringList();
    m_psmBicListModel = new QStandardItemModel();
    goFilter = new qGoFilter();
    goFilter->setGo("");
    goFilter->setSourceModel(m_psmBicListModel);
    m_plvBicList->setModel(goFilter);
    QLabel* text = new QLabel("GO Filter: ");
    percGrid->addWidget(plot,0,0,1,1);
    valGrid->addWidget(m_plvBicList,0,0,1,2);
    valGrid->addWidget(text, 1,0,1,1);
    valGrid->addWidget(m_pleGoDef, 1,1,1,1);
    valGb->setLayout(valGrid);
    percGb->setLayout(percGrid);

    splitter->addWidget(percGb);
    splitter->addWidget(valGb);
    grid->addWidget(splitter,0,0,1,2);
    //grid->addWidget(valGb, 0,1,1,1);
    gb->setLayout(grid);
    return gb;
}

QToolBar *MainWindow::createPathGroup()
{
    QToolBar* tools = new QToolBar("Files",this);
    m_pbtnBrowse = new QPushButton(tr("Biclusters"),this);
    m_pbtnBrowse->setFixedHeight(50);
    m_pbtnBrowse->setDisabled(true);
    m_pbtnBrowseMain = new QPushButton(tr("Dataset"),this);
    m_pbtnBrowseMain->setFixedHeight(50);
    m_pbtnMore = new QPushButton(tr("Occ Hist"),this);
    m_pbtnMore->setFixedHeight(50);
    m_pbtnMore->setDisabled(true);
    m_pbtnCalc = new QPushButton(tr("Coverage"), this);
    m_pbtnCalc->setFixedHeight(50);
    m_pbtnCalc->setDisabled(true);
    //m_pbtnSigma = new QPushButton(tr("S"), this);
    //m_pbtnSigma->setFixedSize(50,50);
    m_pbtnDraw = new QPushButton(tr("Occ heatmap"));
    m_pbtnDraw->setFixedHeight(50);
    m_pbtnDraw->setDisabled(true);
    m_pbtnParser = new QPushButton(tr("Save"), this);
    m_pbtnParser->setFixedHeight(50);
    m_pbtnParser->setDisabled(true);

    m_pbtnHeatSett = new QPushButton(tr("Heatmap Settings"), this);
    m_pbtnHeatSett->setFixedHeight(50);
    m_pbtnHeatSett->setDisabled(true);

    tools->addWidget(m_pbtnBrowseMain);
    tools->addWidget(m_pbtnBrowse);
    tools->addWidget(m_pbtnHeatSett);
    tools->addSeparator();
    tools->addWidget(m_pbtnCalc);
    tools->addWidget(m_pbtnParser);
    tools->addWidget(m_pbtnMore);
    tools->addWidget(m_pbtnDraw);
   // m_ptxtConsole = new QTextEdit(this);
  //  m_ptxtConsole->setReadOnly(true);
   // m_ptxtConsole->setMaximumWidth(220)
    return tools;

}
