#include <gtest/gtest.h>

#define FILE_LEN 5500
extern "C" {
#include "utils.h"
#include "inout.h"
#ifdef REPEATS
#undef REPEATS
#define REPEATS 1
#endif
}



TEST(db, default) {
    EXPECT_EQ(0, 0);
}

TEST(db, test_search) {
    char output[FILE_LEN];
    char etalon[FILE_LEN] = "person № 0\n"
                            "name = костя\n"
                            "surname = антонов\n"
                            "gender = 0\n"
                            "age = 23\n"
                            "experience = 3\n"
                            "salary = 30000.000000\n"
                            "job_index = 0\n"
                            "\n"
                            "person № 0\n"
                            "name = анна\n"
                            "surname = беляева\n"
                            "gender = 1\n"
                            "age = 25\n"
                            "experience = 5\n"
                            "salary = 500000.000000\n"
                            "job_index = 1\n"
                            "\n"
                            "\n"
                            "person № 0\n"
                            "name = костя\n"
                            "surname = антонов\n"
                            "gender = 0\n"
                            "age = 23\n"
                            "experience = 3\n"
                            "salary = 30000.000000\n"
                            "job_index = 0\n"
                            "\n"
                            "person № 0\n"
                            "name = анна\n"
                            "surname = беляева\n"
                            "gender = 1\n"
                            "age = 25\n"
                            "experience = 5\n"
                            "salary = 500000.000000\n"
                            "job_index = 1\n"
                            "\n"
                            "\n";

    FILE *fout = fmemopen(output, FILE_LEN, "w");
    job_array jobs = {.job_names = NULL, 3, 3};
    char* job_s[] = {"продавец", "программист", "грузчик"};
    jobs.job_names = job_s;
    staff_array staff;
    staff.size = 2;
    staff.capacity = 2;
    char gender[] = {0, 1};
    staff.gender = gender;
    char* surname[] = {"антонов", "беляева"};
    staff.surname = surname;
    char* name[] = {"костя", "анна"};
    staff.name = name;
    uint8_t age[] = {23, 25};
    staff.age = age;
    uint8_t experience[] = {3, 5};
    staff.experience = experience;
    double salary[] = {30000, 500000};
    staff.salary = salary;
    uint16_t job_index[] = {0, 1};
    staff.job_index = job_index;
    results young, old;
    find_young(&young, &staff, jobs.size);
    show_list_by_job(&young, fout);
    find_old(&old, &staff, jobs.size);
    show_list_by_job(&old, fout);
    fclose(fout);
    free_result(&young);
    free_result(&old);
    EXPECT_STRCASEEQ(output, etalon);
}


TEST(db, test_write) {
    char output[FILE_LEN];
    char etalon[FILE_LEN] = "3\n"
                            "продавец\n"
                            "программист\n"
                            "грузчик\n"
                            "4\n"
                            "костя\n"
                            "антонов\n"
                            "0\n"
                            "23\n"
                            "3\n"
                            "30000.000000\n"
                            "0\n"
                            "анна\n"
                            "беляева\n"
                            "1\n"
                            "25\n"
                            "5\n"
                            "500000.000000\n"
                            "1\n"
                            "костя\n"
                            "антонов\n"
                            "0\n"
                            "23\n"
                            "3\n"
                            "30000.000000\n"
                            "0\n"
                            "анна\n"
                            "беляева\n"
                            "1\n"
                            "25\n"
                            "5\n"
                            "500000.000000\n"
                            "1\n";

    FILE *fout = fmemopen(output, FILE_LEN, "w");
    job_array jobs = {.job_names = NULL, 3, 3};
    char* job_s[] = {"продавец", "программист", "грузчик"};
    jobs.job_names = job_s;
    staff_array staff;
    staff.size = 2;
    staff.capacity = 2;
    char gender[] = {0, 1};
    staff.gender = gender;
    char* surname[] = {"антонов", "беляева"};
    staff.surname = surname;
    char* name[] = {"костя", "анна"};
    staff.name = name;
    uint8_t age[] = {23, 25};
    staff.age = age;
    uint8_t experience[] = {3, 5};
    staff.experience = experience;
    double salary[] = {30000, 500000};
    staff.salary = salary;
    uint16_t job_index[] = {0, 1};
    staff.job_index = job_index;
    print_jobs(fout, &jobs);
    print_people(fout, &staff);
    fclose(fout);
    EXPECT_STRCASEEQ(output, etalon);
}

