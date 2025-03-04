#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QTcpSocket>
#include <QObject>

class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MyTcpSocket(QObject *parent = nullptr);
    void ConnetToHost(const QString &address,quint16 port);
    void Send(const QByteArray &buffer);
    bool IsConnected();
    bool getReadReady();
    QByteArray Read();
signals:
    void ConnetToHost_Signal(const QString &address,const quint16 port);
    void Send_Signal(const QByteArray &buffer);
    void ReadMessage(QString);
private:
    bool connected = false;
    bool readReady = false;
    QByteArray myBuffer;
};

#endif // MYTCPSOCKET_H
