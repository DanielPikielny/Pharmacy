#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Date.h"
#include "fileHelper.h"
#include "Medicine.h"
#include "CustomerManagement.h"
#include "Pharmacy.h"

static const char* sortOptStr[eNumOfSortOptions] = {
    "None",
    "Name",
    "Brand",
    "Price",
    "Stock Quantity"
};

eSortOption showSortMenu() {
    int opt;
    printf("Based on what field do you want to sort?\n");
    do {
        for (int i = 1; i < eNumOfSortOptions; i++)
            printf("Enter %d for %s\n", i, sortOptStr[i]);
        scanf("%d", &opt);
    } while (opt < 0 || opt >= eNumOfSortOptions);

    return (eSortOption)opt;
}

void sortMedicine(Pharmacy* pharmacy) {
    eSortOption sortOption = showSortMenu();
    int(*compare)(const void*, const void*) = NULL;
    switch (sortOption) {
    case eName:
        compare = compareMedicineByName;
        break;
    case eBrand:
        compare = compareMedicineByBrand;
        break;
    case ePrice:
        compare = compareMedicineByPrice;
        break;
    case eStockQuantity:
        compare = compareMedicineByStockQuantity;
        break;
    default:
        return;
    }
    if (compare != NULL) {
        qsort(pharmacy->medArr, pharmacy->numOfMeds, sizeof(Medicine*), compare);
    }
}

void findMedicine(const Pharmacy* pharmacy) {
    int(*compare)(const void*, const void*) = NULL;
    Medicine med = { 0 };
    Medicine* pMedicine = &med;

    switch (pharmacy->sortOpt) {
    case eName:
        printf("Enter medicine name to search: ");
        scanf("%s", med.medName);
        compare = compareMedicineByName;
        break;

    case eBrand:
        printf("Enter brand to search: ");
        scanf("%s", med.medBrand);
        compare = compareMedicineByBrand;
        break;

    case ePrice:
        printf("Enter price to search: ");
        scanf("%lf", &med.medPrice);
        compare = compareMedicineByPrice;
        break;

    case eStockQuantity:
        printf("Enter stock quantity to search: ");
        scanf("%d", &med.medQInStock);
        compare = compareMedicineByStockQuantity;
        break;

    default:
        printf("The search cannot be performed, array not sorted\n");
        return;
    }

    if (compare != NULL) {
        Medicine** pMed = bsearch(&pMedicine, pharmacy->medArr, pharmacy->numOfMeds, sizeof(Medicine*), compare);
        if (pMed == NULL) {
            printf("Medicine not found\n");
        }
        else {
            printf("Medicine found:\n");
            printMedicine(*pMed);
        }
    }
    else {
        printf("Invalid sort option.\n");
    }
}

void printAllPharmacists(const Pharmacy* pharmacy)
{
    if (pharmacy->numOfPharmacists == 0) {
        printf("No pharmacists available in the pharmacy.\n");
        return;
    }
    printf("Pharmacists in the Pharmacy:\n");
    for (int i = 0; i < pharmacy->numOfPharmacists; i++) {
        printf("\nPharmacist %d:\n", i + 1);
        printPharmacist(&pharmacy->pharmacistArr[i]);
    }
}

void changePharmacistShiftInPharmacy(Pharmacy* pharmacy) {
    if (pharmacy->numOfPharmacists == 0) {
        printf("No pharmacists available to change shift.\n");
        return;
    }
    printAllPharmacists(pharmacy);
    int selectedIndex;
    printf("\nEnter the number of the pharmacist whose shift you want to change (1 to %d): ", pharmacy->numOfPharmacists);
    scanf("%d", &selectedIndex);
    if (selectedIndex < 1 || selectedIndex > pharmacy->numOfPharmacists) {
        printf("Invalid selection. No changes made.\n");
        return;
    }
    Pharmacist* selectedPharmacist = &pharmacy->pharmacistArr[selectedIndex - 1];
    changePharmacistShift(selectedPharmacist);
}

void printAllMedicines(const Pharmacy* pharmacy)
{
    if (pharmacy->numOfMeds == 0) {
        printf("No medicines available in the pharmacy.\n");
        return;
    }
    printf("Available Medicines:\n");
    for (int i = 0; i < pharmacy->numOfMeds; i++) {
        printf("\nMedicine %d:\n", i + 1);
        printMedicine(&pharmacy->medArr[i]);
    }
}

