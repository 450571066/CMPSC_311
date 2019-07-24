/* Yanling Wang Fall 2018 */
#include <stdio.h>
#include <stdlib.h>
#include "doublylinkedlist.h"

void insertHead(doublyLinkedList *listPtr, int value) {
  listNode *temp = (listNode*)malloc(sizeof(listNode)); 
  temp -> value = value;
  temp -> prev = NULL;
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
  return;
}

int removeTail(doublyLinkedList *listPtr) {
  listNode *temp = (listNode*)malloc(sizeof(listNode));
  int ret;
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
  ret = temp -> value;  
  free(temp);         //free the location

  return ret;         //return the node value
} 

void showHead(doublyLinkedList *listPtr) {
  listNode *head = listPtr->head;
  printf("list head: ");
  while (head != NULL) {
    printf("%d ", head->value);
    head = head->next;
  } 
  printf("nil\n");
  return;
} 
void showTail(doublyLinkedList *listPtr) {
  listNode *tail = listPtr->tail;
  printf("list tail: ");
  while (tail != NULL) {
    printf("%d ", tail->value);
    tail = tail->prev;
  } 
  printf("nil\n");
  return;
} 
void freeList(doublyLinkedList *listPtr) { 
  listNode *head = listPtr->head;
  listNode *temp;
  while(head != NULL) {
    temp = head->next;
    free(head);
    head = temp;
  }
  listPtr->head = NULL;
  listPtr->tail = NULL;
  return;
}

