#define true 1
#define false 0


#include <stdio.h>
#include <stdlib.h>
#include "binomialHeap.h"
#include <math.h>
_Bool isEmpty(HeapList *input){
    return input->content == NULL
        && input->next == NULL;
}

_Bool EOL(HeapList *input){
    return input->next == NULL;
}

//creates a new heaplist;
HeapList *createHeapList(){
    HeapList *newList = (HeapList*)malloc(sizeof(HeapList));
    newList->next = NULL;
    newList->content = NULL;
    newList->last = (HeapList**)malloc(sizeof(HeapList*));
    *newList->last = newList;
    return newList;
}
void deconstructList(HeapList *list){
    free(list->last);
    free(list);
}
//adds a new Heap to the List on top of the binomial Heap
void addHeapToList(HeapList *current, BinomialHeap *newHeap){
    HeapList *newListElement = (HeapList*)malloc(sizeof(HeapList));
    newListElement->content = newHeap;
    addListElem(current, newListElement); 
}

int readSmallest(BinomialHeap *heap){
    if(heap->order)
        return readSmallest(heap->left);
    return heap->number;
}

//correct the heaplist after appending a new element to restore the binomial heap;
void correctHeapList(HeapList *list){
    if(!EOL(list)){
        correctHeapList(list->next);
        if(list->content->order == list->next->content->order){
            BinomialHeap *newHeap = malloc(sizeof(BinomialHeap));
            if(readSmallest(list->content) <= readSmallest(list->next->content)){
                newHeap->left = list->content;
                newHeap->right = list->next->content;
            }
            else{
                newHeap->left = list->next->content;
                newHeap->right = list->content;
            }
            newHeap->order = list->content->order + 1;
            list->content = newHeap;
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
//adds a new node in the binomial heap for the added number
void addNode(HeapList *current, int newElement){
    BinomialHeap *newHeap = malloc(sizeof(BinomialHeap));
    newHeap->left = NULL;
    newHeap->right = NULL;
    newHeap->order = 0;
    newHeap->number = newElement;
    addHeapToList(current, newHeap);
}
//builds a new binomial heap from an array of numbers
HeapList *buildHeap(int*input, int length){
    HeapList *newHeapList = createHeapList(); 
    for(int i = 0; i<length; i++)
        addNode(newHeapList, input[i]);    
    return newHeapList;
}

void addListElem(HeapList* current, HeapList* newElem){
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

//merge two binomial heap lists;
HeapList *Union(HeapList *list1, HeapList *list2){
    if(isEmpty(list1)){
        deconstructList(list1);
        return list2;
    }
    if(isEmpty(list2)){
        deconstructList(list2);
        return list1;
    }
    free(list2->last);
    while(list2 != NULL){
        HeapList *cache = list2;
        list2 = list2->next;
        addListElem(list1, cache);
    }
    
    return list1;
}



//creates an array of random numbers in the requested size;
int *createArray(int size){
    int *a = malloc(size);
    for(int i = 0; i<size; i++)
        a[i] = rand()%1000;
    return a;
}
void fillArrayStructure(int **rows, BinomialHeap *curHeap, int rightTurns, int deviation, int shift){
    if(curHeap->order == 0){
        rows[rightTurns][deviation+shift] = curHeap->number;
    }
    else{
        fillArrayStructure(rows, curHeap->left, rightTurns, deviation*2, shift);
        fillArrayStructure(rows, curHeap->right, rightTurns+1, deviation*2+1, shift);
    }
}

void printHeap(HeapList *heap){
    double a = 3.0;
    pow(2.0, a);
    HeapList *iterator = heap;
    int width = 0;
    double intermediate;
    while(iterator != NULL){
        intermediate = iterator->content->order-1;
        width += (int)(pow(2.0, intermediate)+0.8); //hack for 0 => 1, 1 => 1, 2=>2, 3=>4 ... 
        iterator = iterator->next;
    }
    int depth = heap->content->order +1;
    int **rows = malloc(sizeof(int*)*depth);
    for(int i = 0; i<depth; i++){
        rows[i] = malloc(sizeof(int)*width);
    }
    for(int i = 0; i<depth; i++){
        for(int j = 0; j<width; j++){
            rows[i][j] = -1;
        }
    }
    //do print
    iterator = heap;
    int shift=0;
    while(iterator != NULL){
        intermediate = iterator->content->order-1;
        fillArrayStructure(rows, iterator->content, 0, 0, shift);
        shift += (int)(pow(2.0, intermediate)+0.8); //hack for 0 => 1, 1 => 1, 2=>2, 3=>4 ... 
        iterator = iterator->next;
    }
    //fill array structure

    for(int i = 0; i<depth; i++){
        for(int j = 0; j<width; j++){
            if(rows[i][j]>-1)printf("%4d", rows[i][j]);
            else printf("    ");
        }
        printf("\n");
    }
    //end print
    /*
    for(int i = 0; i<depth; i++)
        free(rows[i]);
    free(rows);
    */
}

int main(){
    srand(1000);
    while(true){
        uint control;
        scanf("%u", &control);
        if(control){
            int arrayInput1[17] = {65, 12, 33, 11, 77, 31, 80, 37, 19, 22, 5, 16, 30, 22, 26, 60, 62};
            int arrayInput2[11] = {10, 5, 18, 3, 19, 6, 15, 2, 14, 4, 1};
            HeapList *heap1 = buildHeap(arrayInput1, 17);
            HeapList *heap2 = buildHeap(arrayInput2, 11);
            HeapList *heap3 = Union(heap1, heap2);
            printHeap(heap3);
            int a = 2;
            //task
            

            //end task;
            
            
        }
        else break;

    }
    return 0;
}
