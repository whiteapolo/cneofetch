#define STACK_IMPL
#include "../stack.h"

#define GRAPH_IMPL
#include "../graph.h"

#define STRING_IMPL
#include "../string.h"

#define PATH_IMPL
#include "../path.h"

#define CURSOR_IMPL
#include "../cursor.h"

#define MAP_IMPL
#include "../map.h"

#define LIST_IMPL
#include "../list.h"

#define QUEUE_IMPL
#include "../queue.h"

#define PRIORITYQUEUE_IMPL
#include "../priorityQueue.h"

#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define RESET "\033[0m"

int cmpInt(const void *a, const void *b)
{
    int intA = *(int *)a;
    int intB = *(int *)b;
    return intA - intB;
}

void printInt(void *data)
{
    printf("%d ", *(int *)data);
}

void testPriorityQueue()
{
    priorityQueue q = newPriorityQueue(cmpInt);

    if (priorityQueueIsEmpty(&q))
        printf(GREEN "Test Passed: Queue is initially empty.\n" RESET);
    else
        printf(RED "Test Failed: Queue should be initially empty.\n" RESET);

    int values[] = {5, 3, 10, 1, 4};
    for (int i = 0; i < 5; i++)
        priorityQueuePush(&q, &values[i]);

    int size = priorityQueueGetSize(&q);
    if (size == 5)
        printf(GREEN "Test Passed: Queue size is correct after push operations.\n" RESET);
    else
        printf(RED "Test Failed: Queue size is incorrect. Expected 5, got %d.\n" RESET, size);

    int *peekValue = (int *)priorityQueuePeek(&q);
    if (*peekValue == 10)
        printf(GREEN "Test Passed: Peek returns the correct highest priority element.\n" RESET);
    else
        printf(RED "Test Failed: Peek returned incorrect value. Expected 10, got %d.\n" RESET, *peekValue);

    int expectedOrder[] = {10, 5, 4, 3, 1};
    bool orderCorrect = true;
    for (int i = 0; i < 5; i++) {
        int *poppedValue = (int *)priorityQueuePop(&q);
        if (*poppedValue != expectedOrder[i]) {
            printf(RED "Test Failed: Expected %d but got %d.\n" RESET, expectedOrder[i], *poppedValue);
            orderCorrect = false;
            break;
        }
    }
    if (orderCorrect)
        printf(GREEN "Test Passed: Elements popped in correct priority order.\n" RESET);

    if (priorityQueueIsEmpty(&q))
        printf(GREEN "Test Passed: Queue is empty after popping all elements.\n" RESET);
    else
        printf(RED "Test Failed: Queue should be empty.\n" RESET);

    priorityQueueFree(&q, NULL);
    printf("Test Completed: Queue memory freed.\n");
}

void testStack()
{
    stack s = newStack();

    if (stackIsEmpty(&s))
        printf(GREEN "Test Passed: Stack is initially empty.\n" RESET);
    else
        printf(RED "Test Failed: Stack should be initially empty.\n" RESET);

    int values[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++)
        stackPush(&s, &values[i]);

    int *topValue = (int *)stackTop(&s);
    if (*topValue == 50)
        printf(GREEN "Test Passed: Top returns the correct last pushed element.\n" RESET);
    else
        printf(RED "Test Failed: Top returned incorrect value. Expected 50, got %d.\n" RESET, *topValue);

    int expectedOrder[] = {50, 40, 30, 20, 10};
    bool orderCorrect = true;
    for (int i = 0; i < 5; i++) {
        int *poppedValue = (int *)stackPop(&s);
        if (*poppedValue != expectedOrder[i]) {
            printf(RED "Test Failed: Expected %d but got %d.\n" RESET, expectedOrder[i], *poppedValue);
            orderCorrect = false;
            break;
        }
    }
    if (orderCorrect)
        printf(GREEN "Test Passed: Elements popped in correct LIFO order.\n" RESET);

    if (stackIsEmpty(&s))
        printf(GREEN "Test Passed: Stack is empty after popping all elements.\n" RESET);
    else
        printf(RED "Test Failed: Stack should be empty.\n" RESET);

    stackFree(&s, NULL);
    printf("Test Completed: Stack memory freed.\n");
}

