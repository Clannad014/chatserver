#include "widget.h"
#include "ui_widget.h"
#include "homepage.h"


widget::widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::widget)
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
    ui->connectPushButton->setForegroundColor(QColor(90, 194, 198));
    ui->connectPushButton->setOverlayStyle(Material::TintedOverlay);
    //创建socket
    Socket = new MyTcpSocket();
    myThread = new QThread(this);
    //将socket的连接、读写放到子线程处理
    Socket->moveToThread(myThread);
    myThread->start();
}

bool widget::isIp(QString &ip)
{
    QHostAddress address;
    if(!address.setAddress(ip))
    {
        return false;
    }
    if(ip.isEmpty())
    {
        return false;
    }
    QStringList list = ip.split(".");
    if(list.size() != 4)
    {
        return false;
    }
    for(QString & num : list)
    {
        bool isInt = false;
        int tem = num.toInt(&isInt);
        if( (!isInt) || (tem < 0) || (tem > 255))
        {
            return false;
        }
    }
    return true;
}

widget::~widget()
{
    myThread->quit();
    QElapsedTimer timer;
    timer.start();
    while(timer.elapsed() < 50);//等待子线程完全退出
    delete Socket;
    delete ui;
}

void widget::on_connectPushButton_clicked()
{
    QString ip = ui->ipLineEdit->text();
    QString port = ui->portLineEdit->text();
    if(!isIp(ip))
    {
        QMessageBox::information(this,"message","invalid ip");
        return;
    }
    bool isInt = false;
    int tem = port.toInt(&isInt);
    if(!isInt)
    {
        QMessageBox::information(this,"message","invalid port");
        return;
    }
    Socket->ConnetToHost(ip,port.toInt());//连接服务器
    QElapsedTimer timer;
    timer.start();
    while(timer.elapsed() < 100);//设置100ms延迟等待子线程连接处理完毕
    if(Socket->IsConnected())
    {
        //创建登录界面并显示
        HomePage *homePage = new HomePage();
        homePage->setAttribute(Qt::WA_DeleteOnClose);
        homePage->setSocket(Socket);//传递socket便于后续读写
        this->close();
        homePage->show();
    }
    else
    {
        QMessageBox::information(this,"message","connect server error !");
    }
}

//mouseMoveEvent、mousePressEvent、mouseReleaseEvent实现了窗口拖动
void widget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton && m_bMove)
    {
        QPoint parentPoint = this->pos();
        parentPoint.setX(parentPoint.x() + event->x() - m_pressPoint.x());
        parentPoint.setY(parentPoint.y() + event->y() - m_pressPoint.y());
        this->move(parentPoint);
    }
}

void widget::mousePressEvent(QMouseEvent *event)
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

void widget::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_bMove)
    {
        m_bMove = false;
    }
}

void widget::on_closePushButton_clicked()
{
    this->close();
}


void widget::on_pushButton_2_clicked()
{
    this->showMinimized();
}

