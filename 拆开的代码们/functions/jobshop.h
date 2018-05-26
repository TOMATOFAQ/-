#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#ifndef _JOBSHOP_H_
#define _JOBSHOP_H_

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

//��Ҫ��Ϊ���ε���: 
#define ps 200 //��Ⱥ��С population size
#define mit 100 //�������� mount for iteratior
#define pc 0 //������� probability for crossover
#define pm 0 //ͻ����� probability for mutation

typedef struct GRAPH{//����ͼ�Ľṹ֮�ڵ� 
    struct NODE * children;
}graph;
typedef struct NODE{//����ͼ�Ľṹ֮�ڵ�ָ֮�򱾽ڵ����һ���ڵ��� 
    int * son;
}node;

extern int *** I;	//¼����Ϣ 
extern int n, m;	//������,������ 
extern int length; //Ⱦɫ�峤�� 
extern graph G;
extern int * st;//һ�����ҵ�Ⱦɫ�� 
extern int * Ct;//���ÿ������ʼ�ӹ���ʱ��

/********************************* 1-Input *************************************/ 
void printInformation();
void LoadInstance();//Ĭ���ض���������input.txt�������Ҫ�ֶ�������ע�͸��л����޸������ļ�
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
