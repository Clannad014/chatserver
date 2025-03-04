#ifndef MYLISTITEM_H
#define MYLISTITEM_H

#include <QWidget>
#include "qtmaterialavatar.h"
#include <QString>
#include <QListWidgetItem>
#include "chatdialog.h"
#include "qtmaterialflatbutton.h"
#include <QEvent>
#include <QDebug>
#include <vector>
#include <QImage>
#define MENUITEM 0
#define SHELLITEM 1

namespace Ui {
class MyListItem;
}

class MyListItem : public QWidget
{
    Q_OBJECT

public:
    explicit MyListItem(QWidget *parent = nullptr);
    void setAvatar(int avatarIndex,QImage &image);
    void setTextAvatar(QString name);
    void setName(QString name);
    void setState(QString state);
    void setId(int id){this->id = id;}
    void setItem(QListWidgetItem *item){this->item = item;}
    void setDialog(ChatDialog *dialog){this->dialog = dialog;}
    void setType(int type){this->type = type;}
    int getType(){return type;}
    int getId(){return id;}
    void hideState();
    void hideDeleteButton();
    QString getName();
    int getAvatar(){return avatarIndex;}
    bool eventFilter(QObject *watched, QEvent *event);
    ~MyListItem();

signals:
    void closeClicked(QListWidgetItem *,ChatDialog *);

private slots:
    void on_deletePushButton_clicked();

private:
    Ui::MyListItem *ui;
    int id;
    int avatarIndex;
    QListWidgetItem * item;
    ChatDialog * dialog;
    int type;
};

#endif // MYLISTITEM_H
