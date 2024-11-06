#define NULL ((void *)0)

typedef struct SetNode{
    int i;
    struct SetNode *next;
} SetNode;

typedef struct {
    SetNode *root;
    int size;
} Set;

extern Set* createRoot();
extern void createNode(int i, Set *set);
extern void deleteSet(Set *set);
extern SetNode* getNodeAtPosition(Set *set, int i);