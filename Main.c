#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Pharmacy.h"
#include "CustomerManagement.h"
#include "main.h"

const char* str[eNofOptions] = {
    "Add Medicine",
    "Add supply",
    "Add pharmacist",
    "Add customer",
    "Add perscription",
    "Print medicines",
    "Print supplies",
    "Print pharmacists",
    "Print customers",
    "Print purchases",
    "Print pharmacy",
    "Update stock of existing medicine",
    "Update stock of existing medical supply",
    "Sort madicines"
    "Search for a medicine"
    "Change pharmacist shift",
    "Make a purchase"
};

int main()
{
    Pharmacy pharmacy;
    CustomerManagement cm;
    initPharmacy(&pharmacy);
    initCustomerManagement(&cm);
    //@@@@@@@@@@@
    int loadChoice, saveChoice;
    char* fileName = "finalProjBinarySavedData.bin";
    printf("Do you want to upload data from:\n1) Text file\n2) Binary file\n");
    scanf("%d", &loadChoice);
    if (loadChoice == 1) {
        fileName = "finalProjTextSavedData.txt";
    }
    else
    {
        loadChoice == 2;
    }
    loadEverythingFromTextFile(&pharmacy, &cm, fileName,loadChoice);
    int option;
    int stop = 0;
    do {
        option = menu();
        switch (option) {
        case eAddMedicine:
            if (!addMedicine(&pharmacy))
                printf("Error adding medicine\n");
            break;

        case eAddSupply:
            if (!addSupplies(&pharmacy))
                printf("Error adding supply\n");
            break;

        case eAddPharmacist:
            if (!addPharmacist(&pharmacy))
                printf("Error adding pharmacist\n");
            break;

        case eAddCustomer:
            if (!addCustomer(&cm))
                printf("Error adding customer\n");
            break;

        case eAddPrescription:
            if (!addPrescription(&cm, &pharmacy))
                printf("Error adding prescription\n");
            break;

        case ePrintMedicines:
            printAllMedicines(&pharmacy);
            break;

        case ePrintSupplies:
            printAllSupplies(&pharmacy);
            break;

        case ePrintPharmacists:
            printAllPharmacists(&pharmacy);
            break;

        case ePrintCustomers:
            printAllCustomers(&cm);
            break;

        case ePrintPurchases:
            printAllPurchases(&cm);
            break;

        case ePrintPharmacy:
            printEntirePharmacy(&pharmacy);
            break;

        case eUpdateMedicineStock:
            updateMedicineStockInPharmacy(&pharmacy);
            break;

        case eUpdateSupplyStock:
            updateSupplyStockInPharmacy(&pharmacy);
            break;

        case eSortMedicines:
            sortMedicine(&pharmacy);
            break;

        case eSearchMedicine:
            findMedicine(&pharmacy);
            break;

        case eChangePharmacistShift:
            changePharmacistShift(&pharmacy);
            break;

        case ePurchase:
            purchase(&cm, &pharmacy);
            break;

        case EXIT:
            printf("Bye bye\n");
            stop = 1;
            break;

        default:
            printf("Invalid option\n");
            break;
        }
    } while (!stop);
    freeCustomerManagement(&cm);
    freePharmacy(&pharmacy);
    system("pause");
    return 0;
}

int menu()
{
    int option;
    printf("\n\n");
    printf("Please choose one of the following options:\n");
    for (int i = 0; i < eNofOptions; i++)
        printf("%d - %s\n", i, str[i]);
    printf("%d - Quit\n", EXIT);
    scanf("%d", &option);
    char tav;
    scanf("%c", &tav);
    return option;
}
