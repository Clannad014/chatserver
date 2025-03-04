#include "mainmenu.h"
#include "ui_mainmenu.h"

User g_currentUser;//保存当前登录用户的信息
vector<User> g_currentUserFriendList;//保存当前登录用户的好友列表信息
vector<Group> g_currentUserGroupList;//保存当前登录用户的群组列表信息
vector<ChatDialog *> _dialogVec;/*保存聊天页面对象的指针，凡是有聊天消息的发送或者接收，
                                    就会创建该聊天对象的页面，该数组保存的是被创建的所有页面对象*/
mutex _dialogVecMutex;//聊天页面对象数组的锁
MyTcpSocket *Socket;//socket对象
vector<QString> _avatarVec = {":/new/prefix1/img1.webp",
                              ":/new/prefix1/img2.webp",
                              ":/new/prefix1/img3.webp",
                              ":/new/prefix1/img4.webp",
                              ":/new/prefix1/img5.webp",
                              ":/new/prefix1/img6.webp",
                              ":/new/prefix1/img7.webp",
                              ":/new/prefix1/img8.webp",};//保存系统头像的图片路径
MainMenu::MainMenu(QWidget *parent,json responsejs) :
    QDialog(parent),
    ui(new Ui::MainMenu)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);//设置无边框
    this->setAttribute(Qt::WA_TranslucentBackground);//设置背景完全透明
    //设置按钮样式
    ui->addFriendPushButton->setBackgroundColor(QColor(0,188,212));
    ui->addGroupPushButton->setBackgroundColor(QColor(0,188,212));
    ui->addFriendPushButton->setCornerRadius(5);
    ui->addGroupPushButton->setCornerRadius(5);
    ui->loginoutPushButton->setTextAlignment(Qt::AlignCenter);
    ui->loginoutPushButton->setIcon(QIcon(":/new/prefix1/LoginOutlined.png"));
    ui->loginoutPushButton->setBackgroundColor(QColor(0,188,212));
    ui->loginoutPushButton->setCornerRadius(5);
    //添加菜单栏
    ui->tabsWidget->addTab("friends");
    ui->tabsWidget->addTab("groups");
    //添加阴影
    QGraphicsDropShadowEffect *shadow1 = new QGraphicsDropShadowEffect(this);
    shadow1->setOffset(0,0);
    shadow1->setColor(QColor("#444444"));
    shadow1->setBlurRadius(20);
    ui->userMessageWidget->setGraphicsEffect(shadow1);
    ui->userMessageWidget->setContentsMargins(1,1,1,1);
    QGraphicsDropShadowEffect *shadow2 = new QGraphicsDropShadowEffect(this);
    shadow2->setOffset(0,0);
    shadow2->setColor(QColor("#444444"));
    shadow2->setBlurRadius(20);
    ui->Listwidget->setGraphicsEffect(shadow2);
    ui->Listwidget->setContentsMargins(1,1,1,1);
    QGraphicsDropShadowEffect *shadow3 = new QGraphicsDropShadowEffect(this);
    shadow3->setOffset(0,0);
    shadow3->setColor(QColor("#444444"));
    shadow3->setBlurRadius(20);
    ui->mainWidget->setGraphicsEffect(shadow3);
    ui->mainWidget->setContentsMargins(1,1,1,1);
    //连接菜单当前项切换信号和页面切换槽函数
    connect(ui->tabsWidget,&QtMaterialTabs::currentChanged,this,&MainMenu::stackedWidgetChange);
    //设置页面滚动条
    QtMaterialScrollBar *mscrollBar1 = new QtMaterialScrollBar(this);
    mscrollBar1->setMinimumHeight(8);
    ui->friendListWidget->setVerticalScrollBar(mscrollBar1);
    QtMaterialScrollBar*mscrollBar2 = new QtMaterialScrollBar(this);
    mscrollBar2->setMinimumHeight(8);
    ui->groupListWidget->setVerticalScrollBar(mscrollBar2);
    //创建添加好友页面
    afd = new AddFriendPage;
    afd->setAvatarImgVec(_avatarImgVec);
    afd->setNoticeAvatarImgVec();
    afd->hide();
    //创建添加群组页面
    agp = new AddGroupPage;
    //当收到添加群组结果信息后，在该页面显示
    connect(this,SIGNAL(addGroupAck(QString)),agp,SLOT(showAck(QString)));
    agp->hide();
    //创建聊天页面shell
    this->shell = new ChatShell;
    this->shell->setAvatarImgVec(_avatarImgVec);
    shell->setMinimumSize(850,600);
    shell->hide();
    connect(this,SIGNAL(addFriendAck(QString)),afd,SLOT(showAck(QString)));
    _dialogVec.clear();//重置用户的页面数组
    //连接列表点击信号和在shell中添加聊天页面的槽函数
    connect(this,SIGNAL(ListClicked(int,QString,int,ChatDialog*)),
            shell,SLOT(addDialogToShell(int,QString,int,ChatDialog*)));
    qDebug() << responsejs.dump().c_str();
    //当前用户信息初始化
    qDebug() << "用户信息初始化";
    g_currentUser.setId(responsejs["id"].get<int>());
    g_currentUser.setName(responsejs["name"]);
    g_currentUser.setAvatar(responsejs["avatar"].get<int>());
    //当前用户好友列表初始化
    qDebug() << "好友列表初始化";
    if(responsejs.contains("friends"))
    {
        g_currentUserFriendList.clear();
        vector<string> vec = responsejs["friends"];
        for(string &str:vec)
        {
            json js = json::parse(str);
            User user;
            user.setId(js["id"].get<int>());
            user.setName(js["name"]);
            user.setState(js["state"]);
            user.setAvatar(js["avatar"].get<int>());
            g_currentUserFriendList.push_back(user);
        }
    }
    qDebug() << "群组列表初始化";
    //当前用户群组列表初始化
    if(responsejs.contains("groups"))
    {
        g_currentUserGroupList.clear();
        vector<string> vec1 = responsejs["groups"];
        for(string &groupstr : vec1)
        {
            json grpjs = json::parse(groupstr);
            Group group;
            group.setGroupId(grpjs["id"].get<int>());
            group.setGroupName(grpjs["groupname"]);
            group.setGroupDesc(grpjs["groupdesc"]);
            vector<string> vec2 = grpjs["users"];
            for(string &userstr : vec2)
            {
                GroupUser user;
                json js = json::parse(userstr);
                user.setId(js["id"].get<int>());
                user.setName(js["name"]);
                user.setState(js["state"]);
                user.setAvatar(js["avatar"].get<int>());
                user.setGroupRole(js["role"]);
                group.getGroupUsers().push_back(user);
            }
            g_currentUserGroupList.push_back(group);
        }
    }
    qDebug() << "显示当前用户信息";
    //显示当前用户信息
    showCurrentUserData();
    qDebug() << "加载离线消息到聊天页面";
    //加载离线消息到聊天页面
    if(responsejs.contains("offlinemsg"))
    {
        vector<string> vec = responsejs["offlinemsg"];
        for(string &str : vec)
        {
            json js = json::parse(str);
            newDialog(js);
        }
    }
    qDebug() << "end";
}

