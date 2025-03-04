#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include "json.hpp"
#include <string>
#include <QString>
#include "public.hpp"
#include <QMessageBox>
#include <QCloseEvent>
using json = nlohmann::json;
using namespace std;

namespace Ui {
class ChatDialog;
}

class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr,int toid = 0,QString name = "",int chattype = ONE_CHAT_MSG);
    virtual void displayMsg(json js){}
    virtual void initUi();
    void closeEvent(QCloseEvent *e);
    virtual void updateGroupList(){}
    int getId();
    int getType();
    virtual ~ChatDialog();
signals:
    void closeWindow(int toid,int chattype);

protected:
    Ui::ChatDialog *ui;
    int toid;
    int chattype;
    QString name;
};

#endif // CHATDIALOG_H
