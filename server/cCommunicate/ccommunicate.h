#ifndef CCOMMUNICATE_H
#define CCOMMUNICATE_H

#include <QThread>
#include "cCommunicate/cShmSem/cshmsem.h"

class cCommunicate : public QThread
{
    Q_OBJECT
public:
    explicit cCommunicate(QObject *parent = 0);  
    cShmSem *vShmSem;
signals:
    void sigChangeTextIndication(QString);
public slots:

private:
    void run();
};

#endif // CCOMMUNICATE_H