TEST(db, test_full) {
    char output[FILE_LEN];
    char input[FILE_LEN] = "3\n"
                            "продавец\n"
                            "программист\n"
                            "грузчик\n"
                            "4\n"
                            "костя\n"
                            "антонов\n"
                            "0\n"
                            "23\n"
                            "3\n"
                            "30000.000000\n"
                            "0\n"
                            "анна\n"
                            "беляева\n"
                            "1\n"
                            "25\n"
                            "5\n"
                            "500000.000000\n"
                            "1\n"
                            "костя\n"
                            "антонов\n"
                            "0\n"
                            "23\n"
                            "3\n"
                            "30000.000000\n"
                            "0\n"
                            "анна\n"
                            "беляева\n"
                            "1\n"
                            "25\n"
                            "5\n"
                            "500000.000000\n"
                            "1\n";
    char etalon[FILE_LEN] = "person № 0\n"
                            "name = костя\n"
                            "surname = антонов\n"
                            "gender = 0\n"
                            "age = 23\n"
                            "experience = 3\n"
                            "salary = 30000.000000\n"
                            "job_index = 0\n"
                            "person № 1\n"
                            "name = костя\n"
                            "surname = антонов\n"
                            "gender = 0\n"
                            "age = 23\n"
                            "experience = 3\n"
                            "salary = 30000.000000\n"
                            "job_index = 0\n"
                            "\n"
                            "person № 0\n"
                            "name = анна\n"
                            "surname = беляева\n"
                            "gender = 1\n"
                            "age = 25\n"
                            "experience = 5\n"
                            "salary = 500000.000000\n"
                            "job_index = 1\n"
                            "person № 1\n"
                            "name = анна\n"
                            "surname = беляева\n"
                            "gender = 1\n"
                            "age = 25\n"
                            "experience = 5\n"
                            "salary = 500000.000000\n"
                            "job_index = 1\n"
                            "\n"
                            "\n"
                            "person № 0\n"
                            "name = костя\n"
                            "surname = антонов\n"
                            "gender = 0\n"
                            "age = 23\n"
                            "experience = 3\n"
                            "salary = 30000.000000\n"
                            "job_index = 0\n"
                            "person № 1\n"
                            "name = костя\n"
                            "surname = антонов\n"
                            "gender = 0\n"
                            "age = 23\n"
                            "experience = 3\n"
                            "salary = 30000.000000\n"
                            "job_index = 0\n"
                            "\n"
                            "person № 0\n"
                            "name = анна\n"
                            "surname = беляева\n"
                            "gender = 1\n"
                            "age = 25\n"
                            "experience = 5\n"
                            "salary = 500000.000000\n"
                            "job_index = 1\n"
                            "person № 1\n"
                            "name = анна\n"
                            "surname = беляева\n"
                            "gender = 1\n"
                            "age = 25\n"
                            "experience = 5\n"
                            "salary = 500000.000000\n"
                            "job_index = 1\n"
                            "\n"
                            "\n";

    FILE *fin = fmemopen(input, FILE_LEN, "r");
    FILE *fout = fmemopen(output, FILE_LEN, "w");
    job_array jobs = {.job_names = NULL, 0, 0};
    read_jobs(fin, &jobs);
    staff_array staff;
    init_staff(&staff);
    read_people(fin, &staff, jobs.size);
    double time_old = 0;
    double time_young = 0;
    results young, old;
    fclose(fin);
    find_young(&young, &staff, jobs.size);
    show_list_by_job(&young, fout);
    free_result(&young);

    find_old(&old, &staff, jobs.size);
    show_list_by_job(&old, fout);
    fclose(fout);
    free_result(&old);

    free_people(&staff);
    free_jobs(&jobs);
    EXPECT_STRCASEEQ(output, etalon);
}


