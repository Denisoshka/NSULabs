#include <stdio.h>
#include <stdlib.h>

const int K_loop_not_start = -1;

typedef struct ST_vector
{
    int pattern_len;
    char * pattern;
}ST_vector;

ST_vector CreateVector(void)
{
    ST_vector our_vector = {
            .pattern_len = 0,
            .pattern = NULL,
    };
    return our_vector;
}

void DestroyVector( ST_vector * our_vector)
{
    free(our_vector->pattern);
}

void ScanPattern( ST_vector * our_vector )
{
    char symbol;
    for( int index = 0 ; ; index++)
    {
        if ( 1 != fread(&symbol, sizeof(char), 1, stdin ) ){
            printf("__Scan_pattern__fread\n" );
            exit(EXIT_FAILURE);
        }
        if ( symbol != '\n' )
        {
            our_vector->pattern_len++;
            our_vector->pattern = realloc( our_vector->pattern, our_vector->pattern_len);
            our_vector->pattern[index] = symbol;
        }
        else
        {
            our_vector->pattern = realloc( our_vector->pattern, our_vector->pattern_len+1 );
            our_vector->pattern[ index ] = '\0';
            return;
        }
    }
}

int IsCorrectString( const ST_vector * our_vector )
{
    int arr [10] = {0};
    for(int index = 0; index < our_vector->pattern_len ; index++ )
    {
        if ( '0' <= our_vector->pattern[index ] && our_vector->pattern[ index ] <= '9' )
        {
            arr[our_vector->pattern[index] - '0']++;
        }
        else
        {
            return 0;
        }
    }
    for( int index = 0; index < our_vector->pattern_len; index++ )
    {
        if( arr[ index ] > 1)
        {
            return 0;
        }
    }
    return 1;
}

int FindFirstNecessaryIndex( const ST_vector * our_vector )
{
    int first_necessary_index = K_loop_not_start;

    for ( int first_index = 0; first_index < our_vector->pattern_len - 1; first_index++ )
    {
        if ( our_vector->pattern[ first_index ] < our_vector->pattern[ first_index + 1])
        {
            first_necessary_index = first_index;
        }
    }
    return first_necessary_index;
}

int FindSecondNecessaryIndex( const ST_vector * our_vector, const int first_necessary_index )
{
    int second_necessary_index = 0;
    for ( int second_index = first_necessary_index + 1; second_index < our_vector->pattern_len; second_index++)
    {
        if ( our_vector->pattern[ first_necessary_index ] < our_vector->pattern[ second_index ])
        {
            second_necessary_index = second_index;
        }
    }
    return second_necessary_index;
}

void SwapSymbols( ST_vector * our_vector, const int first_necessary_index, const int second_necessary_index)
{
    char sub_char;

    sub_char = our_vector->pattern[ first_necessary_index ];
    our_vector->pattern[ first_necessary_index ] = our_vector->pattern[ second_necessary_index ];
    our_vector->pattern[ second_necessary_index ] = sub_char;

    int half_of_tail = ( our_vector->pattern_len - ( first_necessary_index + 1 ) ) / 2;


    for(int index = 0; index < half_of_tail; index++  )
    {
        int front_index = first_necessary_index + 1 + index;
        int back_index = our_vector->pattern_len - 1 - index;

        sub_char = our_vector->pattern[ front_index ];
        our_vector->pattern[ front_index ] = our_vector->pattern[ back_index ];
        our_vector->pattern[ back_index ] = sub_char;
    }
}

void PrintPermutation( ST_vector * our_vector , int permutation_quantity )
{
    for(int iteration = 0; iteration < permutation_quantity; iteration++)
    {
        int first_necessary_index = FindFirstNecessaryIndex( our_vector );

        if ( first_necessary_index == K_loop_not_start )
        {
            return;
        }
        else
        {
            int second_necessary_index = FindSecondNecessaryIndex( our_vector, first_necessary_index );

            SwapSymbols( our_vector, first_necessary_index, second_necessary_index );

            printf("%s\n", our_vector->pattern);
        }
    }
}

void FinishWithBadInput( ST_vector * our_vector )
{
    DestroyVector( our_vector );
    printf("bad input\n" );
    exit( EXIT_SUCCESS);
}


void AreAllConditionComplied( ST_vector * our_vector, int count_of_scan)
{
    if ( ( count_of_scan != 1 ) || ( !IsCorrectString( our_vector ) ) )
        FinishWithBadInput( our_vector );
}


void FinishWithExitSuccess(ST_vector * our_vector )
{
    DestroyVector( our_vector );
    exit( EXIT_SUCCESS );
}

int main(void)
{
    int permutations_quantity, count_of_scan;
    ST_vector vector = CreateVector();
    ScanPattern( &vector );
    count_of_scan = scanf("%d", &permutations_quantity);

    AreAllConditionComplied( &vector, count_of_scan );

    PrintPermutation( &vector, permutations_quantity);

    FinishWithExitSuccess( &vector );
}
