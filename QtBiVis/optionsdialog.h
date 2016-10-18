#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QObject>
#include <QPushButton>
#include <QRadioButton>
#include <QGroupBox>
#include <QGridLayout>
#include <QComboBox>
#include "qcustomplot.h"


class OptionsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit OptionsDialog(QWidget *parent = 0);
    bool getStatus();
    void setGradientType(int type);
    int getGradientType();
    int getDataScaleType();
    void setDataScaleType(int type);
public slots:
    void setStatus();
private:
    QPushButton *ok;
    QPushButton *cancel;
    QRadioButton *classic;
    QRadioButton *grayScale;
    QRadioButton *hot;
    QRadioButton *cold;
    QRadioButton *ion;
    QRadioButton *thermal;
    QRadioButton *polar;
    QRadioButton *spectrum;
    QRadioButton *hues;
    QComboBox *interpolation;
    bool ready;
signals:
};

#endif // OPTIONSDIALOG_H
