#ifndef ADDFRIENDPAGE_H
#define ADDFRIENDPAGE_H

#include <QDialog>
#include <QMessageBox>
#include "json.hpp"
#include "public.hpp"
#include <string>
#include <vector>
#include <user.hpp>
#include <QGraphicsDropShadowEffect>
#include "qtmaterialdialog.h"
#include "qtmaterialflatbutton.h"
#include "friendnoticepage.h"
#include <QImage>
using json = nlohmann::json;

namespace Ui {
class AddFriendPage;
}

class AddFriendPage : public QDialog
{
    Q_OBJECT

public:
    explicit AddFriendPage(QWidget *parent = nullptr);
    void addNoticeMsg(json &js);
    void setNoticeAvatarImgVec();
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void setAvatarImgVec(vector<QImage> &_avatarImgVec){this->_avatarImgVec = _avatarImgVec;}
    void createMaterialDialog();
    ~AddFriendPage();

signals:
    void flushsignal();
    void showMaterialDialog();

private slots:
    void on_addPushButton_clicked();
    void on_closePushButton_clicked();
    void on_pushButton_2_clicked();
    void showAck(QString);
    void on_pushButton_clicked();
private:
    Ui::AddFriendPage *ui;
    QtMaterialDialog *mdialog;
    QLabel *label;
    bool m_bMove;
    QPoint m_pressPoint;
    vector<QImage> _avatarImgVec;
    FriendNoticePage *fnp;
};

#endif // ADDFRIENDPAGE_H
