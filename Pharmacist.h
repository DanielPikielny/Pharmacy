#ifndef __PHARMACIST__
#define __PHARMACIST__
#include "Pharmacy.h"

typedef enum { morning, noon, afternoon, evening } eShift;

typedef struct {
    int id;
    char phmtName[50];
    char phmtPhone[15];
    char phmtEmail[50];
    eShift shift;
}Pharmacist;

int addPharmacist(Pharmacy* pharmacy);
void initPharmacist(Pharmacist* newPharmacist, Pharmacy* pharmacy);
void printPharmacist(Pharmacist* pharmacistToPrint);
void changePharmacistShift(Pharmacist* pharmacist);
void freePharmacist(Pharmacist* pharToFree);
void savePharmacistToTextFile(const Pharmacist* phmt, const char* filename);
int loadPharmacistFromTextFile(Pharmacist* phmt, const char* filename);
void savePharmacistToBinaryFile(const Pharmacist* phmt, const char* filename);
int loadPharmacistFromBinaryFile(Pharmacist* phmt, const char* filename);
int isUniquePharmacistID(int id, Pharmacy* pharmacy);
int isUniquePharmacistPhone(const char* phone, Pharmacy* pharmacy);
int isUniquePharmacistEmail(const char* email, Pharmacy* pharmacy);

#endif
