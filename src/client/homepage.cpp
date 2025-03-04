#include "homepage.h"
#include "ui_homepage.h"
#include "mainmenu.h"

HomePage::HomePage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HomePage)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);//设置无边框
    this->setAttribute(Qt::WA_TranslucentBackground);//设置背景完全透明
    //设置自定定义边框的阴影
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0,0);
    shadow->setColor(QColor("#444444"));
    shadow->setBlurRadius(20);
    ui->mainWidget->setGraphicsEffect(shadow);
    ui->mainWidget->setContentsMargins(1,1,1,1);
    //设置输入密码明文的显示/隐藏
    QAction *action = new QAction(this);
    action->setIcon(QIcon(":/new/prefix1/preview-close.png"));
    connect(ui->pwdLineEdit,&QtMaterialAutoComplete::textEdited,this,[=]{
        static bool first = true;
        if(first)
        {
            ui->pwdLineEdit->addAction(action,QLineEdit::TrailingPosition);
            first = false;
        }
    });
    bool open = false;
    connect(action,&QAction::triggered,this,[=]()mutable{
        if(open == false)
        {
            action->setIcon(QIcon(":/new/prefix1/eyes-open.png"));
            ui->pwdLineEdit->addAction(action,QLineEdit::TrailingPosition);
            open = true;
            ui->pwdLineEdit->setEchoMode(QLineEdit::Normal);
        }
        else
        {
            action->setIcon(QIcon(":/new/prefix1/preview-close.png"));
            ui->pwdLineEdit->addAction(action,QLineEdit::TrailingPosition);
            open = false;
            ui->pwdLineEdit->setEchoMode(QLineEdit::Password);
        }
    });
    //设置登录按钮的颜色和悬停效果
    ui->loginPushButton->setForegroundColor(QColor(90, 194, 198));
    ui->loginPushButton->setOverlayStyle(Material::TintedOverlay);
    //设置注册按钮的颜色和悬停效果
    ui->registerPushButton->setForegroundColor(QColor(90, 194, 198));
    ui->registerPushButton->setOverlayStyle(Material::TintedOverlay);
    //创建抽屉式注册界面（hide）
    createMaterialDrawer();
    //创建提示信息的对话框（hide）
    createMaterialDialog();
}

HomePage::~HomePage()
{
    delete ui;
}



void HomePage::on_loginPushButton_clicked()
{
    QString userId = ui->userIdLineEdit->text();
    QString pwd = ui->pwdLineEdit->text();
    int id = userId.toInt();
    //用户id非法输入的提示信息
    if(id <= 0)
    {
        label->setText("the id of user must be number bigger than zero");
        emit showMaterialDialog();
        return;
    }
    //向服务器发送登录请求
    json js;
    js["msgid"] = LOGIN_MSG;
    js["id"] = id;
    js["password"] = pwd.toStdString();
    std::string request = js.dump();
    Socket->Send(QString(request.c_str()).toUtf8());

    while(!Socket->getReadReady());//等待服务器返回用户信息

    //读取并处理服务器返回的用户信息
    QByteArray buffer = Socket->Read();
    json resonsejs = json::parse(buffer.data());
    if(resonsejs["errno"].get<int>() != 0)
    {

          label->setText(resonsejs["errmsg"].get<std::string>().c_str());
          emit showMaterialDialog();
    }
    else
    {
        //创建用户主界面
        MainMenu *mainMenu = new MainMenu(nullptr,resonsejs);
        mainMenu->setSocket(Socket);
        mainMenu->setAttribute(Qt::WA_DeleteOnClose);
        connect(mainMenu,&MainMenu::showParentWindow,this,&HomePage::showWindow);//连接用户loginout的信号和登录界面显示的槽函数
        this->hide();
        ui->pwdLineEdit->clear();
        ui->userIdLineEdit->clear();
        mainMenu->show();
    }
}

void HomePage::showWindow()
{
    this->show();
}


void HomePage::on_closePushButton_clicked()
{
    this->close();
}

