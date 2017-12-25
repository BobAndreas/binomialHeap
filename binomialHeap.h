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

_Bool isEmpty(HeapList *input);

HeapList *createHeapList();

void addHeapToList(HeapList *current, BinomialHeap *newHeap);

void addNode(HeapList *current, int newElement);

HeapList *buildHeap(int*input, int length);

void addListElem(HeapList* current, HeapList* newElem);