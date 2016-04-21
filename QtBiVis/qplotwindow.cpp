#include "qplotwindow.h"

qPlotWindow::qPlotWindow(QWidget *parent) : QWidget(parent)
{
    plot = new QCustomPlot(this);
    plot->setMinimumWidth(550);
    plot->setInteractions(QCP::iRangeZoom|QCP::iRangeDrag);
    plot->axisRect()->setRangeZoom(Qt::Horizontal);

    QGridLayout* mainLayout = new QGridLayout(this);
    m_pbtnOkButton = new QPushButton(tr("Ok"));
    m_pbtnSave = new QPushButton(tr("Save"));
    mainLayout->addWidget(plot,0,0,2,2);
    mainLayout->addWidget(m_pbtnSave,2,0,1,2);
    mainLayout->addWidget(m_pbtnOkButton,3,0,1,2 );
    setLayout(mainLayout);
    connect(m_pbtnOkButton, SIGNAL(clicked()),this, SLOT(goClose()));
    connect(m_pbtnSave, SIGNAL(clicked()),this,SLOT(save()));
}
void qPlotWindow::save()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save graph", "", ".png");
    plot->savePng(fileName, 0, 0, 2.0);
}

void qPlotWindow::goClose()
{
    this->close();
    emit closed();
}
