#include "unity.h"
#include "linked_list.h"

#include <stdio.h>

// I rather dislike keeping line numbers updated, so I made my own
// macro to ditch the line number
#define MY_RUN_TEST(func) RUN_TEST(func, 0)

void setUp(void)
{
    // This is run before EACH test
}

void tearDown(void)
{
    // This is run after EACH test
}

void test_InitializeList_Correct(void)
{
    LinkedList* list = InitializeList(sizeof(int));
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_EQUAL_INT(0, DestructList(&list));
    TEST_ASSERT_NULL(list);
}

void test_InitializeList_NegativeSize(void)
{
    LinkedList* list = InitializeList(-1);
    TEST_ASSERT_NULL(list);
}

void test_InitializeList_ZeroSize(void)
{
    LinkedList* list = InitializeList(0);
    TEST_ASSERT_NULL(list);
}

void test_DestructList_Null(void)
{
    TEST_ASSERT_EQUAL_INT(-1, DestructList(NULL));
}

void test_AddToListTail_Correct(void)
{
    LinkedList* list = InitializeList(sizeof(int));
    int data = 1;
    TEST_ASSERT_EQUAL_INT(0, AddToListTail(list, &data));
    TEST_ASSERT_EQUAL_INT(1, *(int*)RetrieveData(list, GetHead(list)));
    TEST_ASSERT_EQUAL_INT(0, DestructList(&list));
    TEST_ASSERT_NULL(list);
}

void test_AddToListTail_NullList(void)
{
    TEST_ASSERT_EQUAL_INT(-1, AddToListTail(NULL, NULL));
}

void test_AddToListTail_NullData(void)
{
    LinkedList* list = InitializeList(sizeof(int));
    TEST_ASSERT_EQUAL_INT(-1, AddToListTail(list, NULL));
    TEST_ASSERT_EQUAL_INT(0, DestructList(&list));
    TEST_ASSERT_NULL(list);
}

void test_AddToListTail_Correct_Multiple(void)
{
    LinkedList* list = InitializeList(sizeof(int));
    int data = 1;
    TEST_ASSERT_EQUAL_INT(0, AddToListTail(list, &data));
    TEST_ASSERT_EQUAL_INT(1, *(int*)RetrieveData(list, GetHead(list)));

    data = 2;
    TEST_ASSERT_EQUAL_INT(0, AddToListTail(list, &data));
    TEST_ASSERT_EQUAL_INT(2, *(int*)RetrieveData(list,GetNext(list)));

    TEST_ASSERT_EQUAL_INT(0, DestructList(&list));
    TEST_ASSERT_NULL(list);
}

void test_RemoveDataFromList_Correct_Single(void)
{
    LinkedList* list = InitializeList(sizeof(int));
    int data = 1;
    TEST_ASSERT_EQUAL_INT(0, AddToListTail(list, &data));
    TEST_ASSERT_EQUAL_INT(1, RemoveDataFromList(list, 0));
    TEST_ASSERT_EQUAL_INT(0, DestructList(&list));
    TEST_ASSERT_NULL(list);
}

void test_RemoveDataFromList_Correct_AtEnd(void)
{
    LinkedList* list = InitializeList(sizeof(int));
    int data = 1;
    TEST_ASSERT_EQUAL_INT(0, AddToListTail(list, &data));
    data = 2;
    TEST_ASSERT_EQUAL_INT(0, AddToListTail(list, &data));
    TEST_ASSERT_EQUAL_INT(1, RemoveDataFromList(list, 1));

    TEST_ASSERT_EQUAL_INT(1, *(int*)RetrieveData(list, GetHead(list)));
    TEST_ASSERT_EQUAL_INT(0, DestructList(&list));
    TEST_ASSERT_NULL(list);
}

void test_RemoveDataFromList_Correct_Middle(void)
{
    LinkedList* list = InitializeList(sizeof(int));
    int data = 1;
    TEST_ASSERT_EQUAL_INT(0, AddToListTail(list, &data));
    data++;
    TEST_ASSERT_EQUAL_INT(0, AddToListTail(list, &data));
    data++;
    TEST_ASSERT_EQUAL_INT(0, AddToListTail(list, &data));
    
    TEST_ASSERT_EQUAL_INT(1, RemoveDataFromList(list, 1));

    TEST_ASSERT_EQUAL_INT(1, *(int*)RetrieveData(list, GetHead(list)));
    // 2 has been removed, so 3 is now at index 1
    TEST_ASSERT_EQUAL_INT(3, *(int*)RetrieveData(list,GetNext(list)));

    TEST_ASSERT_EQUAL_INT(0, DestructList(&list));
    TEST_ASSERT_NULL(list);
}

void test_RemoveDataFromList_NullList(void)
{
    TEST_ASSERT_EQUAL_INT(-1, RemoveDataFromList(NULL, 0));
}

void test_RemoveDataFromList_NegativeIndex(void)
{
    LinkedList* list = InitializeList(sizeof(int));
    int data = 1;
    TEST_ASSERT_EQUAL_INT(0, AddToListTail(list, &data));
    TEST_ASSERT_EQUAL_INT(-1, RemoveDataFromList(list, -1));
    TEST_ASSERT_EQUAL_INT(0, DestructList(&list));
    TEST_ASSERT_NULL(list);
}

int main(int argc, char* argv[])
{
    UnityBegin();
    argc = argc; // To avoid compiler warning
    argv = argv; // To avoid compiler warning
    MY_RUN_TEST(test_InitializeList_Correct);
    MY_RUN_TEST(test_InitializeList_NegativeSize);
    MY_RUN_TEST(test_InitializeList_ZeroSize);

    MY_RUN_TEST(test_DestructList_Null);

    MY_RUN_TEST(test_AddToListTail_Correct);
    MY_RUN_TEST(test_AddToListTail_NullList);
    MY_RUN_TEST(test_AddToListTail_NullData);
    MY_RUN_TEST(test_AddToListTail_Correct_Multiple);

    MY_RUN_TEST(test_RemoveDataFromList_Correct_Single);
    MY_RUN_TEST(test_RemoveDataFromList_Correct_AtEnd);
    MY_RUN_TEST(test_RemoveDataFromList_Correct_Middle);
    MY_RUN_TEST(test_RemoveDataFromList_NullList);
    MY_RUN_TEST(test_RemoveDataFromList_NegativeIndex);

    return UnityEnd();
}
