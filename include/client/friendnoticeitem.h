#ifndef FRIENDNOTICEITEM_H
#define FRIENDNOTICEITEM_H

#include <QWidget>
#include "qtmaterialflatbutton.h"
#include "qtmaterialavatar.h"
#include <QString>
#include <QImage>
#include "json.hpp"
#include "public.hpp"
using json = nlohmann::json;


namespace Ui {
class FriendNoticeItem;
}

class FriendNoticeItem : public QWidget
{
    Q_OBJECT

public:
    explicit FriendNoticeItem(QWidget *parent = nullptr);
    void setName(QString name);
    void setAvatar(int avatarIndex,QImage &avatar);
    void setIndex(int index){this->index = index;}
    void setId(int id){this->id = id;}
    void setFriendid(int friendid){this->friendid = friendid;}
    void addFriendRes(bool ack);
    ~FriendNoticeItem();

signals:
    void haveProcess(int);
private slots:
    void on_agreePushButton_clicked();

    void on_refusePushButton_clicked();

private:
    Ui::FriendNoticeItem *ui;
    int avatarIndex;
    int id;
    int friendid;
    int index;
};

#endif // FRIENDNOTICEITEM_H
