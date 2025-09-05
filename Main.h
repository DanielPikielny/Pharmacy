#pragma once
#include "Pharmacy.h"
#include "CustomerManagement.h"

typedef enum
{
    eAddMedicine,
    eAddSupply,
    eAddPharmacist,
    eAddCustomer,
    eAddPrescription,
    ePrintMedicines,
    ePrintSupplies,
    ePrintPharmacists,
    ePrintCustomers,
    ePrintPurchases,
    ePrintPharmacy,
    eUpdateMedicineStock,
    eUpdateSupplyStock,
    eSortMedicines,
    eSearchMedicine,
    eChangePharmacistShift,
    ePurchase,
    eNofOptions
} eMenuOptions;


const char* str[eNofOptions];

int menu();

#define EXIT -1
