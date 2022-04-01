#include "inout.h"
#include "utils.h"

#include <time.h>

#define TRIES 10

int main()
{

    FILE *fin = fopen(NAME, "r");
    if (fin == NULL) {
        return -1;
    }

    job_array jobs = {.job_names = NULL, 0, 0};
    read_jobs(fin, &jobs);
    staff_array staff;
    init_staff(&staff);
    read_people(fin, &staff, jobs.size);
    double time_old = 0;
    double time_young = 0;
    results young, old;
    fclose(fin);
    for (int i = 0; i < TRIES; ++i) {
        clock_t t = clock();
        find_young(&young, &staff, jobs.size);
        free_result(&young);
        time_young += (clock() - t) / (double) (CLOCKS_PER_SEC);
        t = clock();

        find_old(&old, &staff, jobs.size);
        free_result(&old);
        time_old += (clock() - t) / (double) (CLOCKS_PER_SEC);
    }

    time_old /= TRIES;
    time_young /= TRIES;

    FILE *fout = fopen(OUT, "w");
    if (fout == NULL) {
        return -1;
    }
    fprintf((stdout), "time_old = %lf seconds\n", time_old);
    fprintf((stdout), "time_young = %lf seconds\n", time_young);

    find_young(&young, &staff, jobs.size);
    show_list_by_job(&young, fout);
    free_result(&young);

    find_old(&old, &staff, jobs.size);
    show_list_by_job(&old, fout);
    free_result(&old);
    free_people(&staff);
    free_jobs(&jobs);

    return 0;
}
