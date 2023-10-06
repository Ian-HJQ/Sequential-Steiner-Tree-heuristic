#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include"pointlist.h"

//#define M 51

struct point {
    int index;
    int id;
    int x;
    int y;
    struct point* next;
};

struct listCDT {
    struct point* head;
};

//struct point *head = NULL;
//struct point *current = NULL;

//create an empty pointlist
pointlistADT emptyPlist() {
    pointlistADT list;

    list = (pointlistADT)malloc(sizeof(*list));
    list->head = NULL;
    return list;
}

//make a copy of the pointlist
void plistcpy(pointlistADT dest, pointlistADT src) {
    point* ptr = src->head;
    //dest = emptyList();
    while (ptr != NULL) {
        insertpoint(ptr->id, ptr->x, ptr->y, dest);
        ptr = ptr->next;
    }
    return;
}

//print pointlist
void printpList(pointlistADT list) {
    if (list->head == NULL) return;

    struct point* ptr = list->head;

    //start from the beginning
    while (ptr != NULL) {
        //printf("%d ", ptr->index);
        printf("%d %d %d %d\n", ptr->index, ptr->id, ptr->x, ptr->y);
        ptr = ptr->next;
    }
    return;
}

//output pointlist to a file
void fprintpList(FILE* fp, char* str, pointlistADT list) {
    if (list->head == NULL) return;

    struct point* ptr = list->head;

    //start from the beginning
    while (ptr != NULL) {
        //printf("%d ", ptr->index);
        fprintf(fp, "%s%d\n", str, ptr->id);
        ptr = ptr->next;
    }
    return;
}

//insert a point to a pointlist
void insertpoint(int id, int x, int y, pointlistADT list) {
    //create a link
    struct point* link = (struct point*)malloc(sizeof(struct point));
    
    link->id = id;
    link->x = x;
    link->y = y;
    link->next = NULL;

    point* ptr = list->head;
    if (ptr == NULL){
        list->head = link;
        link->index =0;
    }
    else{
        while (ptr->next != NULL){
            ptr = ptr->next;
        }
        ptr->next = link;
        link->index = ptr->index + 1;
    }
    return;
}

//delete a point with the given index
void delPoint(int index, pointlistADT list) {

    //start from the first link
    struct point* current = list->head;
    struct point* previous = NULL;

    //if list is empty
    if (list->head == NULL) {
        return;
    }

    //navigate through list
    while (current->index != index) {

        //if it is last point, no match
        if (current->next == NULL) {
            return;
        }
        else {
            //store reference to current link
            previous = current;
            //move to next link
            current = current->next;
        }
    }

    //found a match, update the link
    if (current == list->head) {
        //change first to point to next link
        list->head = list->head->next;
    }
    else {
        //bypass the current link
        previous->next = current->next;
    }
    
    //update index
    point* ptr = current->next;
    while (ptr != NULL) {
        ptr->index--;
        ptr = ptr->next;
    }
    
    free(current);

    return;
}

//delete a point with the given id
void delPointbyid(int id, pointlistADT list) {

    //start from the first link
    struct point* current = list->head;
    struct point* previous = NULL;

    //if list is empty
    if (list->head == NULL) {
        return;
    }

    //navigate through list
    while (current->id != id) {

        //if it is last point, no match
        if (current->next == NULL) {
            return;
        }
        else {
            //store reference to current link
            previous = current;
            //move to next link
            current = current->next;
        }
    }

    //found a match, update the link
    if (current == list->head) {
        //change first to point to next link
        list->head = list->head->next;
    }
    else {
        //bypass the current link
        previous->next = current->next;
    }
    
    //update index
    point* ptr = current->next;
    while (ptr != NULL) {
        ptr->index--;
        ptr = ptr->next;
    }
    
    free(current);

    return;
}

//check if poinlist is empty
bool ispEmpty(pointlistADT list) {
   return (list->head == NULL);
}

//return length of pointlist
int plength(pointlistADT list) {
    if (list->head == NULL) return 0;

    point* ptr = list->head;
    int count = 1;
    while (ptr->next != NULL) {
        ptr = ptr->next;
        count++;
    }
   return count;
}

//return point with given index
point* getPoint(int index, pointlistADT list){
    //start from the first link
   struct point* current = list->head;

   //if list is empty
   if(list->head == NULL) {
      return NULL;
   }

   //navigate through list
   while(current->index != index) {

      //if it is last point, no match
      if(current->next == NULL) {
         return NULL;
      } else {
         //go to next link
         current = current->next;
      }
   }

   //if data found, return the current Link
   return (current);
}

//return point with given id
point* getPointbyid(int id, pointlistADT list){
    //start from the first link
   struct point* current = list->head;

   //if list is empty
   if(list->head == NULL) {
      return NULL;
   }

   //navigate through list
   while(current->id != id) {

      //if it is last point, no match
      if(current->next == NULL) {
         return NULL;
      } else {
         //go to next link
         current = current->next;
      }
   }

   //if data found, return the current Link
   return (current);
}

//return data with given index
int getx(int index, pointlistADT list) {

   //start from the first link
   struct point* current = list->head;

   //if list is empty
   if(list->head == NULL) {
      return -1;
   }

   //navigate through list
   while(current->index != index) {

      //if it is last point, no match
      if(current->next == NULL) {
         return -1;
      } else {
         //go to next link
         current = current->next;
      }
   }

   //if data found, return the current Link
   return current->x;
}

int gety(int index, pointlistADT list) {

   //start from the first link
   struct point* current = list->head;

   //if list is empty
   if(list->head == NULL) {
      return -1;
   }

   //navigate through list
   while(current->index != index) {

      //if it is last point, no match
      if(current->next == NULL) {
         return -1;
      } else {
         //go to next link
         current = current->next;
      }
   }

   //if data found, return the current Link
   return current->y;
}

/*
void sort(pointlistADT list) {

    int i, j, k, tempIndex; 
    char tempData[M];
    struct point *current;
    struct point *next;

    int size = length(list);
    k = size ;

    for ( i = 0 ; i < size - 1 ; i++, k-- ) {
        current = list->head;
        next = list->head->next;

        for ( j = 1 ; j < k ; j++ ) {
            
        if (strcmp(current->data, next->data) > 0) {
            strcpy(tempData, current->data);
            strcpy(current->data, next->data);
            strcpy(next->data, tempData);
            
            tempIndex = current->index;
            current->index = next->index;
            next->index = tempIndex;
            
         }

        current = current->next;
        next = next->next;
        }
    }
}
*/