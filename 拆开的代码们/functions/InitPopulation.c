#include "jobshop.h" 

void shuffle(int * gene) {//洗牌函数.这里对这个基因随便洗牌length/2次.太多也不好浪费时间.这个洗牌次数应该是可以由经验的出来的.
	int i;
	for (i = 0; i<length; i++) {//******************************************随机数是什么情况 
		int loc1 = rand() % length;
		int temp = gene[i];
		gene[i] = gene[loc1];
		gene[loc1] = temp;
	}
}

int ** InitPopulation(){

	int i = 0, j = 0, k = 0;
	//确定一个染色体的长度,即确定工序的总数,下面确定工序的总数//很像所有测试数据里面都是n*m,不存在少用机器的情况 
	length	= n*m;
	
	int * gene = (int *)malloc(sizeof(int)*length);

	 i = 0;
	for (j = 0;j<n; j++) {//j表示对工件的遍历,i表示基因上第i个位置
		int t;//t表示对指定的工件的工序的遍历
		for (t = 0;t<m; t++, i++) {
			gene[i] = j;
		}
	}

	int ** population = (int **)malloc(sizeof(int*)*ps);
	for (i = 0; i<ps; i++) {
		shuffle(gene);
		population[i] = (int *)malloc(sizeof(int)*length);//将ps个基因拼接在一条染色体上
		for (j = 0; j<length; j++) {
			population[i][j] = gene[j];
		}
	}
	return population;
}

void displaypop(int ** pop,int * eachtime){/***********测试用**********/
	int i,j;
	for(i=0;i<ps;i++){ 
		printf("The %2d one use %2d:",i+1,eachtime[i]);
		for(j=0;j<length;j++){
			printf("%2d",pop[i][j]);
		}
		printf("\n");
	}
}
