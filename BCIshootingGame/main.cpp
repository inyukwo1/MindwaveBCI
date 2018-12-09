#include "mindwaveconnector.h"
#include "Game.h"
#include "client.h"
#include "thinkgear.h"
#include <QApplication>
#include <QtOpenGL/QGLFormat>

Game * game;

int main(int argc, char *argv[]){
    srand(time(NULL));
    QGLFormat glFormat(QGL::SampleBuffers);
    glFormat.setSwapInterval(1);
    qRegisterMetaType< std::vector<int> >( "std::vector<int>");
    QApplication a(argc, argv);
    Client* client = new Client( "localhost", 4242 );
    MindwaveConnector* mindwaveConnector = new MindwaveConnector();

    QObject::connect(mindwaveConnector, &MindwaveConnector::toServer, client,  &Client::sendToServer);
    mindwaveConnector->printPackets();
    game = new Game();
    game->mindwave_connector = mindwaveConnector;
    game->client = client;
    game->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers | QGL::DirectRendering)));
    game->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    game->show();

    return a.exec();
}
