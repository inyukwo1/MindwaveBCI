#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QObject>

class Bullet: public QObject,public QGraphicsEllipseItem {
    Q_OBJECT
public:
    Bullet(int kind, int angle, int mode, QGraphicsItem * parent=0);
    void updateSatus();
private:
    float angle;
    int kind;
    int color;
};

#endif // BULLET_H
