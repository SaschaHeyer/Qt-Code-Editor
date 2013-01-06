#include <QtGui/QApplication>
#include "reddeernote.h"

#include <QtCore/QSettings>
#include <QtNetwork/QNetworkConfigurationManager>
#include <QtNetwork/QNetworkSession>
#include <QSplashScreen>
#include <QThread>

#include "chatwidget.h"

class SleeperThread : public QThread
{
public:
static void msleep(unsigned long msecs)
{
QThread::msleep(msecs);
}
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        QNetworkSession *networkSession = new QNetworkSession(config, &a);
        networkSession->open();
        networkSession->waitForOpened();

        if (networkSession->isOpen()) {
            // Save the used configuration
            QNetworkConfiguration config = networkSession->configuration();
            QString id;
            if (config.type() == QNetworkConfiguration::UserChoice) {
                id = networkSession->sessionProperty(
                        QLatin1String("UserChoiceConfiguration")).toString();
            } else {
                id = config.identifier();
            }

            QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
            settings.beginGroup(QLatin1String("QtNetwork"));
            settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
            settings.endGroup();
        }
    }


    RedDeerNote w;



    QSplashScreen *splash=new QSplashScreen;
    splash->setPixmap(QPixmap(":/icons/logord2.png"));
    splash->show();
    splash->showMessage( "Initializing..." );
    SleeperThread::msleep(300);
    splash->showMessage( "Load GUI..." );
    SleeperThread::msleep(300);
    splash->showMessage( "Load Library..." );
    SleeperThread::msleep(300);
    splash->showMessage( "Starting..." );
    SleeperThread::msleep(300);


    w.show();
    splash->finish(&w);
    delete splash;
    return a.exec();
}

