#include "jobshop.h" 

void shuffle(int * gene) {//ϴ�ƺ���.���������������ϴ��length/2��.̫��Ҳ�����˷�ʱ��.���ϴ�ƴ���Ӧ���ǿ����ɾ���ĳ�����.
	int i;
	for (i = 0; i<length; i++) {//******************************************�������ʲô��� 
		int loc1 = rand() % length;
		int temp = gene[i];
		gene[i] = gene[loc1];
		gene[loc1] = temp;
	}
}

int ** InitPopulation(){

	int i = 0, j = 0, k = 0;
	//ȷ��һ��Ⱦɫ��ĳ���,��ȷ�����������,����ȷ�����������//�������в����������涼��n*m,���������û�������� 
	length	= n*m;
	
	int * gene = (int *)malloc(sizeof(int)*length);

	 i = 0;
	for (j = 0;j<n; j++) {//j��ʾ�Թ����ı���,i��ʾ�����ϵ�i��λ��
		int t;//t��ʾ��ָ���Ĺ����Ĺ���ı���
		for (t = 0;t<m; t++, i++) {
			gene[i] = j;
		}
	}

	int ** population = (int **)malloc(sizeof(int*)*ps);
	for (i = 0; i<ps; i++) {
		shuffle(gene);
		population[i] = (int *)malloc(sizeof(int)*length);//��ps������ƴ����һ��Ⱦɫ����
		for (j = 0; j<length; j++) {
			population[i][j] = gene[j];
		}
	}
	return population;
}

void displaypop(int ** pop,int * eachtime){/***********������**********/
	int i,j;
	for(i=0;i<ps;i++){ 
		printf("The %2d one use %2d:",i+1,eachtime[i]);
		for(j=0;j<length;j++){
			printf("%2d",pop[i][j]);
		}
		printf("\n");
	}
}
