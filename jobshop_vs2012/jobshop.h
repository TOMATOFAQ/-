//�����ˣ���Ǭһ ��ͩ ����һ 
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

//��Ҫ��Ϊ���ε���: 
#define ps 25//��Ⱥ��С
#define mit 500 //��������
#define pc 40 //�������
#define pm 90 //ͻ�����
#define totalnum 100 //δ����ʱ��֮ǰ���Ŵ�����,����ʱ��֮��Ͳ����ƴ�����,ֱ���ܵ��涨ʱ��Ϊֹ
#define ThreadTime 65 //�����Ŵ����е�ʱ��
#define programmeTime 75 //����������ʱ��

typedef struct GRAPH{//����ͼ�Ľṹ֮�ڵ� 
    struct NODE * nodes;
}graph;

typedef struct NODE{//����ͼ�Ľṹ֮�ڵ�ָ֮�򱾽ڵ����һ���ڵ��� 
    int * fathers;
}node;

extern int *** I;	//¼����Ϣ 
extern int n, m;	//������,������ 
extern int length; 	//Ⱦɫ�峤�� 
extern int *** result;
extern double duration;
extern double duraorignal;;
extern int ***ganttorignal;

extern int ** overhaul_list;
extern int TIME;//����ͬ������������������е�ʱ��

//���ڶ��̵߳��ĸ�ȫ�ֱ���
extern int final_result; //ʵʱ���´�����ʱ��
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
/********************************* 8-Observer-ɾȥ��Ӱ�캯������ *************************************/ 
void printInformation();
void printfpop(int ** pop,int * eachtime);
void printfgraph();
void printfIndex(int ** idx);
void printfparent(int * p);

#endif