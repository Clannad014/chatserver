//#include"usermodel.hpp"
//#include"db.hpp"
//bool userModel::insert(user& user){//将user对象中的内容插入到user表中
//    char buf[1024]={0};
//    snprintf(buf,1024,"insert into user(name,password,state) values('%s','%s','%s')",
//    user.getName().data(),user.getPassword().data(),user.getState().data());
//    MySQL mysql_;
//    if(mysql_.connect()){
//        if(mysql_.update(buf)){
//            user.setId(mysql_insert_id(mysql_.getConnection()));
//            return true;
//        }
//    }
//    return false;
//}

//user  userModel::query(int id){
//    user user_;
//    char buf[1024]={0};
//    snprintf(buf,1024,"select* from user where id=%d",id);
//    MySQL mysql_;
//    if(mysql_.connect()){
//        MYSQL_RES* res=mysql_.query(buf);
//        if(res!=nullptr){
//            MYSQL_ROW row=mysql_fetch_row(res);
//            if(row!=nullptr){
//                user_.setId(atoi(row[0]));
//                user_.setName(row[1]);
//                user_.setPassword(row[2]);
//                user_.setState(row[3]);
//                mysql_free_result(res);
//            }
//        }
//    }
//    return user_;
//}

//bool userModel::updateState(user user_){
//    char buf[1024]={0};
//    snprintf(buf,1024,"update user set state='%s' where id=%d",user_.getState().data(),user_.getId());
//    MySQL mysql_;
//    if(mysql_.connect()){
//        if(mysql_.update(buf)){
//            return true;
//        }
//    }
//    return false;
//}

//bool userModel::resetOffline(){
//    char buf[1024]={0};
//    snprintf(buf,1024,"update user set state='offline'");
//    MySQL mysql_;
//    if(mysql_.connect()){
//        if(mysql_.update(buf)){
//            return true;
//        }
//    }
//    return false;
//}

//bool userModel::userAlive(int id){
//    char sql[1024]={0};
//    snprintf(sql,1024,"select * from user where id= %d",id);
//    MySQL mysql_;
//    if(mysql_.connect()){
//        MYSQL_RES*res;
//        if((res=mysql_.query(sql))!=nullptr){
//            MYSQL_ROW row=mysql_fetch_row(res);
//            if(row!=nullptr){
//                return true;
//            }
//        }
//    }
//    return false;
//}
