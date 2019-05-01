#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <QtWidgets>
#include "tile.h"
#include "player.h"
#include "castle.h"

Tile::Tile()
{

}

Tile::Tile(QColor color, const int x, const int y, int w, int h)
{
    this->color_ = color;
    x_ = x;
    y_ = y;

    width_ = w;
    height_ = h;

    owner = 0;
    castle = false;
}

QRectF Tile::boundingRect() const
{
    return QRectF(x_, y_, width_, width_);
}

QPainterPath Tile::shape() const
{
    QPainterPath path;
    path.addRect(x_, y_, width_, width_);
    return path;
}

void Tile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QBrush b = painter->brush();
    painter->setBrush(QBrush(color_.dark(option->state & QStyle::State_Sunken ? 120 : 100)));

    painter->drawRect(QRect(this->x_, this->y_, this->width_, this->width_));
    painter->setBrush(b);
}

void Tile::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "cell clicked ";

    if(event->modifiers() == Qt::ShiftModifier){
        qDebug() << "cell shift clicked";
        emit TileShiftClicked(this, Qt::LeftButton);
    }

    emit TileClicked(this, event->button());

    // optional logic
}
