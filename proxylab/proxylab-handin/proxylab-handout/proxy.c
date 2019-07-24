#include <stdio.h>
#include <stdlib.h>
#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 16777216 
#define MAX_OBJECT_SIZE 8388608 

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";
static const char *Connection_hdr = "Connection: close\r\n";
static const char *Proxy_Connection_hdr = "Proxy_Connection: close\r\n";
static const char *endline = "\r\n";

/*Double linklist*/
typedef struct listNode {
  char buf[MAX_OBJECT_SIZE];
  char uri[MAXLINE];
  int Valuesize;
  struct listNode *next;
  struct listNode *prev;
} listNode;

typedef struct doublyLinkedList {
  listNode *head;
  listNode *tail;
  int LinkedListSize;
} doublyLinkedList;
void insertHead(doublyLinkedList *listPtr, listNode *add);
listNode *removeTail(doublyLinkedList *listPtr);
int checkCache(doublyLinkedList *listPtr, char* Uri, int Fd, int send);
listNode *deleteNode(doublyLinkedList *listPtr, char *Uri);
void checkCacheInfo(doublyLinkedList *listPtr);

void doit(int fd, char *port, doublyLinkedList *listPtr);
void parse_uri(char* uri, char* hostname, char* path, char* parsed_ur);
void clienterror(int fd, char *cause, char *errnum, 
		 char *shortmsg, char *longmsg);

/* most of the main are from tiny.c*/
int main(int argc, char **argv) 
{
    int listenfd, connfd;
    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
	doublyLinkedList list = {NULL, NULL, 0};
    
    /* Check command line args */
    if (argc != 2) {
	fprintf(stderr, "usage: %s <port>\n", argv[0]);
	exit(1);
    }
    
    Signal(SIGPIPE, SIG_IGN);
    listenfd = Open_listenfd(argv[1]);
    while (1) {
	clientlen = sizeof(clientaddr);
	connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen); //line:netp:tiny:accept
    Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE, 
                    port, MAXLINE, 0);
    printf("Accepted connection from (%s, %s)\n", hostname, port);
	doit(connfd, port, &list);                                             //line:netp:tiny:doit
	Close(connfd);                                            //line:netp:tiny:close
    }
}
/* $end tinymain */

/*
 * doit - handle one HTTP request/response transaction
 */
