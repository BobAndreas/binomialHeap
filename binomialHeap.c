#define true 1
#define false 0
#define INT_MAX 2147483647


#include <stdio.h>
#include <stdlib.h>
#include "binomialHeap.h"
#include <math.h>

#pragma region helper
_Bool isEmpty(HeapList *input){
    return input->content == NULL
        && input->next == NULL;
}

_Bool EOL(HeapList *input){
    return input->next == NULL;
}


//creates an array of random numbers in the requested size;
int *createArray(int size){
    int *a = malloc(size);
    for(int i = 0; i<size; i++)
        a[i] = rand()%1000;
    return a;
}

//builds a new binomial heap from an array of numbers
HeapList *buildHeap(int*input, int length){
    HeapList *newHeapList = createHeapList(); 
    for(int i = 0; i<length; i++)
        addNode(newHeapList, input[i]);    
    return newHeapList;
}

#pragma region visualisation

void printGrid(int **rows, int width, int depth){
    for(int i = 0; i<depth; i++){
        for(int j = 0; j<width; j++){
            if(rows[i][j]>-1)printf("%4d", rows[i][j]);
            else printf("    ");
        }
        printf("\n");
    }
}

int heapWidth(BinomialHeap *heap){
    return(int)(pow(2.0, heap->order-1)+0.8);//hack for 0 => 1, 1 => 1, 2=>2, 3=>4 ... 
}

void fillArrayStructure(int **rows, BinomialHeap *curHeap, int level, int deviation){
    rows[level][deviation] = curHeap->number;
    if(curHeap->sibbling != NULL)fillArrayStructure(rows, curHeap->sibbling, level, deviation + heapWidth(curHeap));
    if(curHeap->child != NULL)fillArrayStructure(rows, curHeap->child, level +1, deviation);
}


void printHeap(HeapList *heap){
   
    HeapList *iterator = heap;
    int width = 0;
    while(iterator != NULL){
        width += heapWidth(iterator->content)+1;
        iterator = iterator->next;
    }
    int depth = heap->content->order +1;
    int **rows = malloc(sizeof(int*)*depth);
    for(int i = 0; i<depth; i++){
        rows[i] = malloc(sizeof(int)*width);
        
        for(int j = 0; j<width; j++){
            rows[i][j] = -1;
        }
    }
    iterator = heap;
    int shift=0;
    while(iterator != NULL){
        fillArrayStructure(rows, iterator->content, 0, shift);
        shift += heapWidth(iterator->content)+1; 
        iterator = iterator->next;
    }
    printGrid(rows, width, depth);
    
    for(int i = 0; i<depth; i++)
        free(rows[i]);
    free(rows);
}
#pragma endregion visualisation

#pragma endregion helper

#pragma region memory management
void freeList(HeapList *list){
    if(!EOL(list))
        freeList(list);
    free(list);
}


void deconstructList(HeapList *list, _Bool includingHeap){
    if(list->next != NULL)
        freeList(list->next);
    else
        free(list->last);
    if(includingHeap)free(list->content);
    free(list);
}
#pragma endregion memory management

//creates a new heaplist;
HeapList *createHeapList(){
    HeapList *newList = (HeapList*)malloc(sizeof(HeapList));
    newList->next = NULL;
    newList->content = NULL;
    newList->last = (HeapList**)malloc(sizeof(HeapList*));
    *newList->last = newList;
    return newList;
}

