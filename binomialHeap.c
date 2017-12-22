#define true 1
#define false 0


#include <stdio.h>
#include <stdlib.h>
#include "binomialHeap.h"

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

//adds a new Heap to the List on top of the binomial Heap
void addHeapToList(HeapList *current, BinomialHeap *newHeap){
    if(!isEmpty(current)){
        HeapList *newListElement = (HeapList*)malloc(sizeof(HeapList));
        newListElement->content = newHeap;
        newListElement->last = current->last;
        newListElement->next = NULL;
        (*current->last)->next = newListElement;
        *current->last = newListElement;
    }
    else{
        current->content = newHeap;
    }
    
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
    correctHeapList(current); 
}
//builds a new binomial heap from an array of numbers
HeapList *buildHeap(int*input, int length){
    HeapList *newHeapList = createHeapList(); 
    for(int i = 0; i<length; i++)
        addNode(newHeapList, input[i]);    
    return newHeapList;
}

//creates an array of random numbers in the requested size;
int *createArray(int size){
    int *a = malloc(size);
    for(int i = 0; i<size; i++)
        a[i] = rand()%1000;
    return a;
}


int main(){
    srand(1000);
    while(true){
        uint control;
        scanf("%u", &control);
        if(control){
            int *arrayInput = createArray(control);
            HeapList *heap = buildHeap(arrayInput, control);
            //task
            

            //end task;
            free(arrayInput);
        }
        else break;

    }
    return 0;
}
