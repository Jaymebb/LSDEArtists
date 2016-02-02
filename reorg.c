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
			if (person->location != knows->location) continue;

			// friendship must be mutual. Also, if the other appeared before,
      // there'll be an update.. for now, cyclism caused by this treated via
      // copy.
			for (knows_offset2 = knows->knows_first;
				knows_offset2 < knows->knows_first + knows->knows_n;
				knows_offset2++) {

				if (knows_map[knows_offset2] == person_offset) {
          new_knows_n++;
          if(new_knows_first == 0)
            new_knows_first = new_knows_offset;
          fwrite(&knows_map[knows_offset], sizeof(unsigned int), 1, knows_out);
          new_knows_offset++;
          break;
        }
      }
    }
    // really ugly for now, but... Cyclic dependency is ugly.
    copy = clone_person(person);
    copy->knows_first = new_knows_first;
    copy->knows_n = new_knows_n;
	  fwrite(copy, sizeof(Person), 1, person_out);
  }
}

int main(int argc, char *argv[]) {
	char* person_output_file   = makepath(argv[1], "person2",  "bin");
  char* knows_output_file    = makepath(argv[1], "knows2",   "bin");
	person_map   = (Person *)         mmapr(makepath(argv[1], "person",   "bin"), &person_length);
	interest_map = (unsigned short *) mmapr(makepath(argv[1], "interest", "bin"), &interest_length);
	knows_map    = (unsigned int *)   mmapr(makepath(argv[1], "knows",    "bin"), &knows_length);

	// this does not do anything yet. But it could...

  person_out = open_binout(person_output_file);
  knows_out = open_binout(knows_output_file);

  friends_in_same_city();
  // Love mmap!
	return 0;
}

