#ifndef __MEDICINE__
#define __MEDICINE__

#include "Pharmacy.h"
#include "Date.h"


typedef struct {
    char medName[50];
    char medBrand[50];
    Date medExpiry_date;
    double medPrice;
    int medQInStock;
} Medicine;


void initMedicine(Medicine* med);
int addMedicine(Pharmacy* phar);
int updateStock(Medicine* med, int quantityChange);
void printMedicine(const Medicine* med);
void saveMedicineToTextFile(const Medicine* med, const char* filename);
int loadMedicineFromTextFile(Medicine* med, const char* filename);
void saveMedicineToBinaryFile(const Medicine* med, const char* filename);
int loadMedicineFromBinaryFile(Medicine* med, const char* filename);
int saveMedicineToFileCompress(const Medicine* med, FILE* fp);
int loadMedicineFromFileCompress(Medicine* med, FILE* fp);
int compareMedicineByStockQuantity(const void* med1, const void* med2);
int compareMedicineByPrice(const void* med1, const void* med2);
int compareMedicineByBrand(const void* med1, const void* med2);
int compareMedicineByName(const void* med1, const void* med2);
void freeMedicine(Medicine* pMed);


#endif