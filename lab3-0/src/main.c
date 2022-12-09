#include <stdio.h>
#include <stdlib.h>

typedef struct our_custom_array {
  int* array;
  int array_len;
} our_custom_array;

our_custom_array create_custom_array(const int array_len) {
  our_custom_array custom_array = {
          .array = malloc(sizeof(int) * array_len),
          .array_len = array_len,
  };
  return custom_array;
}

void destroy_custom_array(our_custom_array* custom_array) {
  free(custom_array->array);
  custom_array->array = NULL;
}

void swap(int* first, int* second) {
  int additional = *first;
  *first = *second;
  *second = additional;
}

int prepare_custom_array(our_custom_array* custom_array) {
  int array_len;

  FILE* steram_in = fopen("in.txt", "r");
  if (steram_in == NULL) {
    fprintf(stderr, "__LINE__ %d\n", __LINE__);
    return 1;
  }

  if (fscanf(steram_in, "%d", &array_len) != 1) {
    fprintf(stderr, "__LINE__ %d\n", __LINE__);
    fclose(steram_in);
    return 1;
  }

  *custom_array = create_custom_array(array_len);
  if (custom_array->array == NULL) {
    fprintf(stderr, "__LINE__ %d\n", __LINE__);
    fclose(steram_in);
    return 1;
  }

  for (int index = 0; index < custom_array->array_len; index++) {
    if (fscanf(steram_in, "%d", &custom_array->array[index]) != 1) {
      fprintf(stderr, "__LINE__ %d\n", __LINE__);
      fclose(steram_in);
      return 1;
    }
  }
  fclose(steram_in);
  return 0;
}

void make_max_heap(our_custom_array* custom_array, const int array_len,
                   const int root_index) {
  int max_element_index = root_index;
  const int left_index = 2 * root_index + 1, right_index = 2 * root_index + 2;

  if (left_index < array_len
      && custom_array->array[left_index]
              > custom_array->array[max_element_index]) {
    max_element_index = left_index;
  }

  if (right_index < array_len
      && custom_array->array[right_index]
              > custom_array->array[max_element_index]) {
    max_element_index = right_index;
  }

  if (max_element_index != root_index) {
    swap(&custom_array->array[max_element_index],
         &custom_array->array[root_index]);

    make_max_heap(custom_array, array_len, max_element_index);
  }
}

void heap_sort(our_custom_array* custom_array) {
  for (int index = custom_array->array_len / 2 + 1; index >= 0; index--) {
    make_max_heap(custom_array, custom_array->array_len, index);
  }

  for (int index = custom_array->array_len - 1; index >= 0; index--) {
    swap(&custom_array->array[0], &custom_array->array[index]);

    make_max_heap(custom_array, index, 0);
  }
}

int print_array(const our_custom_array* custom_array) {
  FILE* stream_out = fopen("out.txt", "w");
  if (stream_out == NULL) {
    fprintf(stderr, "__LINE__ %d\n", __LINE__);
    return 1;
  }

  for (int index = 0; index < custom_array->array_len; index++) {
    if (fprintf(stream_out, "%d ", custom_array->array[index]) == -1) {
      fprintf(stderr, "__LINE__ %d\n", __LINE__);
      fclose(stream_out);
      return 1;
    }
  }
  fclose(stream_out);
  return 0;
}

int main(void) {
  our_custom_array custom_array;

  if (prepare_custom_array(&custom_array)) {
    destroy_custom_array(&custom_array);
    return 0;
  }

  if (custom_array.array_len <= 1) {
    print_array(&custom_array);

    destroy_custom_array(&custom_array);
    return 0;
  }

  for (int index = 0; index < custom_array.array_len - 1; index++) {
    if (custom_array.array[index] > custom_array.array[index + 1]) {
      heap_sort(&custom_array);
      break;
    }
  }

  print_array(&custom_array);

  destroy_custom_array(&custom_array);
  return 0;
}