//mouseMoveEvent、mousePressEvent、mouseReleaseEvent实现了自定义窗口的拖动功能
void HomePage::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton && m_bMove)
    {
        QPoint parentPoint = this->pos();
        parentPoint.setX(parentPoint.x() + event->x() - m_pressPoint.x());
        parentPoint.setY(parentPoint.y() + event->y() - m_pressPoint.y());
        this->move(parentPoint);
    }
}

void HomePage::mousePressEvent(QMouseEvent *event)
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

void HomePage::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_bMove)
    {
        m_bMove = false;
    }
}

void HomePage::createMaterialDialog()
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
    connect(this,&HomePage::showMaterialDialog,mdialog,&QtMaterialDialog::showDialog);
    connect(closeButton,&QtMaterialFlatButton::pressed,mdialog,&QtMaterialDialog::hideDialog);
}

void HomePage::createMaterialDrawer()
{
    //创建抽屉式注册页面
    mdrawer = new QtMaterialDrawer(this);
    mdrawer->setParent(ui->mainWidget);
    //设置样式，阴影+点击关闭
    mdrawer->setOverlayMode(true);
    mdrawer->setClickOutsideToClose(true);
    //添加垂直布局
    QVBoxLayout *layout = new QVBoxLayout;
    mdrawer->setDrawerLayout(layout);
    //创建标题label
    QLabel *title = new QLabel(this);
    title->setText("REGISTER");
    QFont ft;
    ft.setPixelSize(20);
    title->setFont(ft);
    //创建输入框
    userNameLine = new QtMaterialAutoComplete(this);
    userNameLine->setPlaceholderText("Username");
    userPasswordLine = new QtMaterialAutoComplete(this);
    userPasswordLine->setPlaceholderText("Password");
    //创建提交按钮
    QtMaterialFlatButton *button = new QtMaterialFlatButton(this);
    button->setOverlayStyle(Material::TintedOverlay);
    button->setForegroundColor(QColor(90, 194, 198));
    button->setText("commit");
    //添加以上组件
    layout->addWidget(title);
    layout->addWidget(userNameLine);
    layout->addWidget(userPasswordLine);
    layout->addWidget(button);
    layout->setAlignment(button,Qt::AlignCenter);
    layout->setAlignment(title,Qt::AlignCenter);
    //size设置
    userNameLine->setMinimumHeight(50);
    userPasswordLine->setMinimumHeight(50);
    button->setMinimumHeight(50);
    layout->setContentsMargins(10,50,10,40);
    //连接显示、提交的槽函数
    connect(ui->registerPushButton,&QPushButton::pressed,mdrawer,&QtMaterialDrawer::openDrawer);
    connect(button,&QtMaterialFlatButton::pressed,this,&HomePage::toRegister);
}

void HomePage::toRegister()
{
    QString name = userNameLine->text();
    QString pwd = userPasswordLine->text();
    userNameLine->clear();
    userPasswordLine->clear();
    //空值检测
    if(name == "" || pwd == "")
    {
        label->setText("name or pwd is null,commit error!");
        emit showMaterialDialog();
        return;
    }
    //向服务器发送注册请求
    json js;
    js["msgid"] = REG_MSG;
    js["name"] = name.toStdString();
    js["password"] = pwd.toStdString();
    std::string request = js.dump();
    Socket->Send(QString(request.c_str()).toUtf8());
    while(!Socket->getReadReady());//等待子线程读取应答信息
    QByteArray buffer = Socket->Read();
    json responsejs = json::parse(buffer.data());
    if(responsejs["errno"].get<int>() != 0)
    {
        //用户已存在
        label->setText(name.append(" is already exist,register error!"));
        emit showMaterialDialog();
    }
    else
    {
        //添加成功，返回新用户id信息
        label->setText(name.append(" register success,userid is ").append(QString::number(responsejs["id"].get<int>())).append(",don't forget it!"));
        emit showMaterialDialog();
    }
}

void HomePage::on_pushButton_2_clicked()
{
    this->showMinimized();//窗口最小化
}

