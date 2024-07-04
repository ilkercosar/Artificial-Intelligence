#ifndef __SVR__
#define __SVR__

#ifdef __cplusplus
extern "C"
{
#endif 

#define EPSILON 0.1
#define C 1.0

typedef struct 
{
    double *data;
    unsigned int size;
} svr;

double linearKernel();
void trainSVR();
double predictSVR();
void readDataFromCSV();
svr* createArray();

#ifdef __cplusplus
}
#endif
#endif