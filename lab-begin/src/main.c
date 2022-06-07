#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

int get_values(int length, int* array);
void sum_array(int length, int* array);

int main(){
    int length;
    if(scanf("%d", &length) != 1 || length < 1 || length > 10){
        printf("bad input");
        return 0;
    }
    int* array = (int*)(calloc(length, sizeof(int)));
    if(get_values(length, array)){
        sum_array(length, array);
    }
    free(array);
    return 0;
}

int get_values(int length, int* array){
    long long tmp;
    for(int i = 0; i < length; i++){
        if(scanf("%lld", &tmp) < 1 || tmp > INT_MAX || tmp < INT_MIN){
            printf("bad input");
            return 0;
        }
        array[i] = (int)tmp;
    }
    return 1;
}

void sum_array(int length, int* array){
    long long int sum = 0;
    for(int i = 0; i < length; i++){
        sum += array[i];
        if(sum > INT_MAX || sum < INT_MIN){
            printf("overflow");
            return;
        }
    }
    printf("%d", (int)sum);
}
