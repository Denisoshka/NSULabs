#include <stdio.h>
#include <stdlib.h>

const char H_zero = '0', H_nine = '9';
const int K_true = 1, K_false = 0;

typedef struct ST_vector
{
    int pattern_len;
    char * pattern;
}ST_vector;

ST_vector Create_Vector(void)
{
    ST_vector our_vector = {
            .pattern_len = 0,
            .pattern = NULL,
    };
    return our_vector;
}

void Destroy_Vector( ST_vector * our_vector)
{
    free(our_vector->pattern);
}

void Scan_Pattern( ST_vector * our_vector )
{
    char symbol;
    for( int index = 0 ; ; index++)
    {
        if ( 1 != fread(&symbol, sizeof(char), 1, stdin ) ){
//            должны что то делать с возвращаемым значением
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

size_t Correct_String( ST_vector * our_vector )
{
    int arr [10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for(int index = 0; index < our_vector->pattern_len ; index++ )
    {
        if ( H_zero <= our_vector->pattern[index ] && our_vector->pattern[ index ] <= H_nine )
        {
            arr[our_vector->pattern[index] - H_zero]++;
        }
        else
        {
            return K_false;
        }
    }
    for( int index = 0; index < our_vector->pattern_len; index++ )
    {
        if( arr[ index ] > 1)
        {
            return K_false;
        }
    }
    return K_true;
}

int Find_First_Necessary_Index( ST_vector * our_vector )
{
    int first_necessary_index = -1;
    for ( int first_index = 0; first_index < our_vector->pattern_len - 1; first_index++ )
    {
        if ( our_vector->pattern[ first_index ] < our_vector->pattern[ first_index + 1])
        {
            first_necessary_index = first_index;
        }
    }
    return first_necessary_index;
}

int Find_Second_Necessary_Index( ST_vector * our_vector, const int first_necessary_index )
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

void Swap_Symbols( ST_vector * our_vector, const int first_necessary_index, const int second_necessary_index)
{
    char sub_char;

    sub_char = our_vector->pattern[ first_necessary_index ];
    our_vector->pattern[ first_necessary_index ] = our_vector->pattern[ second_necessary_index ];
    our_vector->pattern[ second_necessary_index ] = sub_char;

    int index = 0;
    int half_of_tail = ( our_vector->pattern_len - ( first_necessary_index + 1 ) ) / 2;

    for(; index < half_of_tail; index++  )
    {
        size_t front_index = first_necessary_index + 1 + index;
        size_t back_index = our_vector->pattern_len - 1 - index;

        sub_char = our_vector->pattern[ front_index ];
        our_vector->pattern[ front_index ] = our_vector->pattern[ back_index ];
        our_vector->pattern[ back_index ] = sub_char;
    }
}

void print_permutation( ST_vector * our_vector , int permutation_quantity )
{
    for(size_t iteration = 0; iteration < permutation_quantity; iteration++)
    {
        int first_necessary_index = Find_First_Necessary_Index( our_vector );

        if ( first_necessary_index == -1 )
        {
            return;
        }
        else {

            int second_necessary_index = Find_Second_Necessary_Index( our_vector, first_necessary_index );

            Swap_Symbols( our_vector, first_necessary_index, second_necessary_index );

            printf("%s\n", our_vector->pattern);
        }
    }
}


int main(void)
{
    int permutations_quantity;
    ST_vector vector;
    vector = Create_Vector();
    Scan_Pattern( &vector );

    if ( scanf("%d", &permutations_quantity) != 1 )
    {
        Destroy_Vector( &vector );
        printf("bad input\n");
        return 0;
    }

    if ( !( Correct_String( &vector ) ) )
    {
        Destroy_Vector( &vector );
        printf("bad input\n");
        return 0;
    }

    print_permutation( &vector, permutations_quantity);

    Destroy_Vector( &vector );
    return 0;
}
