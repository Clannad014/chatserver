#include "mylistitem.h"
#include "ui_mylistitem.h"
#include "mainmenu.h"
extern vector<QString> _avatarVec;//全局变量，保存系统头像的图片路径

MyListItem::MyListItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyListItem)
{
    ui->setupUi(this);
    ui->stateLabel->setWordWrap(true);
    ui->stateLabel->setScaledContents(true);
    //添加item悬停的事件过滤器
    this->setAttribute(Qt::WA_Hover);
    this->installEventFilter(this);
    //设置删除按钮样式
    ui->deletePushButton->setOverlayColor(QColor(237, 51, 59));
    ui->deletePushButton->hide();
}

MyListItem::~MyListItem()
{
    delete ui;
}


void MyListItem::setAvatar(int avatarIndex,QImage &avatar)
{
    //设置item的用户头像
    this->avatarIndex = avatarIndex;
    ui->avatarWidget->setImage(avatar);
}

void MyListItem::setName(QString name)
{
    //设置item的用户名称
    ui->nameLabel->setText(name);
}

void MyListItem::setState(QString state)
{
    //设置item的用户在线状态
    ui->stateLabel->setText(QString("[").append(state).append("]"));
}

QString MyListItem::getName()
{
    //返回item的用户名称
    return ui->nameLabel->text();
}
void MyListItem::setTextAvatar(QString name)
{
    //设置item的群组头像
    ui->avatarWidget->setLetter(name.at(0));
}

void MyListItem::hideState()
{
    //隐藏在线状态部分
    ui->stateLabel->hide();
}

void MyListItem::hideDeleteButton()
{
    //隐藏删除按钮部分
    ui->deletePushButton->hide();
}

void MyListItem::on_deletePushButton_clicked()
{
    //发出删除item信号
    emit closeClicked(item,dialog);
}

bool MyListItem::eventFilter(QObject *watched, QEvent *event)
{
    //鼠标悬停在item上时删除按钮显示，离开item时删除按钮隐藏
    if(type == SHELLITEM && watched == this)
    {
        if(event->type() == QEvent::HoverEnter)
        {
            ui->deletePushButton->show();
            return true;
        }
        else if(event->type() == QEvent::HoverLeave)
        {
            ui->deletePushButton->hide();
            return true;
        }
    }
    return QWidget::eventFilter(watched,event);
}

