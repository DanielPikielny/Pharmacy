#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Date.h"
#include "fileHelper.h"
#include "Medicine.h"
#include "Pharmacy.h"
#include "Supplies.h"

void initSupplies(Supplies* supply) {
    printf("Enter supply name: ");
    scanf("%s", supply->supName);
    printf("Enter supply price: ");
    scanf("%d", &supply->supPrice);
    printf("Enter quantity in stock: ");
    scanf("%d", &supply->supQInStock);
    printf("Enter supply type (0: Sanitizer, 1: Bandage, 2: Vitamins, 3: Mask, 4: Ointment): ");
    int typeInput;
    scanf("%d", &typeInput);
    supply->type = (eSupply)typeInput;
}

int addSupplies(Pharmacy* pharmacy) {
    Supplies* tempArr = (Supplies*)realloc(pharmacy->supplyArr, (pharmacy->numOfSupplies + 1) * sizeof(Supplies));
    if (!tempArr) {
        return 0;
    }
    pharmacy->supplyArr = tempArr;
    initSupplies(&pharmacy->supplyArr[pharmacy->numOfSupplies]);
    pharmacy->numOfSupplies++;
    return 1;
}


void saveSuppliesToTextFile(const Supplies* supply, const char* filename)
{
    FILE* file = fopen(filename, "a");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }
    fprintf(file, "%s\n", supply->supName);
    fprintf(file, "%d\n", supply->supPrice);
    fprintf(file, "%d\n", supply->supQInStock);
    fprintf(file, "%d\n", supply->type);
    fclose(file);
}

int loadSuppliesFromTextFile(Supplies* supply, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return 0;
    }
    fscanf(file, "%[^\n]%*c", supply->supName);
    fscanf(file, "%d\n", &supply->supPrice);
    fscanf(file, "%d\n", &supply->supQInStock);
    fscanf(file, "%d\n", (int*)&supply->type);
    fclose(file);
    return 1;
}

void saveSuppliesToBinaryFile(const Supplies* supply, const char* filename) {
    FILE* file = fopen(filename, "ab");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }
    fwrite(supply, sizeof(Supplies), 1, file);
    fclose(file);
}

int loadSuppliesFromBinaryFile(Supplies* supply, const char* filename)
{
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Failed to open file");
        return 0;
    }

    fread(supply, sizeof(Supplies), 1, file);
    fclose(file);
    return 1;
}

int updateSupplyStock(Supplies* supply, int change) {
    if (change < 0) {
        if (supply->supQInStock + change < 0) {
            printf("Can't sell more than what's in stock!\n");
            return 0;
        }
    }
    supply->supQInStock += change;
    printf("Updated stock quantity for %s: %d\n", supply->supName, supply->supQInStock);
    return 1;
}

void freeSupply(Supplies* supply) {
    supply = NULL;
}

void printSupply(const Supplies* supply)
{
    const char* supplyTypeStr[] = { "Sanitizer", "Bandage", "Vitamins", "Mask", "Ointment" };
    printf("Supply Name: %s\n", supply->supName);
    printf("Price: $%d\n", supply->supPrice);
    printf("Quantity in Stock: %d\n", supply->supQInStock);
    printf("Type: %s\n", supplyTypeStr[supply->type]);
}
