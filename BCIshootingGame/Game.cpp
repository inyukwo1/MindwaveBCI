#include "Game.h"
#include "redbox.h"
#include <QTimer>
#include <QGraphicsTextItem>
#include <QFont>
#include <QBrush>
#include <QImage>

Game::Game(QWidget *parent){
    // create the scene
    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, 1920, 1080);
    setBackgroundBrush(QBrush(QColor(0, 0, 0)));

    // make the newly created scene the scene to visualize (since Game is a QGraphicsView Widget,
    // it can be used to visualize scenes)
    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(1920, 1080);

    // create the player
    player = new Player();
    player->setPos(960,490);
    // make the player focusable and set it to be the current focus
    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();
    // add the player to the scene
    scene->addItem(player);
    // draw stadium
    stadium = scene->addRect(QRectF(250, 50, 1420, 930), QPen(QBrush(QColor(200, 200, 200)), 5));

    // create the score/health
    score = new Score();
    score->setPos(900, 10);
    scene->addItem(score);
    redbox = new RedBox();
    scene->addItem(redbox);
    bluebox = new BlueBox();
    scene->addItem(bluebox);
    health = new Health();
    health->setPos(1000, 12);
    scene->addItem(health);

    poorscore = new PoorScore();
    poorscore->setPos(800, 12);
    scene->addItem(poorscore);

    red_score = new BCIScore();
    red_score->setPos(105, 100);
    scene->addItem(red_score);

    blue_score = new BCIScore();
    blue_score->setPos(1775, 882);
    scene->addItem(blue_score);

    start_button = new QGraphicsTextItem("Space to Start");
    start_button->setFont(QFont("times", 120));
    start_button->setDefaultTextColor(Qt::white);
    start_button->setPos(440, 400);
    scene->addItem(start_button);

    started = false;
    QTimer * timer = new QTimer();
    gameTimer = new QTimer();
    QObject::connect(gameTimer,SIGNAL(timeout()),this,SLOT(updateStatus()));
    gameTimer->start(5);
    game_counter = 0;
    show();
}

void Game::addBullet(Bullet* bullet) {
    bullets.push_back(bullet);
}

void Game::deleteBullet(Bullet* bullet) {
    //TODO can be cause performance issue
    bullets.remove(bullet);
}

void Game::addEnemy(Enemy* enemy) {
    enemies.push_back(enemy);
}

void Game::deleteEnemy(Enemy* enemy) {
    //TODO can be cause performance issue
    enemies.remove(enemy);

}

void Game::increaseScore() {
    score->increase();
}

void Game::decreaseHealth() {
    health->decrease();
    if (health->getHealth() == 0) {
        delete gameTimer;
        QGraphicsTextItem *gameover_text = scene->addText("Game Over", QFont("times", 120));
        gameover_text->setDefaultTextColor(Qt::white);
        gameover_text->setPos(550, 400);
    }
}

void Game::updateStatus() {
    redbox->updateStatus();
    bluebox->updateStatus();
    red_score->changeScore(client->last_result[0] * 100);
    blue_score->changeScore(client->last_result[2] * 100);
    poorscore->changeval(mindwave_connector->getValue(TG_DATA_POOR_SIGNAL));
    if (!started)
        return;
    player->updateStatus();
    auto bulletit = bullets.begin();
    while(bulletit != bullets.end()) {
        Bullet* bullet = *bulletit;
        bulletit++;
        bullet->updateSatus();
    }
    auto enemyit = enemies.begin();
    while(enemyit != enemies.end()) {
        Enemy* enemy = *enemyit;
        enemyit++;
        enemy->updateStatus();
    }

    if (client->last_result[0] > 0.67 && player->waiting_bullet == 0) {
        player->redmode();
    }
    if (client->last_result[2] > 0.90 && player->waiting_bullet == 0) {
        player->bluemode();
    }
    if (game_counter % 1000 == 0)
        player->spawn();
    game_counter++;
}

void Game::startGame() {
    if (started)
        return;
    scene->removeItem(start_button);
    started = true;
}
