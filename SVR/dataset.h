#ifndef __DATASET__
#define __DATASET__

#ifdef __cplusplus
extern "C"
{
#endif 

#define MAX_ELEMENTS 100    //VERİSETİ BÜYÜTÜLMEK İSTENİLİRSE BURADA 100 DEĞERİNİ 1000VS YAPILABİLİR.
#define MAX_LINE_LENGTH 256 //OPTİMİZASYON YAPILACAKSA BU AYARLANIR
#define LABEL 2             //OKUNACAK VERİLERİN HANGİLERİ OLDUĞU BURADA AYARLANIR. 3 YAPILIRSA İLK 3 ETİKETE BAKAR

typedef struct 
{
    char* header[LABEL];
    char* data1[MAX_ELEMENTS];
    char* data2[MAX_ELEMENTS];
    int count1;
    int count2;
} CSVData;

int read_csv(const char* filename, CSVData* csvData);
void free_csv_data(CSVData* csvData);

#ifdef __cplusplus
}
#endif
#endif

