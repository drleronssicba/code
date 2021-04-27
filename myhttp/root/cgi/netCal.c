#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define MAX 1024

//firstdata=100&lastdata=20
//strtok
void mycal(char *buff)
{
	int x,y;
	sscanf(buff, "firstdata=%d&lastdata=%d", &x, &y);

	printf("<html>\n");
	printf("<body>\n");
	printf("<h3>%d + %d = %d</h3>\n", x, y, x + y);
	printf("<h3>%d - %d = %d</h3>\n", x, y, x - y);
	printf("<h3>%d * %d = %d</h3>\n", x, y, x * y);
	if(y == 0){
		printf("<h3>%d / %d = %d</h3>, %s\n", x, y, -1, "(zero)");
		printf("<h3>%d %% %d = %d</h3>, %s\n", x, y, -1, "(zero)");
	}
	else{
		printf("<h3>%d / %d = %d</h3>\n", x, y, x / y);
		printf("<h3>%d %% %d = %d</h3>\n", x, y, x % y);
	}
	printf("</body>\n");
	printf("</html>\n");
}
int main()
{
    printf("200 OK HTTP/1.1\r\n");
    printf("\r\n");
   // char buf[MAX]={0};
   // memset(buf, 0 , MAX);
   // if(getenv("PARA")) {
   //     strcpy(buf, getenv("PARA"));
   // }
   // mycal(buf);
	return 0;
}

















