#ifndef FRIENDNOTICEPAGE_H
#define FRIENDNOTICEPAGE_H

#include <QDialog>
#include <vector>
#include <QMouseEvent>
#include <QPoint>
#include <QGraphicsDropShadowEffect>
#include <QListWidgetItem>
#include "json.hpp"
#include "friendnoticeitem.h"
#include <QDebug>
#include "qtmaterialscrollbar.h"
using json = nlohmann::json;
using std::vector;

namespace Ui {
class FriendNoticePage;
}

class FriendNoticePage : public QDialog
{
    Q_OBJECT

public:
    explicit FriendNoticePage(QWidget *parent = nullptr);
    void addNoticeMsg(json &js);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void setAvatarImgVec(vector<QImage> &_avatarImgVec){this->_avatarImgVec = _avatarImgVec;}
    ~FriendNoticePage();

private slots:
    void on_closePushButton_clicked();
    void deleteItem(int);

private:
    Ui::FriendNoticePage *ui;
    bool m_bMove;
    QPoint m_pressPoint;
    vector<QImage> _avatarImgVec;
};

#endif // FRIENDNOTICEPAGE_H
