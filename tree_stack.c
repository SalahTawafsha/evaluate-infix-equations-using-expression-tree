#define MAX_STRING_LENGTH 10 // length of max int = 2147483647
#define MAX_TOKENS_COUNT 30

typedef struct TreeNode {
    char string[MAX_STRING_LENGTH];
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

TreeNode *createTreeNode(char string[]) {
    struct TreeNode *treeNode = (TreeNode *) malloc(sizeof(TreeNode));
    strcpy(treeNode->string, string);
    treeNode->left = NULL;
    treeNode->right = NULL;

    return treeNode;
}

typedef struct TreeStack {
    TreeNode *data[MAX_TOKENS_COUNT];
    int curr;
} TreeStack;


TreeStack *createTreeStack() {
    TreeStack *stack = (TreeStack *) malloc(sizeof(TreeStack));
    stack->curr = -1;
    return stack;
}

void pushTree(TreeStack *stack, TreeNode *treeNode) {
    stack->data[++stack->curr] = treeNode;
}

TreeNode *popTree(TreeStack *stack) {
    if (stack->curr == -1)
        return NULL;

    TreeNode *treeNode = stack->data[stack->curr];
    stack->curr--;
    return treeNode;
}