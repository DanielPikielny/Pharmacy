#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Date.h"
#include "fileHelper.h"
#include "Medicine.h"
#include "Pharmacy.h"
#include "Perscription.h"
#include "Customer.h"
#include "Supplies.h"
#include "CustomerManagement.h"
#include "Purchase.h"


void initCustomerManagement(CustomerManagement* cm) {
    cm->customerArr = NULL;
    cm->purchaseArr = NULL;
    cm->numOfCustomers = 0;
    cm->numOfPurchases = 0;
    cm->nextPrescriptionID = 1;
    cm->nextPurchaseID = 1;
}


void saveCustomerManagementToTextFile(const CustomerManagement* cm, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error: Could not open file %s for writing.\n", filename);
        return;
    }
    fprintf(file, "%d\n", cm->numOfCustomers);
    for (int i = 0; i < cm->numOfCustomers; i++) {
        fprintf(file, "%d %s %s\n", cm->customerArr[i].custID, cm->customerArr[i].custName, cm->customerArr[i].custPhone);
        savePrescriptionsToTextFile(file, &cm->customerArr[i]);
    }
    fprintf(file, "%d\n", cm->numOfPurchases);
    for (int i = 0; i < cm->numOfPurchases; i++) {
        fprintf(file, "%d %d %s %d %.2f\n", cm->purchaseArr[i].purchaseID, cm->purchaseArr[i].customerID,
            cm->purchaseArr[i].customerName, cm->purchaseArr[i].numOfItems, cm->purchaseArr[i].totalCost);
        for (int j = 0; j < cm->purchaseArr[i].numOfItems; j++) {
            fprintf(file, "%s\n", cm->purchaseArr[i].purchasedItems[j]);
        }
    }
    fclose(file);
}

int loadCustomerManagementFromTextFile(CustomerManagement* cm, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s for reading.\n", filename);
        return 0;
    }
    fscanf(file, "%d", &cm->numOfCustomers);
    cm->customerArr = (Customer*)calloc(cm->numOfCustomers, sizeof(Customer));
    for (int i = 0; i < cm->numOfCustomers; i++) {
        fscanf(file, "%d %s %s", &cm->customerArr[i].custID, cm->customerArr[i].custName, cm->customerArr[i].custPhone);
        loadPrescriptionsFromTextFile(file, &cm->customerArr[i]);
    }
    fscanf(file, "%d", &cm->numOfPurchases);
    cm->purchaseArr = (Purchase*)calloc(cm->numOfPurchases, sizeof(Purchase));
    for (int i = 0; i < cm->numOfPurchases; i++) {
        fscanf(file, "%d %d %s %d %lf", &cm->purchaseArr[i].purchaseID, &cm->purchaseArr[i].customerID,
            cm->purchaseArr[i].customerName, &cm->purchaseArr[i].numOfItems, &cm->purchaseArr[i].totalCost);
        cm->purchaseArr[i].purchasedItems = (char**)calloc(cm->purchaseArr[i].numOfItems, sizeof(char*));
        for (int j = 0; j < cm->purchaseArr[i].numOfItems; j++) {
            cm->purchaseArr[i].purchasedItems[j] = (char*)calloc(50, sizeof(char));
            fscanf(file, "%s", cm->purchaseArr[i].purchasedItems[j]);
        }
    }
    fclose(file);
    return 1;
}

void saveCustomerManagementToBinaryFile(const CustomerManagement* cm, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        printf("Error: Could not open file %s for writing.\n", filename);
        return;
    }
    fwrite(&cm->numOfCustomers, sizeof(int), 1, file);
    for (int i = 0; i < cm->numOfCustomers; i++) {
        fwrite(&cm->customerArr[i].custID, sizeof(int), 1, file);
        fwrite(cm->customerArr[i].custName, sizeof(char), 50, file);
        fwrite(cm->customerArr[i].custPhone, sizeof(char), 15, file);
        savePrescriptionsToBinaryFile(file, &cm->customerArr[i]);
    }
    fwrite(&cm->numOfPurchases, sizeof(int), 1, file);
    for (int i = 0; i < cm->numOfPurchases; i++) {
        fwrite(&cm->purchaseArr[i].purchaseID, sizeof(int), 1, file);
        fwrite(&cm->purchaseArr[i].customerID, sizeof(int), 1, file);
        fwrite(cm->purchaseArr[i].customerName, sizeof(char), 50, file);
        fwrite(&cm->purchaseArr[i].numOfItems, sizeof(int), 1, file);
        fwrite(&cm->purchaseArr[i].totalCost, sizeof(double), 1, file);
        for (int j = 0; j < cm->purchaseArr[i].numOfItems; j++) {
            fwrite(cm->purchaseArr[i].purchasedItems[j], sizeof(char), 50, file);
        }
    }
    fclose(file);
}

