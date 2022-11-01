#include <stdio.h>
#include <stdlib.h>

#define K_max_pattern_len (int)16
#define K_max_chunk_len (int)1024
#define K_shift_table_size (int)256

void print_other_error( int line )
{
    printf( "line %d\n", line);
}

typedef struct vector{
    unsigned char * array;
    int array_len;
    int sub_index;
}vector;

vector create_vector( int necessary_array_len )
{
    vector blank = {
            .array = malloc( sizeof(unsigned char) * necessary_array_len ),
            .array_len = necessary_array_len,
            .sub_index = 0,
    };
    return blank;
}

void destroy_vector( vector * blank )
{
    free( blank->array );
    blank->array = NULL;
}

void scan_pattern( vector * pattern, FILE * thread_in )
{
    int index = 0;
    unsigned char symbol;

    for ( ; ( 1 == fread( &symbol, sizeof(unsigned char), 1, thread_in ) ) && symbol != '\n'; index++)
    {
        pattern->array[index] = symbol;
    }
    pattern->array_len = index;
}

void make_shift_table( const vector * pattern, int * shift_table )
{
    for ( int i = 0; i < K_shift_table_size; i++)
    {
        shift_table[i] = pattern->array_len;
    }

    int shift = 1;

    for ( int i = pattern->array_len - 2; i >= 0; i-- )
    {
        if ( shift_table[ pattern->array[i] ] == pattern->array_len )
        {
            shift_table[ pattern->array[i] ] = shift++;
        }
    }
}

void text_rewrite( vector * text, const vector * pattern, FILE * thread_in )
{
    int where_start_rewrite = text->sub_index - pattern->array_len + 1;
    int iterations_quantity = pattern->array_len - text->sub_index + text->array_len - 1;

    for ( int iteration = 0; iteration < iterations_quantity ; iteration++)
    {
        text->array[iteration] = text->array[where_start_rewrite + iteration];
    }
    text -> array_len = (int)fread( text->array + iterations_quantity, sizeof( unsigned char), text->array_len - iterations_quantity, thread_in) + iterations_quantity;
    text->sub_index = pattern->array_len - 1;// упускаю iterations// заебись начальник, говнокод устранен
}

void search_substring(vector * text, const vector * pattern, int * total_index, const int * shift_table, FILE * thread_out)
{
    for( int pattern_work_index = pattern->array_len - 1, text_work_index = text -> sub_index; pattern_work_index>=0; pattern_work_index--, text_work_index--)
    {
        fprintf( thread_out, "%d ", *total_index + pattern_work_index - pattern->array_len + 1 );

        if( text->array[ text_work_index ] != pattern->array[ pattern_work_index ] )
        {
            *total_index += shift_table[ text->array[ text->sub_index ] ];
            text->sub_index += shift_table[ text->array[ text->sub_index ] ];
            return;
        }
    }
    *total_index += pattern->array_len;
    text->sub_index += pattern->array_len;
}

void booyer_moore_algorithm(vector * text, const vector * pattern, const int * shift_table, FILE * thread_in, FILE * thread_out )
{
    int total_index = pattern->array_len;
    text->sub_index = pattern->array_len - 1;

    text->array_len = (int)fread( text->array, sizeof (unsigned char ), K_max_chunk_len, thread_in);

    while ( pattern->array_len <= text->array_len )
    {
        search_substring( text, pattern, &total_index, shift_table, thread_out);

        if (text->sub_index >= text->array_len )
        {
            text_rewrite(text, pattern, thread_in);
        }
    }
}

void do_before_exit( vector * pattern, vector * text, FILE * thread_in, FILE * thread_out)
{
    destroy_vector( pattern);
    destroy_vector( text);
    fclose( thread_in );
    fclose( thread_out);
}

int main(void)
{
    int shift_table [ K_shift_table_size ];

    FILE * thread_in = fopen( "in.txt", "r");
    FILE * thread_out = fopen( "out.txt", "w");

    vector pattern = create_vector( K_max_pattern_len );
    vector text = create_vector( K_max_chunk_len );


    if ( pattern.array == NULL )
    {
        do_before_exit( &pattern, &text, thread_in, thread_out);
        print_other_error( __LINE__ );
        return 0;
    }
    if ( text.array == NULL )
    {
        do_before_exit( &pattern, &text, thread_in, thread_out);
        print_other_error( __LINE__ );
        return 0;
    }

    scan_pattern( &pattern, thread_in);
    make_shift_table( &pattern, shift_table );
    booyer_moore_algorithm( &text, &pattern, shift_table, thread_in, thread_out );

    do_before_exit( &pattern, &text, thread_in, thread_out);
    return 0;
}
