#ifndef QBICLUSTERWINDOW_H
#define QBICLUSTERWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGroupBox>
#include <QGridLayout>
#include <QOpenGLWidget>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include "common.h"
#include "qplotwindow.h"
class qQuantityWin : public qPlotWindow
{
    Q_OBJECT
public:
    explicit qQuantityWin(QWidget *parent = 0);
    virtual void setData();
    void setData(QVector<int> size, QVector<double> result);
};

#endif // QBICLUSTERWINDOW_H
