#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dataset.h"
#include "svr.h"

int main(int argc, char** argv) 
{
    const char* filename = "/home/hedef-ubuntu/cpp/SVR/dataset/Battery_RUL.csv";
    svr *data1, *data2;

    readDataFromCSV(filename, &data1, &data2);

    double alpha[MAX_ELEMENTS];
    trainSVR(data1, data2, alpha);

    for (int i = 0; i < data1->size; i++) 
    {
        double prediction = predictSVR(data1, alpha, data1->data[i]);
        double error = (prediction - data2->data[i]) * 100 / data2->data[i];  
        printf("Gerçek: %f, Tahmin: %f, Hata(Yüzde): %f\n", data2->data[i], prediction, error);
    }

    free(data1->data);
    free(data1);
    free(data2->data);
    free(data2);

    return 0;
}