int loadCustomerManagementFromBinaryFile(CustomerManagement* cm, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Error: Could not open file %s for reading.\n", filename);
        return 0;
    }
    fread(&cm->numOfCustomers, sizeof(int), 1, file);
    cm->customerArr = (Customer*)calloc(cm->numOfCustomers, sizeof(Customer));
    for (int i = 0; i < cm->numOfCustomers; i++) {
        fread(&cm->customerArr[i].custID, sizeof(int), 1, file);
        fread(cm->customerArr[i].custName, sizeof(char), 50, file);
        fread(cm->customerArr[i].custPhone, sizeof(char), 15, file);
        loadPrescriptionsFromBinaryFile(file, &cm->customerArr[i]);
    }
    fread(&cm->numOfPurchases, sizeof(int), 1, file);
    cm->purchaseArr = (Purchase*)calloc(cm->numOfPurchases, sizeof(Purchase));
    for (int i = 0; i < cm->numOfPurchases; i++) {
        fread(&cm->purchaseArr[i].purchaseID, sizeof(int), 1, file);
        fread(&cm->purchaseArr[i].customerID, sizeof(int), 1, file);
        fread(cm->purchaseArr[i].customerName, sizeof(char), 50, file);
        fread(&cm->purchaseArr[i].numOfItems, sizeof(int), 1, file);
        fread(&cm->purchaseArr[i].totalCost, sizeof(double), 1, file);
        cm->purchaseArr[i].purchasedItems = (char**)calloc(cm->purchaseArr[i].numOfItems, sizeof(char*));
        for (int j = 0; j < cm->purchaseArr[i].numOfItems; j++) {
            cm->purchaseArr[i].purchasedItems[j] = (char*)calloc(50, sizeof(char));
            fread(cm->purchaseArr[i].purchasedItems[j], sizeof(char), 50, file);
        }
    }
    fclose(file);
    return 1;
}

int deletePrescription(Customer* customer, int prescriptionID) {
    if (!customer || !customer->prescriptions) {
        printf("No prescriptions available for this customer.\n");
        return 0;
    }
    Prescription* current = customer->prescriptions;
    Prescription* previous = NULL;
    while (current != NULL) {
        if (current->prescriptionID == prescriptionID) {
            if (previous == NULL) {
                customer->prescriptions = current->next;
            }
            else {
                previous->next = current->next;
            }
            free(current);
            printf("Prescription ID %d deleted successfully.\n", prescriptionID);
            return 1;
        }
        previous = current;
        current = current->next;
    }
    printf("Prescription ID %d not found.\n", prescriptionID);
    return 0;
}

