#include "jobshop.h"

int *** I;	//¼����Ϣ 
int n, m;	//������,������ 
int length = 0; //Ⱦɫ�峤�� 
graph G;
int * st;//һ�����ҵ�Ⱦɫ�� 
int * Ct;//���ÿ������ʼ�ӹ���ʱ��

int main(){
	//0-��ʼ��ʱ�����
	srand((unsigned)time(NULL));

	//1-������Ϣ
	LoadInstance();
	printInformation();

	//2-��ʼ�����ݽṹ
	int * eachtime = (int *)malloc(sizeof(int)*ps);
	memset(eachtime,0,sizeof(int)*ps);
	int ** pop = InitPopulation();

	int i;
	for(i=0;i<ps;i++){
		 eachtime[i] = ComputeStartTimes(pop[i]);
		 eachtime[i] = ComputeStartTimes(pop[i]);
	}

	//3-����
	iterator(pop,eachtime);

	//4-������
	FormatSolution_and_display(pop[0]);
	
	printf("\n��������˳�����"); 
	getch(); 
	return 0;
}
