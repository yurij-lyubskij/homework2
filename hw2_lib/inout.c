#include "inout.h"

void show_buff(FILE *fout, staff_array *staff) {
  for (uint32_t i = 0; i < staff->size; ++i) {
    fprintf(fout, "person № %u\n", i);
    fprintf(fout, "name = %s\n", staff->name[i]);
    fprintf(fout, "surname = %s\n", staff->surname[i]);
    fprintf(fout, "gender = %d\n", staff->gender[i]);
    fprintf(fout, "age = %d\n", staff->age[i]);
    fprintf(fout, "experience = %d\n", staff->experience[i]);
    fprintf(fout, "salary = %lf\n", staff->salary[i]);
    fprintf(fout, "job_index = %d\n", staff->job_index[i]);
  }
}

void print_jobs(FILE *fout, job_array *jobs) {
  fprintf(fout, "%d\n", jobs->size);
  for (uint32_t i = 0; i < jobs->size; ++i) {
    fprintf(fout, "%s\n", jobs->job_names[i]);
  }
}

void print_people(FILE *fout, staff_array *staff) {
  fprintf(fout, "%u\n", staff->size * REPEATS);
  for (int j = 0; j < REPEATS; ++j) {
    for (uint32_t  i = 0; i < staff->size; ++i) {
      fprintf(fout, "%s\n", staff->name[i]);
      fprintf(fout, "%s\n", staff->surname[i]);
      fprintf(fout, "%d\n", staff->gender[i]);
      fprintf(fout, "%d\n", staff->age[i]);
      fprintf(fout, "%d\n", staff->experience[i]);
      fprintf(fout, "%lf\n", staff->salary[i]);
      fprintf(fout, "%d\n", staff->job_index[i]);
    }
  }
}

char *string_input(FILE *fin) {
  char temp[MAX_LEN];
  int args_read = fscanf(fin, "%49s", temp);
  if (args_read != 1) {
    return NULL;
  }
  char *buff = malloc(strlen(temp) + 1);
  if (buff == NULL) {
    return NULL;
  }
  strncpy(buff, temp, strlen(temp) + 1);
  return buff;
}

int read_person(FILE *fin, staff_array *staff, uint16_t i, uint16_t job_count) {
  if (i >= staff->capacity) {
    printf("1\n");
    return -1;
  }
  staff->name[i] = string_input(fin);
  if (staff->name[i] == NULL) {
    printf("2\n");
    return -1;
  }
  staff->surname[i] = string_input(fin);
  if (staff->surname[i] == NULL) {
    printf("3\n");
    free(staff->name[i]);
    free_people(staff);
    return -1;
  }
  int args_read = fscanf(fin, "%hu", (uint16_t *)&(staff->gender[i]));
  if (args_read != 1 || ((staff->gender[i]) != 0 && (staff->gender[i]) != 1)) {
    printf("4\n");
    free(staff->name[i]);
    free(staff->surname[i]);
    free_people(staff);
    return -1;
  }
  args_read = fscanf(fin, "%hu", (uint16_t *)&(staff->age[i]));
  if (args_read != 1 || (staff->age[i] < MIN_AGE) ||
      (staff->age[i] > MAX_AGE)) {
    printf("5\n");
    free(staff->name[i]);
    free(staff->surname[i]);
    free_people(staff);
    return -1;
  }

  args_read = fscanf(fin, "%hu", (uint16_t *)&(staff->experience[i]));
  if (args_read != 1 || (staff->experience[i] > MAX_EXP) ||
      (staff->experience[i] >= staff->age[i])) {
    printf("6\n");
    free(staff->name[i]);
    free(staff->surname[i]);
    free_people(staff);
    return -1;
  }

  args_read = fscanf(fin, "%lf", &(staff->salary[i]));
  if (args_read != 1 || (staff->salary[i] < 0)) {
    printf("7\n");
    free(staff->name[i]);
    free(staff->surname[i]);
    free_people(staff);
    return -1;
  }
  args_read = fscanf(fin, "%hu\n", &(staff->job_index[i]));
  if (args_read != 1 || staff->job_index[i] >= job_count) {
    printf("8\n");
    free(staff->name[i]);
    free(staff->surname[i]);
    free_people(staff);
    return -1;
  }
  staff->size++;
  return 0;
}

