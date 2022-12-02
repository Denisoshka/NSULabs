#include <stdio.h>
#include <stdlib.h>

const int K_max_pattern_len = 16,
          K_max_chunk_len = 1024,
          K_shift_table_size = 256;

typedef struct our_custom_array{
    unsigned char * array;
    int array_len;
    int sub_index;
}our_custom_array;

our_custom_array create_custom_array(const int necessary_array_len )
{
    our_custom_array blank = {
            .array = malloc( sizeof(unsigned char) * necessary_array_len ),
            .array_len = necessary_array_len,
            .sub_index = 0,
            };
    return blank;
}

void destroy_custom_array(our_custom_array * blank )
{
    free( blank->array );
    blank->array = NULL;
}

void scan_pattern(our_custom_array * pattern, FILE * stream_in )
{
    int index = 0;
    unsigned char symbol;

    for ( ; ( 1 == fread(&symbol, sizeof(unsigned char), 1, stream_in ) ) && symbol != '\n'; index++)
    {
        pattern->array[index] = symbol;
    }
    pattern->array_len = index;
}

void make_shift_table(const our_custom_array * pattern, int * shift_table )
{
    for ( int i = 0; i < K_shift_table_size; i++)
    {
        shift_table[i] = pattern->array_len;
    }

    for ( int i = pattern->array_len - 2, shift = 1; i >= 0; i-- )
    {
        if ( shift_table[ pattern->array[i] ] == pattern->array_len )
        {
            shift_table[ pattern->array[i] ] = shift++;
        }
    }
}

void text_rewrite(our_custom_array * text, const our_custom_array * pattern, FILE * stream_in )
{
    const int K_where_start_rewrite = text->sub_index - pattern->array_len + 1;
    const int K_iterations_quantity = pattern->array_len - text->sub_index + text->array_len - 1;

    for ( int iteration = 0; iteration < K_iterations_quantity ; iteration++)
    {
        text->array[iteration] = text->array[K_where_start_rewrite + iteration];
    }
    text -> array_len = (int)fread( text->array + K_iterations_quantity, sizeof( unsigned char), text->array_len - K_iterations_quantity, stream_in) + K_iterations_quantity;
    text -> sub_index = pattern->array_len - 1;
}

void search_substring(our_custom_array * text, const our_custom_array * pattern, int * total_index, const int * shift_table, FILE * stream_out)
{
    for( int pattern_work_index = pattern->array_len - 1, text_work_index = text -> sub_index; pattern_work_index>=0; pattern_work_index--, text_work_index--)
    {
        fprintf(stream_out, "%d ", *total_index + pattern_work_index - pattern->array_len + 1 );

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

void booyer_moore_algorithm(const our_custom_array * pattern, const int * shift_table, FILE * stream_in)
{
    FILE * stream_out = fopen("out.txt", "w");
    if (stream_out == NULL)
    {
        fprintf( stderr, "__LINE__ %d\n", __LINE__);
        return;
    }

    our_custom_array text = create_custom_array(K_max_chunk_len);
    if (text.array == NULL)
    {
        fclose(stream_out);
        fprintf( stderr, "__LINE__ %d\n", __LINE__);
        return;
    }

    int total_index = pattern->array_len;
    text.sub_index = pattern->array_len - 1;
    text.array_len = (int)fread(text.array, sizeof (unsigned char ), K_max_chunk_len, stream_in);

    while ( pattern->array_len <= text.array_len )
    {
        search_substring(&text, pattern, &total_index, shift_table, stream_out);

        if (text.sub_index >= text.array_len )
        {
            text_rewrite(&text, pattern, stream_in);
        }
    }

    fclose(stream_out);
    destroy_custom_array(&text);
}

int main(void)
{
    int shift_table [ K_shift_table_size ];

    FILE * stream_in = fopen("in.txt", "r");
    if (stream_in == NULL )
    {
        fprintf( stderr, "__LINE__ %d\n", __LINE__);
        return 0;
    }

    our_custom_array pattern = create_custom_array(K_max_pattern_len);
    if ( pattern.array == NULL)
    {
        fclose(stream_in );
        fprintf( stderr, "__LINE__ %d\n", __LINE__);
        return 0;
    }

    scan_pattern(&pattern, stream_in);
    make_shift_table( &pattern, shift_table );

    booyer_moore_algorithm(&pattern, shift_table, stream_in );

    fclose(stream_in );
    destroy_custom_array(&pattern);

    return 0;
}
