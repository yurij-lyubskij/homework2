#include <pthread.h>
#include <unistd.h>

#include "inout.h"
#include "utils.h"

typedef struct {
  results *result;
  staff_array *staff;
  uint16_t start;
  uint16_t finish;
  int (*comp)(uint8_t l, uint8_t r);
} thread_arg;

void *thread_search(void *arg) {
  thread_arg *args = (thread_arg *)arg;
  staff_array *staff = args->staff;
  uint16_t start = args->start;
  uint16_t finish = args->finish;
  results *result = args->result;
  int (*comp)(uint8_t l, uint8_t r) = args->comp;
  for (uint16_t i = start; i < finish; ++i) {
    uint16_t index = staff->job_index[i];
    if (staff->age[i] == result->age[index]) {
      int fail = add_elem(staff, result->staff_by_job + index, i);
      if (fail) {
        pthread_exit((void *)-1);
      }
    }

    if (comp(staff->age[i], result->age[index])) {
      result->age[index] = staff->age[i];
      (result->staff_by_job + index)->size = 0;
      int fail = add_elem(staff, result->staff_by_job + index, i);
      if (fail) {
        pthread_exit((void *)-1);
      }
    }
  }
  pthread_exit(0);
}

int is_less(uint8_t l, uint8_t r) { return l < r; }

int is_more(uint8_t l, uint8_t r) { return l > r; }

pthread_t *tid = NULL;
thread_arg *arg = NULL;
results *temp_result = NULL;

void free_temp(size_t num_threads, int size) {
  for (size_t i = 0; i < num_threads; i++) {
    if ((temp_result + i)->size > 0) {
      free_result(temp_result + i);
    }
  }
  free(temp_result);
  free(tid);
  free(arg);
}

int alloc_temp(size_t num_threads, int size, int max) {
  tid = calloc(num_threads, sizeof(pthread_t));
  arg = calloc(num_threads, sizeof(thread_arg));
  temp_result = calloc(num_threads, sizeof(results));
  for (int i = 0; i < num_threads; i++) {
    int fail = init_result(temp_result + i, size,
                           max);  // проверяем, ищем максимум, или минимум
    if (fail) {
      free(tid);
      free(arg);
      free(temp_result);
      return -1;
    }
  }
  return 0;
}

int merge_buffers(staff_array *buff1, staff_array *buff2) {
  for (int i = 0; i < buff2->size; ++i) {
    add_elem(buff2, buff1, i);
  }
  return 0;
}

int unite_buffers(results *temp, uint16_t count, results *result,
                  int (*comp)(uint8_t l, uint8_t r)) {
  for (uint16_t i = 0; i < result->size; ++i) {
    staff_array target = *(result->staff_by_job + i);
    uint16_t target_age = *(result->age + i);
    for (uint16_t j = 0; j < count; ++j) {
      staff_array *source = &temp[j].staff_by_job[i];
      uint16_t src_age = temp[j].age[i];
      if (src_age == target_age) {
        merge_buffers(&target, source);
      }

      if (comp(src_age, target_age)) {
        target_age = src_age;
        target.size = 0;
        merge_buffers(&target, source);
      }

      *(result->staff_by_job + i) = target;
    }
  }
  return 0;
}

int find_by_job(results *result, staff_array *staff, int size,
                int (*comp)(uint8_t l, uint8_t r)) {
  int errflag;
  int fail = init_result(result, size,
                         comp(0, 1));  // проверяем, ищем максимум, или минимум
  if (fail) {
    return -1;
  }
  size_t num_threads = sysconf(_SC_NPROCESSORS_ONLN) - 1;
  fail = alloc_temp(num_threads, size, comp(0, 1));
  if (fail) {
    return -1;
  }

  uint16_t part = staff->size / num_threads;
  for (int i = 0; i < num_threads; i++) {
    arg[i].staff = staff, arg[i].result = &temp_result[i], arg[i].comp = comp;
    arg[i].start = i * part;
    arg[i].finish = (i + 1 == num_threads) ? staff->size : (i + 1) * part;
    errflag = pthread_create(&tid[i], NULL, thread_search, &arg[i]);
    if (errflag != 0) {
      printf("error = %d", errflag);
      free_temp(num_threads, size);
      return -1;
    }
  }
  int retval = 0;
  for (int i = 0; i < num_threads; i++) {
    errflag = pthread_join(tid[i], (void **)&retval);
    if (errflag != 0 || retval != 0) {
      free_temp(num_threads, size);
      printf("errflag = %d, retval = %d", errflag, retval);
      return -1;
    }
  }
  unite_buffers(temp_result, num_threads, result, comp);
  sort_by_job(result);
  free_temp(num_threads, size);
  return 0;
}

int find_young(results *result, staff_array *staff, int size) {
  return find_by_job(result, staff, size, is_less);
}

int find_old(results *result, staff_array *staff, int size) {
  return find_by_job(result, staff, size, is_more);
}
