#include "recvbox.h"
#include "ui_recvbox.h"

RecvBox::RecvBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RecvBox)
{
    ui->setupUi(this);
    //设置文本可复制
    ui->msgLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    //设置文本自动换行+填充
    ui->msgLabel->setWordWrap(true);
    ui->imgLabel->setScaledContents(true);
    //设置样式
    setStyleSheet("#msgLabel{padding:12px;background-color: rgb(255, 255, 255);;border-radius:12px;color: rgb(0, 0, 0);}");
}

RecvBox::~RecvBox()
{
    delete ui;
}

void RecvBox::setText(QString text)
{
    //设置文本
    ui->msgLabel->setText(text);
}

void RecvBox::setAvatar(QString pixPath)
{
    //设置抗锯齿头像
    QPixmap pixmapa(pixPath);
    QPixmap pixmap(40,40);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QPainterPath path;
    path.addEllipse(0,0,40,40);
    painter.setClipPath(path);
    painter.drawPixmap(0,0,40,40,pixmapa);
    ui->imgLabel->setPixmap(pixmap);
}

void RecvBox::hideNameLabel()
{
    //名称部分隐藏
    ui->nameLabel->hide();
}

void RecvBox::setName(QString name)
{
    //设置名称
    ui->nameLabel->setText(name);
}
