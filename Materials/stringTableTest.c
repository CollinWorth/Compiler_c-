#include <stdio.h>

typedef enum { stHi, stBye, stTwoThings, stManagedBadStoCls } strid_t;

char * strget(strid_t id)
{
	switch (id)
	{
		case stHi:
			return "Hiya";
		
		case stBye:
			return "Bye";
		
		case stTwoThings:
			return "%s and %s";
			
        case stManagedBadStoCls:
            return "Managed type %s cannot have storage-class %s";

		default:
			return "strget: Invalid id";
	}
}

int main()
{
	printf("%s\n", strget(stHi));
	printf("..working...\n");
	printf(strget(stTwoThings), strget(stBye), strget(stHi));
    printf(strget(stManagedBadStoCls), "\n sillyString", "\n static");
	printf("%s\n", strget(stBye));
}


//  !@#$%^&*()_+=-
//  {}|[]\:;<>?,./~