MainMenu::~MainMenu()
{
    delete afd;
    delete agp;
    delete shell;
    delete ui;
}

void MainMenu::setSocket(MyTcpSocket *HomeSocket)
{
    //获取到homepage的socket
    Socket = HomeSocket;
    //连接socket读信号和主页面的消息处理槽函数
    connect(Socket,SIGNAL(ReadMessage(QString)),this,SLOT(recvMessage(QString)));
}

void MainMenu::showCurrentUserData()
{
    ui->nameLabel->setText(g_currentUser.getName().c_str());//用户名显示
    ui->pixLabel->setScaledContents(true);//头像大小自适应
    ui->pixLabel->installEventFilter(this);//加载头像切换事件过滤器
    avatarIndex = 0;//头像索引初始化为0，用于头像切换
    setAvatar(_avatarVec[g_currentUser.getAvatar()]);//从用户信息中获取头像索引并设置头像
    friendListReload();//显示好友列表
    groupListReload();//显示群组列表
}


//接受服务器消息的处理函数（消息的json字符串来源于子线程）
void MainMenu::recvMessage(QString buffer)
{
    //消息反序列化
    json js = json::parse(buffer.toStdString().c_str());
    int msgid = js["msgid"].get<int>();
    //聊天消息处理
    if (msgid == ONE_CHAT_MSG || msgid == GROUP_CHAT_MSG)
    {
        newDialog(js);
    }
    //好友列表的更新消息处理
    else if(msgid == RETURN_FRIEND_ACK)
    {
        g_currentUserFriendList.clear();
        vector<string> vec = js["friends"];
        for(string &str:vec)
        {
            json ujs = json::parse(str);
            User user;
            user.setId(ujs["id"].get<int>());
            user.setName(ujs["name"]);
            user.setState(ujs["state"]);
            user.setAvatar(ujs["avatar"].get<int>());
            g_currentUserFriendList.push_back(user);
        }
        friendListReload();
    }
    //群组列表的更新消息处理
    else if(msgid == RETURN_GROUP_ACK)
    {
        g_currentUserGroupList.clear();
        vector<string> vec1 = js["groups"];
        for(string &groupstr : vec1)
        {
            json grpjs = json::parse(groupstr);
            Group group;
            group.setGroupId(grpjs["id"].get<int>());
            group.setGroupName(grpjs["groupname"]);
            group.setGroupDesc(grpjs["groupdesc"]);
            vector<string> vec2 = grpjs["users"];
            for(string &userstr : vec2)
            {
                GroupUser user;
                json ujs = json::parse(userstr);
                user.setId(ujs["id"].get<int>());
                user.setName(ujs["name"]);
                user.setState(ujs["state"]);
                user.setGroupRole(ujs["role"]);
                user.setAvatar(ujs["avatar"].get<int>());
                group.getGroupUsers().push_back(user);
            }
            g_currentUserGroupList.push_back(group);
        }
        groupListReload();
        emit groupListUpdated();
    }
    //添加好友和群组的结果消息处理
    else if(msgid == ADD_FRIEND_ACK || msgid == ADD_GROUP_ACK)
    {
        if(msgid == ADD_FRIEND_ACK)
        {
            emit addFriendAck(js["msg"].get<string>().c_str());
        }
        else
        {
            emit addGroupAck(js["msg"].get<string>().c_str());
        }
       if(js["errno"].get<int>() == 0)
       {
            if(msgid == ADD_GROUP_ACK)
            {
                updateGroupList();
            }
       }
    }
    //好友列表的在线状态实时更新消息处理
    else if(msgid == STATE_UPDATE_ACK)
    {
       int id = js["id"].get<int>();
       QString state = js["state"].get<string>().c_str();
       int row = 0;
       for(User &u : g_currentUserFriendList)
       {
            if(u.getId() == id)
            {
                u.setState(state.toStdString());
                friendListItemUpdate(row,state);
                break;
            }
            row++;
       }
    }
    //好友添加请求的消息处理
    else if(msgid == ADD_FRIEND_MSG)
    {
       afd->addNoticeMsg(js);
    }
}


