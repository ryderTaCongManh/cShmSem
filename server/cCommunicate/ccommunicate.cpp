#include "ccommunicate.h"

cCommunicate::cCommunicate(QObject *parent) :
    QThread(parent)
{
    char semName[] = "sem_ui_screen";
    vShmSem = new cShmSem(true);
    if (!(vShmSem->initParameters((key_t)1234567, semName, sizeof(semName))))
    {
        qCritical("fail to init parameters for ShmSem initialize ...");
    }
    else
    {
        if (vShmSem->initProcess())
        {
            qDebug("init shm & sem successfully ");
        }
        else
        {
            qCritical("fail to init shm & sem ");
        }
    }
}

void cCommunicate::run()
{
    mlsEventUi_t eventGet;
    while(true)
    {
          qDebug("waiting...");
        if (vShmSem->checkSemaphoreStatus(&eventGet))
        {
              qDebug("got semaphore ");
            //. parse data then emit signal to upper layer
            if(eventGet.length > 0)
            {
                switch (eventGet.data[0])
                {
                    case 2:
                        qDebug("recieve request 2 ");
                        emit sigChangeTextIndication(QString::fromUtf8("アップデート準備中..."));
                        break;
                    case 3:
                        qDebug("recieve request 3 ");
                        emit sigChangeTextIndication(QString::fromUtf8("FWアップデート中..."));
                        break;
                    case 4:
                        qDebug("recieve request 4 ");
                        emit sigChangeTextIndication(QString::fromUtf8("端末を再起動します\nしばらくお待ちください"));
                        break;
                    default:
                        break;
                }
            }
        }
    }
}