int read_jobs(FILE *fin, job_array *jobs) {
  uint16_t size;
  int args_read = fscanf(fin, "%hu\n", &size);
  if (args_read != 1) {
    return -1;
  }
  jobs->job_names = grow_str_array(jobs->job_names, &(jobs->capacity), size);
  for (uint16_t i = 0; i < size; ++i) {
    char *position = string_input(fin);
    add_job(jobs, position);
  }
  return 0;
}

int read_people(FILE *fin, staff_array *staff, uint16_t job_size) {
  uint16_t size;
  int args_read = fscanf(fin, "%hu\n", &size);
  if (args_read != 1) {
    return -1;
  }
  if (size >= staff->capacity) {
    int fail = alloc_buff(staff, size + 1);
    if (fail) {
      return -1;
    }
  }
  for (int i = 0; i < size; i++) {
    int fail = read_person(fin, staff, i, job_size);
    if (fail) {
      printf("i = %d", i);
      return -1;
    }
  }
  return 0;
}

void show_list_by_job(results *result, FILE *fout) {
  for (int i = 0; i < result->size; ++i) {
    show_buff(fout, result->staff_by_job + i);
    fprintf(fout, "\n");
  }
}

void write_jobs(FILE *fout) {
  job_array jobs = {.job_names = NULL, 0, 0};
  char *jobnames[JOBS] = {"программист", "инженер",  "продавец", "грузчик",
                          "плотник",     "менеджер", "блоггер",  "бизнесмен",
                          "госслужащий", "спортсмен"};
  jobs.job_names = grow_str_array(jobs.job_names, &(jobs.capacity), JOBS);
  for (int i = 0; i < JOBS; ++i) {
    add_job(&jobs, *(jobnames + i));
  }
  print_jobs(fout, &jobs);
  free(jobs.job_names);
}

void write_random_people(staff_array *staff, FILE *fout) {
  init_staff(staff);
  alloc_buff(staff, PEOPLE);
  char *male_surnames[NAMES] = {"Иванов",  "Петров",   "Сидоров", "Измайлов",
                                "Макаров", "Алексеев", "Павлов"};
  char *male_names[NAMES] = {"Борис", "Глеб",    "Иван",   "Петр",
                             "Артём", "Евгений", "Дмитрий"};
  char *female_surnames[NAMES] = {"Кузнецова",  "Разина",  "Тихонова",
                                  "Третьякова", "Пушкина", "Добрынина",
                                  "Боголюбова"};
  char *female_names[NAMES] = {"Анна",  "Юлия",  "Екатерина", "Надежда",
                               "Дарья", "Софья", "Елизавета"};
  for (int i = 0; i < PEOPLE; i++) {
    int gender = rand_r(0) % 2;
    staff->name[staff->size] = *(male_names + rand_r(0) % NAMES);
    staff->surname[staff->size] = *(male_surnames + rand_r(0) % NAMES);
    if (gender) {
      staff->name[staff->size] = *(female_names + rand_r(0) % NAMES);
      staff->surname[staff->size] = *(female_surnames + rand_r(0) % NAMES);
    }
    staff->gender[staff->size] = gender;
    int age = MIN_AGE + rand_r(0) % (MAX_AGE - MIN_AGE);
    staff->age[staff->size] = age;
    staff->experience[staff->size] = rand_r(0) % (age - MIN_AGE + 1);
    double pay = rand_r(0) % MAX_PAY;
    staff->salary[staff->size] = pay;
    staff->job_index[staff->size] = rand_r(0) % JOBS;
    staff->size++;
  }
  print_people(fout, staff);
}
