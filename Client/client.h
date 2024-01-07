#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);

signals:

public slots:
    void connectToHost();
    void readMessage();
    void displayError(QAbstractSocket::SocketError);

private:
    QTcpSocket *tcpSocket = nullptr;
    QDataStream in;
};

#endif // CLIENT_H
