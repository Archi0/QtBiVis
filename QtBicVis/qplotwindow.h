#ifndef QPLOTWINDOW_H
#define QPLOTWINDOW_H


#include <QWidget>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGroupBox>
#include <QGridLayout>
#include <QOpenGLWidget>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QFileDialog>
#include "common.h"

class qPlotWindow : public QWidget
{
    Q_OBJECT
public:
    explicit qPlotWindow(QWidget *parent = 0);
    virtual void setData() = 0;
signals:
    void closed();
public slots:
    void goClose();
    void save();
protected:
    QPushButton* m_pbtnOkButton;
    QPushButton* m_pbtnSave;
    QCustomPlot* plot;
    QCPBars* bars;
};

#endif // QPLOTWINDOW_H
