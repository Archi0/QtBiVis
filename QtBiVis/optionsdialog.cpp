#include "optionsdialog.h"

OptionsDialog::OptionsDialog(QWidget *parent)
{
    setWindowTitle(tr("Heatmap settings"));
    ok = new QPushButton(tr("Ok"),this);
    cancel = new QPushButton(tr("Cancel"),this);
    classic  = new QRadioButton(tr("Classic"), this);
    classic->setChecked(true);
    grayScale = new QRadioButton(tr("Grayscale"), this);
    hot = new QRadioButton(tr("Hot"), this);
    cold = new QRadioButton(tr("Cold"), this);
    ion = new QRadioButton(tr("Ion"), this);
    thermal = new QRadioButton(tr("Thermal"), this);
    polar = new QRadioButton(tr("Polar"));
    spectrum = new QRadioButton(tr("Spectrum"));
    hues = new QRadioButton(tr("Hues"));
    interpolation = new QComboBox(this);

    interpolation->addItem(tr("Logarithmic"));
    interpolation->addItem(tr("Linear"));

    QGroupBox *gb = new QGroupBox(tr("Gradient Type: "),this);
    QGroupBox *gb2 = new QGroupBox(tr("Data Scale Type: "),this);
    QGridLayout *mainGrid = new QGridLayout(this);
    QGridLayout *grid = new QGridLayout(gb);
    QGridLayout *grid2 = new QGridLayout(gb2);

    grid->addWidget(classic, 0,0,1,1);
    grid->addWidget(grayScale, 1,0,1,1);
    grid->addWidget(hot, 2,0,1,1);
    grid->addWidget(cold, 3,0,1,1);
    grid->addWidget(ion, 4,0,1,1);
    grid->addWidget(thermal, 5,0,1,1);
    grid->addWidget(polar, 6,0,1,1);
    grid->addWidget(spectrum, 7,0,1,1);
    grid->addWidget(hues, 8,0,1,1);
    grid->setAlignment(Qt::AlignJustify);

    grid2->addWidget(interpolation,0,0,1,1);

    mainGrid->addWidget(gb,0,0,2,1);
    mainGrid->addWidget(gb2,0,1,1,1);
    mainGrid->addWidget(ok,2,0,1,1);
    mainGrid->addWidget(cancel,2,1,1,1);

    connect(ok,SIGNAL(clicked()),this,SLOT(accept()));
    connect(ok,SIGNAL(clicked()),this,SLOT(setStatus()));
    connect(cancel,SIGNAL(clicked()),this,SLOT(reject()));

}
bool OptionsDialog::getStatus()
{
    return ready;
}

void OptionsDialog::setGradientType(int type)
{
    switch(type)
    {
    case QCPColorGradient::gpGrayscale:
        grayScale->setChecked(true);
        break;
    case QCPColorGradient::gpCold:
        cold->setChecked(true);
        break;
    case QCPColorGradient::gpHot:
        hot->setChecked(true);
        break;
    case QCPColorGradient::gpHues:
        hues->setChecked(true);
        break;
    case QCPColorGradient::gpIon:
        ion->setChecked(true);
        break;
    case QCPColorGradient::gpSpectrum:
        spectrum->setChecked(true);
        break;
    case QCPColorGradient::gpPolar:
        polar->setChecked(true);
        break;
    case QCPColorGradient::gpThermal:
        thermal->setChecked(true);
        break;
    default:
        classic->setChecked(true);
    }
}

int OptionsDialog::getGradientType()
{
    if(classic->isChecked())
        return 100;
    if(cold->isChecked())
        return QCPColorGradient::gpCold;
    if(hot->isChecked())
        return QCPColorGradient::gpHot;
    if(hues->isChecked())
        return QCPColorGradient::gpHues;
    if(thermal->isChecked())
        return QCPColorGradient::gpThermal;
    if(ion->isChecked())
        return QCPColorGradient::gpIon;
    if(spectrum->isChecked())
        return QCPColorGradient::gpSpectrum;
    if(grayScale->isChecked())
        return QCPColorGradient::gpGrayscale;
    if(polar->isChecked())
        return QCPColorGradient::gpPolar;
    return 100;
}

int OptionsDialog::getDataScaleType()
{
    return interpolation->currentIndex();
}

void OptionsDialog::setDataScaleType(int type)
{
    interpolation->setCurrentIndex(type);
}
void OptionsDialog::setStatus()
{
    ready=true;
}

