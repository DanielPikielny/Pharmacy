#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "fileHelper.h"
#include "Medicine.h"
#include "Pharmacy.h"
#include "Perscription.h"
#include "Customer.h"
#include "Supplies.h"
#include "CustomerManagement.h"
#include "Purchase.h"


void printPurchase(const Purchase* purchase) {
    printf("Purchase ID: %d\n", purchase->purchaseID);
    printf("Customer ID: %09d, Customer Name: %s\n", purchase->customerID, purchase->customerName);
    printf("Pharmacist ID: %d, Pharmacist Name: %s\n", purchase->pharmacistID, purchase->pharmacistName);
    printf("Number of Items: %d\n", purchase->numOfItems);
    printf("Purchased Items:\n");
    for (int i = 0; i < purchase->numOfItems; i++) {
        printf("  %d. %s\n", i + 1, purchase->purchasedItems[i]);
    }
    printf("Total Cost: %.2f\n", purchase->totalCost);
}

void savePurchaseToTextFile(const char* filename, const Purchase* purchase) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file for writing");
        return;
    }
    fprintf(file, "Purchase ID: %d\n", purchase->purchaseID);
    fprintf(file, "Customer ID: %09d\n", purchase->customerID);
    fprintf(file, "Customer Name: %s\n", purchase->customerName);
    fprintf(file, "Pharmacist ID: %d\n", purchase->pharmacistID);
    fprintf(file, "Pharmacist Name: %s\n", purchase->pharmacistName);
    fprintf(file, "Number of Items: %d\n", purchase->numOfItems);
    for (int i = 0; i < purchase->numOfItems; i++) {
        fprintf(file, "Item %d: %s\n", i + 1, purchase->purchasedItems[i]);
    }
    fprintf(file, "Total Cost: %.2f\n", purchase->totalCost);
    fclose(file);
}

void loadPurchaseFromTextFile(const char* filename, Purchase* purchase) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file for reading");
        return;
    }
    fscanf(file, "Purchase ID: %d\n", &purchase->purchaseID);
    fscanf(file, "Customer ID: %d\n", &purchase->customerID);
    fscanf(file, "Customer Name: %s\n", purchase->customerName);
    fscanf(file, "Pharmacist ID: %d\n", &purchase->pharmacistID);
    fscanf(file, "Pharmacist Name: %s\n", purchase->pharmacistName);
    fscanf(file, "Number of Items: %d\n", &purchase->numOfItems);
    purchase->purchasedItems = malloc(purchase->numOfItems * sizeof(char*));
    for (int i = 0; i < purchase->numOfItems; i++) {
        purchase->purchasedItems[i] = malloc(100 * sizeof(char));
        fscanf(file, "Item %*d: %s\n", purchase->purchasedItems[i]);
    }
    fscanf(file, "Total Cost: %lf\n", &purchase->totalCost);
    fclose(file);
}

void savePurchaseToBinaryFile(const char* filename, const Purchase* purchase) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        perror("Failed to open file for writing");
        return;
    }
    fwrite(&purchase->purchaseID, sizeof(purchase->purchaseID), 1, file);
    fwrite(&purchase->customerID, sizeof(purchase->customerID), 1, file);
    fwrite(purchase->customerName, sizeof(purchase->customerName), 1, file);
    fwrite(&purchase->pharmacistID, sizeof(purchase->pharmacistID), 1, file);
    fwrite(purchase->pharmacistName, sizeof(purchase->pharmacistName), 1, file);
    fwrite(&purchase->numOfItems, sizeof(purchase->numOfItems), 1, file);
    for (int i = 0; i < purchase->numOfItems; i++) {
        size_t length = strlen(purchase->purchasedItems[i]) + 1;
        fwrite(&length, sizeof(length), 1, file);
        fwrite(purchase->purchasedItems[i], length, 1, file);
    }
    fwrite(&purchase->totalCost, sizeof(purchase->totalCost), 1, file);
    fclose(file);
}

void loadPurchaseFromBinaryFile(const char* filename, Purchase* purchase) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open file for reading");
        return;
    }
    fread(&purchase->purchaseID, sizeof(purchase->purchaseID), 1, file);
    fread(&purchase->customerID, sizeof(purchase->customerID), 1, file);
    fread(purchase->customerName, sizeof(purchase->customerName), 1, file);
    fread(&purchase->pharmacistID, sizeof(purchase->pharmacistID), 1, file);
    fread(purchase->pharmacistName, sizeof(purchase->pharmacistName), 1, file);
    fread(&purchase->numOfItems, sizeof(purchase->numOfItems), 1, file);
    purchase->purchasedItems = malloc(purchase->numOfItems * sizeof(char*));
    for (int i = 0; i < purchase->numOfItems; i++) {
        size_t length;
        fread(&length, sizeof(length), 1, file);
        purchase->purchasedItems[i] = malloc(length);
        fread(purchase->purchasedItems[i], length, 1, file);
    }
    fread(&purchase->totalCost, sizeof(purchase->totalCost), 1, file);
    fclose(file);
}

void freePurchase(Purchase* purchase) {
    if (purchase->purchasedItems != NULL) {
        for (int i = 0; i < purchase->numOfItems; i++) {
            free(purchase->purchasedItems[i]);
        }
        free(purchase->purchasedItems);
    }
}