void printAllSupplies(const Pharmacy* pharmacy) {
    if (pharmacy->numOfSupplies == 0) {
        printf("No supplies available in the pharmacy.\n");
        return;
    }
    printf("Available Supplies:\n");
    for (int i = 0; i < pharmacy->numOfSupplies; i++) {
        printf("\nSupply %d:\n", i + 1);
        printSupply(&pharmacy->supplyArr[i]);
    }
}

void printEntirePharmacy(const Pharmacy* pharmacy) {
    if (!pharmacy) {
        printf("Invalid Pharmacy data.\n");
        return;
    }
    printAllPharmacists(pharmacy);
    printAllMedicines(pharmacy);
    printAllSupplies(pharmacy);
}


void updateSupplyStockInPharmacy(Pharmacy* pharmacy) {
    if (pharmacy->numOfSupplies == 0) {
        printf("No supplies available in the pharmacy.\n");
        return;
    }
    printf("Available supplies:\n");
    for (int i = 0; i < pharmacy->numOfSupplies; i++) {
        printf("%d. %s - Price: %.2f, Quantity in Stock: %d\n", i + 1,
            pharmacy->supplyArr[i].supName, pharmacy->supplyArr[i].supPrice,
            pharmacy->supplyArr[i].supQInStock);
    }
    int supplyIndex;
    printf("Select the supply by entering the corresponding number: ");
    scanf("%d", &supplyIndex);
    if (supplyIndex < 1 || supplyIndex > pharmacy->numOfSupplies) {
        printf("Invalid selection.\n");
        return;
    }
    int quantityChange;
    printf("Enter the quantity to add: ");
    scanf("%d", &quantityChange);
    if (updateSupplyStock(&pharmacy->supplyArr[supplyIndex - 1], quantityChange)) {
        printf("Stock updated successfully.\n");
    }
    else {
        printf("Failed to update stock.\n");
    }
}


void updateMedicineStockInPharmacy(Pharmacy* pharmacy) {
    if (pharmacy->numOfMeds == 0) {
        printf("No medicines available in the pharmacy.\n");
        return;
    }
    printf("Available medicines:\n");
    for (int i = 0; i < pharmacy->numOfMeds; i++) {
        printf("%d. %s - Price: %.2f, Quantity in Stock: %d\n", i + 1,
            pharmacy->medArr[i].medName, pharmacy->medArr[i].medPrice,
            pharmacy->medArr[i].medQInStock);
    }
    int medIndex;
    printf("Select the medicine by entering the corresponding number: ");
    scanf("%d", &medIndex);
    if (medIndex < 1 || medIndex > pharmacy->numOfMeds) {
        printf("Invalid selection.\n");
        return;
    }
    int quantityChange;
    printf("Enter the quantity to add: ");
    scanf("%d", &quantityChange);
    if (updateStock(&pharmacy->medArr[medIndex - 1], quantityChange)) {
        printf("Stock updated successfully.\n");
    }
    else {
        printf("Failed to update stock.\n");
    }
}

void savePharmacyToTextFile(const Pharmacy* pharmacy, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }
    fprintf(file, "%d\n", pharmacy->numOfSupplies);
    fclose(file);
    for (int i = 0; i < pharmacy->numOfSupplies; i++) {
        saveSuppliesToTextFile(&pharmacy->supplyArr[i], filename);
    }
    file = fopen(filename, "a");
    fprintf(file, "%d\n", pharmacy->numOfMeds);
    fclose(file);
    for (int i = 0; i < pharmacy->numOfMeds; i++) {
        saveMedicineToTextFile(&pharmacy->medArr[i], filename);
    }
    file = fopen(filename, "a");
    fprintf(file, "%d\n", pharmacy->numOfPharmacists);
    fclose(file);
    for (int i = 0; i < pharmacy->numOfPharmacists; i++) {
        savePharmacistToTextFile(&pharmacy->pharmacistArr[i], filename);
    }
    file = fopen(filename, "a");
    fprintf(file, "%d\n", pharmacy->sortOpt);
    fclose(file);
    printf("Pharmacy data saved to %s successfully.\n", filename);
}

