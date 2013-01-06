#include "chatwidget.h"

#include <QGridLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QListWidget>
#include <QTextTable>
#include <QScrollBar>
#include <QMessageBox>
#include <QTimer>
#include <QWidget>


ChatWidget::ChatWidget(QWidget *parent)
    :QWidget(parent), mChatLayout(0), mMsg(0),mChatText(0),
      mUserList(0)
{
    mChatLayout = new QGridLayout(this);
    mMsg = new QLineEdit(this);
    mChatText= new QTextEdit(this);
    mUserList = new QListWidget(this);
    mUserList->setMaximumWidth(100);

    mChatText->setReadOnly(true);
    mChatText->setAcceptRichText(true);
    mChatText->setOverwriteMode(false);

    mMsg->setText(tr("Nachricht hier eingeben"));

    mChatLayout->addWidget(mChatText,0,0);
    mChatLayout->addWidget(mUserList,0,1);
    mChatLayout->addWidget(mMsg,1,0,1,-1);

    connect(mMsg, SIGNAL(returnPressed()), this, SLOT(returnPressed()));
    connect(mMsg, SIGNAL(returnPressed()), this, SLOT(returnPressed()));
    connect(&client, SIGNAL(newMessage(QString,QString)), this, SLOT(appendMessage(QString,QString)));
    connect(&client, SIGNAL(newCode(QString,QString)), this, SLOT(openCode(QString,QString)));
    connect(&client, SIGNAL(newParticipant(QString)), this, SLOT(newParticipant(QString)));
    connect(&client, SIGNAL(participantLeft(QString)), this, SLOT(participantLeft(QString)));

    connect(mUserList,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(userSelected(QListWidgetItem*)));
    //connect(&client, SIGNAL(newCode(QString,QString)), this, SLOT(appendMessage(QString,QString)));
    myNickName = client.nickName();
    newParticipant(myNickName);
    tableFormat.setBorder(0);
}


void ChatWidget::userSelected(QListWidgetItem *item)
{
    selectedUser = &item->text();
    qDebug()<<item->text();
}

void ChatWidget::appendMessage(const QString &from, const QString &message)
{
    if (from.isEmpty() || message.isEmpty())
        return;

    QTextCursor cursor(mChatText->textCursor());
    cursor.movePosition(QTextCursor::End);
    QTextTable *table = cursor.insertTable(1, 2, tableFormat);
    table->cellAt(0, 0).firstCursorPosition().insertText('<' + from + "> ");
    table->cellAt(0, 1).firstCursorPosition().insertText(message);
    QScrollBar *bar = mChatText->verticalScrollBar();
    bar->setValue(bar->maximum());
}

void ChatWidget::openCode(const QString &from, const QString &message)
{
    emit sendCodeToMdiFrame(from,message);
}

void ChatWidget::returnPressed()
{
    QString text = mMsg->text();
    if (text.isEmpty())
        return;

    if (text.startsWith(QChar('/'))) {
        QColor color = mChatText->textColor();
        mChatText->setTextColor(Qt::red);
        mChatText->append(tr("! Unknown command: %1")
                         .arg(text.left(text.indexOf(' '))));
        mChatText->setTextColor(color);
    } else {
        client.sendMessage(text);
        appendMessage(myNickName, text);
    }

    mMsg->clear();
}

void ChatWidget::sendCode(QString code){
    if(code.isEmpty())
        return;

    client.sendCode(code);
}

void ChatWidget::newParticipant(const QString &nick)
{

    qDebug()<<"new partici";
    if (nick.isEmpty())
        return;

    QColor color = mChatText->textColor();
    mChatText->setTextColor(Qt::gray);
    mChatText->append(tr("* %1 online").arg(nick));
    mChatText->setTextColor(color);
    mUserList->addItem(nick);
}

void ChatWidget::participantLeft(const QString &nick)
{
    if (nick.isEmpty())
        return;

    QList<QListWidgetItem *> items = mUserList->findItems(nick, Qt::MatchExactly);
    if (items.isEmpty())
        return;

    delete items.at(0);
    QColor color = mChatText->textColor();
    mChatText->setTextColor(Qt::gray);
    mChatText->append(tr("* %1 offline").arg(nick));
    mChatText->setTextColor(color);
}
