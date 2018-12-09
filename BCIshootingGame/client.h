#ifndef CLIENT_H
#define CLIENT_H

#include "thread"
#include "cstdlib"
#include "Windows.h"

#include <qtcpsocket.h>
#include <qapplication.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <qtextedit.h>
#include <QThread>
#include <QTimer>
#include <qlineedit.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qtextstream.h>
#include <iostream>


class Client : public QObject
{
    Q_OBJECT
public:
    Client( const QString &host, uint port )
    {
        // create the socket and connect various of its signals
        socket = new QTcpSocket( this );
        connect( socket, SIGNAL(connected()),
                SLOT(socketConnected()) );
        connect( socket, SIGNAL(disconnected()),
                SLOT(socketConnectionClosed()) );
        connect( socket, SIGNAL(readyRead()),
                SLOT(socketReadyRead()) );
        connect( socket, SIGNAL(error(QAbstractSocket::SocketError)),
                SLOT(socketError(QAbstractSocket::SocketError)) );

        // connect to the server
        std::cout << "trying to connect to the server" << std::endl;
        socket->connectToHost( host, port );
        last_result[0] = last_result[1] = last_result[2] = 0;
        connected = false;
    }

    ~Client()
    {
    }

public slots:
    void sendToServer(std::vector<int> array)
    {
        if (!connected) {
            return;
        }
        assert(array.size() == 520);
        // write to the server
        socket->write((char*)array.data(), 4 * 520);
    }
private slots:
    void closeConnection()
    {
        socket->close();
        if ( socket->state() == QAbstractSocket::ClosingState ) {
            // We have a delayed close.
            connect( socket, SIGNAL(delayedCloseFinished()),
                    SLOT(socketClosed()) );
        } else {
            // The socket is closed.
            socketClosed();
        }
    }    

    void socketReadyRead()
    {
        // TODO need lock?
        int size_read = 0;
        while(size_read < 12) {
            QByteArray read = socket->read(12);
            memcpy(((char*)last_result) + size_read, read.data(), read.size());
            size_read += read.size();
        }
    }

    void socketConnected()
    {
        connected = true;
        std::cout << "Connected to server" << std::endl;
    }

    void socketConnectionClosed()
    {
        std::cout << "Connection closed by the server" << std::endl;
    }

    void socketClosed()
    {
        std::cout << "Connection closed" << std::endl;
    }

    void socketError( QAbstractSocket::SocketError e )
    {
        std::cout << "Error occurred" << std::endl;
    }
public:
    float last_result[3];
private:
    QTcpSocket *socket;
    QTextEdit *infoText;

    bool connected;
};

#endif // CLIENT_H
