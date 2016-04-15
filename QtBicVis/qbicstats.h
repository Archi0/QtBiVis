#ifndef QBICSTATS_H
#define QBICSTATS_H

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

class qBicStats : public qPlotWindow
{
    Q_OBJECT
public:
    explicit qBicStats();
    virtual void setData();
    void setData(double **values, QList<QString>* biclusters, int index);
};

#endif // QBICSTATS_H
