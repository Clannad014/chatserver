#include "friendnoticepage.h"
#include "ui_friendnoticepage.h"


FriendNoticePage::FriendNoticePage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FriendNoticePage)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);//无边框
    this->setAttribute(Qt::WA_TranslucentBackground);//背景透明
    //设置阴影
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0,0);
    shadow->setColor(QColor("#444444"));
    shadow->setBlurRadius(20);
    ui->mainWidget->setGraphicsEffect(shadow);
    ui->mainWidget->setContentsMargins(1,1,1,1);
    //创建滚动条
    QtMaterialScrollBar *scrollBar = new QtMaterialScrollBar(this);
    scrollBar->setMinimumHeight(8);
    ui->listWidget->setVerticalScrollBar(scrollBar);
}

void FriendNoticePage::addNoticeMsg(json &js)
{
    int avatarIndex = js["avatar"].get<int>();
    QString name = js["name"].get<std::string>().c_str();
    QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
    //创建好友请求的item
    FriendNoticeItem *fnItem =  new FriendNoticeItem(this);
    connect(fnItem,SIGNAL(haveProcess(int)),this,SLOT(deleteItem(int)));//处理完毕后删除item
    //item的初始化
    fnItem->setIndex(ui->listWidget->count() - 1);
    fnItem->setId(js["id"].get<int>());
    fnItem->setFriendid(js["toid"].get<int>());
    fnItem->setAvatar(avatarIndex,_avatarImgVec[avatarIndex]);
    fnItem->setName(name);
    item->setSizeHint(QSize(120,80));
    //添加item
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item,fnItem);
}

FriendNoticePage::~FriendNoticePage()
{
    delete ui;
}

//mouseMoveEvent、mousePressEvent、mouseReleaseEvent实现了窗口拖动
void FriendNoticePage::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton && m_bMove)
    {
        QPoint parentPoint = this->pos();
        parentPoint.setX(parentPoint.x() + event->x() - m_pressPoint.x());
        parentPoint.setY(parentPoint.y() + event->y() - m_pressPoint.y());
        this->move(parentPoint);
    }
}

void FriendNoticePage::mousePressEvent(QMouseEvent *event)
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

void FriendNoticePage::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_bMove)
    {
        m_bMove = false;
    }
}

void FriendNoticePage::on_closePushButton_clicked()
{
    this->hide();
}

void FriendNoticePage::deleteItem(int index)
{
    //删除item
    QListWidgetItem *item = ui->listWidget->item(index);
    ui->listWidget->removeItemWidget(item);
    delete item;
}

