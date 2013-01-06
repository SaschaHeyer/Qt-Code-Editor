#ifndef PEERTOPEERHANDLER_H
#define PEERTOPEERHANDLER_H

#include "client.h"
#include "connection.h"
#include <QTimer>
#include <QUdpSocket>

class Client;
class Connection;

class PeerToPeerHandler : public QObject
{
    Q_OBJECT

public:
    PeerToPeerHandler(Client *client);

    void setServerPort(int port);
    QByteArray userName() const;
    void startBroadcasting();
    bool isLocalHostAddress(const QHostAddress &address);

signals:
    void newConnection(Connection *connection);

private slots:
    void sendBroadcastDatagram();
    void readBroadcastDatagram();

private:
    void updateAddresses();

    Client *client;
    QList<QHostAddress> broadcastAddresses;
    QList<QHostAddress> ipAddresses;
    QUdpSocket broadcastSocket;
    QTimer broadcastTimer;
    QByteArray username;
    int serverPort;
};

#endif // PEERTOPEERHANDLER_H
