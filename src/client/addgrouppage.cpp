#include "addgrouppage.h"
#include "ui_addgrouppage.h"
#include "mainmenu.h"

extern User g_currentUser;//全局变量，保存当前登录用户的信息
extern vector<Group> g_currentUserGroupList;//全局变量，保存当前登录用户的好友列表信息
extern MyTcpSocket *Socket;//全局变量，socket对象

AddGroupPage::AddGroupPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddGroupPage)
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
    //添加群组按钮样式
    ui->addPushButton->setForegroundColor(QColor(90, 194, 198));
    ui->addPushButton->setOverlayStyle(Material::TintedOverlay);
    //创建群组按钮样式
    ui->createPushButton->setForegroundColor(QColor(90, 194, 198));
    ui->createPushButton->setOverlayStyle(Material::TintedOverlay);
    //创建信息提示框
    createMaterialDialog();
    //创建群组创建页面
    createMaterialDrawer();
}

AddGroupPage::~AddGroupPage()
{
    delete ui;
}

void AddGroupPage::on_addPushButton_clicked()
{
    int groupId = ui->idLineEdit->text().toInt();
    ui->idLineEdit->clear();
    //非法输入判断
    if(groupId <= 0)
    {
        label->setText("invalid input,please enter the integer(>0)");
        emit showMaterialDialog();
        return;
    }
    //判断添加群是否已经在群列表中
    for(Group &group : g_currentUserGroupList)
    {
        if(group.getGroupId() == groupId)
        {
            label->setText(QString("group ").append(QString::number(group.getGroupId())).append(" has existed in your grouplist"));
            emit showMaterialDialog();
            return;
        }
    }
    //向服务器发送添加群组请求
    json js;
    js["msgid"] = ADD_GROUP_MSG;
    js["id"] = g_currentUser.getId();
    js["groupId"] = groupId;
    std::string buffer = js.dump();
    Socket->Send(QString(buffer.c_str()).toUtf8());
}


void AddGroupPage::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton && m_bMove)
    {
        QPoint parentPoint = this->pos();
        parentPoint.setX(parentPoint.x() + event->x() - m_pressPoint.x());
        parentPoint.setY(parentPoint.y() + event->y() - m_pressPoint.y());
        this->move(parentPoint);
    }
}

//mouseMoveEvent、mousePressEvent、mouseReleaseEvent实现了窗口拖动
void AddGroupPage::mousePressEvent(QMouseEvent *event)
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

void AddGroupPage::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_bMove)
    {
        m_bMove = false;
    }
}


void AddGroupPage::on_pushButton_2_clicked()
{
    this->showMinimized();
}

void AddGroupPage::on_closePushButton_clicked()
{
    this->close();
}

void AddGroupPage::createMaterialDialog()
{
    //创建信息提示对话框
    mdialog = new QtMaterialDialog(this);
    QWidget *dialogWidget = new QWidget(this);
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
    connect(this,&AddGroupPage::showMaterialDialog,mdialog,&QtMaterialDialog::showDialog);
    connect(closeButton,&QtMaterialFlatButton::pressed,mdialog,&QtMaterialDialog::hideDialog);
}

void AddGroupPage::createMaterialDrawer()
{
    //创建群组创建页面
    mdrawer = new QtMaterialDrawer(this);
    mdrawer->setParent(ui->mainWidget);
    mdrawer->setOverlayMode(true);
    mdrawer->setClickOutsideToClose(true);
    //添加垂直布局
    QVBoxLayout *layout = new QVBoxLayout;
    mdrawer->setDrawerLayout(layout);
    //设置标题
    QLabel *title = new QLabel(this);
    title->setText("CREATE GROUP");
    QFont ft;
    ft.setPixelSize(20);
    title->setFont(ft);
    //创建群组名输入框
    groupNameLine = new QtMaterialAutoComplete(this);
    groupNameLine->setPlaceholderText("Group Name");
    //创建群组描述输入框
    groupDescLine = new QtMaterialAutoComplete(this);
    groupDescLine->setPlaceholderText("Group Description");
    //创建提交按钮
    QtMaterialFlatButton *button = new QtMaterialFlatButton(this);
    button->setOverlayStyle(Material::TintedOverlay);
    button->setForegroundColor(QColor(90, 194, 198));
    button->setText("commit");
    //添加以上组件
    layout->addWidget(title);
    layout->addWidget(groupNameLine);
    layout->addWidget(groupDescLine);
    layout->addWidget(button);
    layout->setAlignment(button,Qt::AlignCenter);
    layout->setAlignment(title,Qt::AlignCenter);
    //设置size
    groupNameLine->setMinimumHeight(50);
    groupDescLine->setMinimumHeight(50);
    button->setMinimumHeight(50);
    layout->setContentsMargins(10,50,10,40);
    //连接显示和创建群组的槽函数
    connect(ui->createPushButton,&QPushButton::pressed,mdrawer,&QtMaterialDrawer::openDrawer);
    connect(button,&QtMaterialFlatButton::pressed,this,&AddGroupPage::toCreateGroup);
}

void AddGroupPage::toCreateGroup()
{
    QString name = groupNameLine->text();
    QString desc = groupDescLine->text();
    //空值判断
    if(name.isEmpty())
    {
        label->setText("the name of group can not be null");
        emit showMaterialDialog();
        return;
    }
    //判断添加群组是否已在群列表中
    for(Group &group : g_currentUserGroupList)
    {
        if(group.getGroupName() == name.toStdString())
        {
            label->setText("the group has existed in your grouplist");
            emit showMaterialDialog();
            return;
        }
    }
    //向服务器发送添加群组的请求
    json js;
    js["msgid"] = CREATE_GROUP_MSG;
    js["id"] = g_currentUser.getId();
    js["groupName"] = name.toStdString();
    js["groupDesc"] = desc.toStdString();
    std::string buffer = js.dump();
    Socket->Send(QString(buffer.c_str()).toUtf8());
}

void AddGroupPage::showAck(QString messages)
{
    //显示添加群组的应答消息
    label->setText(messages);
    emit showMaterialDialog();
}

