#include "mainwindow.h"
#include "mindwaveconnector.h"
#include "client.h"
#include <QApplication>
#include <QtOpenGL/QGLFormat>

int main(int argc, char *argv[])
{
    QGLFormat glFormat(QGL::SampleBuffers);
    glFormat.setSwapInterval(1);
    qRegisterMetaType< std::vector<int> >( "std::vector<int>");
    QApplication a(argc, argv);
    Client client( "localhost", 4242 );
    MindwaveConnector mindwaveConnector;
    MainWindow w;
    w.client = &client;
    w.savemode = false;

    mindwaveConnector.sendmode = true;
    QObject::connect(&mindwaveConnector, &MindwaveConnector::toServer, &client,  &Client::sendToServer);
    mindwaveConnector.printPackets("vibe12_10_add_score_ihna11.txt");

    w.mindwave_connector = &mindwaveConnector;
    w.setLabel();
    w.updateSelf();
    w.show();

    return a.exec();
}
