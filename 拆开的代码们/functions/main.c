#include "jobshop.h"

int *** I;	//录入信息 
int n, m;	//工件数,机器数 
int length = 0; //染色体长度 
graph G;
int * st;//一条打乱的染色体 
int * Ct;//存放每道工序开始加工的时间

int main(){
	//0-初始化时间变量
	srand((unsigned)time(NULL));

	//1-读入信息
	LoadInstance();
	printInformation();

	//2-初始化数据结构
	int * eachtime = (int *)malloc(sizeof(int)*ps);
	memset(eachtime,0,sizeof(int)*ps);
	int ** pop = InitPopulation();

	int i;
	for(i=0;i<ps;i++){
		 eachtime[i] = ComputeStartTimes(pop[i]);
		 eachtime[i] = ComputeStartTimes(pop[i]);
	}

	//3-迭代
	iterator(pop,eachtime);

	//4-输出结果
	FormatSolution_and_display(pop[0]);
	
	printf("\n按任意键退出……"); 
	getch(); 
	return 0;
}
