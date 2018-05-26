#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#ifndef _JOBSHOP_H_
#define _JOBSHOP_H_

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

//需要人为调参的量: 
#define ps 200 //种群大小 population size
#define mit 100 //迭代次数 mount for iteratior
#define pc 0 //交配概率 probability for crossover
#define pm 0 //突变概率 probability for mutation

typedef struct GRAPH{//有向图的结构之节点 
    struct NODE * children;
}graph;
typedef struct NODE{//有向图的结构之节点之指向本节点的上一个节点们 
    int * son;
}node;

extern int *** I;	//录入信息 
extern int n, m;	//工件数,机器数 
extern int length; //染色体长度 
extern graph G;
extern int * st;//一条打乱的染色体 
extern int * Ct;//存放每道工序开始加工的时间

/********************************* 1-Input *************************************/ 
void printInformation();
void LoadInstance();//默认重定向输入至input.txt，如果需要手动输入请注释改行或者修改输入文件
/********************************* 2-InitPopulation ********************************/ 
void shuffle(int * gene);
int ** InitPopulation(); 
void displaypop(int ** pop,int * eachtime);
/********************************* 3-ComputDAG *************************************/ 
void ComputeDAG(int * s);
int ComputeStartTimes(int * s);
void printfgraph();
/********************************* 4-Crossover *************************************/  
int ** Index(int * p);
void printfIndex(int ** idx);
void printfparent(int * p);
int * Crossover(int * p1,int *p2);
void * Mutation(int * p);
/********************************* 5-iterator *************************************/ 
void shufflepop(int * eachtime,int ** pop);
void sortpop(int * eachtime,int ** pop);
int iterator(int ** pop,int * eachtime);
/********************************* 6-Output *************************************/ 
void FormatSolution_and_display(int * s);

#endif