int loadPharmacyFromTextFile(Pharmacy* pharmacy, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return 0;
    }
    fscanf(file, "%d\n", &pharmacy->numOfSupplies);
    fclose(file);
    pharmacy->supplyArr = (Supplies*)malloc(pharmacy->numOfSupplies * sizeof(Supplies));
    if (pharmacy->supplyArr == NULL) {
        perror("Failed to allocate memory for supplies");
        return 0;
    }
    for (int i = 0; i < pharmacy->numOfSupplies; i++) {
        if (!loadSuppliesFromTextFile(&pharmacy->supplyArr[i], filename)) {
            return 0;
        }
    }
    file = fopen(filename, "r");
    fseek(file, ftell(file), SEEK_CUR);
    fscanf(file, "%d\n", &pharmacy->numOfMeds);
    fclose(file);
    pharmacy->medArr = (Medicine*)malloc(pharmacy->numOfMeds * sizeof(Medicine));
    if (pharmacy->medArr == NULL) {
        perror("Failed to allocate memory for medicines");
        return 0;
    }
    for (int i = 0; i < pharmacy->numOfMeds; i++) {
        if (!loadMedicineFromTextFile(&pharmacy->medArr[i], filename)) {
            return 0;
        }
    }
    file = fopen(filename, "r");
    fseek(file, ftell(file), SEEK_CUR);
    fscanf(file, "%d\n", &pharmacy->numOfPharmacists);
    fclose(file);
    pharmacy->pharmacistArr = (Pharmacist*)malloc(pharmacy->numOfPharmacists * sizeof(Pharmacist));
    if (pharmacy->pharmacistArr == NULL) {
        perror("Failed to allocate memory for pharmacists");
        return 0;
    }
    for (int i = 0; i < pharmacy->numOfPharmacists; i++) {
        if (!loadPharmacistFromTextFile(&pharmacy->pharmacistArr[i], filename)) {
            return 0;
        }
    }
    file = fopen(filename, "r");
    fseek(file, ftell(file), SEEK_CUR);
    fscanf(file, "%d\n", (int*)&pharmacy->sortOpt);
    fclose(file);
    printf("Pharmacy data loaded successfully.\n");
    return 1;
}

void savePharmacyToBinaryFile(const Pharmacy* pharmacy, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }
    fwrite(&pharmacy->numOfSupplies, sizeof(int), 1, file);
    for (int i = 0; i < pharmacy->numOfSupplies; i++) {
        saveSuppliesToBinaryFile(&pharmacy->supplyArr[i], filename);
    }
    fwrite(&pharmacy->numOfMeds, sizeof(int), 1, file);
    for (int i = 0; i < pharmacy->numOfMeds; i++) {
        saveMedicineToBinaryFile(&pharmacy->medArr[i], filename);
    }
    fwrite(&pharmacy->numOfPharmacists, sizeof(int), 1, file);
    for (int i = 0; i < pharmacy->numOfPharmacists; i++) {
        savePharmacistToBinaryFile(&pharmacy->pharmacistArr[i], filename);
    }
    fwrite(&pharmacy->sortOpt, sizeof(eSortOption), 1, file);
    fclose(file);
    printf("Pharmacy data saved to %s successfully.\n", filename);
}

int loadPharmacyFromBinaryFile(Pharmacy* pharmacy, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Failed to open file");
        return 0;
    }
    fread(&pharmacy->numOfSupplies, sizeof(int), 1, file);
    pharmacy->supplyArr = (Supplies*)malloc(pharmacy->numOfSupplies * sizeof(Supplies));
    if (pharmacy->supplyArr == NULL) {
        perror("Failed to allocate memory for supplies");
        fclose(file);
        return 0;
    }
    for (int i = 0; i < pharmacy->numOfSupplies; i++) {
        if (!loadSuppliesFromBinaryFile(&pharmacy->supplyArr[i], filename)) {
            fclose(file);
            return 0;
        }
    }
    fread(&pharmacy->numOfMeds, sizeof(int), 1, file);
    pharmacy->medArr = (Medicine*)malloc(pharmacy->numOfMeds * sizeof(Medicine));
    if (pharmacy->medArr == NULL) {
        perror("Failed to allocate memory for medicines");
        fclose(file);
        return 0;
    }
    for (int i = 0; i < pharmacy->numOfMeds; i++) {
        if (!loadMedicineFromBinaryFile(&pharmacy->medArr[i], filename)) {
            fclose(file);
            return 0;
        }
    }
    fread(&pharmacy->numOfPharmacists, sizeof(int), 1, file);
    pharmacy->pharmacistArr = (Pharmacist*)malloc(pharmacy->numOfPharmacists * sizeof(Pharmacist));
    if (pharmacy->pharmacistArr == NULL) {
        perror("Failed to allocate memory for pharmacists");
        fclose(file);
        return 0;
    }
    for (int i = 0; i < pharmacy->numOfPharmacists; i++) {
        if (!loadPharmacistFromBinaryFile(&pharmacy->pharmacistArr[i], filename)) {
            fclose(file);
            return 0;
        }
    }
    fread(&pharmacy->sortOpt, sizeof(eSortOption), 1, file);
    fclose(file);
    printf("Pharmacy data loaded from %s successfully.\n", filename);
    return 1;
}

