#ifndef CHATSHELL_H
#define CHATSHELL_H

#include <QWidget>
#include "chatdialog.h"
#include <QListWidgetItem>
#include "mylistitem.h"
#include <vector>
#include <QImage>

namespace Ui {
class ChatShell;
}

class ChatShell : public QDialog
{
    Q_OBJECT

public:
    explicit ChatShell(QWidget *parent = nullptr);
    void setAvatarImgVec(vector<QImage> &_avatarImgVec){this->_avatarImgVec = _avatarImgVec;}
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    ~ChatShell();

private slots:
    void on_checkListWidget_itemClicked(QListWidgetItem *item);
    void addDialogToShell(int,QString,int,ChatDialog *);
    void deleteItem(QListWidgetItem *,ChatDialog *);
    void on_closePushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::ChatShell *ui;
    bool m_bMove;
    QPoint m_pressPoint;
    vector<QImage> _avatarImgVec;
};

#endif // CHATSHELL_H
