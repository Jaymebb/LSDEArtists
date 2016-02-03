#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "utils.h"

Person *person_map;
unsigned int *knows_map;
unsigned short *interest_map;
unsigned long person_length, knows_length, interest_length;

unsigned long new_knows_offset = 0;
unsigned long new_person_offset = 0;
FILE *knows_out;
FILE *person_out;

void friends_in_same_city(){
	unsigned int person_offset;
	unsigned long knows_offset, knows_offset2;
  unsigned long new_knows_first, new_knows_n;

	Person *person, *knows, *copy;

	for (person_offset = 0; person_offset < person_length/sizeof(Person); person_offset++) {
		person = &person_map[person_offset];
    new_knows_n = new_knows_first = 0;

		for (knows_offset = person->knows_first;
			knows_offset < person->knows_first + person->knows_n;
			knows_offset++) {
			knows = &person_map[knows_map[knows_offset]];
			if (person->location != knows->location){
        continue;
      }
      new_knows_n++;
      if(new_knows_first == 0)
        new_knows_first = new_knows_offset;
      fwrite(&knows_map[knows_offset], sizeof(unsigned int), 1, knows_out);
      new_knows_offset++;
    }
    person->knows_first = new_knows_first;
    person->knows_n = new_knows_n;
  }
}

int main(int argc, char *argv[]) {
  char* knows_output_file    = makepath(argv[1], "knows2",   "bin");
	person_map   = (Person *)         mmaprw(makepath(argv[1], "person",   "bin"), &person_length);
	knows_map    = (unsigned int *)   mmapr(makepath(argv[1], "knows",    "bin"), &knows_length);

	// this does not do anything yet. But it could...

  knows_out = open_binout(knows_output_file);

  friends_in_same_city();
  // Love mmap!
	return 0;
}

