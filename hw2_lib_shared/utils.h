#ifndef HW2_UTILS_H
#define HW2_UTILS_H

#define DEFAULT_CAP 2
#define JOBS 10
#define MAX_LEN 50
#define MIN_AGE 18
#define MAX_AGE 120
#define MAX_EXP 110
#define MAX_PAY 150000
#define NAMES 7
#define PEOPLE 2000
// #define PEOPLE 20

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct staff_array {
    char**  name;
    char** surname;
    char* gender; //0 = man, 1 = woman
    uint8_t* age;
    uint8_t* experience;
    double* salary;
    uint16_t* job_index; //индекс в массиве должностей
    uint32_t size; //сколько людей в компании
    uint32_t capacity;
}staff_array; //структура динамических массивов вместо массива структур


typedef struct job_array{
    char ** job_names;
    uint16_t size;
    uint16_t capacity;
}job_array;

typedef struct results{
    staff_array* staff_by_job;
    uint8_t* age;
    uint16_t size;
}results;

char ** grow_str_array(char **buffer, uint16_t *capacity, uint16_t new_capacity);
int add_job(job_array *jobs, char *element);
void free_people();
int find_young(results* result, staff_array *staff, int size);
int find_old(results* result, staff_array *staff, int size);
void show_buff(FILE *fout, staff_array*staff);
void init_staff(staff_array *staff);
int init_result(results* result, int size, int max);
int add_elem(staff_array *staff, staff_array *arr, uint16_t num);
int alloc_buff(staff_array *staff, int new_capacity);
void heap_sort(staff_array *staff, int n);
void sort_by_job(results* result);
int unite_buffers(results* temp, uint16_t count, results *result, int (*comp)(uint8_t l, uint8_t r));
int free_result(results *result);
void free_jobs(job_array *jobs);
void free_staff(staff_array *staff);
#endif //HW2_UTILS_H