void purchase(CustomerManagement* cm, Pharmacy* pharmacy) {
    if (cm->numOfCustomers == 0) {
        printf("No customers available for purchase.\n");
        return;
    }
    if (pharmacy->numOfPharmacists == 0) {
        printf("No pharmacists available to process the purchase.\n");
        return;
    }
    printf("Select a customer by entering their ID:\n");
    for (int i = 0; i < cm->numOfCustomers; i++) {
        printf("Customer Name: %s, ID: %09d\n", cm->customerArr[i].custName, cm->customerArr[i].custID);
    }
    int selectedID;
    printf("Enter customer ID: ");
    scanf("%d", &selectedID);
    Customer* selectedCustomer = NULL;
    for (int i = 0; i < cm->numOfCustomers; i++) {
        if (cm->customerArr[i].custID == selectedID) {
            selectedCustomer = &cm->customerArr[i];
            break;
        }
    }
    if (!selectedCustomer) {
        printf("Customer ID not found.\n");
        return;
    }
    printf("Select a pharmacist by entering their ID:\n");
    for (int i = 0; i < pharmacy->numOfPharmacists; i++) {
        printf("Pharmacist Name: %s, ID: %d\n", pharmacy->pharmacistArr[i].phmtName, pharmacy->pharmacistArr[i].id);
    }
    int selectedPharmacistID;
    printf("Enter pharmacist ID: ");
    scanf("%d", &selectedPharmacistID);
    Pharmacist* selectedPharmacist = NULL;
    for (int i = 0; i < pharmacy->numOfPharmacists; i++) {
        if (pharmacy->pharmacistArr[i].id == selectedPharmacistID) {
            selectedPharmacist = &pharmacy->pharmacistArr[i];
            break;
        }
    }
    if (!selectedPharmacist) {
        printf("Pharmacist ID not found.\n");
        return;
    }
    Purchase purchase;
    purchase.purchaseID = cm->nextPurchaseID++;
    purchase.customerID = selectedCustomer->custID;
    strcpy(purchase.customerName, selectedCustomer->custName);
    purchase.purchasedItems = NULL;
    purchase.numOfItems = 0;
    purchase.totalCost = 0.0;
    purchase.pharmacistID = selectedPharmacist->id;
    strcpy(purchase.pharmacistName, selectedPharmacist->phmtName);
    int continueBuying = 1;
    while (continueBuying) {
        printf("\nAvailable options:\n");
        printf("1) Purchase from supplies\n");
        printf("2) Purchase from prescriptions\n");
        printf("3) Finish purchase\n");
        int choice;
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
        case 1: {
            if (pharmacy->numOfSupplies == 0) {
                printf("No supplies available for purchase.\n");
                break;
            }
            printf("Available supplies:\n");
            for (int i = 0; i < pharmacy->numOfSupplies; i++) {
                printf("%d. %s - Price: %d, Quantity in Stock: %d\n", i + 1,
                    pharmacy->supplyArr[i].supName, pharmacy->supplyArr[i].supPrice,
                    pharmacy->supplyArr[i].supQInStock);
            }
            int supplyIndex, quantity;
            printf("Enter the supply number to purchase: ");
            scanf("%d", &supplyIndex);
            printf("Enter quantity to purchase: ");
            scanf("%d", &quantity);
            if (supplyIndex < 1 || supplyIndex > pharmacy->numOfSupplies) {
                printf("Invalid supply selection.\n");
            }
            else if (updateSupplyStock(&pharmacy->supplyArr[supplyIndex - 1], -quantity)) {
                purchase.numOfItems++;
                purchase.purchasedItems = realloc(purchase.purchasedItems, purchase.numOfItems * sizeof(char*));
                purchase.purchasedItems[purchase.numOfItems - 1] = strdup(pharmacy->supplyArr[supplyIndex - 1].supName);
                purchase.totalCost += pharmacy->supplyArr[supplyIndex - 1].supPrice * quantity;
                printf("Added to total cost: %d\n", pharmacy->supplyArr[supplyIndex - 1].supPrice * quantity);
            }
            break;
        }
        case 2: {
            if (!selectedCustomer->prescriptions) {
                printf("No prescriptions available for this customer.\n");
                break;
            }
            printf("Available prescriptions:\n");
            Prescription* current = selectedCustomer->prescriptions;
            while (current) {
                printf("Prescription ID: %d, Medicine: %s\n", current->prescriptionID, current->medicineName);
                current = current->next;
            }
            int prescriptionID;
            printf("Enter the prescription ID to purchase: ");
            scanf("%d", &prescriptionID);
            Prescription* selectedPrescription = NULL;
            current = selectedCustomer->prescriptions;
            while (current) {
                if (current->prescriptionID == prescriptionID) {
                    selectedPrescription = current;
                    break;
                }
                current = current->next;
            }
            if (!selectedPrescription) {
                printf("No prescription with this ID.\n");
            }
            else {
                Medicine* med = NULL;
                for (int i = 0; i < pharmacy->numOfMeds; i++) {
                    if (strcmp(pharmacy->medArr[i].medName, selectedPrescription->medicineName) == 0) {
                        med = &pharmacy->medArr[i];
                        break;
                    }
                }
                if (!med) {
                    printf("Medicine not found in the pharmacy.\n");
                }
                else if (updateStock(med, -1)) {
                    purchase.numOfItems++;
                    purchase.purchasedItems = realloc(purchase.purchasedItems, purchase.numOfItems * sizeof(char*));
                    purchase.purchasedItems[purchase.numOfItems - 1] = strdup(selectedPrescription->medicineName);
                    purchase.totalCost += med->medPrice;
                    printf("Added to total cost: %.2f\n", med->medPrice);
                    deletePrescription(selectedCustomer, prescriptionID);
                }
            }
            break;
        }
        case 3:
            continueBuying = 0;
            printf("Purchase completed. Total cost: %.2f\n", purchase.totalCost);
            if (purchase.numOfItems > 0) {
                cm->purchaseArr = realloc(cm->purchaseArr, (cm->numOfPurchases + 1) * sizeof(Purchase));
                cm->purchaseArr[cm->numOfPurchases++] = purchase;
            }
            else {
                printf("No items were purchased, purchase was not saved.\n");
            }
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
}

void printAllCustomers(const CustomerManagement* cm) {
    if (!cm || cm->numOfCustomers == 0) {
        printf("No customers available.\n");
        return;
    }
    for (int i = 0; i < cm->numOfCustomers; i++) {
        printf("Customer %d:\n", i + 1);
        printCustomer(&cm->customerArr[i]);
        printf("\n");
    }
}

void printAllPurchases(const CustomerManagement* cm) {
    if (!cm || cm->numOfPurchases == 0) {
        printf("No purchases to display.\n");
        return;
    }
    printf("Displaying all purchases:\n\n");
    for (int i = 0; i < cm->numOfPurchases; i++) {
        printPurchase(&cm->purchaseArr[i]);
    }
}

void freeCustomerManagement(CustomerManagement* cm) {
    for (int i = 0; i < cm->numOfCustomers; i++) {
        freePrescriptions(&cm->customerArr[i]);
    }
    free(cm->customerArr);
    for (int i = 0; i < cm->numOfPurchases; i++) {
        freePurchase(&cm->purchaseArr[i]);
    }
    free(cm->purchaseArr);
}

