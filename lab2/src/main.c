#include <stdio.h>
#include <stdlib.h>

const int K_chunk_len = 1024;
const int K_loop_not_start = -1;

typedef struct our_custom_array {
  char* pattern;
  int pattern_len;
  int permutations_quantity;
} our_custom_array;

our_custom_array create_custom_array(int array_size) {
  our_custom_array custom_array = {
          .pattern = malloc(sizeof(char) * array_size),
          .pattern_len = 0,
          .permutations_quantity = 0,
  };
  return custom_array;
}

void swap(char* first, char* second) {
  char additional = *first;
  *first = *second;
  *second = additional;
}

void destroy_custom_array(our_custom_array* custom_array) {
  free(custom_array->pattern);
  custom_array->pattern = NULL;
}

int scan_pattern(our_custom_array* custom_array) {
  FILE* thread_in = fopen("in.txt", "r");
  if (thread_in == NULL) {
    fprintf(stderr, "__LINE__ %d\n", __LINE__);
    return 1;
  }

  int iteration = 0;

  for (char symbol;
       fread(&symbol, 1, sizeof(char), thread_in) != 0 && symbol != '\n';
       iteration++) {
    if (iteration >= K_chunk_len) {
      custom_array->pattern = realloc(custom_array->pattern,
                                      sizeof(char) * (iteration + K_chunk_len));
      if (custom_array == NULL) {
        fprintf(stderr, "__LINE__ %d\n", __LINE__);
        fclose(thread_in);
        return 1;
      }
      custom_array->pattern[iteration] = symbol;
    }
    else {
      custom_array->pattern[iteration] = symbol;
    }
  }
  custom_array->pattern[iteration] = '\0';
  custom_array->pattern_len = iteration;

  if (1 != fscanf(thread_in, "%d", &custom_array->permutations_quantity)) {
    fprintf(stderr, "__LINE__ %d\n", __LINE__);
    fclose(thread_in);
    return 1;
  }
  fclose(thread_in);
  return 0;
}

int is_string_correct(const our_custom_array* custom_array) {
  int arr[10] = {0};

  for (int index = 0; index < custom_array->pattern_len; index++) {
    if ('0' <= custom_array->pattern[index]
        && custom_array->pattern[index] <= '9') {
      if (arr[custom_array->pattern[index] - '0'] >= 1) {
        return 0;
      }
      arr[custom_array->pattern[index] - '0']++;
    }
    else {
      return 0;
    }
  }
  return 1;
}

int find_first_necessary_index(const our_custom_array* custom_array) {
  int first_necessary_index = K_loop_not_start;

  for (int first_index = 0; first_index < custom_array->pattern_len - 1;
       first_index++) {
    if (custom_array->pattern[first_index]
        < custom_array->pattern[first_index + 1]) {
      first_necessary_index = first_index;
    }
  }
  return first_necessary_index;
}

int find_second_necessary_index(const our_custom_array* custom_array,
                                const int first_necessary_index) {
  int second_necessary_index = 0;

  for (int second_index = first_necessary_index + 1;
       second_index < custom_array->pattern_len; second_index++) {
    if (custom_array->pattern[first_necessary_index]
        < custom_array->pattern[second_index]) {
      second_necessary_index = second_index;
    }
  }
  return second_necessary_index;
}

void swap_some_symbols(our_custom_array* custom_array,
                       const int first_necessary_index,
                       const int second_necessary_index) {
  swap(&custom_array->pattern[first_necessary_index],
       &custom_array->pattern[second_necessary_index]);

  for (int front_index = first_necessary_index + 1,
           back_index = custom_array->pattern_len - 1;
       front_index < back_index; front_index++, back_index--) {
    swap(&custom_array->pattern[front_index],
         &custom_array->pattern[back_index]);
  }
}

int print_permutation(our_custom_array* custom_array, int flag) {
  FILE* stream_out = fopen("out.txt", "w");
  if (stream_out == NULL) {
    fprintf(stderr, "__LINE__ %d\n", __LINE__);
    return 1;
  }

  if (flag) {
    fprintf(stream_out, "bad input\n");
    fclose(stream_out);
    return 0;
  }

  for (int iteration = 0; iteration < custom_array->permutations_quantity;
       iteration++) {
    int first_necessary_index = find_first_necessary_index(custom_array);

    if (first_necessary_index != K_loop_not_start) {
      int second_necessary_index = find_second_necessary_index(
              custom_array, first_necessary_index);

      swap_some_symbols(custom_array, first_necessary_index,
                        second_necessary_index);

      fprintf(stream_out, "%s\n", custom_array->pattern);
    }
    else {
      fclose(stream_out);
      return 0;
    }
  }
  fclose(stream_out);
  return 0;
}

int main(void) {
  our_custom_array custom_array = create_custom_array(K_chunk_len);
  if (custom_array.pattern == NULL) {
    fprintf(stderr, "__LINE__ %d\n", __LINE__);
    return 0;
  }

  if (scan_pattern(&custom_array)) {
    destroy_custom_array(&custom_array);
    return 0;
  }

  if (is_string_correct(&custom_array)) {
    print_permutation(&custom_array, 0);
  }
  else {
    print_permutation(&custom_array, 1);
  }

  destroy_custom_array(&custom_array);

  return 0;
}
