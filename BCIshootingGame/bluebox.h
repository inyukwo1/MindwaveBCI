#ifndef BLUEBOX_H
#define BLUEBOX_H

#include <QGraphicsRectItem>
#include "Windows.h"

class BlueBox: public QGraphicsRectItem
{
public:
    BlueBox(QGraphicsItem * parent=0);
    void updateStatus();
    double get_wall_time(){
        LARGE_INTEGER time,freq;
        if (!QueryPerformanceFrequency(&freq)){
            //  Handle error
            return 0;
        }
        if (!QueryPerformanceCounter(&time)){
            //  Handle error
            return 0;
        }
        return (double)time.QuadPart / freq.QuadPart;
    }
private:
    bool showing;
    double m_t_start;
};


#endif // BLUEBOX_H
