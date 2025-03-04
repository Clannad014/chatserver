#ifndef ONECHATDIALOG_H
#define ONECHATDIALOG_H

#include <QDialog>
#include "chatdialog.h"
#include "recvbox.h"
#include "sendbox.h"
#include <QScrollBar>
#include <QDebug>
#include <QThread>
#include <QElapsedTimer>
#include "qtmaterialscrollbar.h"

namespace Ui {
class OneChatDialog;
}

class OneChatDialog : public ChatDialog
{
    Q_OBJECT

public:
    explicit OneChatDialog(QWidget *parent = nullptr,int toid = 0,QString name = "",int chattype = ONE_CHAT_MSG);
    void displayMsg(json js);
    void initUi();
    ~OneChatDialog();

private slots:
    void on_sendLineEdit_returnPressed();

private:
    Ui::OneChatDialog *ui;
    vector<QImage> _avatarImgVec;
};

#endif // ONECHATDIALOG_H
