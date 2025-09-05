#ifndef __CUSTOMERMANAGEMENT__
#define __CUSTOMERMANAGEMENT__
#include "Customer.h"
#include "Purchase.h"
#include "Perscription.h"
#include "Pharmacy.h"


typedef struct {
    Customer* customerArr;
    int numOfCustomers;
    int nextPrescriptionID;
    Purchase* purchaseArr;
    int numOfPurchases;
    int nextPurchaseID;
} CustomerManagement;


void initCustomerManagement(CustomerManagement* cm);
int loadCustomerManagementFromTextFile(CustomerManagement* cm, const char* filename);
void saveCustomerManagementToTextFile(const CustomerManagement* cm, const char* filename);
void saveCustomerManagementToBinaryFile(const CustomerManagement* cm, const char* filename);
int loadCustomerManagementFromBinaryFile(CustomerManagement* cm, const char* filename);
void freeCustomerManagement(CustomerManagement* cm);
void printAllPurchases(const CustomerManagement* cm);
void printAllCustomers(const CustomerManagement* cm);
int deletePrescription(Customer* customer, int prescriptionID);
void purchase(CustomerManagement* cm, Pharmacy* pharmacy);

#endif