#ifndef CSHMSEM_H
#define CSHMSEM_H
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <semaphore.h>
#include <errno.h>
#include <time.h>

#define MAXIMUM_LENGTH_NAME   30
#define MAXIMUM_LENGTH_EVENT  8
#pragma pack(1)
/*define data structure which using for share memory*/
typedef struct
{
    unsigned char length;
    unsigned char data[MAXIMUM_LENGTH_EVENT];
}mlsEventUi_t;
#pragma pack()

class cShmSem
{
public:
    cShmSem(bool vIsInitator);
    bool initParameters(key_t key, char *vSemName, u_int8_t lengthOfName);
    bool initProcess();
    bool checkSemaphoreStatus(mlsEventUi_t* eventId);
    bool raiseEventSem(mlsEventUi_t* eventId);
private:
     bool isInitator;
     /*Init parameters for share memory*/
     key_t gKeyShm;
     int gShmId;
     caddr_t gShmPtr;
     /*Init parameters for semaphore*/
     char gSemName[MAXIMUM_LENGTH_NAME];
     sem_t *gSemPtr;
};

#endif // CSHMSEM_H
