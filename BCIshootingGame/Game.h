#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsScene>
#include <QObject>
#include <QPushButton>
#include "Player.h"
#include "Bullet.h"
#include "Score.h"
#include "Health.h"
#include "Enemy.h"
#include "redbox.h"
#include "bluebox.h"
#include "bciscore.h"
#include "mindwaveconnector.h"
#include "client.h"
#include "poorscore.h"

class Game: public QGraphicsView {
    Q_OBJECT
public:
    Game(QWidget * parent=0);
    void addBullet(Bullet* bullet);
    void deleteBullet(Bullet* bullet);
    void addEnemy(Enemy* enemy);
    void deleteEnemy(Enemy* enemy);
    void increaseScore();
    void decreaseHealth();
    void startGame();
    Player * player;
    QGraphicsScene * scene;
    BCIScore *red_score;
    BCIScore *blue_score;
    MindwaveConnector *mindwave_connector;
    Client* client;
    QGraphicsRectItem* stadium;
private:
    std::list<Bullet*> bullets;
    std::list<Enemy*> enemies;
    RedBox* redbox;
    BlueBox* bluebox;
    Score * score;
    PoorScore * poorscore;
    Health * health;
    QTimer * gameTimer;
    int game_counter;
    QGraphicsTextItem* start_button;
    bool started;
public slots:
    void updateStatus();
};

#endif // GAME_H
