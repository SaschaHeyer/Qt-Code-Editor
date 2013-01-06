#ifndef CLIENT_H
 #define CLIENT_H

 #include <QAbstractSocket>
 #include <QHash>
 #include <QHostAddress>

 #include "server.h"
#include "peertopeerhandler.h"

 class PeerToPeerHandler;

 class Client : public QObject
 {
     Q_OBJECT

 public:
     Client();

     void sendMessage(const QString &message);
     void sendCode(const QString &code);
     QString nickName() const;
     bool hasConnection(const QHostAddress &senderIp, int senderPort = -1) const;
     void loadConnectionsNew();
 signals:
     void newMessage(const QString &from, const QString &message);
     void newCode(const QString &from, const QString &message);
     void newParticipant(const QString &nick);
     void participantLeft(const QString &nick);

 private slots:
     void newConnection(Connection *connection);
     void connectionError(QAbstractSocket::SocketError socketError);
     void disconnected();
     void readyForUse();

 private:
     void removeConnection(Connection *connection);

     PeerToPeerHandler *peerHandler;
     Server server;
     QMultiHash<QHostAddress, Connection *> peers;
     QList<Connection *> connections;
 };

 #endif
