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
#include "CustomerManagement.h"


void initPrescription(Prescription* prescription, Customer* customer, Pharmacy* pharmacy, int* nextPrescriptionID) {
    prescription->prescriptionID = (*nextPrescriptionID)++;
    printf("Available Medicines:\n");
    for (int i = 0; i < pharmacy->numOfMeds; i++) {
        printf("%d. %s\n", i + 1, pharmacy->medArr[i].medName);
    }
    int choice;
    printf("Choose a medicine by entering the corresponding number: ");
    scanf("%d", &choice);
    while (choice < 1 || choice > pharmacy->numOfMeds) {
        printf("Invalid choice. Please select a valid medicine number: ");
        scanf("%d", &choice);
    }
    strcpy(prescription->medicineName, pharmacy->medArr[choice - 1].medName);
    prescription->customer = customer;
    prescription->next = NULL;
}

int addPrescription(CustomerManagement* cm, Pharmacy* pharmacy) {
    if (cm->numOfCustomers == 0) {
        printf("No customers available to add a prescription.\n");
        return 0;
    }
    printf("Select a customer by entering their ID:\n");
    for (int i = 0; i < cm->numOfCustomers; i++) {
        printf("Customer Name: %s, ID: %09d\n", cm->customerArr[i].custName, cm->customerArr[i].custID);
    }
    int selectedID;
    printf("Enter customer ID: ");
    scanf("%d", &selectedID);
    Customer* customer = NULL;
    for (int i = 0; i < cm->numOfCustomers; i++) {
        if (cm->customerArr[i].custID == selectedID) {
            customer = &cm->customerArr[i];
            break;
        }
    }
    if (!customer) {
        printf("Customer ID %d not found.\n", selectedID);
        return 0;
    }
    Prescription* newPrescription = (Prescription*)malloc(sizeof(Prescription));
    if (!newPrescription) return 0;
    initPrescription(newPrescription, customer, pharmacy, &cm->nextPrescriptionID);
    if (!customer->prescriptions) {
        customer->prescriptions = newPrescription;
    }
    else {
        Prescription* temp = customer->prescriptions;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = newPrescription;
    }
    return 1;
}