void freePharmacy(Pharmacy* pharmacy) {
    if (!pharmacy) {
        return;
    }
    for (int i = 0; i < pharmacy->numOfSupplies; i++) {
        freeSupply(&pharmacy->supplyArr[i]);
    }
    free(pharmacy->supplyArr);
    for (int i = 0; i < pharmacy->numOfMeds; i++) {
        freeMedicine(&pharmacy->medArr[i]);
    }
    free(pharmacy->medArr);
    for (int i = 0; i < pharmacy->numOfPharmacists; i++) {
        freePharmacist(&pharmacy->pharmacistArr[i]);
    }
    free(pharmacy->pharmacistArr);
    free(pharmacy);
}

/*void saveEverythingToTextFile(const Pharmacy* pharmacy, const CustomerManagement* cm, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error: Could not open file %s for writing.\n", filename);
        return;
    }
    fprintf(file, "%d\n", pharmacy->numOfSupplies);
    for (int i = 0; i < pharmacy->numOfSupplies; i++) {
        saveSuppliesToTextFile(&pharmacy->supplyArr[i], filename);
    }
    fprintf(file, "%d\n", pharmacy->numOfMeds);
    for (int i = 0; i < pharmacy->numOfMeds; i++) {
        saveMedicineToTextFile(&pharmacy->medArr[i], filename);
    }
    fprintf(file, "%d\n", pharmacy->numOfPharmacists);
    for (int i = 0; i < pharmacy->numOfPharmacists; i++) {
        savePharmacistToTextFile(&pharmacy->pharmacistArr[i], filename);
    }
    fprintf(file, "%d\n", pharmacy->sortOpt);
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
    printf("All the data saved successfully.\n");
}

int loadEverythingFromTextFile(Pharmacy* pharmacy, CustomerManagement* cm, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s for reading.\n", filename);
        return 0;
    }
    fscanf(file, "%d\n", &pharmacy->numOfSupplies);
    pharmacy->supplyArr = (Supplies*)malloc(pharmacy->numOfSupplies * sizeof(Supplies));
    if (pharmacy->supplyArr == NULL) {
        perror("Failed to allocate memory for supplies");
        return 0;
    }
    for (int i = 0; i < pharmacy->numOfSupplies; i++) {
        if (!loadSuppliesFromTextFile(&pharmacy->supplyArr[i], filename)) {
            return 0;
        }
    }
    fscanf(file, "%d\n", &pharmacy->numOfMeds);
    pharmacy->medArr = (Medicine*)malloc(pharmacy->numOfMeds * sizeof(Medicine));
    if (pharmacy->medArr == NULL) {
        perror("Failed to allocate memory for medicines");
        return 0;
    }
    for (int i = 0; i < pharmacy->numOfMeds; i++) {
        if (!loadMedicineFromTextFile(&pharmacy->medArr[i], filename)) {
            return 0;
        }
    }
    fscanf(file, "%d\n", &pharmacy->numOfPharmacists);
    pharmacy->pharmacistArr = (Pharmacist*)malloc(pharmacy->numOfPharmacists * sizeof(Pharmacist));
    if (pharmacy->pharmacistArr == NULL) {
        perror("Failed to allocate memory for pharmacists");
        return 0;
    }
    for (int i = 0; i < pharmacy->numOfPharmacists; i++) {
        if (!loadPharmacistFromTextFile(&pharmacy->pharmacistArr[i], filename)) {
            return 0;
        }
    }
    fscanf(file, "%d\n", (int*)&pharmacy->sortOpt);
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
    printf("All the data was loaded successfully.\n");
    return 1;
}

void saveEverythingToBinaryFile(const Pharmacy* pharmacy, const CustomerManagement* cm, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        printf("Error: Could not open file %s for writing.\n", filename);
        return;
    }
    fwrite(&pharmacy->numOfSupplies, sizeof(int), 1, file);
    for (int i = 0; i < pharmacy->numOfSupplies; i++) {
        saveSuppliesToBinaryFile(&pharmacy->supplyArr[i], filename);
    }
    fwrite(&pharmacy->numOfMeds, sizeof(int), 1, file);
    for (int i = 0; i < pharmacy->numOfMeds; i++) {
        saveMedicineToBinaryFile(&pharmacy->medArr[i], filename);
    }
    fwrite(&pharmacy->numOfPharmacists, sizeof(int), 1, file);
    for (int i = 0; i < pharmacy->numOfPharmacists; i++) {
        savePharmacistToBinaryFile(&pharmacy->pharmacistArr[i], filename);
    }
    fwrite(&pharmacy->sortOpt, sizeof(eSortOption), 1, file);
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
    printf("All the data saved successfully.\n");
}

int loadPharmacyAndCustomerManagementFromBinaryFile(Pharmacy* pharmacy, CustomerManagement* cm, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Error: Could not open file %s for reading.\n", filename);
        return 0;
    }
    fread(&pharmacy->numOfSupplies, sizeof(int), 1, file);
    pharmacy->supplyArr = (Supplies*)malloc(pharmacy->numOfSupplies * sizeof(Supplies));
    if (pharmacy->supplyArr == NULL) {
        perror("Failed to allocate memory for supplies");
        return 0;
    }
    for (int i = 0; i < pharmacy->numOfSupplies; i++) {
        if (!loadSuppliesFromBinaryFile(&pharmacy->supplyArr[i], filename)) {
            return 0;
        }
    }
    fread(&pharmacy->numOfMeds, sizeof(int), 1, file);
    pharmacy->medArr = (Medicine*)malloc(pharmacy->numOfMeds * sizeof(Medicine));
    if (pharmacy->medArr == NULL) {
        perror("Failed to allocate memory for medicines");
        return 0;
    }
    for (int i = 0; i < pharmacy->numOfMeds; i++) {
        if (!loadMedicineFromBinaryFile(&pharmacy->medArr[i], filename)) {
            return 0;
        }
    }
    fread(&pharmacy->numOfPharmacists, sizeof(int), 1, file);
    pharmacy->pharmacistArr = (Pharmacist*)malloc(pharmacy->numOfPharmacists * sizeof(Pharmacist));
    if (pharmacy->pharmacistArr == NULL) {
        perror("Failed to allocate memory for pharmacists");
        return 0;
    }
    for (int i = 0; i < pharmacy->numOfPharmacists; i++) {
        if (!loadPharmacistFromBinaryFile(&pharmacy->pharmacistArr[i], filename)) {
            return 0;
        }
    }
    fread(&pharmacy->sortOpt, sizeof(eSortOption), 1, file);
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
    printf("All the data loaded successfully.\n");
    return 1;
}*/

