#include <TXLib.h>
#include <stdio.h>

#define POISON 239
#define STACK_VERIFY    if(stackError(stk) != NO_ERR){\
                            printf("Err = %d\n", (int) stackError(stk));\
                            assert(stackError(stk) == NO_ERR);\
                        }
                    

struct Stack_t{
    double* data;
    size_t size;
    size_t capacity;
};

enum Errors{
    NO_ERR = 0,
    ERR_1 = 1,
    ERR_2 = 2,
    ERR_3 = 3,
    ERR_4 = 4,
    ERR_5 = 5
};

Errors stackInit(Stack_t* stk, size_t capacity);
Errors stackDestroy(Stack_t* stk);

Errors stackPush(Stack_t* stk, double value);
double stackPop(Stack_t* stk, Errors* err);

Errors stackError(Stack_t* stk);
Errors stackDump(const char* fileName, Stack_t* stk);

Errors reallocUp(Stack_t* stk);
Errors reallocDown(Stack_t* stk);



int main(){
    
    Errors err = NO_ERR;
    Stack_t stk = {};
    stackInit(&stk, 4);
    //assert(1 == 0);

    stackPush(&stk, 1);
    //assert(1 == 0);
    stackPush(&stk, 2);

    //assert(1 == 0);
    stackPush(&stk, 3);
    stackPush(&stk, 4);
    // stackPush(&stk, 4);
    // stackPush(&stk, 4);

    //stackPop(&stk, &err);
    stackPop(&stk, &err);
    stackPop(&stk, &err);
    // stackPop(&stk, &err);
    // stackPop(&stk, &err);
    stackPop(&stk, &err);
    stackPop(&stk, &err);
    stackPop(&stk, &err);
    //stackPush(&stk, 1);
    //stackPush(&stk, 1);

    
    //stackPop(&stk, &err);


    stackDump("stack.log", &stk);

    stackDestroy(&stk);

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
    if(stk->size == stk->capacity){
        reallocUp(stk);
    }
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
    if(stk->size == 0){
        assert(1 == 0);
    }
    // assert(stk->size != 0);
    double popValue = stk->data[stk->size - 1];
    stk->data[stk->size - 1] = POISON;
    stk->size--;

    STACK_VERIFY;

    return popValue;
}

Errors stackDestroy(Stack_t* stk){
    STACK_VERIFY;
    //
    Errors err = NO_ERR;
    for(size_t i = 0; i < stk->size; i++){
        stackPop(stk, &err);
    }

    stk->size = 0;
    stk->capacity = 0;

    free(stk->data);
    stk->data = NULL;
    
    stk = NULL;

    return NO_ERR;
}


Errors stackDump(const char* fileName, Stack_t* stk){
    STACK_VERIFY;

    FILE* filePtr = fopen(fileName, "w");

    fprintf(filePtr, "--------------------------------------------------------------------------------------------------------------------------\n");
    fprintf(filePtr, "                   Information about our stack: \n");

    fprintf(filePtr, "    capacity = %lu\n", (unsigned long) stk->capacity);
    fprintf(filePtr, "    size = %lu\n\n", (unsigned long) stk->size);

    for(size_t i = 0; i < stk->capacity; i++){
        fprintf(filePtr, "     [%lu] = %lg\n\n", (unsigned long) i, stk->data[i]);
    }

    fprintf(filePtr, "----------------------------------------------------------------------------------------------------------------------\n");

    fclose(filePtr);
    return NO_ERR;
}

// void printStackData(Stack_t* stk){
//     STACK_VERIFY;

//     for(size_t i = 0; i < stk->capacity; i++){
//         printf("     [%lu] = %lg\n\n", (unsigned long) i, stk->data[i]);
//     }
// }

Errors reallocUp(Stack_t* stk){
    STACK_VERIFY;

    const size_t scaleFactor = 2;
    stk->capacity *= scaleFactor;
    stk->data = (double*) realloc((void*) stk->data, stk->capacity);
    assert(stk->data);
    for(size_t i = stk->size; i < stk->capacity; i++){
        stk->data[i] = POISON;
    }

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
    if(stk->capacity == 0){
        return ERR_3;
    }

    // if(stk->capacity < 0){
    //     return ERR_3;
    // }
    if(stk->size > stk->capacity /* ????? */){
        printf("size = %lu\n", (unsigned long) stk->size);
        return ERR_5;
    }

    return NO_ERR;
}


// stackDump  in log file




