#include "utils.h"


void swap(staff_array *staff, uint16_t l, uint16_t r) {
    char *name = staff->name[l];
    char *surname = staff->surname[l];
    short gender = staff->gender[l];
    short age = staff->age[l];
    short exp = staff->experience[l];
    double salary = staff->salary[l];
    uint16_t index = staff->job_index[l];

    staff->name[l] = staff->name[r];
    staff->surname[l] = staff->surname[r];
    staff->gender[l] = staff->gender[r];
    staff->age[l] = staff->age[r];
    staff->experience[l] = staff->experience[r];
    staff->salary[l] = staff->salary[r];
    staff->job_index[l] = staff->job_index[r];

    staff->name[r] = name;
    staff->surname[r] = surname;
    staff->gender[r] = gender;
    staff->age[r] = age;
    staff->experience[r] = exp;
    staff->salary[r] = salary;
    staff->job_index[r] = index;
}

int cmp(staff_array *staff, uint16_t l, uint16_t r) {
    return strcmp(staff->surname[l], staff->surname[r]);
}

void heapify(staff_array *staff, int n, int i) {
    int largest = i;
    int left = 2 * i + 1; // левый
    int right = 2 * i + 2; // правый

    // Если левый дочерний элемент больше корня
    if (left < n && cmp(staff, left, largest) > 0)
        largest = left;

    // Если правый дочерний элемент больше, чем самый большой элемент на данный момент
    if (right < n && cmp(staff, right, largest) > 0)
        largest = right;

    // Если самый большой элемент не корень
    if (largest != i) {
        swap(staff, i, largest);

        // Рекурсивно преобразуем в двоичную кучу затронутое поддерево
        heapify(staff, n, largest);
    }
}

// Основная функция, выполняющая пирамидальную сортировку
void heap_sort(staff_array *staff, int n) {
    // Построение кучи
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(staff, n, i);

    // Один за другим извлекаем элементы из кучи
    for (int i = n - 1; i >= 0; i--) {
        // Перемещаем текущий корень в конец
        swap(staff, 0, i);

        // вызываем процедуру heapify на уменьшенной куче
        heapify(staff, i, 0);
    }
}

char **grow_str_array(char **buffer, uint16_t *capacity, uint16_t new_capacity) {
    char **new_buffer = realloc(buffer, new_capacity * sizeof(char *));
    *capacity = new_capacity;
    return new_buffer;
}

void free_jobs(job_array *jobs) {
    for (int i = 0; i < jobs->size; i++) {
        free(jobs->job_names[i]);
    }
    free(jobs->job_names);
}

int add_job(job_array *jobs, char *element) {
    if (jobs->size >= jobs->capacity) {
        return -1;
    }
    jobs->job_names[jobs->size] = element;
    jobs->size++;
    return 0;
}


void init_staff(staff_array *staff) {
    staff->name = NULL;
    staff->surname = NULL;
    staff->gender = NULL;
    staff->age = NULL;
    staff->experience = NULL;
    staff->salary = NULL;
    staff->job_index = NULL;
    staff->size = 0;
    staff->capacity = 0;
}

int alloc_buff(staff_array *arr, unsigned long new_capacity) {
    if (new_capacity <= arr->capacity) {
        return -1;
    }
    unsigned long size = new_capacity * sizeof(char *);
    char **new_name = realloc(arr->name, size);
    if (new_name == NULL) {
        return -1;
    }
    size = new_capacity * sizeof(char *);
    char **new_surname = realloc(arr->surname, size);
    if (new_surname == NULL) {
        free(new_name);
        return -1;
    }
    size = new_capacity * sizeof(char);
    char *new_gender = realloc(arr->gender, size);
    if (new_gender == NULL) {
        free(new_surname);
        free(new_name);
        return -1;
    }
    size = new_capacity * sizeof(uint8_t);
    uint8_t *new_age = realloc(arr->age, size);
    if (new_age == NULL) {
        free(new_gender);
        free(new_surname);
        free(new_name);
        return -1;
    }
    size = new_capacity * sizeof(uint8_t);
    uint8_t *new_experience = realloc(arr->experience, size);
    if (new_experience == NULL) {
        free(new_age);
        free(new_gender);
        free(new_surname);
        free(new_name);
        return -1;
    }
    size = new_capacity * sizeof(double);
    double *new_salary = realloc(arr->salary, size);
    if (new_salary == NULL) {
        free(new_experience);
        free(new_age);
        free(new_gender);
        free(new_surname);
        free(new_name);
        return -1;
    }
    size = new_capacity * sizeof(uint16_t);
    uint16_t *new_job_index = realloc(arr->job_index, size);
    if (new_job_index == NULL) {
        free(new_salary);
        free(new_experience);
        free(new_age);
        free(new_gender);
        free(new_surname);
        free(new_name);
        return -1;
    }
    arr->name = new_name;
    arr->surname = new_surname;
    arr->gender = new_gender;
    arr->age = new_age;
    arr->experience = new_experience;
    arr->salary = new_salary;
    arr->job_index = new_job_index;
    arr->capacity = new_capacity;
    return 0;
}

