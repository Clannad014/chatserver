#ifndef SENDBOX_H
#define SENDBOX_H

#include <QWidget>
#include "user.hpp"
#include <QPainter>
#include <QPainterPath>

namespace Ui {
class SendBox;
}

class SendBox : public QWidget
{
    Q_OBJECT

public:
    explicit SendBox(QWidget *parent = nullptr);
    void setText(QString text);
    void setAvatar(QString pixPath);
    void hideNameLabel();
    void setName(QString name);
    ~SendBox();

private:
    Ui::SendBox *ui;
};

#endif // SENDBOX_H
