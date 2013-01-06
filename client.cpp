#include <QtNetwork>

 #include "client.h"
 #include "connection.h"
 #include "peertopeerhandler.h"

 Client::Client()
 {
     peerHandler = new PeerToPeerHandler(this);
     peerHandler->setServerPort(server.serverPort());
     peerHandler->startBroadcasting();

     QObject::connect(peerHandler, SIGNAL(newConnection(Connection*)), this, SLOT(newConnection(Connection*)));
     QObject::connect(&server, SIGNAL(newConnection(Connection*)), this, SLOT(newConnection(Connection*)));
 }

 void Client::sendMessage(const QString &message)
 {
     if (message.isEmpty())
         return;

     QList<Connection *> connections = peers.values();
     foreach (Connection *connection, connections)
         connection->sendMessage(message);
 }

 void Client::sendCode(const QString &code)
 {
     if (code.isEmpty())
         return;

     QList<Connection *> connections = peers.values();
     foreach (Connection *connection, connections)
         connection->sendCode(code);
 }

 QString Client::nickName() const
 {
     return QString(peerHandler->userName()) + '@' + QHostInfo::localHostName()
            + ':' + QString::number(server.serverPort());
 }

 bool Client::hasConnection(const QHostAddress &senderIp, int senderPort) const
 {
     if (senderPort == -1)
         return peers.contains(senderIp);

     if (!peers.contains(senderIp))
         return false;

     QList<Connection *> connections = peers.values(senderIp);
     foreach (Connection *connection, connections) {
         if (connection->peerPort() == senderPort)
             return true;
     }

     return false;
 }

 void Client::newConnection(Connection *connection)
 {
     connection->setGreetingMessage(peerHandler->userName());

     connect(connection, SIGNAL(error(QAbstractSocket::SocketError)),
             this, SLOT(connectionError(QAbstractSocket::SocketError)));
     connect(connection, SIGNAL(disconnected()), this, SLOT(disconnected()));
     connect(connection, SIGNAL(readyForUse()), this, SLOT(readyForUse()));
 }

 void Client::readyForUse()
 {
     Connection *connection = qobject_cast<Connection *>(sender());
     if (!connection || hasConnection(connection->peerAddress(),
                                      connection->peerPort()))
         return;

     connect(connection, SIGNAL(newMessage(QString,QString)), this, SIGNAL(newMessage(QString,QString)));
    connect(connection, SIGNAL(newCode(QString,QString)), this, SIGNAL(newCode(QString,QString)));
     peers.insert(connection->peerAddress(), connection);
     QString nick = connection->name();
     if (!nick.isEmpty())
         emit newParticipant(nick);
 }

 void Client::disconnected()
 {
     if (Connection *connection = qobject_cast<Connection *>(sender()))
         removeConnection(connection);
 }

 void Client::connectionError(QAbstractSocket::SocketError /* socketError */)
 {
     if (Connection *connection = qobject_cast<Connection *>(sender()))
         removeConnection(connection);
 }

 void Client::removeConnection(Connection *connection)
 {
     if (peers.contains(connection->peerAddress())) {
         peers.remove(connection->peerAddress());
         QString nick = connection->name();
         if (!nick.isEmpty())
             emit participantLeft(nick);
     }
     connection->deleteLater();
 }
