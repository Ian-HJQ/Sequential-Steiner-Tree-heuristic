#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<argp.h>
#include "pointlist.h"
#include "edgelist.h"

#define MAX_TAPS 32

typedef pointlistADT pointlist;
typedef edgelistADT edgelist;

typedef struct point {
    int index;
    int id;
    int x;
    int y;
    struct point* next;
}point;

typedef struct edge {
    int index;
    int x0;
    int y0;
    int x1;
    int y1;
    struct edge* next;
}edge;

//struct for tap pins assignment and routing
struct tap { 
    int tap_id;         //id of tap
    int num_of_pins;    //number of pins assigned to the tap
    pointlist pinslist; //list of pins assgined to the tap
    int num_of_edges;   //number of edges in the tap tree
    edgelist edgeslist; //list of the edges in the tap tree
};


/*STRUCTS AND FUNCTIONS FOR ARGP*/

static struct argp_option options[]={
                                {"input", 'i', "FILEPATH", 0, "Path to the input file"},
                                {"output", 'o', "FILEPATH", 0, "Path to the output file"},
                                {0}
                                };

struct arguments{
    int argc;
    char *in_file;
    char *out_file;
};

static error_t parse_opt (int key, char* arg, struct argp_state *state){
    struct arguments *arguments = state->input;
    int count = 0;
    switch (key){
        case 'i': 
            arguments->in_file = arg;
            break;

        case 'o':
            arguments->out_file = arg;
            break;
        
        case ARGP_KEY_END:
            if (arguments->argc < 2){
                printf("Error: Missing arguments\n");
                argp_usage(state);
            }
            else if (arguments->in_file == NULL){
                argp_failure(state, 1, 0, "Missing argument --input");
                exit (ARGP_ERR_UNKNOWN);
            }
            else if (arguments->out_file == NULL){
                argp_failure(state, 1, 0, "Missing argument --output");
                exit (ARGP_ERR_UNKNOWN);
            }
            
            break;
        
        default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
};

static struct argp argp = { options, parse_opt };



/*GLOBAL VARIABLES*/

int MAX_RUNTIME, MAX_LOAD, GRID_SIZE, CAPACITY, PINS, TAPS;
pointlist pins; //list of all pins
pointlist taps; //list of all taps
pointlist edges;//list of edges
struct tap tap[MAX_TAPS]; //tap pins assignment and routing for each tap


/*FUNCTIONS*/

//create rectilinear edges
void createEdge(int x0, int y0, int x1, int y1, edgelist edges) { 
    if ((x0 == x1) || (y0 == y1)){ //same x or y
        insertedge(x0, y0, x1, y1, edges);
    }
    else{ // create a L shape
        insertedge(x0, y0, x0, y1, edges);
        insertedge(x0, y1, x1, y1, edges);
    }
    return;
}

//calculate the rectilinear distance between two points
int rect_dist(point* a, point*b){
    return (abs(a->x - b->x) + abs(a->y - b->y));
}

//create min bounding box
void mbb(point* pmbb, point* closest_pin, pointlist points) { 
    if ((pmbb->x == closest_pin->x) || (pmbb->y == closest_pin->y)){
        insertpoint(0, pmbb->x, pmbb->y, points);
        insertpoint(1, closest_pin->x, closest_pin->y, points);
    }
    else {
        insertpoint(0, pmbb->x, pmbb->y, points); // id 0 for pmbb
        insertpoint(1, closest_pin->x, closest_pin->y, points); // id 1 for pin
        insertpoint(-1, pmbb->x, closest_pin->y, points); //id -1 for non pin/pmbb
        insertpoint(-1, closest_pin->x, pmbb->y, points); //id -1 for non pin/pmbb
    }
    return;
}

//find the closest point to the source from a list of points
point* closest_pt(point* source, pointlist points){
    int min_dist = GRID_SIZE * GRID_SIZE;
    point* min_point = NULL;
    for(int i=0; i<plength(points); i++){
        point* dest = getPoint(i, points);
        int dist = rect_dist(source, dest);
        if (dist < min_dist && dist != 0){
            min_dist = dist;
            min_point = dest;
        }
    }
    if (min_point == NULL){
        printf("Error, function \'closest_pt\': min_point is NULL\n");
        exit(1);
    }
    return min_point;
}

void closest_pin_to_mbb(pointlist mbb, pointlist pinslist, point* pmbb, point* closest_pin){
    point *closest_source = NULL; 
    point *closest_dest = NULL;
    int min_dist = GRID_SIZE * GRID_SIZE;
    for(int i=0; i<plength(mbb); i++){
        point* source = getPoint(i, mbb);
        point* dest = closest_pt(source, pinslist);
        int dist = rect_dist(source, dest);
        if(dist < min_dist){
            min_dist = dist;
            closest_source = source;
            closest_dest = dest;
        }
    }

    if(closest_source == NULL){
        printf("Error, function \'closest_pin_to_mbb\': \'closest_source\' is NULL\n");
        exit(1);
    }
    if(closest_dest == NULL){
        printf("Error, function \'closest_pin_to_mbb\': \'closest_dest\' is NULL\n");
        exit(1);
    }

    memmove(pmbb, closest_source, sizeof(*closest_source));
    memmove(closest_pin, closest_dest, sizeof(*closest_dest));
    return;
}

//check whether pmbb is a pin
bool pmbb_is_pin(point* pmbb, pointlist pinslist){
    for(int i=0; i<plength(pinslist); i++){
        point* pin = getPoint(i, pinslist);
        if((pmbb->x == pin->x) && (pmbb->y == pin->y)){
            return true;
        }
    }
    return false;
}


//main function
int main  (int argc, char** argv){

    //initialise all the lists
    pins = emptyPlist();
    taps = emptyPlist();
    edges = emptyElist();
    
    //initialize tap struct
    for(int i=0; i< MAX_TAPS; i++){
        tap[i].tap_id = i;
        tap[i].num_of_pins = 0;
        tap[i].pinslist = emptyPlist();
        tap[i].num_of_edges = 0;
        tap[i].edgeslist = emptyElist();
    }


    //parse arguments
    struct arguments arguments;
    arguments.argc = argc;
    arguments.in_file = NULL;
    arguments.out_file = NULL;
    argp_parse(&argp, argc, argv, 0, 0, &arguments);


    //read input file
    printf("Reading input file...\n");
    FILE* fp_in;
    fp_in = fopen(arguments.in_file, "r");
    if (fp_in == NULL){
        printf("Error opening input file!\n");
        exit(1);
    }
    char param[20];
    fscanf(fp_in, "%s %d", param, &MAX_RUNTIME);
    fscanf(fp_in, "%s %d", param, &MAX_LOAD);
    fscanf(fp_in, "%s %d", param, &GRID_SIZE);
    fscanf(fp_in, "%s %d", param, &CAPACITY);
    fscanf(fp_in, "%s %d", param, &PINS);
    printf("MAX_RUNTIME: %d\n", MAX_RUNTIME);
    printf("MAX_LOAD: %d\n", MAX_LOAD);
    printf("GRID_SIZE: %d\n", GRID_SIZE);
    printf("CAPACITY: %d\n", CAPACITY);
    printf("PINS: %d\n", PINS);

    for(int i=0; i<PINS; i++){
        int pin_id, pin_x, pin_y;
        fscanf(fp_in, "%s %d %d %d", param, &pin_id, &pin_x, &pin_y);
        insertpoint(pin_id, pin_x, pin_y, pins);
    }

    fscanf(fp_in, "%s %d", param, &TAPS);
    for(int i=0; i<TAPS; i++){
        int tap_id, tap_x, tap_y;
        fscanf(fp_in, "%s %d %d %d", param, &tap_id, &tap_x, &tap_y);
        insertpoint(tap_id, tap_x, tap_y, taps);
    }

    fclose(fp_in);
    printf("TAPS: %d\n", TAPS);


    //pin assignment
    printf("Running pin assignment...\n");
    for(int i=0; i<plength(pins); i++){
        //printf("Assigning pin %d...\n", i);
        int pin_min = 0;
        int min_dist = GRID_SIZE * GRID_SIZE;
        point* pin = getPoint(i, pins);
        //find the closest tap
        for(int j=0; j<plength(taps); j++){
            int dist = rect_dist(pin, getPoint(j, taps));
            if (dist < min_dist){
                pin_min = j;
                min_dist = dist;
            }
        }
        //assign pin to tap
        insertpoint(pin->id, pin->x, pin->y, tap[pin_min].pinslist);
        tap[pin_min].num_of_pins++;
    }


    // pin routing
    printf("Routing pins...\n");
    for(int i=0; i<TAPS; i++){
        // printf("Routing pins for tap %d\n", i);
        //find the closest pin pair
        int min_dist = GRID_SIZE * GRID_SIZE;
        point* min_pair1, *min_pair2;
        for(int j=0; j<plength(tap[i].pinslist); j++){
            point* source = getPoint(j, tap[i].pinslist);
            point* dest = closest_pt(source, tap[i].pinslist);
            int dist = rect_dist(source, dest);
            if(dist < min_dist){
                min_dist = dist;
                min_pair1 = source;
                min_pair2 = dest;
            } 
        }

        pointlist curr_mbb, next_mbb;
        curr_mbb = emptyPlist();
        next_mbb = emptyPlist();
        //construct the mbb
        mbb(min_pair1, min_pair2, curr_mbb);
        
        pointlist cur_pinslist = emptyPlist();
        plistcpy(cur_pinslist, tap[i].pinslist);
        delPointbyid(min_pair1->id, cur_pinslist);
        delPointbyid(min_pair2->id, cur_pinslist);
        point* cur_tap = getPointbyid(i, taps);
        insertpoint(cur_tap->id, cur_tap->x, cur_tap->y, cur_pinslist);

        while(!(ispEmpty(cur_pinslist))){
            //find the closest pin to the mbb
            point closest_pin, pmbb;
            closest_pin_to_mbb(curr_mbb, cur_pinslist, &pmbb, &closest_pin);
            //construct mbb to the closest pin
            mbb(&pmbb, &closest_pin, next_mbb);
            //add the L shape to edgelist
            if (plength(curr_mbb) == 2){ //mbb is a line, add the edge directly
                point *p1 = getPoint(0, curr_mbb);
                point *p2 = getPoint(1, curr_mbb);
                insertedge(p1->x, p1->y, p2->x, p2->y, tap[i].edgeslist);
                tap[i].num_of_edges++;
            }
            else{
               if(pmbb.id != -1){ // if pmbb is a pin or is the previous pmbb
                    // del point that is not prev pmbb or pin
                    // for(int j=0; j<plength(curr_mbb); j++){
                    //    point *pt = getPoint(j, curr_mbb);
                    //    if(pt->id == -1){
                    //        delPoint(j, curr_mbb);
                    //        j--;
                    //    }
                    // }
                    //construct edge with the remaining two points
                    point *pt1 = getPoint(0, curr_mbb);
                    point *pt2 = getPoint(1, curr_mbb);
                    createEdge(pt1->x, pt1->y, pt2->x, pt2->y, tap[i].edgeslist);
                    tap[i].num_of_edges += 2;
               }
               else{
                //    //del the unwanted point from cur_mbb
                //    for(int j=0; j<plength(curr_mbb); j++){
                //        point *pt = getPoint(j, curr_mbb);
                //        if (pt->id == -1){
                //            if( !((pt->x == pmbb.x) && (pt->y == pmbb.y)) ){
                //                delPoint(pt->index, curr_mbb);
                //                break;
                //            }
                //        }
                //    }
                   //construct edges with remaining 3 points
                    point *pt1 = getPoint(0, curr_mbb);
                    point *pt2 = getPoint(1, curr_mbb);
                    point *pt3 = getPoint(2, curr_mbb);
                    insertedge(pt1->x, pt1->y, pmbb.x, pmbb.y, tap[i].edgeslist);
                    insertedge(pt2->x, pt2->y, pmbb.x, pmbb.y, tap[i].edgeslist);
                    tap[i].num_of_edges += 2;
               }
            }
            
            //delete pins from pinslist
            delPoint(closest_pin.index, cur_pinslist);
            curr_mbb = next_mbb;
            next_mbb = emptyElist();
        }

        //last pin
        point *pt1 = getPoint(0, curr_mbb);
        point *pt2 = getPoint(1, curr_mbb);
        createEdge(pt1->x, pt1->y, pt2->x, pt2->y, tap[i].edgeslist);
        tap[i].num_of_edges += 2;
    }


    // for(int i=0; i<TAPS; i++){
    //     point* tappoint;
    //     tappoint = getPoint(i, taps);
    //     printf("TAP%d %d %d\n", tap[i].tap_id, tappoint->x, tappoint->y);
    //     printf("PINS %d\n", tap[i].num_of_pins);
    //     printpList(tap[i].pinslist);
    //     printf("ROUTING %d\n", tap[i].num_of_edges);
    //     printeList(tap[i].edgeslist);
    //     printf("\n");
    // }

    //write output file
    printf("Writing output file...\n");
    FILE* fp_out;
    fp_out = fopen(arguments.out_file, "w+");
    if (fp_out == NULL){
        printf("Error opening output file!\n");
        exit(1);
    }
    for(int i=0; i<TAPS; i++){
        fprintf(fp_out, "TAP %d\n", tap[i].tap_id);
        fprintf(fp_out, "PINS %d\n", plength(tap[i].pinslist));
        fprintpList(fp_out, "PIN ", tap[i].pinslist);
        fprintf(fp_out, "ROUTING %d\n", elength(tap[i].edgeslist));
        fprinteList(fp_out, "EDGE ", tap[i].edgeslist);
    }

    fclose(fp_out);
    printf("Done.\n");


    return 0;
}