void MainMenu::on_friendListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    //向shell中添加所点击好友的聊天页面并显示，如果已经添加就不再添加只单纯显示
    MyListItem *myitem = (MyListItem *)ui->friendListWidget->itemWidget(item);
    newDialog(myitem->getId(),myitem->getName(),myitem->getAvatar(),ONE_CHAT_MSG);
}

//获取系统当前时间
string MainMenu::getCurrentTime()
{
    auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    struct tm *ptm = localtime(&tt);
    char date[60] = {0};
    sprintf(date, "%d-%02d-%02d %02d:%02d:%02d",
            (int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1, (int)ptm->tm_mday, (int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec);

    return std::string(date);
}


void MainMenu::on_groupListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    //向shell中添加所点击群组的聊天页面并显示，如果已经添加就不再添加只单纯显示
    MyListItem *myitem = (MyListItem *)ui->groupListWidget->itemWidget(item);
    newDialog(myitem->getId(),myitem->getName(),-1,GROUP_CHAT_MSG);
}


void MainMenu::on_addFriendPushButton_clicked()
{
    afd->show();
}


void MainMenu::on_addGroupPushButton_clicked()
{
    agp->show();
}


void MainMenu::on_loginoutPushButton_clicked()
{
    //向服务器发送用户注销请求
    json js;
    js["msgid"] = LOGIN_OUT_MSG;
    js["id"] = g_currentUser.getId();
    string buffer = js.dump();
    Socket->Send(QString(buffer.c_str()).toUtf8());
    Socket->Read();
    emit showParentWindow();//发出注销信号，显示登录界面
    this->close();
}

void MainMenu::removepointer(int toid,int chattype)
{
    //删除好友或者群组聊天页面数组中的页面对象
    if(chattype == ONE_CHAT_MSG)
    {
    lock_guard<mutex> lock(_friendDialogMutex);
    auto it = _friendDialoglist.find(toid);
    if(it != _friendDialoglist.end())
    {
        _friendDialoglist.erase(it);
    }
    }
    else if(chattype == GROUP_CHAT_MSG)
    {
        lock_guard<mutex> lock(_groupDialogMutex);
        auto it = _groupDialoglist.find(toid);
        if(it != _groupDialoglist.end())
        {
            _groupDialoglist.erase(it);
        }
    }
}

void MainMenu::newDialog(int id,QString name,int avatarIndex,int chattype)
{
    if(chattype == ONE_CHAT_MSG)
    {
        lock_guard<mutex> lock(_friendDialogMutex);//保证好友聊天页面数组的线程安全
        auto it = _friendDialoglist.find(id);
        //判断聊天页面是否已经创建过并添加到了shell
        if(it != _friendDialoglist.end())
        {
            int index = 0;
            for(ChatDialog * d : _dialogVec)
            {                
                if(id == d->getId() && chattype == d->getType())
                {
                    //已经创建而且添加到了shell，显示页面
                    emit ListClicked(-1 * index,"",-1,d);
                    shell->show();
                    return;
                }
                index++;
            }
            {
                //已经创建但是并未添加到shell，则添加到shell然后显示
                lock_guard<mutex> lock(_dialogVecMutex);
                _dialogVec.push_back(it->second);
                emit ListClicked(id,name,avatarIndex,it->second);
            }
            shell->show();
            return;
        }
        //未创建，则创建页面对象并添加到shell
        ChatDialog *chatDialog = new OneChatDialog(this,id,name,ONE_CHAT_MSG);
        connect(chatDialog,SIGNAL(closeWindow(int,int)),this,SLOT(removepointer(int,int)));
        chatDialog->setAttribute(Qt::WA_DeleteOnClose);
        chatDialog->setWindowTitle(name);
        //将创建的好友聊天页面对象存到好友聊天页面数组中
        _friendDialoglist.insert({id,chatDialog});
        {
            lock_guard<mutex> lock(_dialogVecMutex);
            _dialogVec.push_back(chatDialog);
            emit ListClicked(id,name,avatarIndex,chatDialog);
        }
        shell->show();
    }
    else if(chattype == GROUP_CHAT_MSG)
    {
        lock_guard<mutex> lock(_groupDialogMutex);//保证群组聊天页面数组的线程安全
        auto it = _groupDialoglist.find(id);
        //判断聊天页面是否已经创建过并添加到了shell
        if(it != _groupDialoglist.end())
        {

            int index = 0;
            for(ChatDialog * d : _dialogVec)
            {
                if(id == d->getId() && chattype == d->getType())
                {
                    //已经创建而且添加到了shell，显示页面
                    emit ListClicked(-1 * index,"",-1,d);
                    shell->show();
                    return;
                }
                index++;
            }
            {
                //已经创建但是并未添加到shell，则添加到shell然后显示
                lock_guard<mutex> lock(_dialogVecMutex);
                _dialogVec.push_back(it->second);
                emit ListClicked(id,name,avatarIndex,it->second);
            }
            shell->show();
            return;
        }
        //未创建，则创建页面对象并添加到shell
        ChatDialog *chatDialog = new GroupChatDialog(this,id,name,GROUP_CHAT_MSG);
        ((GroupChatDialog *)chatDialog)->setAvatarImgVec(_avatarImgVec);
        connect(this,&MainMenu::groupListUpdated,(GroupChatDialog *) chatDialog,&GroupChatDialog::reloadMembers);//有群成员状态更新信号则重新加载群成员列表
        connect(chatDialog,SIGNAL(closeWindow(int,int)),this,SLOT(removepointer(int,int)));
        chatDialog->setAttribute(Qt::WA_DeleteOnClose);
        chatDialog->setWindowTitle(name);
        //将创建的群组聊天页面对象存到好友聊天页面数组中
        _groupDialoglist.insert({id,chatDialog});
        {
            lock_guard<mutex> lock(_dialogVecMutex);
            _dialogVec.push_back(chatDialog);
            emit ListClicked(id,name,avatarIndex,chatDialog);
        }
        shell->show();
    }
}

//根据Json字符串（包含聊天的消息）创建聊天页面对象
void MainMenu::newDialog(json js)
{
    int msgid = js["msgid"].get<int>();
    if(msgid == ONE_CHAT_MSG)
    {
        lock_guard<mutex> lock(_friendDialogMutex);
        auto it = _friendDialoglist.find(js["id"].get<int>());
        //判断聊天页面对象是否被创建过
        if(it != _friendDialoglist.end())
        {
            //已经创建过，显示聊天消息
            it->second->displayMsg(js);
            return;
        }
        //未被创建过，先创建再显示聊天消息
        ChatDialog *chatDialog = new OneChatDialog(this,js["id"].get<int>(),js["name"].get<string>().c_str(),ONE_CHAT_MSG);
        connect(chatDialog,SIGNAL(closeWindow(int,int)),this,SLOT(removepointer(int,int)));
        chatDialog->setAttribute(Qt::WA_DeleteOnClose);
        chatDialog->setWindowTitle(js["name"].get<string>().c_str());
        _friendDialoglist.insert({js["id"].get<int>(),chatDialog});
        chatDialog->displayMsg(js);
    }
    else if(msgid == GROUP_CHAT_MSG)
    {
        lock_guard<mutex> lock(_groupDialogMutex);
        auto it = _groupDialoglist.find(js["groupId"].get<int>());
        //判断聊天页面对象是否被创建过
        if(it != _groupDialoglist.end())
        {
            //已经创建过，显示聊天消息
            it->second->displayMsg(js);
            return;
        }
        //未被创建过，先创建再显示聊天消息
        ChatDialog *chatDialog = new GroupChatDialog(this,js["groupId"].get<int>(),js["groupName"].get<string>().c_str(),GROUP_CHAT_MSG);
        ((GroupChatDialog *)chatDialog)->setAvatarImgVec(_avatarImgVec);
        connect(this,&MainMenu::groupListUpdated,(GroupChatDialog *)chatDialog,&GroupChatDialog::reloadMembers);//有群成员状态更新信号则重新加载群成员列表
        connect(chatDialog,SIGNAL(closeWindow(int,int)),this,SLOT(removepointer(int,int)));
        chatDialog->setAttribute(Qt::WA_DeleteOnClose);
        chatDialog->setWindowTitle(js["groupName"].get<string>().c_str());
        _groupDialoglist.insert({js["groupId"].get<int>(),chatDialog});
        chatDialog->displayMsg(js);
    }
}

void MainMenu::updateGroupList()
{
    //向服务器发送更新群组列表的请求，以获取最新的群组列表信息
    json js;
    js["msgid"] = UPDATE_GROUP_MSG;
    js["id"] = g_currentUser.getId();
    std::string request = js.dump();
    Socket->Send(QString(request.c_str()).toUtf8());
}

void MainMenu::setAvatar(QString pixPath)
{
    //设置头像的样式，实现抗锯齿的圆形头像
    QPixmap pixmapa(pixPath);
    QPixmap pixmap(60,60);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QPainterPath path;
    path.addEllipse(0,0,60,60);
    painter.setClipPath(path);
    painter.drawPixmap(0,0,60,60,pixmapa);
    ui->pixLabel->setPixmap(pixmap);
}

bool MainMenu::eventFilter(QObject *obj, QEvent *event)
{
    //点击头像更换头像图片
    if(obj == ui->pixLabel)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if(mouseEvent->button() == Qt::LeftButton)
            {
                QString avatar = _avatarVec[(++avatarIndex) % _avatarVec.size()];
                setAvatar(avatar);
                updateAvatar(avatarIndex);
                return true;
            }
        }
    }
    return QWidget::eventFilter(obj,event);
}

