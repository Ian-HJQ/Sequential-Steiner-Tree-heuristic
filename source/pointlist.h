#ifndef POINTLIST_H
#define POINTLIST_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct listCDT* pointlistADT;
typedef struct point point;

pointlistADT emptyPlist();
void plistcpy(pointlistADT dest, pointlistADT src);
void printpList(pointlistADT list);
void fprintpList(FILE* fp, char* str, pointlistADT list);
void insertpoint(int id, int x, int y, pointlistADT list);
void delPoint(int index, pointlistADT list);
void delPointbyid(int id, pointlistADT list);
bool ispEmpty(pointlistADT list);
int plength(pointlistADT list);
point* getPoint(int index, pointlistADT list);
point* getPointbyid(int id, pointlistADT list);
int getx(int index, pointlistADT list);
int gety(int index, pointlistADT list);
//void sort(pointlistADT list);

#endif // !POINTLIST_H