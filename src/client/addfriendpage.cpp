#include "addfriendpage.h"
#include "ui_addfriendpage.h"
#include "mainmenu.h"
extern User g_currentUser;//全局变量，保存当前登录用户的信息
extern vector<User> g_currentUserFriendList;//全局变量，保存当前登录用户的好友列表信息
extern MyTcpSocket *Socket;//全局变量，socket对象


AddFriendPage::AddFriendPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddFriendPage)
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
    //添加好友按钮样式
    ui->addPushButton->setForegroundColor(QColor(90, 194, 198));
    ui->addPushButton->setOverlayStyle(Material::TintedOverlay);
    //好友通知按钮样式
    ui->pushButton->setForegroundColor(QColor(90, 194, 198));
    ui->pushButton->setOverlayStyle(Material::TintedOverlay);
    //创建好友通知页面
    fnp = new FriendNoticePage(this);
    fnp->hide();
    //创建添加好友提示信息对话框
    createMaterialDialog();
}

AddFriendPage::~AddFriendPage()
{
    delete ui;
}


void AddFriendPage::addNoticeMsg(json &js)
{
    //将好友添加请求的信息加入到好友通知的页面中
    fnp->addNoticeMsg(js);
}

void AddFriendPage::setNoticeAvatarImgVec()
{
    //获取系统头像数组
    fnp->setAvatarImgVec(_avatarImgVec);
}

void AddFriendPage::on_addPushButton_clicked()
{
    int friendId = ui->idLineEdit->text().toInt();
    ui->idLineEdit->clear();
    //非法输入判断
    if(friendId <= 0)
    {
        label->setText("invalid input,please enter the integer(>0)");
        emit showMaterialDialog();
        return;
    }
    //判断添加用户是否已经为好友
    for(User &user : g_currentUserFriendList)
    {
        if(user.getId() == friendId)
        {
            label->setText(QString("user ").append(QString::number(user.getId())).append(" has been in your friendlist"));
            emit showMaterialDialog();
            return;
        }
    }
    //向服务器发送添加好友请求
    json js;
    js["msgid"] = ADD_FRIEND_MSG;
    js["id"] = g_currentUser.getId();
    js["friendId"] = friendId;
    js["avatar"] = g_currentUser.getAvatar();
    js["name"] = g_currentUser.getName();
    if(friendId == g_currentUser.getId())
    {
        label->setText("don't add yourself into friendlist!");
        emit showMaterialDialog();
        return;
    }
    std::string buffer = js.dump();
    Socket->Send(QString(buffer.c_str()).toUtf8());
}


void AddFriendPage::on_closePushButton_clicked()
{
    this->hide();
}

//mouseMoveEvent、mousePressEvent、mouseReleaseEvent实现了窗口拖动
void AddFriendPage::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton && m_bMove)
    {
        QPoint parentPoint = this->pos();
        parentPoint.setX(parentPoint.x() + event->x() - m_pressPoint.x());
        parentPoint.setY(parentPoint.y() + event->y() - m_pressPoint.y());
        this->move(parentPoint);
    }
}

void AddFriendPage::mousePressEvent(QMouseEvent *event)
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

void AddFriendPage::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_bMove)
    {
        m_bMove = false;
    }
}


void AddFriendPage::on_pushButton_2_clicked()
{
    this->showMinimized();
}

void AddFriendPage::createMaterialDialog()
{
    //创建信息提示对话框
    mdialog = new QtMaterialDialog(this);
    QWidget *dialogWidget = new QWidget(mdialog);
    QVBoxLayout *dialogWidgetLayout = new QVBoxLayout;
    dialogWidget->setLayout(dialogWidgetLayout);
    //创建信息提示label
    label = new QLabel(this);
    QFont ft;
    ft.setPointSize(13);
    label->setFont(ft);
    label->setWordWrap(true);
    label->setAlignment(Qt::AlignCenter);
    label->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    dialogWidgetLayout->addWidget(label);
    dialogWidgetLayout->setAlignment(label,Qt::AlignCenter);
    //创建关闭按钮
    QtMaterialFlatButton *closeButton = new QtMaterialFlatButton(this);
    closeButton->setText("close");
    closeButton->setMaximumWidth(250);
    closeButton->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    dialogWidgetLayout->addWidget(closeButton);
    dialogWidgetLayout->setAlignment(closeButton,Qt::AlignCenter | Qt::AlignBottom);
    //添加垂直布局
    QVBoxLayout *layout = new QVBoxLayout;
    mdialog->setWindowLayout(layout);
    //设置size
    dialogWidget->setMinimumHeight(150);
    dialogWidget->setMaximumHeight(200);
    dialogWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    layout->addWidget(dialogWidget);
    //连接显示和隐藏的槽函数
    connect(this,&AddFriendPage::showMaterialDialog,mdialog,&QtMaterialDialog::showDialog);
    connect(closeButton,&QtMaterialFlatButton::pressed,mdialog,&QtMaterialDialog::hideDialog);
}

void AddFriendPage::showAck(QString message)
{
    //显示添加好友回复信息
    label->setText(message);
    emit showMaterialDialog();
}


void AddFriendPage::on_pushButton_clicked()
{
    //显示好友通知页面
    fnp->show();
}

