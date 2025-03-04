#ifndef ADDGROUPPAGE_H
#define ADDGROUPPAGE_H

#include <QDialog>
#include "json.hpp"
#include "public.hpp"
#include <string>
//#include "sys/socket.h"
#include <QMessageBox>
#include "group.hpp"
#include <QGraphicsDropShadowEffect>
#include "qtmaterialdialog.h"
#include "qtmaterialflatbutton.h"
#include "qtmaterialdrawer.h"
#include "qtmaterialautocomplete.h"
using json = nlohmann::json;

namespace Ui {
class AddGroupPage;
}

class AddGroupPage : public QDialog
{
    Q_OBJECT

public:
    explicit AddGroupPage(QWidget *parent = nullptr);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void createMaterialDialog();
    void createMaterialDrawer();
    ~AddGroupPage();
signals:
        void showMaterialDialog();
private slots:
    void on_addPushButton_clicked();
    void on_closePushButton_clicked();
    void on_pushButton_2_clicked();
    void toCreateGroup();
    void showAck(QString);

private:
    Ui::AddGroupPage *ui;
    QtMaterialDialog *mdialog;
    QLabel *label;
    bool m_bMove;
    QPoint m_pressPoint;
    QtMaterialDrawer *mdrawer;
    QtMaterialAutoComplete *groupNameLine;
    QtMaterialAutoComplete *groupDescLine;
};

#endif // ADDGROUPPAGE_H