/* $begin doit */
/*part of doit are from tiny.c*/
void doit(int fd, char *port, doublyLinkedList *listPtr) 
{
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    rio_t rio, server_rio;
    char hostname[MAXLINE], path[MAXLINE], parsed_uri[MAXLINE], new_host[MAXLINE], new_port[MAXLINE];
	char *default_port = "80";  /*saved for default port*/
    int i;						/*loop counter*/
	int save_number = 0;		/*use for save the position of ':'*/
    char *save = "Host: ";		
    int clientfd;
	int send;  					/*1 to send, 0 to default*/
	listNode *savedCache; 		/*the node need to be saved*/
	listNode *removedCache;		/*the node that is already removed or going to be removed*/
	
    /* Read request line and headers */
    rio_readinitb(&rio, fd); 
    if (!rio_readlineb(&rio, buf, MAXLINE))  			//line:netp:doit:readrequest
        return;
    printf("%s", buf);
    sscanf(buf, "%s %s %s", method, uri, version);       //line:netp:doit:parserequest
	if (strcasecmp(method, "GET")) {                     //line:netp:doit:beginrequesterr
		clienterror(fd, method, "501", "Not Implemented",
                    "Tiny does not implement this method");
        return;
    }             
	else{
		save_number = 0;
		/*check the cache*/
		send = 1;
		printf("\ncheck saved cache: ");
		if(checkCache(listPtr, uri, fd, send) == 1){	/*check if the uri is already in the cache*/ 
			listNode *temp = (listNode*)malloc(sizeof(listNode)); 
			temp = listPtr -> head;
			if(temp != NULL){							/*check if the list is empty*/
				while(temp -> next != NULL)				/*check all nodes in the list*/
				{
					if(!strcmp(temp -> uri, uri)){
						removedCache = deleteNode(listPtr, uri);
						insertHead(listPtr, removedCache);	
						free(removedCache);
					}
					temp = temp -> next;
				}
				if(!strcmp(temp -> uri, uri)){
					removedCache = deleteNode(listPtr, uri);
					insertHead(listPtr, removedCache);
					free(removedCache);
				}
			}
			checkCacheInfo(listPtr);					/*easy check*/
			return;		
		}
		send = 0;										
		
		parse_uri(uri, hostname, path, parsed_uri);  	/* parse the uri*/
		for(i = 0; i < MAXLINE; i++){				    /*initialize variables*/
			new_host[i] = '\0';
			new_port[i] = '\0';
		}
		for(i = 0; i < MAXLINE; i++){	
			if(hostname[i] != ':')						/*separate hostname and port*/
				new_host[i] = hostname[i];
			else{
				save_number = i;
				break;
			}
		}
		if (save_number != 0){							/*uri has port*/
			for(i = save_number + 1; i < MAXLINE; i++){	
				new_port[i - save_number - 1] = hostname[i];
			}	
		}
		else
		  strcpy(new_port, default_port);				/*uri doesn't have port, set to default*/

		
	  
//		printf("host: %s,  port: %s", new_host, new_port);        //bug tester

		clientfd = Open_clientfd(new_host, new_port);	/*open client file discripter*/

	  
		strcpy(buf, parsed_uri);						/*make header buf*/
		strcat(buf, endline);
		strcat(buf, save);
		strcat(buf, new_host);
		strcat(buf, endline);
		strcat(buf, user_agent_hdr);
		strcat(buf, Connection_hdr);
		strcat(buf, Proxy_Connection_hdr);
		strcat(buf, endline);
	  
		rio_writen(clientfd, buf, strlen(buf)); 		/*send to server*/

    }                                                   //line:netp:doit:endrequesterr
	
    send = 0;
	printf("\ncheck if already in cache: ");
	if(checkCache(listPtr, uri, fd, send) == 0){		/*useless check*/
		savedCache = (listNode*)malloc(sizeof(listNode)); 
		strcpy(savedCache -> uri, uri);
	}
	send = 0;
		
    /*receive message from end server and send to the client*/
	Rio_readinitb(&server_rio, clientfd);	
    size_t num;
	int count = 0;

    while((num = Rio_readlineb(&server_rio,buf,MAXLINE)) != 0)
    {
		count += (int)num;								/*count for buf size*/
		strcat(savedCache -> buf, buf);					/*save buf in the cache*/
        Rio_writen(fd,buf,(int)num);					/*send buf to client*/
    }
	savedCache -> Valuesize = count;
	printf("proxy received %d bytes from server, and send %d bytes to client\n",count, count);
	
	if(savedCache -> Valuesize <= MAX_OBJECT_SIZE)		/*object is not oversize*/
	{
		/*total length is not oversize*/
		if(listPtr -> LinkedListSize + savedCache -> Valuesize <= MAX_CACHE_SIZE)
			insertHead(listPtr, savedCache);
		else{
			/*LRU*/
			while(listPtr -> LinkedListSize + savedCache -> Valuesize > MAX_CACHE_SIZE){
				removedCache = removeTail(listPtr);
				listPtr -> LinkedListSize -= removedCache -> Valuesize;
				free(removedCache);
			}
			insertHead(listPtr, savedCache);
			}
	}
	else
		printf("Out of range");
	free(savedCache);
    Close(clientfd);
	
	checkCacheInfo(listPtr);					/*check the sequence of the node in the list*/

 }
/* $end doit */

/*parse usi*/
void parse_uri(char* uri, char* hostname, char* path, char* parsed_uri)
{
  int i, save = 0;
  char *Http_test = "http://";
  char *get = "GET ";
  char *Http_1 = " HTTP/1.0";
  
  for(i = 0; i < 7; i ++){
    if(uri[i] != Http_test[i]){
      printf("input error");					/*check for http://*/
      exit(1);
    }
  }
  
  for(i = 0; i < MAXLINE; i++){					/*initailize variables*/
      hostname[i] = '\0';
	  path[i] = '\0';
	  parsed_uri[i] = '\0';
  }

  for(i = 7; i < MAXLINE; i++){					/*get hostname with port(if has one)*/
    if(uri[i] != '/'){
      hostname[i-7] = uri[i];
	}else{
      save = i;
      break;
    }
  }
  
  if(save != 0){								/*get the path*/
	for(i = save; i < MAXLINE; i ++){
		if(uri[i] != ' '){						
			path[i-save] = uri[i];
			parsed_uri[i-save+4] = uri[i];
		}
		else{
			save = i-save+4;
			break;
		}
	}
  }

  strcpy(parsed_uri, get);						/*make a parsed uri*/
  strcat(parsed_uri, path);
  strcat(parsed_uri, Http_1);
}  
/*from tiny.c*/
void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg) 
{
    char buf[MAXLINE], body[MAXBUF];

       /* Build the HTTP response body */
    sprintf(body, "<html><title>Tiny Error</title>");
    sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em>The Tiny Web server</em>\r\n", body);

       /* Print the HTTP response */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    sprintf(buf, "%sContent-type: text/html\r\n", buf);
    sprintf(buf, "%sContent-length: %d\r\n\r\n", buf, (int)strlen(body));
    rio_writen(fd, buf, strlen(buf));
    rio_writen(fd, body, strlen(body));
}

