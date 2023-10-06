#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include"edgelist.h"

//#define M 51

struct edge {
    int index;
    int x0;
    int y0;
    int x1;
    int y1;
    struct edge* next;
};

struct listCDT {
    struct edge* head;
};

//struct edge *head = NULL;
//struct edge *current = NULL;

edgelistADT emptyElist() {
    edgelistADT list;

    list = (edgelistADT)malloc(sizeof(*list));
    list->head = NULL;
    return list;
}

void elistcpy(edgelistADT dest, edgelistADT src) {
    edge* ptr = src->head;
    //dest = emptyList();
    while (ptr != NULL) {
        insertedge(ptr->x0, ptr->y0, ptr->x1, ptr->y1, dest);
        ptr = ptr->next;
    }
    return;
}

//display the list
void printeList(edgelistADT list) {
    if (list->head == NULL) return;

    struct edge* ptr = list->head;

    //start from the beginning
    while (ptr != NULL) {
        //printf("%d ", ptr->index);
        printf("%d %d %d %d %d\n", ptr->index, ptr->x0, ptr->y0, ptr->x1, ptr->y1);
        ptr = ptr->next;
    }
    return;
}

void fprinteList(FILE* fp, char* str, edgelistADT list) {
    if (list->head == NULL) return;

    struct edge* ptr = list->head;

    //start from the beginning
    while (ptr != NULL) {
        //printf("%d ", ptr->index);
        fprintf(fp, "%s%d %d %d %d\n", str, ptr->x0, ptr->y0, ptr->x1, ptr->y1);
        ptr = ptr->next;
    }
    return;
}

void insertedge(int x0, int y0, int x1, int y1, edgelistADT list) {
   if((x0 == x1) && (y0 == y1)){ //start and end point are same
      return;
   }

    //create a link
    struct edge* link = (struct edge*)malloc(sizeof(struct edge));
    
    //link->index = index;
    link->x0 = x0;
    link->y0 = y0;
    link->x1 = x1;
    link->y1 = y1;
    link->next = NULL;

    edge* ptr = list->head;
    if (ptr == NULL){
        list->head = link;
        link->index = 0;
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

void delEdge(int x0, int y0, int x1, int y1, edgelistADT list) {

    //start from the first link
    struct edge* current = list->head;
    struct edge* previous = NULL;

    //if list is empty
    if (list->head == NULL) {
        return;
    }

    

    //navigate through list
    while (true) {
        if ((current->x0 == x0) && (current->y0 == y0) && (current->x1 == x1) && (current->y1 == y1)){
            break;
        }

        //if it is last edge, no match
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
        //change first to edge to next link
        list->head = list->head->next;
    }
    else {
        //bypass the current link
        previous->next = current->next;
    }
    
    //update index
    edge* ptr = current->next;
    while (ptr != NULL) {
        ptr->index--;
        ptr = ptr->next;
    }
    
    free(current);

    return;
}

void delEdgebyindex(int index, edgelistADT list) {

    //start from the first link
    struct edge* current = list->head;
    struct edge* previous = NULL;

    //if list is empty
    if (list->head == NULL) {
        return;
    }

    

    //navigate through list
    while (true) {
        if (current->index == index){
            break;
        }

        //if it is last edge, no match
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
        //change first to edge to next link
        list->head = list->head->next;
    }
    else {
        //bypass the current link
        previous->next = current->next;
    }
    
    //update index
    edge* ptr = current->next;
    while (ptr != NULL) {
        ptr->index--;
        ptr = ptr->next;
    }
    
    free(current);

    return;
}

bool iseEmpty(edgelistADT list) {
   return (list->head == NULL);
}

int elength(edgelistADT list) {
    if (list->head == NULL) return 0;

    edge* ptr = list->head;
    int count = 1;
    while (ptr->next != NULL) {
        ptr = ptr->next;
        count++;
    }
   return count;
}

edge* getEdge(int index, edgelistADT list){
    //start from the first link
   struct edge* current = list->head;

   //if list is empty
   if(list->head == NULL) {
      return NULL;
   }

   //navigate through list
   while(current->index != index) {

      //if it is last edge, no match
      if(current->next == NULL) {
        return NULL;
      } else {
        //go to next link
        current = current->next;
      }
   }

   //if data found, return the current Link
   return current;
}


//return data with given index
int getx0(int index, edgelistADT list) {

   //start from the first link
   struct edge* current = list->head;

   //if list is empty
   if(list->head == NULL) {
      return -1;
   }

   //navigate through list
   while(current->index != index) {

      //if it is last edge, no match
      if(current->next == NULL) {
         return -1;
      } else {
         //go to next link
         current = current->next;
      }
   }

   //if data found, return the current Link
   return current->x0;
}

int gety0(int index, edgelistADT list) {

   //start from the first link
   struct edge* current = list->head;

   //if list is empty
   if(list->head == NULL) {
      return -1;
   }

   //navigate through list
   while(current->index != index) {

      //if it is last edge, no match
      if(current->next == NULL) {
         return -1;
      } else {
         //go to next link
         current = current->next;
      }
   }

   //if data found, return the current Link
   return current->y0;
}

int getx1(int index, edgelistADT list) {

   //start from the first link
   struct edge* current = list->head;

   //if list is empty
   if(list->head == NULL) {
      return -1;
   }

   //navigate through list
   while(current->index != index) {

      //if it is last edge, no match
      if(current->next == NULL) {
         return -1;
      } else {
         //go to next link
         current = current->next;
      }
   }

   //if data found, return the current Link
   return current->x1;
}

int gety1(int index, edgelistADT list) {

   //start from the first link
   struct edge* current = list->head;

   //if list is empty
   if(list->head == NULL) {
      return -1;
   }

   //navigate through list
   while(current->index != index) {

      //if it is last edge, no match
      if(current->next == NULL) {
         return -1;
      } else {
         //go to next link
         current = current->next;
      }
   }

   //if data found, return the current Link
   return current->y1;
}

/*
void sort(edgelistADT list) {

    int i, j, k, tempIndex; 
    char tempData[M];
    struct edge *current;
    struct edge *next;

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