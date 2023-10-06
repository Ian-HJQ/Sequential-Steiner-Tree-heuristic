#ifndef EDGELIST_H
#define EDGELIST_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct listCDT* edgelistADT;
typedef struct edge edge;

edgelistADT emptyElist();
void elistcpy(edgelistADT dest, edgelistADT src);
void printeList(edgelistADT list);
void fprinteList(FILE* fp, char* str, edgelistADT list);
void insertedge(int x0, int y0, int x1, int y1, edgelistADT list);
void delEdge(int x0, int y0, int x1, int y1, edgelistADT list);
void delEdgebyindex(int index, edgelistADT list);
bool iseEmpty(edgelistADT list);
int elength(edgelistADT list);
edge* getEdge(int index, edgelistADT list);
int getx0(int index, edgelistADT list);
int gety0(int index, edgelistADT list);
int getx1(int index, edgelistADT list);
int gety1(int index, edgelistADT list);
//void sort(edgelistADT list);

#endif // !EDGELIST_H