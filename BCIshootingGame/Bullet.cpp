#include "Bullet.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QList>
#include <QtMath>
#include "Enemy.h"
#include "Game.h"

extern Game * game; // there is an external global object called game

Bullet::Bullet(int kind, int angle, int mode, QGraphicsItem * parent): QObject(), QGraphicsEllipseItem(parent){
    this->kind = kind;
    this->angle = qDegreesToRadians((float)(angle + 270));
    setRect(0, 0, 4, 4);
    if (mode == 0) {
        setBrush(QBrush(Qt::white));
    } else if (mode == 1) {
        setBrush(QBrush(Qt::blue));
    } else {
        setBrush(QBrush(Qt::red));
    }
    color = mode;
}

void Bullet::updateSatus() {
    setPos(x() + 5 * qCos(angle), y() + 5 * qSin(angle));
    // get a list of all the items currently colliding with this bullet
    QList<QGraphicsItem *> colliding_items = collidingItems();

    // if one of the colliding items is an Enemy, destroy both the bullet and the enemy
    for (int i = 0, n = colliding_items.size(); i < n; ++i){
        if (typeid(*(colliding_items[i])) == typeid(Enemy)){
            if (((Enemy*) colliding_items[i]) -> color == 1) {
                if (color != 1)
                    continue;
            }
            if (((Enemy*) colliding_items[i]) -> color == 2) {
                if (color != 2)
                    continue;
            }
            // increase the score
            game->increaseScore();

            // remove them from the scene (still on the heap)
            scene()->removeItem(this);
            ((Enemy*) colliding_items[i])->destroy(qRadiansToDegrees(angle));

            //TODO delete from game's bullet
            game->deleteBullet(this);
            delete this;
            return;
        }
    }

    // if the bullet is off the screen, destroy it
    if (pos().y() < 0){
        scene()->removeItem(this);
        game->deleteBullet(this);
        delete this;
    } else if (pos().x() < 250) {
        scene()->removeItem(this);
        game->deleteBullet(this);
        delete this;
    } else if (pos().x() > 1670) {
        scene()->removeItem(this);
        game->deleteBullet(this);
        delete this;
    } else if (pos().y() < 50) {
        scene()->removeItem(this);
        game->deleteBullet(this);
        delete this;
    } else if (pos().y() > 980) {
        scene()->removeItem(this);
        game->deleteBullet(this);
        delete this;
    }
}
