#ifndef __CUSTOMER__
#define __CUSTOMER__

#include "Perscription.h"
#include "CustomerManagement.h"

typedef struct {
    int custID;
    char custName[50];
    char custPhone[15];
    Prescription* prescriptions;
} Customer;




int addCustomer(CustomerManagement* cm);
int isValidCustomerID(int id, CustomerManagement* cm);
int isValidCustomerID(int custID, const CustomerManagement* cm);
void initCustomer(Customer* customer, CustomerManagement* cm);
void printCustomer(const Customer* cust);
void freeCustomer(Customer* cust);


#endif