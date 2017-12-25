typedef struct bHeap{
    int order;
    struct bHeap *child;
    struct bHeap *parent;
    struct bHeap *sibbling;
    int number;
}BinomialHeap;

typedef struct heapList{
    BinomialHeap *content;
    struct heapList *next;
    struct heapList **last;
}HeapList;

typedef struct int_ptr_ptr_tuple{
    int value;
    HeapList *smallest;
    HeapList *front;
}IntPtrPtr_Tuple;

_Bool isEmpty(HeapList *input);

HeapList *createHeapList();

void addHeapToList(HeapList *current, BinomialHeap *newHeap);

void addNode(HeapList *current, int newElement);

HeapList *buildHeap(int*input, int length);

void addList(HeapList* current, HeapList* newElem);