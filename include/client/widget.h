#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <QMessageBox>
#include "json.hpp"
#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
using namespace std;
using json = nlohmann::json;

#include <unistd.h>
#include <QTcpSocket>
#include "mytcpsocket.h"
#include <QThread>
#include <QElapsedTimer>
#include "group.hpp"
#include "user.hpp"
#include "public.hpp"
#include "qtmaterialautocomplete.h"
#include "qtmaterialflatbutton.h"
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <QHostAddress>
#include <QStringList>
namespace Ui {
class widget;
}

class widget : public QWidget
{
    Q_OBJECT

public:
    explicit widget(QWidget *parent = nullptr);
    bool isIp(QString &ip);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    ~widget();

private slots:
    void on_connectPushButton_clicked();

    void on_closePushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::widget *ui;
    MyTcpSocket *Socket;
    QThread *myThread;
    bool m_bMove;
    QPoint m_pressPoint;
};

#endif // WIDGET_H
