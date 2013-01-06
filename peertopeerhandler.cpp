#include "peertopeerhandler.h"

#include <QtNetwork>

 #include "client.h"
 #include "connection.h"


 static const qint32 BroadcastInterval = 2000;
 static const unsigned broadcastPort = 45000;

 PeerToPeerHandler::PeerToPeerHandler(Client *client)
     : QObject(client)
 {
     this->client = client;

     QStringList envVariables;
     envVariables << "USERNAME.*" << "USER.*" << "USERDOMAIN.*"
                  << "HOSTNAME.*" << "DOMAINNAME.*";

     QStringList environment = QProcess::systemEnvironment();
     foreach (QString string, envVariables) {
         int index = environment.indexOf(QRegExp(string));
         if (index != -1) {
             QStringList stringList = environment.at(index).split('=');
             if (stringList.size() == 2) {
                 username = stringList.at(1).toUtf8();
                 break;
             }
         }
     }

     if (username.isEmpty())
         username = "unknown";

     updateAddresses();
     serverPort = 0;

     broadcastSocket.bind(QHostAddress::Any, broadcastPort, QUdpSocket::ShareAddress
                          | QUdpSocket::ReuseAddressHint);
     connect(&broadcastSocket, SIGNAL(readyRead()),
             this, SLOT(readBroadcastDatagram()));

     broadcastTimer.setInterval(BroadcastInterval);
     connect(&broadcastTimer, SIGNAL(timeout()),
             this, SLOT(sendBroadcastDatagram()));
 }

 void PeerToPeerHandler::setServerPort(int port)
 {
     serverPort = port;
 }

 QByteArray PeerToPeerHandler::userName() const
 {
     return username;
 }

 void PeerToPeerHandler::startBroadcasting()
 {
     broadcastTimer.start();
 }

 bool PeerToPeerHandler::isLocalHostAddress(const QHostAddress &address)
 {
     foreach (QHostAddress localAddress, ipAddresses) {
         if (address == localAddress)
             return true;
     }
     return false;
 }

 void PeerToPeerHandler::sendBroadcastDatagram()
 {
     QByteArray datagram(username);
     datagram.append('@');
     datagram.append(QByteArray::number(serverPort));

     bool validBroadcastAddresses = true;
     foreach (QHostAddress address, broadcastAddresses) {
         if (broadcastSocket.writeDatagram(datagram, address,
                                           broadcastPort) == -1)
             validBroadcastAddresses = false;
     }

     if (!validBroadcastAddresses)
         updateAddresses();
 }

 void PeerToPeerHandler::readBroadcastDatagram()
 {
     while (broadcastSocket.hasPendingDatagrams()) {
         QHostAddress senderIp;
         quint16 senderPort;
         QByteArray datagram;
         datagram.resize(broadcastSocket.pendingDatagramSize());
         if (broadcastSocket.readDatagram(datagram.data(), datagram.size(),
                                          &senderIp, &senderPort) == -1)
             continue;

         QList<QByteArray> list = datagram.split('@');
         if (list.size() != 2)
             continue;

         int senderServerPort = list.at(1).toInt();
         if (isLocalHostAddress(senderIp) && senderServerPort == serverPort)
             continue;

         if (!client->hasConnection(senderIp)) {
             Connection *connection = new Connection(this);
             emit newConnection(connection);
             connection->connectToHost(senderIp, senderServerPort);
         }
     }
 }

 void PeerToPeerHandler::updateAddresses()
 {
     broadcastAddresses.clear();
     ipAddresses.clear();
     foreach (QNetworkInterface interface, QNetworkInterface::allInterfaces()) {
         foreach (QNetworkAddressEntry entry, interface.addressEntries()) {
             QHostAddress broadcastAddress = entry.broadcast();
             if (broadcastAddress != QHostAddress::Null && entry.ip() != QHostAddress::LocalHost) {
                 broadcastAddresses << broadcastAddress;
                 ipAddresses << entry.ip();
             }
         }
     }
 }
