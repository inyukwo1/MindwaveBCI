#include "bciscore.h"

#include <QFont>

BCIScore::BCIScore(QGraphicsItem *parent): QGraphicsTextItem(parent){
        // initialize the score to 0
        score = 50;

        // draw the text
        setPlainText(QString::number(score)); // Score: 0
        setDefaultTextColor(Qt::white);
        setFont(QFont("times", 24, 3));
}

void BCIScore::changeScore(int score) {
    this->score = score;
    setPlainText(QString::number(score));
}
