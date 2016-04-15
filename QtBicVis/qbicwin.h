#ifndef QBICWIN_H
#define QBICWIN_H

#include <QWidget>
#include <QString>
#include <QTableView>
#include <QPushButton>
#include <QGroupBox>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QGridLayout>
#include "qparallelplot.h"

class qBicWin : public QWidget
{
    Q_OBJECT
public:
    explicit qBicWin(QWidget *parent = 0);
    void init(double** perc, QList<QStringList>* vals, QString Bic, QStringList* rowN, QStringList* colN, QStringList* bics);

signals:
    void closed();
public slots:
    void goClose();
    void showParallelCords();
    void showStats();
    void showPerc();
protected:
    QPushButton* m_pbtnOkButton;
    QTableView* m_ptbValView;
    QPushButton* m_pbtnParallel;
    QStringList* m_biclusters;
    QTableView* m_ptbPercView;
    QPushButton* m_pbtnStats;
    QPushButton* m_pbtnPerc;
    QStringList* colNames;
    QStringList* rowNames;
    QString m_bic;
    QStandardItemModel* m_pValModel;
    QStandardItemModel* m_pPercModel;
    double** percVals;
    QList<QStringList>* m_plValues;
};

#endif // QBICWIN_H
