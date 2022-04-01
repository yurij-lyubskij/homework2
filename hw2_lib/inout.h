#ifndef HW2_STATIC_INOUT_H
#define HW2_STATIC_INOUT_H
#include "utils.h"

#define REPEATS 2
#define NAME "test"
#define OUT "1.txt"

void show_list_by_job(results* result, FILE *fout);
void show_buff(FILE *fout, staff_array*staff);
int read_people(FILE *fin, staff_array *staff, uint16_t job_size);
void write_random_people(staff_array* staff, FILE *fout);
void write_jobs(FILE *fout);
void print_jobs(FILE *fout, job_array *jobs);
void print_people(FILE *fout, staff_array *staff);
int read_jobs(FILE *fin, job_array *jobs);
extern int rand_r (unsigned int *__seed);
#endif //HW2_STATIC_INOUT_H
