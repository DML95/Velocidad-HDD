#include <iostream>

#include "wait.h"

Wait::Wait(){
    std::clog<<"[Wait] Loadding"<<std::endl;
    this->event=std::make_shared<std::condition_variable>();
}

void Wait::wait(){
    std::unique_lock<std::mutex> uniqueLock(this->mutex);
    this->event->wait(uniqueLock);
}

void Wait::notify_all(){
    this->event->notify_all();
}
