#include <stdio.h>
#include <stdlib.h>
void good_echo();

int main()
{
  printf("Enter the text here: \n");
  good_echo();
  return 0;
}

void good_echo()
{
  char buf[8];
  fgets(buf, 8, stdin);
  fputs(buf, stdout);
  if (buf[0] != '\n' && buf[1] != '\n' && buf[2] != '\n' && buf[3] != '\n'  
	&& buf[4] != '\n' && buf[5] != '\n' && buf[6] != '\n' && buf[7] != '\n')
    good_echo();   //if there is no \n in the string, then recall echo 
}
