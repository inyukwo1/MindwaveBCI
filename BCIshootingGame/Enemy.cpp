#include "Enemy.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QList>
#include <QtMath>
#include <iostream>
#include <stdlib.h> // rand() -> really large int
#include "Game.h"

extern Game * game;

Enemy::Enemy(QGraphicsItem *parent): QObject(), QGraphicsPolygonItem(polygon(5), parent) {
    //set random position
    level = 5;
    int pos_x, pos_y;
    int pos_seed = rand() % 4;
    if (pos_seed == 0) {
        pos_x = 250 + radius();
        pos_y = 50 + radius();
    } else if (pos_seed == 1) {
        pos_x = 250 + radius();
        pos_y = 980 - radius();
    } else if (pos_seed == 2) {
        pos_x = 1670 - radius();
        pos_y = 980 - radius();
    } else {
        pos_x = 1670 - radius();
        pos_y = 50 + radius();
    }
    setPos(pos_x, pos_y);
    //set random color
    int seed = rand() % 100;
    if (seed < 80) {
        color = 0;
    } else if (seed < 88) {
        color = 2;
    } else {
        color = 1;
    }
    //set random velocity
    velx = rand() % 4 + 1;
    vely = rand() % 4 + 1;
    velang = (rand() % 5 + 1) * ((rand() % 2) * 2 - 1);
}

Enemy::Enemy(int posx, int posy, int velx, int vely, int velang, int level, QGraphicsItem * parent): QObject(), QGraphicsPolygonItem(polygon(level), parent){
    setPos(posx, posy);
    //set random color
    int seed = rand() % 100;
    if (seed < 80) {
        color = 0;
    } else if (seed < 88) {
        color = 2;
    } else {
        color = 1;
    }
    this->velx = velx;
    this->vely = vely;
    this->velang = velang;
    this->level = level;
}

void Enemy::updateStatus() {
    setPos(x() + velx, y() + vely);
    setRotation(rotation() + velang);
    if (x() < 250 + radius() || x() > 1670 - radius()) {
        velx = -velx;
        setPos(x() + velx, y());
    }
    if (y() < 50 + radius() || y() > 980 - radius()) {
        vely = -vely;
        setPos(x(), y() + vely);
    }
    if (collidesWithItem(game->player)) {
        game->decreaseHealth();
        destroy(-1);
    }
}

void Enemy::destroy(int bullet_angle) {
    if (bullet_angle == -1) {
        game->deleteEnemy(this);
        scene()->removeItem(this);
        delete this;
        return;
    }
    float vel_coef = (float)(rand() % 100 + 30) / 50;
    float radian1 = qDegreesToRadians((float)(bullet_angle + 90));
    float radian2 = qDegreesToRadians((float)(bullet_angle - 90));
    float vel1_x = velx + vel_coef * qCos(radian1);
    float vel1_y = vely + vel_coef * qSin(radian1);
    float vel2_x = velx + vel_coef * qCos(radian2);
    float vel2_y = vely + vel_coef * qSin(radian2);
    int velang1, velang2;
    while(true){
        velang1 = (rand() % 5 + 1) * ((rand() % 2) * 2 - 1);
        velang2 = velang - velang1;
        if (abs(velang1) < 6 &&  abs(velang2) < 6) {
            break;
        }
    }
    if (level == 5) {
        Enemy * enemy1 = new Enemy(x(), y(), vel1_x, vel1_y, velang1, 4);
        Enemy * enemy2 = new Enemy(x(), y(), vel2_x, vel2_y, velang2, 4);
        scene()->addItem(enemy1);
        scene()->addItem(enemy2);
        game->addEnemy(enemy1);
        game->addEnemy(enemy2);
    } else if (level == 4) {
        Enemy * enemy1 = new Enemy(x(), y(), vel1_x, vel1_y, velang1, 3);
        Enemy * enemy2 = new Enemy(x(), y(), vel2_x, vel2_y, velang2, 3);
        scene()->addItem(enemy1);
        scene()->addItem(enemy2);
        game->addEnemy(enemy1);
        game->addEnemy(enemy2);
    }
    game->deleteEnemy(this);
    scene()->removeItem(this);
    delete this;
}

void Enemy::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    if (color == 0) {
        painter->setPen(QPen(QBrush(Qt::white), 3));
    } else if (color == 1) {
        painter->setPen(QPen(QBrush(Qt::blue), 3));
    }  else {
        painter->setPen(QPen(QBrush(Qt::red), 3));
    }
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::HighQualityAntialiasing);
    painter->drawPolygon(polygon(level));
}

int Enemy::radius() {
    if (level == 5) {
        return 100;
    } else if (level == 4) {
        return 80;
    } else {
        return 60;
    }
}

QPolygonF Enemy::polygon(int level) {
    // based on https://www.mathopenref.com/coordpolycalc.html
    if (level == 5) {
        return QPolygonF( QVector<QPointF>() << QPointF( 0, -100 ) << QPointF( -96, -30 ) << QPointF( -58, 80 ) << QPointF( 58, 80 ) << QPointF( 96, -30 ) );
    } else if (level == 4) {
        return QPolygonF( QVector<QPointF>() << QPointF( -56, 56 ) << QPointF( 56, 56 ) << QPointF( 56, -56 ) << QPointF( -56, -56 ));
    } else if (level == 3) {
        return QPolygonF( QVector<QPointF>() << QPointF( 0, -60 ) << QPointF( -52, 30 ) << QPointF( 52, 30 ) );
    }
}