//adds a new Heap to the List on top of the binomial Heap
void addHeapToList(HeapList *current, BinomialHeap *newHeap){
    HeapList *newListElement = (HeapList*)malloc(sizeof(HeapList));
    newListElement->content = newHeap;
    newHeap->sibbling = NULL;
    newHeap->parent = NULL;
    addList(current, newListElement); 
}
//adds an heap to the end of the list of childs of an bigger heap
void addAsSibbling(BinomialHeap *child, BinomialHeap *newChild){
    if(child->sibbling == NULL)
        child->sibbling = newChild;
    else
        addAsSibbling(child->sibbling, newChild);
}
//correct the heaplist after appending a new element to restore the binomial heap;
void correctHeapList(HeapList *list){
    if(!EOL(list)){
        correctHeapList(list->next);
        if(list->content->order == list->next->content->order){
            BinomialHeap *smaller;
            BinomialHeap *bigger;
            if(list->content->number <= list->next->content->number){
                smaller = list->content;
                bigger = list->next->content;
            }
            else{
                smaller = list->next->content;
                bigger = list->content;
            }
            smaller->order++;
            list->content = smaller;
            if(smaller->child != NULL)
                addAsSibbling(smaller->child, bigger);
            else
                smaller->child = bigger;
            HeapList *cache = list->next;
            list->next = list->next->next;
            free(cache);
            if(list->next == NULL)*list->last = list;
        }
        else if(list->content->order < list->next->content->order){
            BinomialHeap *cacheHeap = list->content;
            list->content = list->next->content;
            list->next->content = cacheHeap;
        }
    }

}
//creates a new heap and appends it to the root list
void addNode(HeapList *current, int newElement){
    BinomialHeap *newHeap = malloc(sizeof(BinomialHeap));
    newHeap->child = NULL;
    newHeap->parent = NULL;
    newHeap->sibbling = NULL;
    newHeap->order = 0;
    newHeap->number = newElement;
    addHeapToList(current, newHeap);
}
//adds a new list element with an heap to an existing root list and restores heap property
void addList(HeapList* current, HeapList* newElem){
    if(!isEmpty(current)){
        
        newElem->last = current->last;
        newElem->next = NULL;
        (*current->last)->next = newElem;
        *current->last = newElem;
        correctHeapList(current); 
    }
    else{
        current->content = newElem->content;
        free(newElem);
    }
}

//merge two binomial heap root lists;
HeapList *Union(HeapList *list1, HeapList *list2){
    if(isEmpty(list1)){
        deconstructList(list1, false);
        return list2;
    }
    if(isEmpty(list2)){
        deconstructList(list2, false);
        return list1;
    }
    free(list2->last);
    while(list2 != NULL){
        HeapList *cache = list2;
        list2 = list2->next;
        addList(list1, cache);
    }
    
    return list1;
}

IntPtrPtr_Tuple find_min(HeapList *list){
    if(list == NULL)return (IntPtrPtr_Tuple){.value = INT_MAX, .smallest = NULL, .front = NULL };
    IntPtrPtr_Tuple prevMin = find_min(list->next);
    if(list->content->number < prevMin.value)
        return (IntPtrPtr_Tuple){.value = list->content->number, .smallest = list, .front = NULL};
    else
        if(prevMin.smallest == list->next)prevMin.front = list;
    return prevMin;
}
void fixHeapCascading(BinomialHeap *target){
    if(target->parent != NULL){
        if(target->number < target->parent->number){
            int cache = target->parent->number;
            target->parent->number = target->number;
            target->number = cache;
            fixHeapCascading(target->parent);
        }
    }
}

void decreaseKey(BinomialHeap *target, int value){
    target->number = value;
    fixHeapCascading(target);
}

void removeRootAndFix(HeapList *list, HeapList *target, HeapList *front){
    HeapList *newList = createHeapList();
    BinomialHeap *iterator = target->content->child,
        *next;
    while(iterator != NULL){
        next = iterator->sibbling;
        addHeapToList(newList, iterator);
        iterator = next;
    }
    front->next = target->next;
    if(front->next == NULL)*(list->last)=front;
    Union(list, newList);
}


int extract_min(HeapList *list){
    IntPtrPtr_Tuple min = find_min(list);
    removeRootAndFix(list, min.smallest, min.front);
    return min.value;
}

int main(){
    srand(1000);
    while(true){
        unsigned int control;
        scanf("%u", &control);
        if(control){
            int arrayInput1[17] = {65, 12, 33, 11, 77, 31, 80, 37, 19, 22, 5, 16, 30, 22, 26, 60, 62};
            int arrayInput2[11] = {10, 5, 18, 3, 19, 6, 15, 2, 14, 4, 1};
            HeapList *heap1 = buildHeap(arrayInput1, 17);
            printf("Heap 1\n");
            printHeap(heap1);
            printf("\n\nHeap2\n");
            HeapList *heap2 = buildHeap(arrayInput2, 11);
            printHeap(heap2);
            printf("\n\nHeap 3\n");
            HeapList *heap3 = Union(heap1, heap2);
            printHeap(heap3);
            printf("\nextract minimum of heap 3\n");
            extract_min(heap3);
            printHeap(heap3);
            printf("\nextract minimum of heap 3 again\n");
            extract_min(heap3);
            printHeap(heap3);

            printf("\n");
            int a = 2;
            //task
            

            //end task;
            
            
        }
        else break;

    }
    return 0;
}