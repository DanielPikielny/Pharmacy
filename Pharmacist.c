#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "Date.h"
#include "fileHelper.h"
#include "Medicine.h"
#include "Pharmacist.h"

#define SHIFT_TO_STRING(shift) \
    ((shift) == morning ? "Morning" : \
    (shift) == noon ? "Noon" : \
    (shift) == afternoon ? "Afternoon" : \
    (shift) == evening ? "Evening" : "Unknown")

void initPharmacist(Pharmacist* newPharmacist, Pharmacy* pharmacy) {
    do {
        newPharmacist->id = rand() % 90000000 + 10000000;
    } while (!isUniquePharmacistID(newPharmacist->id, pharmacy));
    printf("Enter pharmacist's name: ");
    scanf("%s", newPharmacist->phmtName);
    char phoneInput[15];
    int validPhone = 0;
    while (!validPhone) {
        printf("Enter pharmacist's 10-digit phone number: ");
        scanf("%s", phoneInput);
        if (strlen(phoneInput) == 10) {
            validPhone = 1;
            for (int i = 0; i < 10; i++) {
                if (!isdigit(phoneInput[i])) {
                    validPhone = 0;
                    break;
                }
            }
        }
        if (!validPhone) {
            printf("Invalid phone number. Please enter exactly 10 digits.\n");
        }
        else if (!isUniquePharmacistPhone(phoneInput, pharmacy)) {
            printf("Phone number already exists. Please enter a different phone number.\n");
            validPhone = 0;
        }
    }
    strcpy(newPharmacist->phmtPhone, phoneInput);
    snprintf(newPharmacist->phmtEmail, sizeof(newPharmacist->phmtEmail), "%s@pharmacy.com", newPharmacist->phmtName);
    while (!isUniquePharmacistEmail(newPharmacist->phmtEmail, pharmacy)) {
        printf("Email already exists. Please enter a different name for email.\n");
        printf("Enter pharmacist's name: ");
        scanf("%s", newPharmacist->phmtName);
        snprintf(newPharmacist->phmtEmail, sizeof(newPharmacist->phmtEmail), "%s@pharmacy.com", newPharmacist->phmtName);
    }
    int shiftChoice;
    printf("Choose shift (0 = morning, 1 = noon, 2 = afternoon, 3 = evening): ");
    scanf("%d", &shiftChoice);
    newPharmacist->shift = (eShift)shiftChoice;
}


int addPharmacist(Pharmacy* pharmacy) {
    Pharmacist* newPharmacist = (Pharmacist*)calloc(1, sizeof(Pharmacist));
    if (!newPharmacist)
        return 0;
    initPharmacist(newPharmacist, pharmacy);
    Pharmacist* temp = (Pharmacist*)realloc(pharmacy->pharmacistArr,
        (pharmacy->numOfPharmacists + 1) * sizeof(Pharmacist));
    if (!temp) {
        free(newPharmacist);
        return 0;
    }
    pharmacy->pharmacistArr = temp;
    pharmacy->pharmacistArr[pharmacy->numOfPharmacists] = *newPharmacist;
    pharmacy->numOfPharmacists++;
    free(newPharmacist);
    return 1;
}

void savePharmacistToTextFile(const Pharmacist* phmt, const char* filename)
{
    FILE* file = fopen(filename, "a");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }
    fprintf(file, "%d\n", phmt->id);
    fprintf(file, "%s\n", phmt->phmtName);
    fprintf(file, "%s\n", phmt->phmtPhone);
    fprintf(file, "%s\n", phmt->phmtEmail);
    fprintf(file, "%d\n", phmt->shift);
    fclose(file);
}

int loadPharmacistFromTextFile(Pharmacist* phmt, const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return 0;
    }
    fscanf(file, "%d\n", &phmt->id);
    fscanf(file, "%[^\n]%*c", phmt->phmtName);
    fscanf(file, "%[^\n]%*c", phmt->phmtPhone);
    fscanf(file, "%[^\n]%*c", phmt->phmtEmail);
    int shiftValue;
    fscanf(file, "%d\n", &shiftValue);
    phmt->shift = (eShift)shiftValue;
    fclose(file);
    return 1;
}

void savePharmacistToBinaryFile(const Pharmacist* phmt, const char* filename)
{
    FILE* file = fopen(filename, "ab");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }
    fwrite(phmt, sizeof(Pharmacist), 1, file);
    fclose(file);
}

int loadPharmacistFromBinaryFile(Pharmacist* phmt, const char* filename)
{
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Failed to open file");
        return 0;
    }
    fread(phmt, sizeof(Pharmacist), 1, file);
    fclose(file);
    return 1;
}


void changePharmacistShift(Pharmacist* pharmacist) {
    if (!pharmacist) {
        printf("Invalid Pharmacist data.\n");
        return;
    }
    int newShift;
    printf("Select new shift for Pharmacist %s:\n", pharmacist->phmtName);
    printf("1. Morning\n");
    printf("2. Noon\n");
    printf("3. Afternoon\n");
    printf("4. Evening\n");
    printf("Enter your choice (1-4): ");
    scanf("%d", &newShift);
    switch (newShift) {
    case 1:
        pharmacist->shift = morning;
        break;
    case 2:
        pharmacist->shift = noon;
        break;
    case 3:
        pharmacist->shift = afternoon;
        break;
    case 4:
        pharmacist->shift = evening;
        break;
    default:
        printf("Invalid choice. No changes made.\n");
        return;
    }
    printf("Shift changed successfully to %s.\n", SHIFT_TO_STRING(pharmacist->shift));
}

void printPharmacist(Pharmacist* pharmacistToPrint) {
    if (!pharmacistToPrint) {
        printf("Invalid Pharmacist data.\n");
        return;
    }
    printf("Pharmacist ID: %d\n", pharmacistToPrint->id);
    printf("Name: %s\n", pharmacistToPrint->phmtName);
    printf("Phone: %s\n", pharmacistToPrint->phmtPhone);
    printf("Email: %s\n", pharmacistToPrint->phmtEmail);
    printf("Shift: %s\n", SHIFT_TO_STRING(pharmacistToPrint->shift));
}

void freePharmacist(Pharmacist* pharToFree) {
    if (!pharToFree) {
        return;
    }
    free(pharToFree->phmtName);
    free(pharToFree->phmtPhone);
    free(pharToFree->phmtEmail);
    free(pharToFree);
}

int isUniquePharmacistID(int id, Pharmacy* pharmacy) {
    for (int i = 0; i < pharmacy->numOfPharmacists; i++) {
        if (pharmacy->pharmacistArr[i].id == id) {
            return 0;
        }
    }
    return 1;
}

int isUniquePharmacistPhone(const char* phone, Pharmacy* pharmacy) {
    for (int i = 0; i < pharmacy->numOfPharmacists; i++) {
        if (strcmp(pharmacy->pharmacistArr[i].phmtPhone, phone) == 0) {
            return 0;
        }
    }
    return 1;
}

int isUniquePharmacistEmail(const char* email, Pharmacy* pharmacy) {
    for (int i = 0; i < pharmacy->numOfPharmacists; i++) {
        if (strcmp(pharmacy->pharmacistArr[i].phmtEmail, email) == 0) {
            return 0;
        }
    }
    return 1;
}
