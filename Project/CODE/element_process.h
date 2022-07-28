#ifndef __ELEMENT_PROCESS_H
#define __ELEMENT_PROCESS_H


#define LEFT  1
#define RIGHT 2


struct ElementIntegral
{
    int16 Start;
    int16 Now;
    int16 Error;
};

struct RoundaboutSetIntegral
{
    int16 In;
    int16 Out1;
    int16 Out2;
};


// typedef enum
// {
//     IN = 0;
//     OUT1 = 1;
//     OUT2 = 2;
// }RoundaboutState;


extern int16 RoundaboutSwitch,ForkSwitch;
extern uint8 RoundaboutNum,ForkNum;
extern uint8 RoundaboutSize[4];
extern uint8 RoundaboutFlag,ForkFlag;
extern uint8 RoundaboutDir,ForkDir;
extern struct ElementIntegral RdIntegral;
extern struct RoundaboutSetIntegral RdSetIntegral[3];

void element_process();

#endif