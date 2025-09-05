#ifndef __PERSCRIPTION__
#define __PERSCRIPTION__

#include "Perscription.h"

typedef struct Prescription {
    int prescriptionID;
    char medicineName[50];
    struct Prescription* next;
    Customer* customer;
} Prescription;


void initPrescription(Prescription* prescription, Customer* customer, Pharmacy* pharmacy, int* nextPrescriptionID);
int addPrescription(CustomerManagement* cm, Pharmacy* pharmacy);
#endif