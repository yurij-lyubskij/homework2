#include "inout.h"
#include "utils.h"

int main() {
  FILE *fout = fopen(NAME, "w");
  staff_array staff;
  write_jobs(fout);
  write_random_people(&staff, fout);
  free_staff(&staff);
  fclose(fout);
}