int init_result(results *result, int size, int max) {
    result->size = size;
    result->age = calloc(result->size, sizeof(uint8_t));
    if (result->age == NULL) {
        return -1;
    }
    result->staff_by_job = calloc(result->size, sizeof(staff_array));
    if (result->staff_by_job == NULL) {
        free(result->age);
        return -1;
    }
    for (int i = 0; i < result->size; ++i) {
        init_staff(&result->staff_by_job[i]);
        result->age[i] = MIN_AGE - 1;
        if (max) {
            result->age[i] = MAX_AGE + 1;
        }
    }
    return 0;
}

int free_result(results *result) {
    free(result->age);
    for (int i = 0; i < result->size; ++i) {
        free_staff(&result->staff_by_job[i]);
    }
    free(result->staff_by_job);
    return 0;
}


int add_elem(staff_array *staff, staff_array *arr, uint16_t num) {
    if (arr->size >= arr->capacity) {
        unsigned long new_capacity = DEFAULT_CAP;
        if (new_capacity <  (arr->capacity)*2) {
            new_capacity = 2 * (arr->capacity);
        }
        int fail = alloc_buff(arr, new_capacity);
        if (fail) {
            return -1;
        }
    }
    if (num >= staff->size) {
        return -1;
    }
    arr->name[arr->size] = staff->name[num];
    arr->surname[arr->size] = staff->surname[num];
    arr->gender[arr->size] = staff->gender[num];
    arr->age[arr->size] = staff->age[num];
    arr->experience[arr->size] = staff->experience[num];
    arr->salary[arr->size] = staff->salary[num];
    arr->job_index[arr->size] = staff->job_index[num];
    arr->size++;
    return 0;
}


int is_less(short l, short r) {
    return l < r;
}

int is_more(short l, short r) {
    return l > r;
}

int find_by_job(results *result, staff_array *staff, int size, int (*comp)(short l, short r)) {
    int fail = init_result(result, size, comp(0,1)); // проверяем, ищем максимум, или минимум
    if (fail) {
        return -1;
    }
    for (unsigned long i = 0; i < staff->size; ++i) {
        uint16_t index = staff->job_index[i];
        staff_array* res = &((result->staff_by_job)[index]);
        if (staff->age[i] == result->age[index]) {
            fail = add_elem(staff, res, i);
            if (fail) {
                return -1;
            }
        }

        if (comp(staff->age[i], result->age[index])) {
            result->age[index] = staff->age[i];
            res->size = 0;
            fail = add_elem(staff, res, i);
            if (fail) {
                return -1;
            }
        }
    }
    sort_by_job(result);
    return 0;
}

int find_young(results *result, staff_array *staff, int size) {
    return find_by_job(result, staff, size, is_less);
}

int find_old(results *result, staff_array *staff, int size) {
    return find_by_job(result, staff, size, is_more);
}




void free_people(staff_array *staff) {
    for (size_t i = 0; i < staff->size; ++i) {
        free(staff->name[i]);
        free(staff->surname[i]);
    }
    free(staff->name);
    free(staff->surname);
    free(staff->gender);
    free(staff->age);
    free(staff->experience);
    free(staff->salary);
    free(staff->job_index);
    staff->size = 0;
    staff->capacity = 0;
}

void free_staff(staff_array *staff) {
    for (size_t i = 0; i < staff->size; ++i) {
        staff->name[i] = NULL;
        staff->surname[i] = NULL;
    }
    free(staff->name);
    free(staff->surname);
    free(staff->gender);
    free(staff->age);
    free(staff->experience);
    free(staff->salary);
    free(staff->job_index);
    staff->size = 0;
}


void sort_by_job(results *result) {
    for (int i = 0; i < result->size; ++i) {
        heap_sort(result->staff_by_job + i, (result->staff_by_job + i)->size);
    }
}