void saveEverythingToFile(const Pharmacy* pharmacy, const CustomerManagement* cm, const char* filename, int mode) {
    FILE* file;
    if (mode == 1) {
        file = fopen(filename, "w");
        if (!file) {
            printf("Error: Could not open file %s for writing.\n", filename);
            return;
        }
        fprintf(file, "%d\n", pharmacy->numOfSupplies);
        for (int i = 0; i < pharmacy->numOfSupplies; i++) {
            saveSuppliesToTextFile(&pharmacy->supplyArr[i], filename);
        }
        fprintf(file, "%d\n", pharmacy->numOfMeds);
        for (int i = 0; i < pharmacy->numOfMeds; i++) {
            saveMedicineToTextFile(&pharmacy->medArr[i], filename);
        }
        fprintf(file, "%d\n", pharmacy->numOfPharmacists);
        for (int i = 0; i < pharmacy->numOfPharmacists; i++) {
            savePharmacistToTextFile(&pharmacy->pharmacistArr[i], filename);
        }
        fprintf(file, "%d\n", pharmacy->sortOpt);
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
    }
    else if (mode == 2) {
        file = fopen(filename, "wb");
        if (!file) {
            printf("Error: Could not open file %s for writing.\n", filename);
            return;
        }
        fwrite(&pharmacy->numOfSupplies, sizeof(int), 1, file);
        for (int i = 0; i < pharmacy->numOfSupplies; i++) {
            saveSuppliesToBinaryFile(&pharmacy->supplyArr[i], filename);
        }
        fwrite(&pharmacy->numOfMeds, sizeof(int), 1, file);
        for (int i = 0; i < pharmacy->numOfMeds; i++) {
            saveMedicineToBinaryFile(&pharmacy->medArr[i], filename);
        }
        fwrite(&pharmacy->numOfPharmacists, sizeof(int), 1, file);
        for (int i = 0; i < pharmacy->numOfPharmacists; i++) {
            savePharmacistToBinaryFile(&pharmacy->pharmacistArr[i], filename);
        }
        fwrite(&pharmacy->sortOpt, sizeof(eSortOption), 1, file);
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
    }
    fclose(file);
    printf("All the data saved successfully.\n");
}

