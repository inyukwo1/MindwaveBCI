#include "bluebox.h"
#include "redbox.h"

#include <QBrush>

BlueBox::BlueBox(QGraphicsItem * parent): QGraphicsRectItem(parent)
{
    m_t_start = get_wall_time();
    setBrush(QBrush(Qt::blue));
    setRect(1695, 790, 200, 200);
}

void BlueBox::updateStatus() {
    double t_current = get_wall_time();
    double elapsed = t_current - m_t_start;

    int cnt = (int) (elapsed * 600) % 60;
    if (0 <= cnt && cnt < 20) {
        setBrush(QBrush(Qt::blue));
        setRect(1695, 790, 200, 200);
    } else if (20 <= cnt) {
        setRect(0, 0, 0, 0);
    }
}
