#ifndef QPARALLELPLOT_H
#define QPARALLELPLOT_H

#include <QWidget>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGroupBox>
#include <QGridLayout>
#include <QOpenGLWidget>
#include <QGraphicsPixmapItem>
#include <QString>
#include <QPixmap>
#include "common.h"
#include "qplotwindow.h"

class qParallelPlot : public qPlotWindow
{
    Q_OBJECT
public:
    qParallelPlot();
    virtual void setData();
    void setData(QList<QStringList>* values, QString Bicluster, QCPRange range);
};

#endif // QPARALLELPLOT_H
