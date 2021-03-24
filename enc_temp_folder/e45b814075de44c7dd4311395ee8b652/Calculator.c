#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>
#include <windows.h>
#include <ctype.h>
#include "malloc.h"

#define STACK_OVERFLOW  -100
#define STACK_UNDERFLOW -101
#define OUT_OF_MEMORY   -102

typedef int T;
typedef char E;

typedef struct DataCalc {
    T value;
    struct DataCalc* next;
} Data;

typedef struct DataOperation {
    E value;
    struct DataOperation* next;
} DataOperations;


void pushDataOperations(DataOperations** head, E value) {
    DataOperations* tmp = malloc(sizeof(DataOperations));
    if (tmp == NULL) exit(STACK_OVERFLOW);
    tmp->next = *head;
    tmp->value = value;
    *head = tmp;
}


char popDataOperations(DataOperations** head) {
    DataOperations* out; E value;
    if (*head == NULL) exit(STACK_UNDERFLOW);
    out = *head;
    *head = (*head)->next;
    value = out->value;
    free(out);
    return value;
}

void pushData(Data** head, T value) {
    Data* tmp = malloc(sizeof(Data));
    if (tmp == NULL) exit(STACK_OVERFLOW);
    tmp->next = *head;
    tmp->value = value;
    *head = tmp;
}


int popData(Data** head) {
    Data* out; T value;
    if (*head == NULL) exit(STACK_UNDERFLOW);
    out = *head;
    *head = (*head)->next;
    value = out->value;
    free(out);
    return value;
}

int priority(char operation) {
    switch (operation)
    {
        case '^': return 3; break;
        case '+': 
        case '-': return 1; break;
        case '*': 
        case '/': return 2; break;
        case 't':   // tg
        case 'c':   // cos
        case 's':   // sin
        case 'g':   // ctg
        case 'q':   // sqrt
        case 'o':   // arctg
        case 'i':   // arcctg
        case 'u':   // arcsin
        case 'p': return 4; break; // arccos
        case '(': 
        case ')': return -1; break;
    }

 
}

void parseData(char* data) {
    Data* headData = NULL;
    DataOperations* headOperation = NULL;

    char tokenOperation[10] = "";
    int i = 0;

    for (unsigned int j = 0; data[j]; j++) {
        if (isspace(data[j])) continue;
        if (isalpha(data[j])) {
            tokenOperation[i] = data[j]; i++;
            if (strcmp(tokenOperation, "sqrt") == 0 || strcmp(tokenOperation, "cos") == 0 || strcmp(tokenOperation, "sin") == 0) {
                i = 0;
                pushDataOperations(&headOperation, tokenOperation);
                memset(&tokenOperation, 0, sizeof(tokenOperation));
            }
        }
        else if ('(' == data[j] || ')' == data[j] || '+'  == data[j] || '*' == data[j] || '/'  == data[j] || '-' == data[j] || '^' ==  data[j]) {
            pushDataOperations(&headOperation, data[j]);
        }
        else if (isdigit(data[j])) {
            pushData(&headData, data[j]);
        }
        else {
            printf("Invalid expression");
            return;
        }
        
    }
}

int main() {
    char data[256];
    scanf("%s", data);
    parseData(data);
    return 0;
}
