//
// Created by eftales on 2022/6/6.
//

#include <thread>
#include "ShareBuffer.h"

ShareBuffer L3EventBuffers;
ShareBuffer L2EventBuffers;
ShareBuffer sockSendBuffers;

ShareBuffer::ShareBuffer() {
    for(int i =0;i<SHAREBUFFER_SIZE;i++){
        this->buffer[i] = nullptr;
    }

    this->ptrWrite = 0;
    if(!ptrWrite.is_lock_free()){// !CAS
        exit(-1);
    }
    this->ptrRead = 0;
    if(!ptrRead.is_lock_free()){// !CAS
        exit(-1);
    }
}

bool ShareBuffer::putEvent(unsigned char*  buff){
    int ptrW = this->ptrWrite;
    int ptrR = this->ptrRead;

    do{
        if((ptrW+1)%SHAREBUFFER_SIZE==ptrR){
            return false;
        }
    }
    while (!this->ptrWrite.compare_exchange_strong(ptrW,(ptrW+1)%SHAREBUFFER_SIZE));
    while(this->buffer[ptrW] != nullptr); //!!! 确保事件已经取走
    this->buffer[ptrW] = buff ;
    return true;
}



void ShareBuffer::putEventSyn(unsigned char*  buff){
    while(!putEvent(buff)){
    };
}

int asynID = 0; // TODO: 测试用，为了看是否创建了多个线程
void ShareBuffer::putEventAsyn(unsigned char*  buff){
    std::thread thd(&ShareBuffer::putEventSyn, this,buff);
    // char name[16] = {0};
    // asynID = (asynID+1)%(4096);
    // sprintf(name,"asyn-%d",asynID);
    // pthread_setname_np(thd.native_handle(),name);
    thd.detach();
}

unsigned char*  ShareBuffer::getEvent( ){
    // std::lock_guard<std::mutex> lg(mtx);
    int ptrW = this->ptrWrite;
    int ptrR = this->ptrRead;

    do{
        if(ptrR==ptrW){
            return nullptr;
        }
    }
    while (!this->ptrRead.compare_exchange_strong(ptrR,(ptrR+1)%SHAREBUFFER_SIZE));
    while (this->buffer[ptrR] == nullptr); //!!! 确保事件已经被写入
    // printf("ptrR:%d\n",ptrR);

    unsigned char* buffer = this->buffer[ptrR];
    this->buffer[ptrR] = nullptr;
    return buffer;
}