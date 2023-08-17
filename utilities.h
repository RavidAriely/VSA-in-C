#include <stdio.h>

/*Prints only failures
#define TESTS(x) if((x) == 0) { printf("FAILURE %s AT LINE %d\n", __FILE__, __LINE__); }; */


/*Prints both failures and successes*/
/*#define TESTS(x) ((x)==0)? printf("FAILURE %s AT LINE %d\n", __FILE__, __LINE__) : printf("SUCCESS\n");*/

#define TESTS(x) (x) ? printf("SUCCESS\n") : printf("\033[0;31mFAILURE: file %s line %d\033[0m\n", __FILE__, __LINE__);
