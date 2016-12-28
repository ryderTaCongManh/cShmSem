#include "cshmsem.h"

cShmSem::cShmSem(bool vIsInitator)
{
    isInitator = vIsInitator;
    gSemPtr = NULL;
}
bool cShmSem::initParameters(key_t key, char *vSemName, u_int8_t lengthOfName)
{
    gKeyShm = key;
    memset(gSemName, '\0', MAXIMUM_LENGTH_NAME);
    if (lengthOfName > MAXIMUM_LENGTH_NAME)
    {
        printf("length of semaphore name is over maximum lenght \n");
        return false;
    }
    else
    {
        memcpy(gSemName, vSemName, lengthOfName);
    }
    return true;
}
bool cShmSem::initProcess()
{
    if(isInitator)
    {
        //. This is object creates sharememory and semaphore
        /*init share memory pointer*/
           int SHM_SIZE = sizeof(mlsEventUi_t);
           gShmId = shmget(gKeyShm, SHM_SIZE, IPC_CREAT | 0666);
           if (gShmId < 0)
           {
               perror("shmget");
               return false;
           }

           gShmPtr = (caddr_t )shmat(gShmId, NULL, 0);
           if (gShmPtr == (char *) -1)
           {
               perror("shmat");
               return false;
           }

           /* Open the Semaphore */
             gSemPtr = sem_open(gSemName, O_CREAT, 0644,1);
             if(gSemPtr == SEM_FAILED)
             {
                 perror("sem_open");
                 return false;
             }
             sem_trywait(gSemPtr);
             memset(gShmPtr, 0, SHM_SIZE);
    }
    else
    {
        //. This is object get then use sharememory and semaphore
        /*init share memory pointer*/
           int SHM_SIZE = sizeof(mlsEventUi_t);
           gShmId = shmget(gKeyShm, SHM_SIZE, 0666);
           if (gShmId < 0)
           {
               perror("shmget");
               return false;
           }

           gShmPtr = (caddr_t )shmat(gShmId, NULL, 0);
           if (gShmPtr == (char *) -1)
           {
               perror("shmat");
               return false;
           }

         /* Open the Semaphore */
           gSemPtr = sem_open(gSemName, 0, 0644, 0);
           if(gSemPtr == SEM_FAILED)
           {
               perror("sem_open");
               return false;
           }
           sem_trywait(gSemPtr);
           memset(gShmPtr, 0, SHM_SIZE);
    }
    return true;
}

bool cShmSem::checkSemaphoreStatus(mlsEventUi_t* eventId)
{
    sem_wait(gSemPtr);
    memcpy(eventId, gShmPtr, sizeof(mlsEventUi_t));
    return true;
}

bool cShmSem::raiseEventSem(mlsEventUi_t* eventId)
{
     sem_trywait(gSemPtr);
     memcpy(gShmPtr, eventId, sizeof(mlsEventUi_t));
     sem_post(gSemPtr);
     return true;
}


