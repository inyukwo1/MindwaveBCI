#include "mainwindow.h"
#include "mindwaveconnector.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    MindwaveConnector mindwaveConnector;
    mindwaveConnector.printPackets("10hz4.txt");
    w.mindwave_connector = &mindwaveConnector;
    w.setLabel();
    w.update();
    w.show();

    return a.exec();
}
