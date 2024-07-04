#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "svr.h"
#include "dataset.h"

svr* createArray(int size) 
{
    svr *array = (svr*) malloc(sizeof(svr));
    array->data = (double*) malloc(size * sizeof(double));
    array->size = size;
    return array;
}

void readDataFromCSV(const char* filename, svr **data1, svr **data2) 
{
    CSVData csvData;
    if (read_csv(filename, &csvData) != 0)  exit(EXIT_FAILURE);
    
    *data1 = createArray(csvData.count1);
    *data2 = createArray(csvData.count2);

    for (int i = 0; i < csvData.count1; i++) 
    {
        (*data1)->data[i] = atof(csvData.data1[i]);
    }

    for (int i = 0; i < csvData.count2; i++) 
    {
        (*data2)->data[i] = atof(csvData.data2[i]);
    }

    free_csv_data(&csvData);
}

double linearKernel(double x1, double x2)
{
    return x1 * x2;
}

void trainSVR(svr *X, svr *y, double *alpha) 
{
    int n = X->size;
    double K[MAX_ELEMENTS][MAX_ELEMENTS];

    for (int i = 0; i < n; i++) 
    {
        for (int j = 0; j < n; j++) 
        {
            K[i][j] = linearKernel(X->data[i], X->data[j]);
        }
    }

    for (int i = 0; i < n; i++) 
    {
        alpha[i] = 0;
    }

    for (int iter = 0; iter < 1000; iter++) 
    {
        for (int i = 0; i < n; i++) 
        {
            double error = y->data[i] - alpha[i];
            if (fabs(error) > EPSILON)  alpha[i] += C * error;
            
        }
    }
}

double predictSVR(svr *X, double *alpha, double x) 
{
    double result = 0;
    for (int i = 0; i < X->size; i++) 
    {
        result += alpha[i] * linearKernel(X->data[i], x);
    }
    return result;
}