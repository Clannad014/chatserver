#include "mytcpsocket.h"

MyTcpSocket::MyTcpSocket(QObject *parent)
    : QTcpSocket(parent)
{
    //连接服务器
    connect(this,&MyTcpSocket::ConnetToHost_Signal,this,[&](const QString &address, quint16 port){
            this->abort();
            this->connectToHost(address,port,QIODevice::ReadWrite);
        });
    //连接状态更新
    connect(this,&QAbstractSocket::connected,this,[&](){
        connected = true;
        });
    connect(this,&MyTcpSocket::disconnected,this,[&](){
        connected = false;
    });
    //发送消息
    connect(this,&MyTcpSocket::Send_Signal,this,[&](const QByteArray &buffer){
        this->write(buffer);
        this->flush();
    });
    //读取消息保存到缓冲区
    connect(this,&MyTcpSocket::readyRead,this,[&](){
        QByteArray buffer;
        buffer.resize(this->bytesAvailable());
        this->read(buffer.data(),buffer.size());
        myBuffer = buffer;
        readReady = true;
        emit ReadMessage(QString(buffer.data()));
    });
}

//本类的方法主要供主线程调用，在子线程中运行，便于主线程将网络读写和连接的请求交给子线程处理，提高主线程的ui处理能力

//连接服务器
void MyTcpSocket::ConnetToHost(const QString &address,const quint16 port)
{
    emit ConnetToHost_Signal(address,port);
}

//发送消息
void MyTcpSocket::Send(const QByteArray &buffer)
{
    emit Send_Signal(buffer);
}

//返回连接状态
bool MyTcpSocket::IsConnected()
{
    return connected;
}

//返回缓冲区是否可读状态
bool MyTcpSocket::getReadReady()
{
    return readReady;
}

//读取缓冲区中的数据
QByteArray MyTcpSocket::Read()
{
    readReady = false;
    return myBuffer;
}
