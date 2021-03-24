#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>
#include <windows.h>
#include <ctype.h>
#include < math.h >
#include "malloc.h"

#define STACK_OVERFLOW  -100
#define STACK_UNDERFLOW -101
#define OUT_OF_MEMORY   -102

typedef float T;
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


float popData(Data** head) {
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
            if (strcmp(tokenOperation, "sqrt") == 0 || strcmp(tokenOperation, "cos") == 0 || strcmp(tokenOperation, "sin") == 0 || strcmp(tokenOperation, "tg") == 0 || strcmp(tokenOperation, "ctg") == 0 || strcmp(tokenOperation, "arctg") == 0 || strcmp(tokenOperation, "arcctg") == 0 || strcmp(tokenOperation, "arcsin") == 0 || strcmp(tokenOperation, "arccos") == 0) {
                i = 0;
                pushDataOperations(&headOperation, tokenOperation);
                memset(&tokenOperation, 0, sizeof(tokenOperation));
            }
        }
        else if ('(' == data[j] || ')' == data[j] || '+'  == data[j] || '*' == data[j] || '/'  == data[j] || '-' == data[j] || '^' ==  data[j]) {
            if (headOperation == NULL)
            {
                pushDataOperations(&headOperation, data[j]);
            }
            else if (headOperation != NULL) {
                if (priority(headOperation) > priority(data[j]))
                {
                    int tmp = popData(&headData);
                    pushData(&headData, count(tmp, (float) headData, data[j]));
                }
            }
            
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

int count(float f, float t, char operation) {
    switch (operation) {
    case '+': return f + t; break;
    case '-': return f - t; break;
    case '*': return f * t; break;
    case '/': return f / t; break;
    case '^': return pow(f , t); break;
    case 'q': return sqrt(f); break; //sqrt
    case 't': return tan(f); break; //tg
    case 'c': return cos(f); break; //cos
    case 's': return sin(f); break; //sin
    case 'o': return atan(f); break; //arctg
    case 'i': return ((M_PI_2 - atan(f)) * 180) / M_PI; break; //arcctg
    case 'u': return asin(f); break; //arcsin
    case 'p': return acos(f); break; //arccos
    }
}



int main() {
    Data* headData = NULL;
    DataOperations* headOperation = NULL;
    char data[256];
    scanf("%s", &data);
    parseData(data);
    return 0;
}
