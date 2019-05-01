#ifndef TILE_H
#define TILE_H

#include <QColor>
#include <QGraphicsItem>
#include "player.h"


class Tile : public QObject, public QGraphicsItem {

    Q_OBJECT

public:

    Tile();
    Tile(QColor color, const int x, const int y, int w, int h);   // constructor

    int get_x() { return x_; }
    int get_y() { return y_; }

    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    int get_width() { return width_; }
    int get_height() { return height_; }

    int stone, army, gold;
    int tileAlias;
    std::string tileAliasStr;

    int owner;

    bool castle;

    QColor color_;

signals:

    void TileClicked(Tile * t, Qt::MouseButton b);
    void TileShiftClicked(Tile * t, Qt::MouseButton b);

protected:

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:

    int x_;
    int y_;

    int width_;
    int height_;

};  // class Tile

#endif // TILE_H
