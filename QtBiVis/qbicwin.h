#ifndef QBICWIN_H
#define QBICWIN_H

#include <QWidget>
#include <QString>
#include <QTableView>
#include <QPushButton>
#include <QGroupBox>
#include <QSplitter>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QSortFilterProxyModel>
#include <QGridLayout>
#include "qparallelplot.h"
#include "qcustomplot.h"

class qGoFilter;
class qBicWin : public QWidget
{
    Q_OBJECT
public:
    explicit qBicWin(QWidget *parent = 0);
    void init(double** perc, QList<QStringList>* vals, QString Bic, QStringList* rowN, QStringList* colN, QStringList* bics, QStringList* goStats, int rowsn, int colsn);

signals:
    void closed();
public slots:
    void goClose();
    void showParallelCords();
    void showStats();
    void showPerc();
    void sort();
protected:
    void draw();
    QPushButton* m_pbtnOkButton;
    QTableView* m_ptbValView;
    QPushButton* m_pbtnParallel;
    QStringList* m_biclusters;
    QTableView* m_ptbPercView;
    QTableView* m_ptbGoView;
    QPushButton* m_pbtnStats;
    QPushButton* m_pbtnPerc;
    QPushButton* m_pbtnSort;
    QStringList* colNames;
    QStringList* rowNames;
    QString m_bic;
    QStandardItemModel* m_pValModel;
    QStandardItemModel* m_pPercModel;
    QStandardItemModel* m_pGoModel;
    double** percVals;
    QList<QStringList>* m_plValues;
    QStringList* m_plGoStats;
    QCustomPlot* plot;
    QCPColorMap* colorMap;
    QCPCurve *curve;
    int colC;
    int rowC;

};
class qGoFilter : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    void setGo(QString go);
protected:
    QString m_go;
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
};

#endif // QBICWIN_H
