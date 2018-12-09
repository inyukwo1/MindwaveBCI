#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsPolygonItem>
#include <QObject>
#include <QGraphicsItem>
#include <atomic>

class Player: public QObject, public QGraphicsPolygonItem {
    Q_OBJECT
public:
    Player(QGraphicsItem * parent=0);
    void bluemode();
    void redmode();
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent * event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void updateStatus();
    std::atomic_int waiting_bullet;
private:
    int mode;
    int bulletnum;
    QPolygonF polygon();
    int angle;
    float vel_x;
    float vel_y;
    float pos_x;
    float pos_y;
    bool left_pressed;
    bool right_pressed;
    bool up_pressed;
    bool down_pressed;
    int bullet_ready;
public slots:
    void spawn();
    void waitmode();
};

#endif // PLAYER_H