void testQueue()
{
    queue q = newQueue();

    if (queueIsEmpty(&q))
        printf(GREEN "Test Passed: Queue is initially empty.\n" RESET);
    else
        printf(RED "Test Failed: Queue should be initially empty.\n" RESET);

    int values[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++)
        enqueue(&q, &values[i]);

    printf("Queue contents after enqueuing: ");
    queuePrint(&q, printInt);
    printf("\n");

    int expectedOrder[] = {10, 20, 30, 40, 50};
    bool orderCorrect = true;
    for (int i = 0; i < 5; i++) {
        int *dequeuedValue = (int *)dequeue(&q);
        if (*dequeuedValue != expectedOrder[i]) {
            printf(RED "Test Failed: Expected %d but got %d.\n" RESET, expectedOrder[i], *dequeuedValue);
            orderCorrect = false;
            break;
        }
    }
    if (orderCorrect)
        printf(GREEN "Test Passed: Elements dequeued in correct FIFO order.\n" RESET);

    if (queueIsEmpty(&q))
        printf(GREEN "Test Passed: Queue is empty after dequeuing all elements.\n" RESET);
    else
        printf(RED "Test Failed: Queue should be empty.\n" RESET);

    queueFree(&q, NULL);
    printf("Test Completed: Queue memory freed.\n");
}

void printMapEntry(const void *key, const void *data, void *arg)
{
    printf("Key: %d, Data: %d\n", *(int *)key, *(int *)data);
}

void testMap()
{
    map m = newMap(cmpInt);

    if (mapIsEmpty(&m))
        printf(GREEN "Test Passed: Map is initially empty.\n" RESET);
    else
        printf(RED "Test Failed: Map should be initially empty.\n" RESET);

    int key1 = 1, data1 = 100;
    mapInsert(&m, &key1, &data1);
    if (mapIsExists(&m, &key1))
        printf(GREEN "Test Passed: Key 1 exists after insertion.\n" RESET);
    else
        printf(RED "Test Failed: Key 1 should exist after insertion.\n" RESET);

    int *foundData = (int *)mapFind(&m, &key1);
    if (foundData && *foundData == data1)
        printf(GREEN "Test Passed: Correct value found for key 1.\n" RESET);
    else
        printf(RED "Test Failed: Incorrect value found for key 1. Expected %d, got %d.\n" RESET, data1, foundData ? *foundData : -1);

    int key2 = 2, data2 = 200;
    int key3 = 3, data3 = 300;
    mapInsert(&m, &key2, &data2);
    mapInsert(&m, &key3, &data3);

    if (mapIsExists(&m, &key2) && mapIsExists(&m, &key3))
        printf(GREEN "Test Passed: Keys 2 and 3 exist after insertion.\n" RESET);
    else
        printf(RED "Test Failed: Keys 2 and/or 3 should exist after insertion.\n" RESET);

    int newData2 = 250;
    mapUpdate(m, &key2, NULL, &newData2);
    foundData = (int *)mapFind(&m, &key2);
    if (foundData && *foundData == newData2)
        printf(GREEN "Test Passed: Key 2 updated successfully.\n" RESET);
    else
        printf(RED "Test Failed: Key 2 update failed. Expected %d, got %d.\n" RESET, newData2, foundData ? *foundData : -1);

    mapRemove(&m, &key1, NULL, NULL);
    if (!mapIsExists(&m, &key1))
        printf(GREEN "Test Passed: Key 1 removed successfully.\n" RESET);
    else
        printf(RED "Test Failed: Key 1 should be removed.\n" RESET);

    printf("Map contents after operations:\n");
    mapOrderTraverse(&m, printMapEntry, NULL);

    mapFree(&m, NULL, NULL);
    printf("Test Completed: Map memory freed.\n");
}

void test_newStrSlice()
{
	string s = newStr("123456");
	string slice = newStrSlice(s, -1, 0, -1);
	assert(strIsEqualC(slice, "654321"));
	strFree(&slice);

	slice = newStrSlice(s, 0, s.len - 1, 1);
	assert(strIsEqualC(slice, "123456"));
	strFree(&slice);

	slice = newStrSlice(s, 1, 4, 1);
	assert(strIsEqualC(slice, "2345"));
	strFree(&slice);

	slice = newStrSlice(s, 0, s.len - 1, 2);
	assert(strIsEqualC(slice, "135"));
	strFree(&slice);

	slice = newStrSlice(s, 10, 12, 1);
	assert(strIsEqualC(slice, ""));
	strFree(&slice);

	slice = newStrSlice(s, -4, -2, 1);
	assert(strIsEqualC(slice, "345"));
	strFree(&slice);

	slice = newStrSlice(s, 3, 3, 1);
	assert(strIsEqualC(slice, "4"));
	strFree(&slice);

	slice = newStrSlice(s, -1, -6, -1);
	assert(strIsEqualC(slice, "654321"));
	strFree(&slice);

	strFree(&s);
	printf("All tests passed.\n");
}

int main()
{
	test_newStrSlice();
	testPriorityQueue();
	testStack();
	testQueue();
	testMap();
	return 0;
}
