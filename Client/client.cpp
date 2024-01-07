#include "client.h"
#include <QtCore>

Client::Client(QObject *parent)
    : QObject{parent}
{
    connectToHost();

    in.setDevice(tcpSocket);
    in.setVersion(QDataStream::Qt_6_5);

    connect(tcpSocket, &QIODevice::readyRead, this, &Client::readMessage);
    connect(tcpSocket, &QAbstractSocket::errorOccurred, this, &Client::displayError);
}

void Client::connectToHost()
{
    tcpSocket = new QTcpSocket(this);
    tcpSocket->connectToHost(QHostAddress(QString("192.168.0.101")), 40023);
}

void Client::readMessage()
{
    qDebug() << "Waiting for message from server...";

    in.startTransaction();

    QString nextFortune;
    in >> nextFortune;

    qDebug() << "The message from server is: " << "\n" << nextFortune.toStdString() << "\n";

    if (!in.commitTransaction())
        return;

    tcpSocket->close();

    QCoreApplication::quit();
}

void Client::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug() << "The host was not found." << "\n"
                 << "Please check the host name and port settings.";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qDebug() << "The connection was refused by the peer." << "\n"
                 << "Make sure the server is running, " << "\n"
                 << "and check that the host name and port " << "\n"
                 << "settings are correct.";
        break;
    default:
        qDebug() << "The following error occurred:" << "\n"
                 << tcpSocket->errorString();
    }

    QCoreApplication::quit();
}
