#include <iostream>
#include "cShmSem/cshmsem.h"

using namespace std;
static int powFunc(int n)
{
    int retVal = 1;
    for(int idx=0; idx<n ; idx++)
    {
        retVal = retVal*10;
    }
    return retVal;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "please check again input parameters "<<endl;
        return 1;
    }
    unsigned char idxScreen, idx, numOfChar, temp;
    numOfChar = 0;
    idxScreen = 0;
    temp =0;
    while(argv[1][numOfChar] != '\0') numOfChar++;
    while(numOfChar > 0)
    {
        if((argv[1][numOfChar-1] < 48)||
           (argv[1][numOfChar-1] > 57))
        {
            cout << "please check again input parameters "<<argv[1][1]<<endl;
            return 2;
        }
        idxScreen = idxScreen + (argv[1][numOfChar-1]- 48)*powFunc(temp);
        temp++;
        numOfChar--;
    }
    cout << "[MarlinUiCtrl] request screen index " << idxScreen + 0 <<endl;

    /*init share memory and semaphore*/
    cShmSem *cShmSemVar = new cShmSem(false);
    char semName[] = "sem_ui_screen";
    cShmSemVar->initParameters((key_t)1234567, semName, sizeof(semName));
    if (cShmSemVar->initProcess())
    {
        mlsEventUi_t eventStruct;
        eventStruct.length  = 1;
        eventStruct.data[0]    = (unsigned char)idxScreen;
        cShmSemVar->raiseEventSem(&eventStruct);
    }

    delete cShmSemVar;
    return 0;
}

