#ifndef MAINMENU_H
#define MAINMENU_H

#include <QDialog>
#include "user.hpp"
#include "group.hpp"
#include <vector>
#include <string>
#include "json.hpp"
#include <QString>
#include <QListWidgetItem>
#include <QStringList>
#include "public.hpp"
#include <QTcpSocket>
#include "mytcpsocket.h"
#include <QThread>
#include <QDebug>
#include <unistd.h>
#include <chrono>
#include <ctime>
#include <QMessageBox>
#include <unordered_map>
#include "chatdialog.h"
#include <mutex>
#include <QTime>
#include <QElapsedTimer>
#include "chatshell.h"
#include "onechatdialog.h"
#include "groupchatdialog.h"
#include <QPainter>
#include <QBitmap>
#include <QPainterPath>
#include "qtmaterialflatbutton.h"
#include "qtmaterialraisedbutton.h"
#include <QGraphicsDropShadowEffect>
#include "mylistitem.h"
#include "qtmaterialscrollbar.h"
#include "addfriendpage.h"
#include "addgrouppage.h"

using json = nlohmann::json;


namespace Ui {
class MainMenu;
}

class MainMenu : public QDialog
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr,json responsejs = nullptr);
    void setSocket(MyTcpSocket *HomeSocket);
    void showCurrentUserData();
    static string getCurrentTime();
    void newDialog(int id,QString name,int avatarIndex,int chattype);
    void newDialog(json js);
    void updateGroupList();
    void setAvatar(QString pixPath);
    bool eventFilter(QObject *, QEvent *);
    void updateAvatar(int avatarIndex);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void friendListReload();
    void groupListReload();
    void friendListItemUpdate(int row,QString state);
    ~MainMenu();
signals:
    void showParentWindow();
    void ListClicked(int,QString,int,ChatDialog *);
    void groupListUpdated();
    void addFriendAck(QString);
    void addGroupAck(QString);
private:
    Ui::MainMenu *ui;
    unordered_map<int,ChatDialog *> _friendDialoglist;
    unordered_map<int,ChatDialog *> _groupDialoglist;
    AddFriendPage *afd;
    AddGroupPage *agp;
    mutex _friendDialogMutex;
    mutex _groupDialogMutex;
    ChatShell *shell;
    int chattype;
    int toid;
    int avatarIndex;
    bool m_bMove;
    QPoint m_pressPoint;
    vector<QImage> _avatarImgVec = {QImage(":/new/prefix1/img1.webp"),
                                    QImage(":/new/prefix1/img2.webp"),
                                    QImage(":/new/prefix1/img3.webp"),
                                    QImage(":/new/prefix1/img4.webp"),
                                    QImage(":/new/prefix1/img5.webp"),
                                    QImage(":/new/prefix1/img6.webp"),
                                    QImage(":/new/prefix1/img7.webp"),
                                    QImage(":/new/prefix1/img8.webp")};

private slots:
    void recvMessage(QString);
    void removepointer(int toid,int chattype);
    void on_friendListWidget_itemDoubleClicked(QListWidgetItem *item);
    void stackedWidgetChange();
    void on_groupListWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_addFriendPushButton_clicked();
    void on_addGroupPushButton_clicked();
    void on_loginoutPushButton_clicked();
    void on_closePushButton_clicked();
    void on_pushButton_2_clicked();
};

#endif // MAINMENU_H
