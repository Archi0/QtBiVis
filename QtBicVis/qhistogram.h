#ifndef QHISTOGRAM_H
#define QHISTOGRAM_H

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
class qHistogram : public qPlotWindow
{
    Q_OBJECT
public:
    explicit qHistogram(QWidget *parent = 0);
    virtual void setData();
    void setData(double** values, int size);
};

#endif // QHISTOGRAM_H
