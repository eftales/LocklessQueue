//
// Created by eftales on 2022/6/6.
//

#ifndef SDN_SOFT_SHAREBUFFER_H
#define SDN_SOFT_SHAREBUFFER_H


#include <atomic>
#include <memory>

#define SHAREBUFFER_SIZE    4096



class ShareBuffer {
public:
    ShareBuffer();

    unsigned char*  getEvent();
    void putEventSyn(unsigned char*  buff);
    void putEventAsyn(unsigned char*  buff);

private:
    unsigned char*  buffer[SHAREBUFFER_SIZE];
    std::atomic<int> ptrRead ;
    std::atomic<int> ptrWrite ;
    std::mutex mtx;

    bool putEvent(unsigned char*  buff);
};

extern ShareBuffer L3EventBuffers;
extern ShareBuffer L2EventBuffers;
extern ShareBuffer sockSendBuffers;

#endif //SDN_SOFT_SHAREBUFFER_H
