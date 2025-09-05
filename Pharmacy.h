#ifndef __PHARMACY__
#define __PHARMACY__
#include "Supplies.h"
#include "Pharmacist.h"
#include "Medicine.h"
#include "CustomerManagement.h"

typedef enum { eNone, eName, eBrand, ePrice, eStockQuantity, eNumOfSortOptions } eSortOption;
static const char* sortOptStr[eNumOfSortOptions];

typedef struct {
    Supplies* supplyArr;
    int numOfSupplies;
    Medicine* medArr;
    int numOfMeds;
    Pharmacist* pharmacistArr;
    int numOfPharmacists;
    eSortOption	sortOpt; // for medicine
} Pharmacy;

eSortOption showSortMenu();
void sortMedicine(Pharmacy* pharmacy);
void findMedicine(const Pharmacy* pharmacy);
void printAllPharmacists(const Pharmacy* pharmacy);
void changePharmacistShiftInPharmacy(Pharmacy* pharmacy);
void printAllMedicines(const Pharmacy* pharmacy);
void printAllSupplies(const Pharmacy* pharmacy);
void printEntirePharmacy(const Pharmacy* pharmacy);
void updateSupplyStockInPharmacy(Pharmacy* pharmacy);
void updateMedicineStockInPharmacy(Pharmacy* pharmacy);
void savePharmacyToTextFile(const Pharmacy* pharmacy, const char* filename);
int loadPharmacyFromTextFile(Pharmacy* pharmacy, const char* filename);
void savePharmacyToBinaryFile(const Pharmacy* pharmacy, const char* filename);
int loadPharmacyFromBinaryFile(Pharmacy* pharmacy, const char* filename);
void freePharmacy(Pharmacy* pharmacy);
void saveEverythingToFile(const Pharmacy* pharmacy, const CustomerManagement* cm, const char* filename, int mode);
int loadEverything(Pharmacy* pharmacy, CustomerManagement* cm, const char* filename, int format);
/*void saveEverythingToTextFile(const Pharmacy* pharmacy, const CustomerManagement* cm, const char* filename);
int loadEverythingFromTextFile(Pharmacy* pharmacy, CustomerManagement* cm, const char* filename);
void saveEverythingToBinaryFile(const Pharmacy* pharmacy, const CustomerManagement* cm, const char* filename);
int loadPharmacyAndCustomerManagementFromBinaryFile(Pharmacy* pharmacy, CustomerManagement* cm, const char* filename);*/

#endif