/* $end clienterror */

/*Double link list part*/

void insertHead(doublyLinkedList *listPtr, listNode* add) 
{
  listNode *temp = (listNode*)malloc(sizeof(listNode)); 
  strcpy(temp -> buf, add -> buf);
  strcpy(temp -> uri, add -> uri);
  temp -> Valuesize = add -> Valuesize;
  temp -> next = NULL;   //initail the temp node
  if(listPtr -> head == NULL && listPtr -> tail == NULL)
  {
    listPtr -> head = temp;
    listPtr -> tail = temp;
  }                     //if the list is empty 
  else
  {
    listPtr -> head -> prev = temp;
    temp -> next = listPtr -> head;
    listPtr -> head = temp; 
  }                    //if the list is not empty 
  
  listPtr -> LinkedListSize += temp -> Valuesize;	/*set the total list size*/
  return;
}

listNode *removeTail(doublyLinkedList *listPtr) {
	listNode *temp = (listNode*)malloc(sizeof(listNode));
	temp = listPtr -> tail;
	if(listPtr -> head -> next == NULL)
	{
		listPtr -> head = NULL;
		listPtr -> tail = NULL;
	}                   //if the list has only one node
	else
	{
		listPtr -> tail = temp -> prev;
		temp -> prev -> next = NULL;
	}                   //the list has more than one nodes

	return temp;         //return the node value
} 

int checkCache(doublyLinkedList *listPtr, char* Uri, int Fd, int send)  
{
	listNode *temp = (listNode*)malloc(sizeof(listNode)); 
	temp = listPtr -> head;
	if(temp != NULL){								/*the list is not empty*/
		while(temp -> next != NULL)					/*check all the nodes*/
		{
			if(!strcmp(temp -> uri, Uri)){			/*data in the cache*/
				if(send == 1)
					Rio_writen(Fd, temp -> buf, temp -> Valuesize);
				else if(send != 0)
					printf("\n\nreset error\n\n");
				printf("Cache hit!\n");			
				return 1;
			}	
			temp = temp -> next;
		}
		if(!strcmp(temp -> uri, Uri)){
			if(send == 1)
				Rio_writen(Fd, temp -> buf, temp -> Valuesize);
			else if(send != 0)
				printf("\n\nreset error\n\n");
			printf("Cache hit!\n");
			return 1;
		}		
	}
	printf("Cache miss!\n"); 						/*data not in the cache*/
	return 0;
}

listNode *deleteNode(doublyLinkedList *listPtr, char *Uri) 
{
	listNode *temp = (listNode*)malloc(sizeof(listNode));
	temp = listPtr -> head;
	if(listPtr -> head == NULL)						/*check list isEmpty*/
		return NULL;
	else
	{
		while(temp -> next != NULL)					
		{
			if(!strcmp(temp -> uri, Uri)){
				if(temp == listPtr -> head)			/*if the node is the first node*/
				{
					listPtr -> head = temp -> next;
					temp -> next -> prev = NULL;
				}
				else								/*if the node is the middle node*/
				{
					temp -> prev -> next = temp -> next;
					temp -> next -> prev = temp -> prev;
				}
				return temp;						
			}
			temp = temp -> next;
		}
		if(!strcmp(temp -> uri, Uri))
		{
													/*if the node is the only node*/
			if(temp == listPtr -> head && temp == listPtr -> tail)
			{
				listPtr -> head = NULL;
				listPtr -> tail = NULL;
			}
			else if(temp == listPtr -> tail)		/*if the node is the last node*/
			{
				listPtr -> tail = temp -> prev;
				temp -> prev -> next = NULL;
			}			
			return temp;
		}
	}
	printf("No such Node");

	return NULL;         //return the null value
} 

void checkCacheInfo(doublyLinkedList *listPtr) 		/*display all saved uri in nodes*/
{
	listNode *temp = (listNode*)malloc(sizeof(listNode)); 
	temp = listPtr -> head;
	if(temp != NULL){
		while(temp -> next != NULL)
		{
			printf("URI is: %s\n", temp -> uri);
			temp = temp -> next;
		}
		printf("URI is: %s\n", temp -> uri);
	}
	printf("Cache info print!\n"); 
	return;
}