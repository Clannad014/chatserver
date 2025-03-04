#ifndef RECVBOX_H
#define RECVBOX_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
namespace Ui {
class RecvBox;
}

class RecvBox : public QWidget
{
    Q_OBJECT

public:
    explicit RecvBox(QWidget *parent = nullptr);
    void setText(QString);
    void setAvatar(QString pixPath);
    void hideNameLabel();
    void setName(QString name);
    ~RecvBox();

private:
    Ui::RecvBox *ui;
};

#endif // RECVBOX_H
