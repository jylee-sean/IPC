#include <stdio.h>

int main()
{


	char str[10];

	while(1)
	{
		printf("result: %d\n", sscanf("%s",str)     );

		printf("\n*********\n");
		printf("%s\n", str);
	}
	return 0;
}