
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataset.h"

int read_csv(const char* filename, CSVData* csvData) 
{
    FILE* file = fopen(filename, "r");
    if (file == NULL) 
    {
        perror("Dosya açılmadı");
        return EXIT_FAILURE;
    }

    char line[MAX_LINE_LENGTH];
    char* token;

    if (fgets(line, sizeof(line), file) != NULL) 
    {
        token = strtok(line, ",");
        for (int i = 0; i < 2 && token != NULL; i++) 
        {
            csvData->header[i] = strdup(token);
            token = strtok(NULL, ",");
        }
    } 
    else 
    {
        perror("Etiket satırı okunamadı");
        fclose(file);
        return EXIT_FAILURE;
    }

    csvData->count1 = 0;
    csvData->count2 = 0;
    while (fgets(line, sizeof(line), file) && (csvData->count1 < MAX_ELEMENTS || csvData->count2 < MAX_ELEMENTS)) 
    {
        token = strtok(line, ",");
        if (csvData->count1 < MAX_ELEMENTS && token != NULL) 
        {
            csvData->data1[csvData->count1++] = strdup(token);
            token = strtok(NULL, ",");
        }
        if (csvData->count2 < MAX_ELEMENTS && token != NULL) csvData->data2[csvData->count2++] = strdup(token);
        
    }

    fclose(file);
    return EXIT_SUCCESS;
}

void free_csv_data(CSVData* csvData) 
{
    for (int i = 0; i < csvData->count1; i++) 
    {
        free(csvData->data1[i]);
    }

    for (int i = 0; i < csvData->count2; i++) 
    {
        free(csvData->data2[i]);
    }

    for (int i = 0; i < 2; i++) 
    {
        free(csvData->header[i]);
    }
}
