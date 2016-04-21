#ifndef QSIGMAWINDOW_H
#define QSIGMAWINDOW_H

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
class qSigmaWindow : public qPlotWindow
{
    Q_OBJECT
public:
    explicit qSigmaWindow(QWidget *parent = 0);
    virtual void setData();
    void setData(QList<QStringList>* values, QString Bicluster);
};

#endif // QSIGMAWINDOW_H
