typedef struct bHeap{
    int order;
    struct bHeap *left;
    struct bHeap *right;
    int number;
}BinomialHeap;

typedef struct heapList{
    BinomialHeap *content;
    struct heapList *next;
    struct heapList **last;
}HeapList;

_Bool emptyList(HeapList *input);

HeapList *createHeapList();

void addHeapToList(HeapList *current, BinomialHeap *newHeap);

void addNode(HeapList *current, int newElement);

HeapList *buildHeap(int*input, int length);