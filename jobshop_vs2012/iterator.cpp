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

void sortpop(int * eachtime,int ** pop){//升序排列 
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


//禁止乱伦版
int iterator(int ** pop,int * eachtime,int * Ct){
	 int i,j,k; //循环辅助变量 
	 int it;	//iterator 迭代器

	//为了满足一次迭代内,染色体只与同代的染色体杂交,需要把新成生成的优秀子代给保存下来.等到所有杂交完毕之后,再加入淘汰落后的子代
	int ** temp = (int **)malloc(sizeof(int *)*ps);//存染色体顺序
	for(i=0;i<ps;i++){
		temp[i] = (int *)malloc(sizeof(int)*length);
		memset(temp[i],-1,sizeof(int)*length);//未存入优秀子代的部分设置为负一,以作标记
	}
	int * temptime = (int *)malloc(sizeof(int)*ps);//该染色体对应的时间
	memset(temptime,-1,sizeof(int)*ps);


	for(it=1;it<=mit;it++){
		int * ch1;
		int * ch2;
		int time1;
		int time2;

		int son = 0;//用于记录的序号,指向temp中序号最小的空位.记录生成到几个子代,便于插入进temp中的空位中

	 	for(i=0;i<(ps/2);i++){//进行交叉互换
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
	 			
				//存储
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
		 
		//子代替换父代,顺便清空
		for(i=0;i<son;i++){
			for(j=0;j<ps;j++){
				if(temptime[i] < eachtime[j]){
					eachtime[j] = temptime[i];
					for(k=0;k<length;k++){
						pop[j][k] = temp[i][k];
					}
					break;	//只要替换了一次就退出,防止重复
				}
			}
			memset(temp[i],-1,sizeof(int)*length);
			temptime[i] = -1;
		}
		
		shufflepop(eachtime,pop);
		///***********测试用*********
		//  printf("迭代第%d次的答案:%d\n",it,eachtime[0]); //删去最大加速 
		// printf("%d  ",eachtime[0]); 
		// printfpop(pop,eachtime);//删去加速 
	 }

	//内存释放
	free(temptime);
	for(i=0;i<ps;i++) free(temp[i]);
	free(temp);

	//排序并返回答案(若需要)
	sortpop(eachtime,pop);
	return eachtime[0];
}
