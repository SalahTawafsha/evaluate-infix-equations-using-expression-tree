#include <stdio.h>
#include <strings.h>
#include <malloc.h>
#include <stdbool.h>
#include "string_stack.c"
#include "tree_stack.c"

#define MAX_LINES_IN_FILE 100
#define MAX_EXPRESSION_LENGTH 30

char *charToString(char ch) {
    char *str = malloc(sizeof(char) * 2);
    str[0] = ch;
    str[1] = '\0';
    return str;
}

int isLessPriority(char ch) {
    return ch == '+' || ch == '-';
}

char *toPostfix(char infix[]) {
    StringStack *stack = createStringStack();


    unsigned long length = strlen(infix);
    char *postfix = malloc(sizeof(char) * MAX_EXPRESSION_LENGTH);

    int currPostfixIndex = 0;
    for (int i = 0; i < length; ++i) {
        switch (infix[i]) {
            case '+':
            case '-':
                postfix[currPostfixIndex++] = ' ';
                while (!isStringStackEmpty(stack) && peekString(stack)[0] != '(') {
                    if (postfix[currPostfixIndex - 1] != ' ')
                        postfix[currPostfixIndex++] = ' ';

                    postfix[currPostfixIndex++] = popString(stack)[0];
                }

                pushString(stack, charToString(infix[i]));
                break;
            case '*':
            case '/':
            case '%':
                postfix[currPostfixIndex++] = ' ';
                while (!isStringStackEmpty(stack) && !isLessPriority(peekString(stack)[0]) &&
                       peekString(stack)[0] != '('){
                    if (postfix[currPostfixIndex - 1] != ' ')
                        postfix[currPostfixIndex++] = ' ';

                    postfix[currPostfixIndex++] = popString(stack)[0];
                }

                pushString(stack, charToString(infix[i]));
                break;
            case '(':
                pushString(stack, charToString(infix[i]));
                break;
            case ')':
                postfix[currPostfixIndex++] = ' ';
                while (!isStringStackEmpty(stack) && peekString(stack)[0] != '('){
                    if (postfix[currPostfixIndex - 1] != ' ')
                        postfix[currPostfixIndex++] = ' ';

                    postfix[currPostfixIndex++] = popString(stack)[0];
                }

                popString(stack);
                break;
            default:
                if (postfix[currPostfixIndex - 1] != ' ' && '0' > postfix[currPostfixIndex - 1] ||
                    '9' < postfix[currPostfixIndex - 1])
                    postfix[currPostfixIndex++] = ' ';

                postfix[currPostfixIndex++] = infix[i];

        }
    }

    while (!isStringStackEmpty(stack)) {
        postfix[currPostfixIndex++] = ' ';
        postfix[currPostfixIndex++] = popString(stack)[0];
    }


    postfix[currPostfixIndex] = '\0';

    return postfix;
}

char **readFile(const char *filename) {
    char **infixEquations = (char **) malloc(sizeof(char *) * MAX_LINES_IN_FILE);
    for (int i = 0; i < MAX_LINES_IN_FILE; ++i) {
        infixEquations[i] = (char *) malloc(MAX_EXPRESSION_LENGTH);
        strcpy(infixEquations[i], "");
    }

    FILE *in = fopen(filename, "r");
    for (int i = 0; fgets(infixEquations[i], MAX_EXPRESSION_LENGTH, in) != NULL; i++) {
        unsigned long length = strlen(infixEquations[i]);
        infixEquations[i][length - 1] = '\0';
    }

    printf("File read successful\n\n");
    return infixEquations;
}

char **generatePostfix(char **infixEquations) {
    char **postfixEquations = (char **) malloc(sizeof(char *) * MAX_LINES_IN_FILE);
    for (int i = 0; i < MAX_LINES_IN_FILE; ++i) {
        postfixEquations[i] = (char *) malloc(MAX_EXPRESSION_LENGTH);
        strcpy(postfixEquations[i], "");
    }

    for (int i = 0; strlen(infixEquations[i]) != 0; ++i)
        postfixEquations[i] = toPostfix(infixEquations[i]);


    return postfixEquations;
}

