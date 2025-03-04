#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QDialog>
#include <QString>
#include <QWidget>
#include "json.hpp"
#include "public.hpp"
#include <string>
#include <QTcpSocket>
#include "mytcpsocket.h"
#include <QMessageBox>
#include <QMouseEvent>
#include <QAction>
#include "qtmaterialautocomplete.h"
#include "qtmaterialdialog.h"
//#include "qtmaterialflatbutton.h"
#include "qtmaterialdrawer.h"
using json = nlohmann::json;


namespace Ui {
class HomePage;
}

class HomePage : public QDialog
{
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent = nullptr);
    void setSocket(MyTcpSocket *Socket){this->Socket = Socket;}
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void createMaterialDialog();
    void createMaterialDrawer();
    ~HomePage();

private slots:
    void showWindow();   
    void toRegister();
    void on_loginPushButton_clicked();
    void on_closePushButton_clicked();
    void on_pushButton_2_clicked();

signals:
    void showMaterialDialog();

private:
    Ui::HomePage *ui;
    QtMaterialDialog *mdialog;
    QLabel *label;
    QtMaterialDrawer *mdrawer;
    QtMaterialAutoComplete *userNameLine;
    QtMaterialAutoComplete *userPasswordLine;
    MyTcpSocket *Socket;
    bool m_bMove;
    QPoint m_pressPoint;
};

#endif // HOMEPAGE_H
