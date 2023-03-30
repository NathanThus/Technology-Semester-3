#ifndef LINKED_LIST_H
#define LINKED_LIST_H
typedef struct linkedList LinkedList;

// Returns a pointer to a new linked list
// Returns NULL if objectSize is less than 1
LinkedList* InitializeList(int objectSize);

// Returns a 0 if the list is sucessfully destroyed
// Returns a -1 if the list is NULL
int DestructList(LinkedList** list);

// Returns a 0 if the data is sucessfully added to the list
// Returns a -1 if the data is NULL or the list is NULL
int AddToListTail(LinkedList* list, void* data);

// Returns a 0 if the data is sucessfully added to the list
// Returns a -1 if the data is NULL or the list is NULL
int AddToHead(LinkedList* list, void* data);

// Returns a 0 if the data is succesfully added to the list.
// Returns a -1 if either list / data are NULL, or index is out of range.
int InsertAtIndex(LinkedList* list, void* data, int index);

// Returns a 0 if the data is sucessfully removed from the list
// Returns a -1 if the data is NULL or the list is NULL
int RemoveDataFromList(LinkedList* list, int index);

// Returns a 0 if the list is cleared
// Returns a -1 if the list is NULL
int ClearList(LinkedList* list);

// Returns a pointer to the data at the head of the list
// Returns NULL if the list is NULL or empty
void* GetHead(LinkedList* list);

// Returns a pointer to the next element in the list
// Returns NULL if the list is NULL or empty
void* GetNext(LinkedList* list);

// Returns a pointer to the data
// Returns NULL if the list is NULL or empty
void* RetrieveData(LinkedList* list, void* data);

#endif
