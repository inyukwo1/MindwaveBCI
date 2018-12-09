#ifndef REDSCORE_H
#define REDSCORE_H

#include <QGraphicsTextItem>

class BCIScore: public QGraphicsTextItem {
public:
    BCIScore(QGraphicsItem * parent=0);
    void changeScore(int score);
private:
    int score;
};

#endif // REDSCORE_H
