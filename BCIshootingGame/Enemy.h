#ifndef ENEMY_H
#define ENEMY_H

#include <QGraphicsPolygonItem>
#include <QObject>
#include <QGraphicsItem>

class Enemy: public QObject,public QGraphicsPolygonItem {
    Q_OBJECT
public:
    Enemy(QGraphicsItem * parent=0);
    Enemy(int posx, int posy, int velx, int vely, int velang, int level, QGraphicsItem * parent=0);
    void updateStatus();
    void destroy(int bullet_angle);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int color;
private:
    QPolygonF polygon(int level);
    int radius();
    int velx, vely, velang;
    int level;
};

#endif // ENEMY_H
