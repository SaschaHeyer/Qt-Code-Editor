#ifndef SERVER_H
 #define SERVER_H

 #include <QTcpServer>

 class Connection;

 class Server : public QTcpServer
 {
     Q_OBJECT

 public:
     Server(QObject *parent = 0);

 signals:
     void newConnection(Connection *connection);

 protected:
     void incomingConnection(int socketDescriptor);
 };

 #endif
