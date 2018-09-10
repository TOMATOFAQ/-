#include "jobshop.h"

void shufflepop(int * eachtime,int ** pop){
	int i;
	for(i=0;i<(ps/2);i++){
		int loc = rand()%(ps);
		
		int * temp = pop[i];
		pop[i] = pop[loc];
		pop[loc] = temp;
		
		int temp2 = eachtime[i];
		eachtime[i] = eachtime[loc];
		eachtime[loc] = temp2;
	}
}

void sortpop(int * eachtime,int ** pop){//�������� 
	int i,j;
	for(i=0;i<ps;i++){
		for(j=0;j<ps-1;j++){
			if(eachtime[j]>eachtime[j+1]){
				int temp = eachtime[j];
				eachtime[j] = eachtime[j+1];
				eachtime[j+1] = temp;
				
				int *temp2 = pop[j];
				pop[j] = pop[j+1];
				pop[j+1] = temp2;
			}
		}
	}
}


//��ֹ���װ�
int iterator(int ** pop,int * eachtime,int * Ct){
	 int i,j,k; //ѭ���������� 
	 int it;	//iterator ������

	//Ϊ������һ�ε�����,Ⱦɫ��ֻ��ͬ����Ⱦɫ���ӽ�,��Ҫ���³����ɵ������Ӵ�����������.�ȵ������ӽ����֮��,�ټ�����̭�����Ӵ�
	int ** temp = (int **)malloc(sizeof(int *)*ps);//��Ⱦɫ��˳��
	for(i=0;i<ps;i++){
		temp[i] = (int *)malloc(sizeof(int)*length);
		memset(temp[i],-1,sizeof(int)*length);//δ���������Ӵ��Ĳ�������Ϊ��һ,�������
	}
	int * temptime = (int *)malloc(sizeof(int)*ps);//��Ⱦɫ���Ӧ��ʱ��
	memset(temptime,-1,sizeof(int)*ps);


	for(it=1;it<=mit;it++){
		int * ch1;
		int * ch2;
		int time1;
		int time2;

		int son = 0;//���ڼ�¼�����,ָ��temp�������С�Ŀ�λ.��¼���ɵ������Ӵ�,���ڲ����temp�еĿ�λ��

	 	for(i=0;i<(ps/2);i++){//���н��滥��
	 		if((rand() % 100) > pc){

	 			ch1 = Crossover(pop[i],pop[ps/2+i]);
	 			ch2 = Crossover(pop[ps/2+i],pop[i]);
	 			
	 			if((rand() % 100) > pm){
					Mutation(ch1);
				 }

				if((rand() % 100) > pm){
					Mutation(ch2);
				}
				
	 			time1 = ComputeStartTimes(ch1,Ct);
	 			time2 = ComputeStartTimes(ch2,Ct);
	 			
				//�洢
				for(j=0;j<length;j++) temp[son][j] = ch1[j];
				temptime[son] = time1;
				son++;
				for(j=0;j<length;j++) temp[son][j] = ch2[j];
				temptime[son] = time2;
				son++;

				free(ch1);
				free(ch2);
			 }
		}
		 
		//�Ӵ��滻����,˳�����
		for(i=0;i<son;i++){
			for(j=0;j<ps;j++){
				if(temptime[i] < eachtime[j]){
					eachtime[j] = temptime[i];
					for(k=0;k<length;k++){
						pop[j][k] = temp[i][k];
					}
					break;	//ֻҪ�滻��һ�ξ��˳�,��ֹ�ظ�
				}
			}
			memset(temp[i],-1,sizeof(int)*length);
			temptime[i] = -1;
		}
		
		shufflepop(eachtime,pop);
		///***********������*********
		//  printf("������%d�εĴ�:%d\n",it,eachtime[0]); //ɾȥ������ 
		// printf("%d  ",eachtime[0]); 
		// printfpop(pop,eachtime);//ɾȥ���� 
	 }

	//�ڴ��ͷ�
	free(temptime);
	for(i=0;i<ps;i++) free(temp[i]);
	free(temp);

	//���򲢷��ش�(����Ҫ)
	sortpop(eachtime,pop);
	return eachtime[0];
}
