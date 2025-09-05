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

int addCustomer(CustomerManagement* cm) {
    Customer* newCustomer = (Customer*)calloc(1, sizeof(Customer));
    if (!newCustomer) return 0;
    initCustomer(newCustomer, cm);
    Customer* temp = realloc(cm->customerArr, (cm->numOfCustomers + 1) * sizeof(Customer));
    if (!temp) {
        free(newCustomer);
        return 0;
    }
    cm->customerArr = temp;
    cm->customerArr[cm->numOfCustomers] = *newCustomer;
    cm->numOfCustomers++;
    free(newCustomer);
    return 1;
}

void initCustomer(Customer* customer, CustomerManagement* cm) {
    printf("Enter customer name: ");
    scanf("%s", customer->custName);
    do {
        printf("Enter 9-digit customer ID: ");
        scanf("%d", &customer->custID);
    } while (!isValidCustomerID(customer->custID, cm));

    printf("Enter customer phone number: ");
    scanf("%s", customer->custPhone);
    customer->prescriptions = NULL;
}


int isValidCustomerID(int custID, const CustomerManagement* cm) {
    if (custID < 100000000 || custID > 999999999) {
        printf("Error: Customer ID must be exactly 9 digits.\n");
        return 0;
    }
    for (int i = 0; i < cm->numOfCustomers; i++) {
        if (cm->customerArr[i].custID == custID) {
            printf("Error: Customer ID already exists.\n");
            return 0;
        }
    }
    return 1;
}

void printCustomer(const Customer* cust) {
    if (!cust) return;
    printf("Customer ID: %09d\n", cust->custID);
    printf("Customer Name: %s\n", cust->custName);
    printf("Customer Phone: %s\n", cust->custPhone);
    if (cust->prescriptions) {
        printf("Prescriptions:\n");
        Prescription* current = cust->prescriptions;
        while (current) {
            printPrescription(current);
            current = current->next;
        }
    }
    else {
        printf("No prescriptions available for this customer.\n");
    }
}

void freeCustomer(Customer* cust) {
    Prescription* currentPrescription = cust->prescriptions;
    while (currentPrescription != NULL) {
        Prescription* temp = currentPrescription;
        currentPrescription = currentPrescription->next;
        free(temp);
    }
}
