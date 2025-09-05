#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Date.h"
#include "fileHelper.h"
#include "Medicine.h"

void initMedicine(Medicine* med)
{
    char str[50];
    printf("Enter medicine name: ");
    scanf("%49s", str);
    strcpy(med->medName, str);
    printf("Enter medicine brand name: ");
    scanf("%49s", str);
    strcpy(med->medBrand, str);
    getCorrectDate(&med->medExpiry_date);
    printf("Enter medicine price: ");
    scanf("%lf", &med->medPrice);
    printf("Enter medicine quantity in stock: ");
    scanf("%d", &med->medQInStock);
}


int addMedicine(Pharmacy* phar)
{
    Medicine* newMed = (Medicine*)calloc(1, sizeof(Medicine));
    if (!newMed)
        return 0;
    initMedicine(newMed);
    Medicine** tempArr = (Medicine**)realloc(phar->medArr, (phar->numOfMeds + 1) * sizeof(Medicine*));
    if (!tempArr) {
        free(newMed);
        return 0;
    }
    phar->medArr = tempArr;
    phar->medArr[phar->numOfMeds] = newMed;
    phar->numOfMeds++;
    return 1;
}

int updateStock(Medicine* med, int quantityChange)
{
    if (med->medQInStock + quantityChange < 0) {
        printf("Cannot apply change. The stock quantity cannot be negative.\n");
        return 0;
    }
    else {
        med->medQInStock += quantityChange;
        return 1;
    }
}

void printMedicine(const Medicine* med)
{
    printf("Name: %s\n", med->medName);
    printf("Brand: %s\n", med->medBrand);
    printf("Expiry Date: ");
    printDate(&(med->medExpiry_date));
    printf("Price: $%.2f\n", med->medPrice);
    printf("Stock Quantity: %d\n", med->medQInStock);
}


void saveMedicineToTextFile(const Medicine* med, const char* filename)
{
    FILE* file = fopen(filename, "a");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    fprintf(file, "%s\n", med->medName);
    fprintf(file, "%s\n", med->medBrand);
    fprintf(file, "%02d/%02d/%04d\n", med->medExpiry_date.day, med->medExpiry_date.month, med->medExpiry_date.year);
    fprintf(file, "%.2f\n", med->medPrice);
    fprintf(file, "%d\n", med->medQInStock);
    fclose(file);
}

int loadMedicineFromTextFile(Medicine* med, const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return 0;
    }
    fscanf(file, "%[^\n]%*c", med->medName);
    fscanf(file, "%[^\n]%*c", med->medBrand);
    fscanf(file, "%d/%d/%d\n", &med->medExpiry_date.day, &med->medExpiry_date.month, &med->medExpiry_date.year);
    fscanf(file, "%lf\n", &med->medPrice);
    fscanf(file, "%d\n", &med->medQInStock);
    fclose(file);
    return 1;
}

void saveMedicineToBinaryFile(const Medicine* med, const char* filename)
{
    FILE* file = fopen(filename, "ab");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }
    fwrite(med, sizeof(Medicine), 1, file);
    fclose(file);
}

int loadMedicineFromBinaryFile(Medicine* med, const char* filename)
{
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Failed to open file");
        return 0;
    }

    fread(med, sizeof(Medicine), 1, file);
    fclose(file);
    return 1;
}

int saveMedicineToFileCompress(const Medicine* med, FILE* fp)
{
    BYTE data[2];
    int lenName = (int)strlen(med->medName);
    int lenBrand = (int)strlen(med->medBrand);
    data[0] = (BYTE)(lenName << 3 | lenBrand >> 2);
    data[1] = (BYTE)((lenBrand & 0x3) << 6 | med->medExpiry_date.month);
    if (fwrite(data, sizeof(BYTE), 2, fp) != 2)
        return 0;
    if (fwrite(&med->medPrice, sizeof(double), 1, fp) != 1)
        return 0;
    if (fwrite(&med->medQInStock, sizeof(int), 1, fp) != 1)
        return 0;
    BYTE dateData[2];
    dateData[0] = (BYTE)med->medExpiry_date.day;
    dateData[1] = (BYTE)(med->medExpiry_date.year - 2020);
    if (fwrite(dateData, sizeof(BYTE), 2, fp) != 2)
        return 0;
    if (fwrite(med->medName, sizeof(char), lenName, fp) != lenName)
        return 0;
    if (fwrite(med->medBrand, sizeof(char), lenBrand, fp) != lenBrand)
        return 0;

    return 1;
}


int loadMedicineFromFileCompress(Medicine* med, FILE* fp)
{
    BYTE data[2];
    if (fread(data, sizeof(BYTE), 2, fp) != 2)
        return 0;
    int lenName = (data[0] >> 3) & 0x1F;
    int lenBrand = ((data[0] & 0x7) << 2) | (data[1] >> 6);
    med->medExpiry_date.month = data[1] & 0x3F;
    if (fread(&med->medPrice, sizeof(double), 1, fp) != 1)
        return 0;
    if (fread(&med->medQInStock, sizeof(int), 1, fp) != 1)
        return 0;
    BYTE dateData[2];
    if (fread(dateData, sizeof(BYTE), 2, fp) != 2)
        return 0;
    med->medExpiry_date.day = dateData[0];
    med->medExpiry_date.year = dateData[1] + 2020;
    if (fread(med->medName, sizeof(char), lenName, fp) != lenName)
        return 0;
    med->medName[lenName] = '\0';
    if (fread(med->medBrand, sizeof(char), lenBrand, fp) != lenBrand)
        return 0;
    med->medBrand[lenBrand] = '\0';

    return 1;
}


int compareMedicineByName(const void* med1, const void* med2) {
    const Medicine* pMed1 = *(const Medicine**)med1;
    const Medicine* pMed2 = *(const Medicine**)med2;
    return strcmp(pMed1->medName, pMed2->medName);
}

int compareMedicineByBrand(const void* med1, const void* med2) {
    const Medicine* pMed1 = *(const Medicine**)med1;
    const Medicine* pMed2 = *(const Medicine**)med2;
    return strcmp(pMed1->medBrand, pMed2->medBrand);
}

int compareMedicineByPrice(const void* med1, const void* med2) {
    const Medicine* pMed1 = *(const Medicine**)med1;
    const Medicine* pMed2 = *(const Medicine**)med2;
    if (pMed1->medPrice < pMed2->medPrice) return -1;
    if (pMed1->medPrice > pMed2->medPrice) return 1;
    return 0;
}

int compareMedicineByStockQuantity(const void* med1, const void* med2) {
    const Medicine* pMed1 = *(const Medicine**)med1;
    const Medicine* pMed2 = *(const Medicine**)med2;
    return pMed1->medQInStock - pMed2->medQInStock;
}

void freeMedicine(Medicine* pMed) {
    if (!pMed) {
        return;
    }
    free(pMed);
}
