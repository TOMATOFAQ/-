//负责人：范乾一 汪桐 田中一 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#ifndef _JOBSHOP_H_
#define _JOBSHOP_H_
#define SHOW_CONSOLE
#define ONE EGERGB(244, 204, 204) 
#define TWO EGERGB(252, 229, 205)
#define THR EGERGB(255, 242, 204)
#define FOR EGERGB(217, 234, 211)
#define FIV EGERGB(208, 224, 227)
#define SIX EGERGB(201, 218, 248)
#define SEV EGERGB(207, 226, 243)
#define EIG EGERGB(217, 210, 233)
#define NIN EGERGB(234, 209, 220)
#define TEN EGERGB(213, 166, 189)
#define ELE EGERGB(180, 167, 214)
#define TWE EGERGB(159, 197, 232)
#define THI EGERGB(164, 194, 244)
#define FOU EGERGB(162, 196, 201)
#define FIF EGERGB(182, 215, 168)

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

//需要人为调参的量: 
#define ps 25//种群大小
#define mit 500 //迭代次数
#define pc 40 //交配概率
#define pm 90 //突变概率
#define totalnum 100 //未限制时间之前的遗传次数,限制时间之后就不限制次数了,直接跑到规定时间为止
#define ThreadTime 65 //单次遗传运行的时间
#define programmeTime 75 //程序总运行时间

typedef struct GRAPH{//有向图的结构之节点 
    struct NODE * nodes;
}graph;

typedef struct NODE{//有向图的结构之节点之指向本节点的上一个节点们 
    int * fathers;
}node;

extern int *** I;	//录入信息 
extern int n, m;	//工件数,机器数 
extern int length; 	//染色体长度 
extern int *** result;
extern double duration;
extern double duraorignal;;
extern int ***ganttorignal;

extern int ** overhaul_list;
extern int TIME;//用来同步与输入与输出过程中的时间

//用于多线程的四个全局变量
extern int final_result; //实时更新存放最短时间
extern int finalorignal;
extern int * final_s;
extern int * final_Ct;

/********************************* 1-Input *************************************/ 
void LoadInstance();
/********************************* 2-InitPopulation ********************************/ 
void shuffle(int * gene);
int ** InitPopulation(); 
/********************************* 3-ComputDAG *************************************/ 
int ComputeStartTimes(int * s,int * Ct);
/********************************* 4-Crossover *************************************/  
int ** Index(int * p);
int * Crossover(int * p1,int *p2);
void Mutation(int * p);
/********************************* 5-iterator *************************************/ 
void shufflepop(int * eachtime,int ** pop);
void sortpop(int * eachtime,int ** pop);
int iterator(int ** pop,int * eachtime,int * Ct);
/********************************* 6-Output *************************************/ 
void FormatSoverhaul_listution_and_display(int * s,int * Ct,int choice);
/********************************* 7-Overhaul *************************************/ 
void overhaulrun();
int checkstate(int aimm,int time);
void putforward(int aimi,int aimj);
void insert_overhaul(int aimi,int aimj);
/********************************* 8-image *************************************/
void image_checklist();
void image_ganttorignal();
void image_list();
void image_ganttcheckwhole();

void image_ganttoverhaul();

void image_whole();
void image_ganttwhole();
void image_wholepage(int page);

void image_checklistpage(int page);
void image_checklist();
void image_ganttorignal();
/********************************* 8-Observer-删去不影响函数功能 *************************************/ 
void printInformation();
void printfpop(int ** pop,int * eachtime);
void printfgraph();
void printfIndex(int ** idx);
void printfparent(int * p);

#endif