#define MAX_STRING_LENGTH 10 // length of max int = 2147483647
#define MAX_TOKENS_COUNT 30

typedef struct StringStack {
    char strings[MAX_TOKENS_COUNT][MAX_STRING_LENGTH];
    int curr;
} StringStack;

StringStack *createStringStack() {
    StringStack *stack = (StringStack *) malloc(sizeof(StringStack));
    stack->curr = -1;
    return stack;
}

void pushString(StringStack *stack, char string[]) {
    strcpy(stack->strings[++stack->curr], string);
}

char *popString(StringStack *stack) {
    if (stack->curr == -1)
        return "empty stack";

    char *string = malloc(sizeof(char) * MAX_STRING_LENGTH);
    strcpy(string, stack->strings[stack->curr]);
    stack->curr--;
    return string;
}

char *peekString(StringStack *stack) {
    if (stack->curr == -1)
        return "empty stack";
    char *string = malloc(sizeof(char) * MAX_STRING_LENGTH);
    strcpy(string, stack->strings[stack->curr]);
    return string;
}

int isStringStackEmpty(StringStack *stack) {
    return stack->curr == -1;
}
