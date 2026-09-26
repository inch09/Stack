#include <TXLib.h>
#include <stdio.h>

#define POISON 239
#define STACK_VERIFY assert(stackError(stk) == NO_ERR)

struct Stack_t{
    double* data;
    size_t size;
    size_t capacity;
};

enum Errors{
    NO_ERR = 0,
    ERR_1,
    ERR_2,
    ERR_3,
    ERR_4,
    ERR_5
};

Errors stackInit(Stack_t* stk, size_t capacity);
Errors stackDestroy(Stack_t* stk);

Errors stackPush(Stack_t* stk, double value);
double stackPop(Stack_t* stk, Errors* err);

Errors stackError(Stack_t* stk);
Errors stackDump(Stack_t* stk);

void printStackData(Stack_t* stk);

Errors reallocUp(Stack_t* stk);
Errors reallocDown(Stack_t* stk);



int main(){

    Stack_t stk = {};
    stackInit(&stk, 15);

    stackPush(&stk, 1);
    stackPush(&stk, 2);
    stackPush(&stk, 3);
    stackPush(&stk, 4);

    stackDump(&stk);

    return 0;
}

Errors stackInit(Stack_t* stk, size_t capacity){
    //chack errors
    assert(stk);

    stk->data = (double*) calloc(capacity, sizeof(stk->data[0]));
    assert(stk->data);

    stk->size = 0;
    stk->capacity = capacity;
    for(size_t i = 0; i < capacity; i++){
        stackPush(stk, POISON);
    }
    stk->size = 0; 

    STACK_VERIFY;

    return NO_ERR;
}

Errors stackPush(Stack_t* stk, double value){
    STACK_VERIFY;
    //realloc

    stk->data[stk->size] = value;
    stk->size++;

    //printf("Pushing is all\n");

    return NO_ERR;
}

double stackPop(Stack_t* stk, Errors* err){
    STACK_VERIFY;
    assert(err);

    //check errors to err
    //realloc
    double popValue = stk->data[stk->size - 1];
    stk->data[stk->size - 1] = POISON;
    stk->size--;

    return popValue;
}

Errors stackDestroy(Stack_t* stk){
    STACK_VERIFY;
    //
    stk->size = 0;
    Errors err = NO_ERR;
    for(size_t i = 0; i < stk->capacity; i++){
        stackPop(stk, &err);
    }
    stk->capacity = 0;
    free(stk->data);

    return NO_ERR;
}


Errors stackDump(Stack_t* stk){
    STACK_VERIFY;

    printf("capacity = %lu\n\n", (unsigned long) stk->capacity);
    printf("    size = %lu\n\n", (unsigned long) stk->size);
    printStackData(stk);

    return NO_ERR;
}

void printStackData(Stack_t* stk){
    STACK_VERIFY;

    assert(stk);
    assert(stk->data);

    for(size_t i = 0; i < stk->capacity; i++){
        printf("     [%lu] = %lg\n\n", (unsigned long) i, stk->data[i]);
    }
}

Errors reallocUp(Stack_t* stk){
    STACK_VERIFY;

    const int scaleFactor = 2;
    stk->capacity *= scaleFactor;
    stk->data = (double*) realloc(stk->data, stk->capacity);
    assert(stk->data);

    return NO_ERR;
}

Errors reallocDown(Stack_t* stk){
    STACK_VERIFY;

    const int scaleFactor = 2;
    stk->capacity /= scaleFactor;
    stk->data = (double*) realloc(stk->data, stk->capacity);
    assert(stk->data);

    return NO_ERR;
}

Errors stackError(Stack_t* stk){
    if(stk == NULL){
        return ERR_1;
    }
    if(stk->data == NULL){
        return ERR_2;
    }
    if(stk->capacity < 0){
        return ERR_3;
    }
    if(stk->size >= stk->capacity || stk->size < 0){
        return ERR_4;
    }

    return NO_ERR;
}


// stackDump  in log file




