#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGroupBox>
#include <QGridLayout>
#include <QThread>
#include <QFileDialog>
#include <QTextEdit>
#include <QList>
#include <QFile>
#include <QListView>
#include <QLineEdit>
#include <QLabel>
#include <QRectF>
#include <QTextStream>
#include <QStringList>
#include <qmath.h>
#include <QVector>
#include <QPair>
#include <QGraphicsRectItem>
#include <QOpenGLWidget>
#include <QStringListModel>
#include <QToolBar>
#include <QMap>
#include <QWheelEvent>
#include <QTableView>
#include <QPixmap>
#include <sstream>
#include <QStandardItemModel>
#include "ccell.h"
#include "qbiclusterwindow.h"
#include "qhistogram.h"
#include "qsigmawindow.h"
#include "qbicstats.h"
#include "common.h"
#include "qbicwin.h"
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void draw();
    void selectedList(QModelIndex index);
    void drawBicluster(QString Bicluster);
    void browseFile();
    void showStats();
    void quantityStats();
    void percStats();
    void calcSaveSigma();
    void testStats();
    void browseMainFile();
    void openBiclusterWin();
    void select(QMouseEvent* event);
    void setRange(QCPRange newRange);
    void setGoText(QString text);
private:

    bool find(TileData* input, QString Bic);
    bool findBic(TileData* input);
    void generateCountMap();
    void clearPerc();
    void clearData();
    void prepareTableView();
    qQuantityWin* m_pQuantityWin;
    QPushButton* m_pbtnBrowse;
    QPushButton* m_pbtnBrowseMain;
    QPushButton* m_pbtnCalc;
    QPushButton* m_pbtnMore;
    QPushButton* m_pbtnSigma;
    QPushButton* m_pbtnParser;
    QPushButton* m_pbtnDraw;
    QPushButton* m_pbtnStats;
    QLineEdit* m_peSourcePath;
    QLineEdit* m_pleGoDef;
    QLabel* m_plblSourceMain;
    QTextEdit* m_ptxtConsole;
    QTableView* m_plvBicList;
    QStandardItemModel* m_psmBicListModel;
    QStringList* m_pslBicStrList;
    QCustomPlot* plot;
    double** percVals;
    //QGroupBox* m_pVisualGroup;
   // QGroupBox* m_pInfoGroup;
    QGridLayout* m_pMainLayout;
    QVector<TileData* >* m_pvData;
    QStringList* m_plBiclusters;
    QMap<int,QVector<TileData*> >* m_pmCountMap;
    QStringList* m_plRowNames;
    QStringList* m_plColNames;
   QMap<QString, TileData*>* m_pmCellMap;
    QMap<QString, QStringList>* m_pmGOMap;
    QGroupBox* createPaintGroup();
    QToolBar *createPathGroup();
    QList<QStringList>* m_plValues;
    QCPColorMap *colorMap;
    qBicWin* m_bicWin;
    qGoFilter* goFilter;
};

#endif // MAINWINDOW_H
