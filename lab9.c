#include <stdio.h>
#include <stdlib.h>

// This program was written by Michael Schlosbon on 4/12/2024. The purpose was to use a hash function to input elements into an array that would then be printed out.

// RecordType
struct RecordType
{
    int     id;
    char    name;
    int     order; 
};

// Node for linked list
struct Node
{
    struct RecordType record;
    struct Node* next;
};

// HashType
struct HashType
{
    struct Node* head;
};

// Compute the hash function
int hash(int x, int hashSz)
{
    return x % hashSz;
}

// Parses input file to an array of RecordType
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*)malloc(sizeof(struct RecordType) * dataSz);
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }
        fclose(inFile);
    }

    return dataSz;
}

// Prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// Display records in the hash structure
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
    int i;
    struct Node* temp;

    for (i = 0; i < hashSz; ++i)
    {
        printf("Index %d -> ", i);
        temp = pHashArray[i].head;
        while (temp != NULL)
        {
            printf("%d %c %d", temp->record.id, temp->record.name, temp->record.order);
            if (temp->next != NULL)
            {
                printf(" -> ");
            }
            temp = temp->next;
        }
        printf("\n");
    }
}

// Insert a record into the hash table using separate chaining
void insertRecord(struct HashType* pHashArray, int hashSz, struct RecordType record)
{
    int index = hash(record.id, hashSz);
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL)
    {
        printf("Cannot allocate memory\n");
        exit(-1);
    }
    newNode->record = record;
    newNode->next = NULL;

    if (pHashArray[index].head == NULL)
    {
        pHashArray[index].head = newNode;
    }
    else
    {
        // Insert at the beginning of the linked list
        newNode->next = pHashArray[index].head;
        pHashArray[index].head = newNode;
    }
}

int main(void)
{
    struct RecordType *pRecords;
    struct HashType *pHashArray;
    int recordSz = 0, i;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Initialize hash table
    int hashSz = 52;
    pHashArray = (struct HashType*)malloc(sizeof(struct HashType) * hashSz);
    if (pHashArray == NULL)
    {
        printf("Cannot allocate memory\n");
        exit(-1);
    }
    for (i = 0; i < hashSz; ++i)
    {
        pHashArray[i].head = NULL;
    }

    // Insert records into hash table
    for (i = 0; i < recordSz; ++i)
    {
        insertRecord(pHashArray, hashSz, pRecords[i]);
    }

    // Display records in the hash table
    displayRecordsInHash(pHashArray, hashSz);

    // Free memory
    for (i = 0; i < hashSz; ++i)
    {
        struct Node* temp = pHashArray[i].head;
        while (temp != NULL)
        {
            struct Node* prev = temp;
            temp = temp->next;
            free(prev);
        }
    }
    free(pRecords);
    free(pHashArray);

    return 0;
}
