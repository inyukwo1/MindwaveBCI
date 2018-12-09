#include "Player.h"
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QPainter>
#include <iostream>
#include "Bullet.h"
#include <math.h>
#include "Enemy.h"
#include "Game.h"

extern Game* game;

Player::Player(QGraphicsItem *parent): QGraphicsPolygonItem(polygon(), parent) {
    angle = 0;
    vel_x = 0;
    vel_y = 0;
    pos_x = 960;
    pos_y = 490;
    left_pressed = right_pressed = up_pressed = down_pressed = false;
    bullet_ready = 0;
    mode = 0;
    waiting_bullet = 0;
}

void Player::bluemode() {
    mode = 1;
    bulletnum = 5;
    game->scene->removeItem(game->stadium);
    game->stadium = game->scene->addRect(QRectF(250, 50, 1420, 930), QPen(QBrush(QColor(0, 0, 200)), 5));
}

void Player::redmode() {
    if (mode == 0) {
        mode = 2;
        bulletnum = 5;
        game->scene->removeItem(game->stadium);
        game->stadium = game->scene->addRect(QRectF(250, 50, 1420, 930), QPen(QBrush(QColor(200, 0, 0)), 5));
    }
}

void Player::keyPressEvent(QKeyEvent *event){
    game->startGame();
    // move the player left and right
    if (event->key() == Qt::Key_Left){
        left_pressed = true;
    }
    if (event->key() == Qt::Key_Right){
        right_pressed = true;
    }
    if (event->key() == Qt::Key_Up){
        up_pressed = true;
    }
    if (event->key() == Qt::Key_Down){
        down_pressed = true;
    }
    if (event->key() == Qt::Key_Space){
        // create a bullet
        Bullet * bullet = new Bullet(bullet_ready, angle, mode);
        game->addBullet(bullet);
        if (mode != 0) {
            bulletnum--;
            if (bulletnum == 0) {
                game->scene->removeItem(game->stadium);
                game->stadium = game->scene->addRect(QRectF(250, 50, 1420, 930), QPen(QBrush(QColor(200, 200, 200)), 5));
                mode = 0;
            }
        }
        bullet->setPos(x() + 3, y());
        scene()->addItem(bullet);
        waiting_bullet++;
        QTimer::singleShot(1000, this, SLOT(waitmode()));
    }
}


void Player::keyReleaseEvent(QKeyEvent * event) {
    if (event->key() == Qt::Key_Left){
        left_pressed = false;
    }
    if (event->key() == Qt::Key_Right){
        right_pressed = false;
    }
    if (event->key() == Qt::Key_Up){
        up_pressed = false;
    }
    if (event->key() == Qt::Key_Down){
        down_pressed = false;
    }
}

void Player::spawn(){
    // create an enemy
    Enemy * enemy = new Enemy();
    scene()->addItem(enemy);
    game->addEnemy(enemy);
}

void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    if (mode == 0) {
        painter->setPen(Qt::white);
        painter->setBrush(Qt::white);
    } else if (mode == 1) {
        painter->setPen(Qt::blue);
        painter->setBrush(Qt::blue);
    } else {
        painter->setPen(Qt::red);
        painter->setBrush(Qt::red);
    }
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::HighQualityAntialiasing);
    painter->drawPolygon(polygon());
}

void Player::updateStatus() {
    float acc_val = 0.15;
    float fric_val = 0.98;
    float ang_vel = 7;

    int key_dir_x = 0;
    int key_dir_y = 0;
    if (left_pressed) {
        key_dir_x -= 1;
    }
    if (right_pressed) {
        key_dir_x += 1;
    }
    if (up_pressed) {
        key_dir_y -= 1;
    }
    if (down_pressed) {
        key_dir_y += 1;
    }

    vel_x += key_dir_x * acc_val;
    vel_y += key_dir_y * acc_val;
    vel_x *= fric_val;
    vel_y *= fric_val;

    pos_x += vel_x;
    pos_y += vel_y;

    if (pos_x < 250) {
        pos_x = 250;
    } else if (pos_x > 1670) {
        pos_x = 1670;
    }

    if (pos_y < 50) {
        pos_y = 50;
    } else if (pos_y > 980) {
        pos_y = 980;
    }

    int target_angle = -1;
    if (key_dir_x == 0) {
        if (key_dir_y == 0) {
            target_angle = -1;
        } else if (key_dir_y == 1) {
            target_angle = 180;
        } else {
            target_angle = 0;
        }
    } else if (key_dir_x == 1) {
        if (key_dir_y == 0) {
            target_angle = 90;
        } else if (key_dir_y == 1) {
            target_angle = 135;
        } else {
            target_angle = 45;
        }
    } else {
        if (key_dir_y == 0) {
            target_angle = 270;
        } else if (key_dir_y == 1) {
            target_angle = 225;
        } else {
            target_angle = 315;
        }
    }
    if (target_angle != -1) {
        if (abs(target_angle - angle) < ang_vel) {
            angle = target_angle;
        } else {
            if ((360 + target_angle - angle) % 360 < 180) {
                angle += ang_vel;
                angle = angle % 360;
            } else {
                angle += 360 - ang_vel;
                angle = angle % 360;
            }
        }
    }
    setPos(pos_x, pos_y);
    setRotation(angle);
}

QPolygonF Player::polygon() {
    return QPolygonF( QVector<QPointF>() << QPointF( 0, -6 ) << QPointF( -5, 6 ) << QPointF( 0, 4 ) << QPointF( 5, 6 ) << QPointF( 0, -6 ) );
}

void Player::waitmode() {
    waiting_bullet--;
}
