#ifndef __PURCHASE__
#define __PURCHASE__

typedef struct {
    int purchaseID;
    int customerID;
    char customerName[50];
    char** purchasedItems;
    int numOfItems;
    double totalCost;
    int pharmacistID;
    char pharmacistName[50];
} Purchase;


void printPurchase(const Purchase* purchase);
void savePurchaseToTextFile(const char* filename, const Purchase* purchase);
void loadPurchaseFromTextFile(const char* filename, Purchase* purchase);
void savePurchaseToBinaryFile(const char* filename, const Purchase* purchase);
void loadPurchaseFromBinaryFile(const char* filename, Purchase* purchase);
void freePurchase(Purchase* purchase);

#endif