int loadEverything(Pharmacy* pharmacy, CustomerManagement* cm, const char* filename, int format) {
    FILE* file = (format == 1) ? fopen(filename, "r") : fopen(filename, "rb");
    if (!file) {
        printf("Error: Could not open file %s for reading.\n", filename);
        return 0;
    }
    if (format == 1) {
        fscanf(file, "%d\n", &pharmacy->numOfSupplies);
        pharmacy->supplyArr = (Supplies*)malloc(pharmacy->numOfSupplies * sizeof(Supplies));
        if (pharmacy->supplyArr == NULL) {
            perror("Failed to allocate memory for supplies");
            fclose(file);
            return 0;
        }
        for (int i = 0; i < pharmacy->numOfSupplies; i++) {
            if (!loadSuppliesFromTextFile(&pharmacy->supplyArr[i], filename)) {
                fclose(file);
                return 0;
            }
        }
        fscanf(file, "%d\n", &pharmacy->numOfMeds);
        pharmacy->medArr = (Medicine*)malloc(pharmacy->numOfMeds * sizeof(Medicine));
        if (pharmacy->medArr == NULL) {
            perror("Failed to allocate memory for medicines");
            fclose(file);
            return 0;
        }
        for (int i = 0; i < pharmacy->numOfMeds; i++) {
            if (!loadMedicineFromTextFile(&pharmacy->medArr[i], filename)) {
                fclose(file);
                return 0;
            }
        }
        fscanf(file, "%d\n", &pharmacy->numOfPharmacists);
        pharmacy->pharmacistArr = (Pharmacist*)malloc(pharmacy->numOfPharmacists * sizeof(Pharmacist));
        if (pharmacy->pharmacistArr == NULL) {
            perror("Failed to allocate memory for pharmacists");
            fclose(file);
            return 0;
        }
        for (int i = 0; i < pharmacy->numOfPharmacists; i++) {
            if (!loadPharmacistFromTextFile(&pharmacy->pharmacistArr[i], filename)) {
                fclose(file);
                return 0;
            }
        }
        fscanf(file, "%d\n", (int*)&pharmacy->sortOpt);
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
    }
    else if (format == 2) {
        fread(&pharmacy->numOfSupplies, sizeof(int), 1, file);
        pharmacy->supplyArr = (Supplies*)malloc(pharmacy->numOfSupplies * sizeof(Supplies));
        if (pharmacy->supplyArr == NULL) {
            perror("Failed to allocate memory for supplies");
            fclose(file);
            return 0;
        }
        for (int i = 0; i < pharmacy->numOfSupplies; i++) {
            if (!loadSuppliesFromBinaryFile(&pharmacy->supplyArr[i], filename)) {
                fclose(file);
                return 0;
            }
        }
        fread(&pharmacy->numOfMeds, sizeof(int), 1, file);
        pharmacy->medArr = (Medicine*)malloc(pharmacy->numOfMeds * sizeof(Medicine));
        if (pharmacy->medArr == NULL) {
            perror("Failed to allocate memory for medicines");
            fclose(file);
            return 0;
        }
        for (int i = 0; i < pharmacy->numOfMeds; i++) {
            if (!loadMedicineFromBinaryFile(&pharmacy->medArr[i], filename)) {
                fclose(file);
                return 0;
            }
        }
        fread(&pharmacy->numOfPharmacists, sizeof(int), 1, file);
        pharmacy->pharmacistArr = (Pharmacist*)malloc(pharmacy->numOfPharmacists * sizeof(Pharmacist));
        if (pharmacy->pharmacistArr == NULL) {
            perror("Failed to allocate memory for pharmacists");
            fclose(file);
            return 0;
        }
        for (int i = 0; i < pharmacy->numOfPharmacists; i++) {
            if (!loadPharmacistFromBinaryFile(&pharmacy->pharmacistArr[i], filename)) {
                fclose(file);
                return 0;
            }
        }
        fread(&pharmacy->sortOpt, sizeof(eSortOption), 1, file);
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
    }
    fclose(file);
    printf("All the data was loaded successfully.\n");
    return 1;
}
