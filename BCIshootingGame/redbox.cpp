#include "redbox.h"

#include <QBrush>

RedBox::RedBox(QGraphicsItem * parent): QGraphicsRectItem(parent)
{
    m_t_start = get_wall_time();
    setBrush(QBrush(Qt::red));
    setRect(25, 25, 200, 200);
}

void RedBox::updateStatus() {
    double t_current = get_wall_time();
    double elapsed = t_current - m_t_start;

    int cnt = (int) (elapsed * 600) % 50;
    if (0 <= cnt && cnt < 20) {
        setBrush(QBrush(Qt::red));
        setRect(25, 25, 200, 200);
    } else if (20 <= cnt) {
        setRect(0, 0, 0, 0);
    }
}
