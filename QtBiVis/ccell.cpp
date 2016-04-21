#include "ccell.h"

CCell::CCell()
{
    sValue = "";
    sNumbers = "";
    bState =false;
  //  m_pRect = new QRectF(0,0,8,8);
   // setFlags(ItemIsSelectable);
}

QString CCell::getSValue() const
{
    return sValue;
}

void CCell::setSValue(const QString &value)
{
    sValue = value;
}

void CCell::setState(bool s)
{
    bState = s;
}

void CCell::select()
{
    QPixmap* pix;
    if(!bState)
    {
        pix = new QPixmap(10,10);
        pix->fill(Qt::red);
        bState = true;
        this->setPixmap(*pix);
    }

}

void CCell::unselect()
{
    QPixmap* pix;
    if(bState)
    {
        pix = new QPixmap(10,10);
        pix->fill(m_color);
        bState = false;
        this->setPixmap(*pix);
    }

}

void CCell::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    emit selected(this);
}

QString CCell::getSNumbers() const
{
    return sNumbers;
}
bool CCell::getState() const
{
    return bState;
}

void CCell::setSNumbers(const QString &value)
{
    sNumbers = value;
}

int CCell::getNIndex() const
{
    return nIndex;
}

void CCell::setNIndex(int value)
{
    nIndex = value;
}

QColor CCell::getColor() const
{
    return m_color;
}

void CCell::setColor(const QColor &color)
{
    m_color = color;
}


/*
void CCell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen(cLineColor);
    if (option->state & QStyle::State_Selected) {
        pen.setStyle(Qt::DotLine);
        pen.setWidth(2);
        emit selected();
    }

    painter->setPen(pen);
    painter->setBrush(cBgColor);


    painter->drawRect(*m_pRect);
}*/
