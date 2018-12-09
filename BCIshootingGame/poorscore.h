#ifndef POORSCORE_H
#define POORSCORE_H

#include <QGraphicsTextItem>

class PoorScore: public QGraphicsTextItem{
public:
    PoorScore(QGraphicsItem * parent=0);
    void changeval(int score);
};

#endif // POORSCORE_H