void MainMenu::updateAvatar(int avatarIndex)
{
    //向服务器发送用户头像更新的请求
    json js;
    js["msgid"] = UPDATE_AVATAR_MSG;
    js["id"] = g_currentUser.getId();
    js["avatar"] = avatarIndex;
    std::string buffer = js.dump();
    Socket->Send(QString(buffer.c_str()).toUtf8());
    g_currentUser.setAvatar(avatarIndex);
}

void MainMenu::stackedWidgetChange()
{
    ui->stackedWidget->setCurrentIndex(ui->tabsWidget->currentIndex());
}


//mouseMoveEvent、mousePressEvent、mouseReleaseEvent实现了窗口拖动
void MainMenu::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton && m_bMove)
    {
        QPoint parentPoint = this->pos();
        parentPoint.setX(parentPoint.x() + event->x() - m_pressPoint.x());
        parentPoint.setY(parentPoint.y() + event->y() - m_pressPoint.y());
        this->move(parentPoint);
    }
}

void MainMenu::mousePressEvent(QMouseEvent *event)
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

void MainMenu::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_bMove)
    {
        m_bMove = false;
    }
}

void MainMenu::on_closePushButton_clicked()
{
    this->close();
}

void MainMenu::friendListReload()
{
    //好友列表ui重新加载
    ui->friendListWidget->clear();
    for(int i = 0;i < g_currentUserFriendList.size();i++)
    {
        QListWidgetItem *item = new QListWidgetItem(ui->friendListWidget);
        item->setSizeHint(QSize(120,80));
        MyListItem *myItem = new MyListItem(this);
        myItem->setAvatar(g_currentUserFriendList[i].getAvatar(),_avatarImgVec[g_currentUserFriendList[i].getAvatar()]);
        myItem->setName(g_currentUserFriendList[i].getName().c_str());
        myItem->setState(g_currentUserFriendList[i].getState().c_str());
        myItem->setId(g_currentUserFriendList[i].getId());
        myItem->setType(MENUITEM);
        ui->friendListWidget->addItem(item);
        ui->friendListWidget->setItemWidget(item,myItem);
    }
}

void MainMenu::groupListReload()
{
    //群组列表ui重新加载
    ui->groupListWidget->clear();
    for(int i = 0;i < g_currentUserGroupList.size();i++)
    {
        QListWidgetItem *item = new QListWidgetItem(ui->groupListWidget);
        item->setSizeHint(QSize(120,80));
        MyListItem *myItem = new MyListItem(this);
        myItem->setTextAvatar(g_currentUserGroupList[i].getGroupName().c_str());
        myItem->setName(g_currentUserGroupList[i].getGroupName().c_str());
        myItem->hideState();
        myItem->setId(g_currentUserGroupList[i].getGroupId());
        myItem->setType(MENUITEM);
        ui->groupListWidget->addItem(item);
        ui->groupListWidget->setItemWidget(item,myItem);
    }
}


void MainMenu::on_pushButton_2_clicked()
{
    this->showMinimized();
}

void MainMenu::friendListItemUpdate(int row, QString state)
{
    //更新好友列表ui的单个好友的状态
    MyListItem *item = (MyListItem *)ui->friendListWidget->itemWidget(ui->friendListWidget->item(row));
    item->setState(state);
}

