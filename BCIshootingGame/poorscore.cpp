#include "poorscore.h"
#include <QFont>

PoorScore::PoorScore(QGraphicsItem *parent): QGraphicsTextItem(parent){
    // draw the text
    setPlainText(QString("Poor: ") + QString::number(200)); // Health: 3
    setDefaultTextColor(Qt::red);
    setFont(QFont("times",16));
}

void PoorScore::changeval(int score) {
    setPlainText(QString("Poor: ") + QString::number(score));
}
