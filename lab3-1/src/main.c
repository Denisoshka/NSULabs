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

int prepare_custom_array(our_custom_array* custom_array) {
  int scanned_array_len;

  FILE* stream_in = fopen("in.txt", "r");
  if (stream_in == NULL) {
    fprintf(stderr, "__LINE__ %d\n", __LINE__);
    return 1;
  }

  if (fscanf(stream_in, "%d", &scanned_array_len) != 1) {
    fprintf(stderr, "__LINE__ %d\n", __LINE__);
    fclose(stream_in);
    return 1;
  }

  *custom_array = create_custom_array(scanned_array_len);
  if (custom_array->array == NULL) {
    fprintf(stderr, "__LINE__ %d\n", __LINE__);
    fclose(stream_in);
    return 1;
  }

  for (int index = 0; index < custom_array->array_len; index++) {
    if (fscanf(stream_in, "%d", &custom_array->array[index]) != 1) {
      fprintf(stderr, "__LINE__ %d\n", __LINE__);
      fclose(stream_in);
      return 1;
    }
  }

  fclose(stream_in);
  return 0;
}

void swap(int* first_element, int* second_element) {
  int sub_element = *first_element;
  *first_element = *second_element;
  *second_element = sub_element;
}

void quick_sort(our_custom_array* custom_array, const int begin_index,
                const int end_index) {
  int first_index = begin_index, second_index = end_index;
  const int patrition_element
          = custom_array->array[(begin_index + end_index) / 2];

  for (; first_index < second_index;) {
    for (; (first_index < custom_array->array_len)
         && custom_array->array[first_index] < patrition_element;
         first_index++)
      ;
    for (; (second_index >= 0)
         && patrition_element < custom_array->array[second_index];
         second_index--)
      ;

    if (first_index <= second_index) {
      swap(&custom_array->array[first_index],
           &custom_array->array[second_index]);

      first_index++;
      second_index--;
    }
  }
  if (begin_index < second_index) {
    quick_sort(custom_array, begin_index, second_index);
  }
  if (first_index < end_index) {
    quick_sort(custom_array, first_index, end_index);
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
      quick_sort(&custom_array, 0, custom_array.array_len - 1);
      break;
    }
  }

  print_array(&custom_array);

  destroy_custom_array(&custom_array);
  return 0;
}
