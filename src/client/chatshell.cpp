#include "chatshell.h"
#include "ui_chatshell.h"
#include "mainmenu.h"

extern vector<ChatDialog *> _dialogVec;//全局变量，保存聊天页面对象的指针
extern mutex _dialogVecMutex;//全局变量，聊天页面对象数组的锁

ChatShell::ChatShell(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatShell)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);//无边框
    this->setAttribute(Qt::WA_TranslucentBackground);//背景完全透明
    //设置阴影
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0,0);
    shadow->setColor(QColor("#444444"));
    shadow->setBlurRadius(20);
    ui->mainWidget->setGraphicsEffect(shadow);
    ui->mainWidget->setContentsMargins(1,1,1,1);
}

ChatShell::~ChatShell()
{
    delete ui;
}

void ChatShell::on_checkListWidget_itemClicked(QListWidgetItem *item)
{
    //shell中的群组聊天页面切换
    int index = ui->checkListWidget->currentRow();
    ui->pageStackedWidget->setCurrentWidget(_dialogVec[index]);
    if(_dialogVec[index]->getType() == GROUP_CHAT_MSG)
    {
        _dialogVec[index]->updateGroupList();//每次切换的时候会更新群成员的最新状态信息
    }
}

void ChatShell::addDialogToShell(int id, QString name,int avatarIndex, ChatDialog *dialog)
{
    //id > 0表示该页面未添加到shell中，id < 0 表示该页面已经被添加到了页面中，只需要显示即可
    if(id > 0)
    {
        QListWidgetItem * item = new QListWidgetItem(ui->checkListWidget);
        item->setSizeHint(QSize(120,60));
        MyListItem *myItem = new MyListItem(this);
        connect(myItem,&MyListItem::closeClicked,this,&ChatShell::deleteItem);/*点击聊天对象右上角的关闭按钮，则将其从shell中移除
                                                                                  但不删除页面对象本身*/
        //根据点击的聊天页面对象类型加载不同类型的头像，好友页面对象显示系统图片头像，群组页面对象显示文本头像
        if(strcmp(dialog->metaObject()->className(),"OneChatDialog") == 0)
        {
            myItem->setAvatar(avatarIndex,_avatarImgVec[avatarIndex]);

        }
        else
        {
            myItem->setTextAvatar(name);
        }
        myItem->setId(id);
        myItem->setName(name);
        myItem->hideState();//由于本项目中所使用到的item全是同一个类，因此对于有区别的ui部分进行隐藏和显示，以此来达到不同的ui效果
        myItem->setItem(item);
        myItem->setDialog(dialog);
        myItem->setType(SHELLITEM);
        ui->checkListWidget->addItem(item);
        ui->checkListWidget->setItemWidget(item,myItem);
        ui->checkListWidget->setCurrentRow(ui->checkListWidget->count() - 1);
        ui->pageStackedWidget->addWidget(dialog);
    }
    else
    {
        ui->checkListWidget->setCurrentRow(id * -1);
    }
    ui->pageStackedWidget->setCurrentWidget(dialog);
    if(dialog->getType() == GROUP_CHAT_MSG)
    {
        dialog->updateGroupList();//每次显示群聊页面都会更新群成员列表
    }
}


void ChatShell::deleteItem(QListWidgetItem *item,ChatDialog *dialog)
{
    ui->checkListWidget->removeItemWidget(item);
    delete item;
    for(auto i = _dialogVec.begin();i != _dialogVec.end();i++)
    {
        if(*i == dialog)
        {
            _dialogVec.erase(i);/*删除对应的聊天页面对象指针，但是对象本身并未删除，
                                    在群组和好友聊天页面数组中还保存着对象的指针，因此不会出现对象无法再访问情况*/
            break;
        }
    }
    ui->pageStackedWidget->removeWidget(dialog);
}


//mouseMoveEvent、mousePressEvent、mouseReleaseEvent实现了窗口拖动
void ChatShell::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton && m_bMove)
    {
        QPoint parentPoint = this->pos();
        parentPoint.setX(parentPoint.x() + event->x() - m_pressPoint.x());
        parentPoint.setY(parentPoint.y() + event->y() - m_pressPoint.y());
        this->move(parentPoint);
    }
}

void ChatShell::mousePressEvent(QMouseEvent *event)
{
    m_pressPoint = event->pos();
    if(m_pressPoint.y() < 50)
    {
        m_bMove = true;
    }
    else
    {
        m_bMove = false;
    }
}

void ChatShell::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_bMove)
    {
        m_bMove = false;
    }
}

void ChatShell::on_closePushButton_clicked()
{
    this->hide();
}


void ChatShell::on_pushButton_2_clicked()
{
    this->showMinimized();
}

