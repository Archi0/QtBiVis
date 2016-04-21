#ifndef CCELL_H
#define CCELL_H

#include <QGraphicsItem>
#include <QString>
#include <QColor>
#include <QStyle>
#include <QPen>
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QStyleOption>
#include <QPainter>
class CCell : public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
signals:
    void selected(CCell* cell);
public:
    CCell();
    QString getSValue() const;
    bool getState() const;
    void setSValue(const QString &value);
    void setState(bool s);
    void select();
    void unselect();
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    QString getSNumbers() const;
    void setSNumbers(const QString &value);
   // QRectF boundingRect() const;
    //void paint(QPainter *painter,const QStyleOptionGraphicsItem *option, QWidget *widget);
    int getNIndex() const;
    void setNIndex(int value);

    QColor getColor() const;
    void setColor(const QColor &color);

private:
    bool bState;
    int nIndex;
    QRectF* m_pRect;
    QColor m_color;
    QString sValue;
    QString sNumbers;
    QColor cBgColor;
    QColor cLineColor;
};

#endif // CCELL_H
