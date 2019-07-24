#include "csapp.h"

int main(int argc, char **argv) 
{
  int n;
  rio_t rio;
  char buf[MAXLINE];
  
  if(argc == 2)          //if there exit a file name
  {
    int fd;              //file descriptor
    if((fd = Open(argv[1], O_RDONLY, 0)) < 0)
    {                    //check the value
      perror("open");
      exit(1);
    }
    /*if the letter read in is not zero, redo the read function*/ 
    while((n = Rio_readn(fd, buf, MAXLINE)) != 0)
      Rio_writen(STDOUT_FILENO, buf, n);
    exit(0);             //exit after the file input, skip the standard input
  }  
  
  Rio_readinitb(&rio, STDIN_FILENO);
  while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) 
    Rio_writen(STDOUT_FILENO, buf, n);

  return 0;
}