void printEquations(char **equations) {
    for (int i = 0; strlen(equations[i]) != 0; ++i)
        printf("%s\n", equations[i]);
    printf("\n");
}

bool isOperator(const char *token) {
    return token[0] == '+' || token[0] == '-' ||
           token[0] == '*' || token[0] == '/' ||
           token[0] == '%';
}

TreeNode *generateEvaluationTree(char *postfix) {
    TreeStack *stack = createTreeStack();
    char postfixCopy[MAX_EXPRESSION_LENGTH];
    strcpy(postfixCopy, postfix);

    char *token = strtok(postfixCopy, " ");
    while (token != NULL) {
        if (!isOperator(token))
            pushTree(stack, createTreeNode(token));
        else {
            TreeNode *rightOperand = popTree(stack);
            TreeNode *leftOperand = popTree(stack);

            TreeNode *operatorNode = createTreeNode(token);
            operatorNode->left = leftOperand;
            operatorNode->right = rightOperand;
            pushTree(stack, operatorNode);
        }
        token = strtok(NULL, " ");
    }

    return popTree(stack);
}

int evaluateEquationTree(TreeNode *equationTree) {
    int left, right;
    if (equationTree->left == NULL && equationTree->right == NULL)
        return atoi(equationTree->string);

    left = evaluateEquationTree(equationTree->left);
    right = evaluateEquationTree(equationTree->right);

    switch (equationTree->string[0]) {
        case '+':
            return left + right;
        case '-':
            return left - right;
        case '*':
            return left * right;
        case '/':
            if (right == 0) {
                fprintf(stderr, "Can't divide by zero !!");
                exit(0);
            }
            return left / right;
        case '%':
            if (right == 0) {
                fprintf(stderr, "Can't take mod of zero !!");
                exit(1);
            }

            return left % right;

    }

    return 0;
}


int *generateAndEvaluateEquationTree(char **postfixEquations) {
    int *results = malloc(sizeof(int) * MAX_LINES_IN_FILE);
    for (int i = 0; strlen(postfixEquations[i]) != 0; ++i) {
        TreeNode *equationTree = generateEvaluationTree(postfixEquations[i]);
        results[i] = evaluateEquationTree(equationTree);
    }
    return results;
}


int main() {
    char **infixEquations = NULL;
    char **postfixEquations = NULL;
    int *results = NULL;

    char choice;
    while (1) {
        printf("1. Read equations\n");
        printf("2. Print equations\n");
        printf("3. Print postfix expressions\n");
        printf("4. Evaluate using Expression tree\n");
        printf("5. Save to output file (postfix and results)\n");
        printf("6. Exit\n");
        printf("Select choice:");
        scanf(" %c", &choice);
        switch (choice) {
            case '1':
                infixEquations = readFile("input.txt");
                postfixEquations = generatePostfix(infixEquations);

                break;
            case '2':
                if (infixEquations == NULL) {
                    printf("You must read file first !!\n\n");
                    break;
                }
                printEquations(infixEquations);
                break;
            case '3':
                if (postfixEquations == NULL) {
                    printf("You must read file first !!\n\n");
                    break;
                }

                printEquations(postfixEquations);
                break;
            case '4':
                if (postfixEquations == NULL) {
                    printf("You must read file first !!\n\n");
                    break;
                }

                results = generateAndEvaluateEquationTree(postfixEquations);
                for (int i = 0; strlen(infixEquations[i]) != 0; ++i)
                    printf("%s = %d\n", infixEquations[i], results[i]);
                printf("\n");
                break;

            case '5':
                if (results == NULL) {
                    printf("You must evaluate Expression tree first !!\n\n");
                    break;
                }

                FILE *out = fopen("output.txt", "w");
                for (int i = 0; strlen(postfixEquations[i]) != 0; ++i)
                    fprintf(out, "%s = %d\n", postfixEquations[i], results[i]);

                printf("Results wrote successful !\n\n");
                
                break;
            case '6':
                exit(0);
            default:
                printf("please select 1 - 6.\n");
        }
    }
}

