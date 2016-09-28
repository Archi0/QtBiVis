#ifndef COMMON
#define COMMON
#include <QList>
#include <QVector>
#include <stdlib.h>
#include "qcustomplot.h"

struct CellData
{
    QList<QString> sBicluster;
    QString sValue;
    QString sCoords;
    int nCounts;
};
void qs( QVector<double>* tab, int left, int right, QStringList* list );
void qs2( QVector<int>* tab, int left, int right, QVector<double>* list );
void qs3( QVector<double>* tab, int left, int right, QVector<int>* list);
QVector<double> CalcSigma(QList<QStringList>* values, QList<QString>* biclusters, QVector<double>* outAvg);
#endif // COMMON

