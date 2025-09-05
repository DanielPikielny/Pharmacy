#ifndef __SUPPLIES__
#define __SUPPLIES__
#include "Pharmacy.h"


#define MAX_NAME_LEN 50
typedef enum { Sanitizer, Bandage, Vitamins, Mask, Ointment, eNoOfSupplyTypes } eSupply;

typedef struct
{
    char supName[MAX_NAME_LEN];
    int supPrice;
    int supQInStock;
    eSupply type;
}Supplies;

void initSupplies(Supplies* supply);
int addSupplies(Pharmacy* pharmacy);
void saveSuppliesToTextFile(const Supplies* supply, const char* filename);
int loadSuppliesFromTextFile(Supplies* supply, const char* filename);
void saveSuppliesToBinaryFile(const Supplies* supply, const char* filename);
int loadSuppliesFromBinaryFile(Supplies* supply, const char* filename);
void printSupply(const Supplies* supply);
void freeSupply(Supplies* supply);
int updateSupplyStock(Supplies* supply, int change);

#endif

