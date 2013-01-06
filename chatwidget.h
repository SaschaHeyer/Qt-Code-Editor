#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QListWidget>
#include <QString>
#include <QTextTableFormat>

#include "client.h"

class ChatWidget : public QWidget
{
    Q_OBJECT

public:
    ChatWidget(QWidget *parent = 0);

public slots:
    void appendMessage(const QString &from, const QString &message);
    void openCode(const QString &from, const QString &message);
    void sendCode(QString code);

private slots:
    void returnPressed();
    void newParticipant(const QString &nick);
    void participantLeft(const QString &nick);
    void userSelected(QListWidgetItem* item);

signals:
    void sendCodeToMdiFrame(const QString &from, const QString &message);

private:
    // GUI
    QGridLayout* mChatLayout;
    QLineEdit* mMsg;
    QTextEdit* mChatText;
    QListWidget* mUserList;

    Client client;
    QString myNickName;
    QTextTableFormat tableFormat;

    QString* selectedUser;
};

#endif // CHATWIDGET_H
