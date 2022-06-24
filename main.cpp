#include <iostream>
#include <thread>
#include "ShareBuffer.h"
#include <climits>
#include "stdlib.h"

#define THREADS 2
#define THREADS_C 3
#define JOBS    INT_MAX
#define ASYN_probability  0


bool producter(unsigned char id){
    for(int i=0;i<=JOBS;++i){
        //printf("producter %d is running..\n",id);

        std::unique_ptr<unsigned char> buffer(new unsigned char[4096] ) ;


        buffer.get()[0] = id;
        L3EventBuffers.putEventSyn(buffer.release());
    }

}

int count[THREADS+1] = {0};

int loop = 0;
bool consumer(unsigned char id){
    int i=0;
    while (1){
        std::unique_ptr<unsigned char> buffer(L3EventBuffers.getEvent()) ;


        if(buffer.get()!= nullptr){ //
            i += 1;
            if(i%SHAREBUFFER_SIZE==0){
                printf("%d\n",loop++);
            }
        //    printf("consumer %d get event\n",id);
            unsigned char *b = buffer.get();
            count[b[0]]++;
//            for(int i=0;i<THREADS+1;++i){
//                printf("%d ",count[i]);
//            }
//            printf("\n");
            if(rand()%100<ASYN_probability){

                L3EventBuffers.putEventAsyn(buffer.release());
            }


        }


    }
}


std::atomic<int>  ai;

int  tst_val= 4;
int  new_val= 5;
bool exchanged= false;

void valsout()
{
    std::cout << "ai= " << ai
          << "  tst_val= " << tst_val
          << "  new_val= " << new_val
          << "  exchanged= " << std::boolalpha << exchanged
          << "\n";
}

int main() {

        ai= 3;
    valsout();

    // tst_val != ai   ==>  tst_val is modified
    exchanged= ai.compare_exchange_strong( tst_val, new_val );
    valsout();

    // tst_val == ai   ==>  ai is modified
    exchanged= ai.compare_exchange_strong( tst_val, new_val );
    valsout();


    setbuf(stdout, 0); // 加上这个之后可以在调试中实时看到 printf 输出
    std::thread thds[THREADS];
    for(int i = 0 ;i<THREADS;i++){
        unsigned char id = i;
        thds[i] = std::thread(producter,id);
    }

    std::thread thdsC[THREADS_C];
    for(int i = 0 ;i<THREADS_C;i++){
        thdsC[i] = std::thread(consumer,i);
    }
   thdsC[0].join();
}

