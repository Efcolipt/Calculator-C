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

typedef char T;
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


T peekData(const Data* head) {
    if (head == NULL) {
        exit(STACK_UNDERFLOW);
    }
    return head->value;
}

E peekDataOperations(const DataOperations* head) {
    if (head == NULL) {
        exit(STACK_UNDERFLOW);
    }
    return head->value;
}

int priority(const DataOperations* head, const char* current) {
    int headFlag = 0;
    int currentFlag = 0;
    switch ((char) head->value)
    {
        case '^': headFlag = 3; break;
        case '+': 
        case '-': headFlag = 1; break;
        case '*': 
        case '/': headFlag = 2; break;
        case 't':   // tg
        case 'c':   // cos
        case 's':   // sin
        case 'g':   // ctg
        case 'q':   // sqrt
        case 'o':   // arctg
        case 'i':   // arcctg
        case 'u':   // arcsin
        case 'p': headFlag = 4; break; // arccos
    }

    switch ((char)current)
    {
    case '^': currentFlag = 3; break;
    case '+':
    case '-': currentFlag = 1; break;
    case '*':
    case '/': currentFlag = 2; break;
    case 't':   // tg
    case 'c':   // cos
    case 's':   // sin
    case 'g':   // ctg
    case 'q':   // sqrt
    case 'o':   // arctg
    case 'i':   // arcctg
    case 'u':   // arcsin
    case 'p': currentFlag = 4; break; // arccos
    }

    if (currentFlag > headFlag)
    {
        return 1;
    }
    else if (currentFlag < headFlag) {
        return 0;
    }
    else {
        return -1;
    }

 
}

char* whatIsTrigonomy(char* token) {
    if (!strcmp(token, "sqrt")) {
        return 'q';
    }
    else if (!strcmp(token, "cos")) {
        return 'c';
    }
    else if (!strcmp(token, "sin")) {
        return 's';
    }
    else if (!strcmp(token, "tg")) {
        return 't';
    }
    else if (!strcmp(token, "ctg")) {
        return 'g';
    }
    else if (!strcmp(token, "arctg")) {
        return 'o';
    }
    else if (!strcmp(token, "arcctg")) {
        return 'i';
    }
    else if (!strcmp(token, "arcsin")) {
        return 'u';
    }
    else if (!strcmp(token, "arccos")) {
        return 'p';
    }
    return '_';
}

