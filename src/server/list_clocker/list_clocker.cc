#include"list_clocker.h"

clocker_lst::clocker_lst():head(nullptr),tail(nullptr){}

clocker_lst::~clocker_lst(){
    clocker* temp=head;
    while(temp){
        head=head->next;
        delete temp;
        temp=head;
    }
}

void clocker_lst::add_clocker(clocker*object){

    if(!head){
        head=tail=object;
        return;
    }
    if(object->expire<head->expire){
        head->prev=object;
        object->next=head;
        head=object;
        return;
    }
    add_clocker(object,head);
}
void clocker_lst::add_clocker(clocker*object,clocker*head){
    clocker*temp=head->next;
    while(temp){
        if(object->expire<temp->expire){
            temp->prev->next=object;
            object->next=temp;
            object->prev=temp->prev;
            temp->prev=object;
            return;
        }
        temp=temp->next;
    }
    tail->next=object;
    object->prev=tail;
}

void clocker_lst::adjust_clocker(clocker*object){
    if(object==head&&object==tail){
        return;
    }
    if(object==head){
        head=object->next;
        head->prev=nullptr;
        add_clocker(object);
        return;
    }
    if(object==tail){
        tail=tail->prev;
        tail->prev=nullptr;
        add_clocker(object);
        return;
    }
    object->prev->next=object->next;
    object->next->prev=object->prev;
    add_clocker(object);
}

void clocker_lst::del_clocker(clocker*object){
    if(object==head&&object==tail){
        delete object;
        head=tail=nullptr;
        return;
    }
    if(object==head){
        head=head->next;
        head->prev=nullptr;
        delete object;
        return;
    }
    if(object==tail){
        tail=tail->prev;
        tail->next=nullptr;
        delete object;
        return;
    }
    object->prev->next=object->next;
    object->next->prev=object->prev;
    delete object;
}

void clocker_lst::tick(){
    if(!head){
        return;
    }
    time_t cur=time(NULL);
    clocker*temp=head;
    while(temp){
        if(cur<temp->expire){
           break;
        }
        temp->doNow();
        del_clocker(temp);
    }
}