void parseData(char* data) {
    Data* headData = NULL;
    DataOperations* headOperation = NULL;

    char digitTmp[1000] = {0};
    char tokenOperation[10] = "";
    size_t k = 0, s = 0, j = 0, i = 0;

    for (j = 0; data[j] != '\0'; j++) {
        if (isspace(data[j])) continue;
        if (isalpha(data[j])) {
            tokenOperation[i] = data[j]; i++;
            if (!isalpha(data[j + 1]) && whatIsTrigonomy(tokenOperation) != "_") {
                i = 0;
                pushDataOperations(&headOperation, whatIsTrigonomy(tokenOperation));
                memset(&tokenOperation, 0, sizeof(tokenOperation));
            }
        }
        else if ('(' == data[j] || ')' == data[j] || '+'  == data[j] || '*' == data[j] || '/'  == data[j] || '-' == data[j] || '^' ==  data[j]) {
            if ('(' == headOperation->value || ')' == headOperation->value || '(' == data[j] || ')' == data[j])
            {
                pushDataOperations(&headOperation, data[j]);
            }
            else if (headOperation == NULL)
            {
                pushDataOperations(&headOperation, data[j]);
            }
            else if (headOperation != NULL) {
                if (priority(&headOperation, data[j])) // если прошлая операция больше текущего
                {
                    float num1 = popData(&headData);
                    float num2 = popData(&headData);
                    switch ((char) headOperation->value) {
                    case '+': pushData(&headData, (float)num1 + num2); popDataOperations(&headOperation); break;
                    case '-': pushData(&headData, (float)num1 - num2); popDataOperations(&headOperation); break;
                    case '*': pushData(&headData, (float)num1 * num2); popDataOperations(&headOperation); break;
                    case '/': pushData(&headData, (float)num1 / num2); popDataOperations(&headOperation); break;
                    case '^': pushData(&headData, (float)pow(num1, num2)); popDataOperations(&headOperation); break;
                    case 'q': pushData(&headData, (float)sqrt(num1)); popDataOperations(&headOperation); break; //sqrt
                    case 't': pushData(&headData, (float)tan(num1)); popDataOperations(&headOperation); break; //tg
                    case 'c': pushData(&headData, (float)cos(num1)); popDataOperations(&headOperation); break; //cos
                    case 'g': pushData(&headData, (float)1/tan(num1)); popDataOperations(&headOperation); break; // ctg
                    case 's': pushData(&headData, (float)sin(num1)); popDataOperations(&headOperation); break; //sin
                    case 'o': pushData(&headData, (float)atan(num1)); popDataOperations(&headOperation); break; //arctg
                    case 'i': pushData(&headData, (float)atan(((M_PI_2 - atan(num1)) * 180) / M_PI)); popDataOperations(&headOperation); break; //arcctg
                    case 'u': pushData(&headData, (float)asin(num1)); popDataOperations(&headOperation); break; //arcsin
                    case 'p': pushData(&headData, (float)acos(num1)); popDataOperations(&headOperation); break; //arccos
                    }

                }
                else if (priority(headOperation, data[j]) == -1) { // если прошлая операция равна текущей
                    for (; priority(headOperation , data[j]) && ((char)headOperation->value != '(' && headOperation->value != ')');)
                    {
                        float num1 = popData(&headData);
                        float num2 = popData(&headData);
                        switch ((char) headOperation->value) {
                        case '+': pushData(&headData, (float)num1 + num2); popDataOperations(&headOperation); break;
                        case '-': pushData(&headData, (float)num1 - num2); popDataOperations(&headOperation); break;
                        case '*': pushData(&headData, (float)num1 * num2); popDataOperations(&headOperation); break;
                        case '/': pushData(&headData, (float)num1 / num2); popDataOperations(&headOperation); break;
                        case '^': pushData(&headData, (float)pow(num1, num2)); popDataOperations(&headOperation); break;
                        case 'q': pushData(&headData, (float)sqrt(num1)); popDataOperations(&headOperation); break; //sqrt
                        case 't': pushData(&headData, (float)tan(num1)); popDataOperations(&headOperation); break; //tg
                        case 'c': pushData(&headData, (float)cos(num1)); popDataOperations(&headOperation); break; //cos
                        case 'g': pushData(&headData, (float)1 / tan(num1)); popDataOperations(&headOperation); break; // ctg
                        case 's': pushData(&headData, (float)sin(num1)); popDataOperations(&headOperation); break; //sin
                        case 'o': pushData(&headData, (float)atan(num1)); popDataOperations(&headOperation); break; //arctg
                        case 'i': pushData(&headData, (float)atan(((M_PI_2 - atan(num1)) * 180) / M_PI)); popDataOperations(&headOperation); break; //arcctg
                        case 'u': pushData(&headData, (float)asin(num1)); popDataOperations(&headOperation); break; //arcsin
                        case 'p': pushData(&headData, (float)acos(num1)); popDataOperations(&headOperation); break; //arccos
                        } 
                    }
                   
                    pushDataOperations(&headOperation, data[j]);
                }
                else if (priority(headOperation,data[j]) == 0) {
                    pushDataOperations(&headOperation, data[j]);
                }
            }
            
        }
        else if (isdigit(data[j])) {
            for (k = j; isdigit(data[k]); k++) pushData(&headData, data[k]);
            for (s = 0; s < k; s++) digitTmp[s] = popData(&headData);
            char temp = 0;
            size_t kl = 0, ka = strlen(digitTmp) - 1;

            while (kl < ka)
            {
                temp = digitTmp[kl];
                digitTmp[kl] = digitTmp[ka];
                digitTmp[ka] = temp;
                kl++;
                ka--;
            }
            j += strlen(digitTmp) - 1;

            pushData(&headData, digitTmp);
        }
        else {
            printf("Invalid expression");
            return;
        }
        
    }
}

int main() {
    char data[256] = {0};
    gets(data);
    parseData(data);
    return 0;
}
