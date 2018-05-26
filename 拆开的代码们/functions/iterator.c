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

int iterator(int ** pop,int * eachtime){
	 int i,j; //循环辅助变量 
	 int it;	//iterator 迭代器

	 for(it=1;it<=mit;it++){
//	 	printf("----------第%d次迭代----------\n",it);
		int * ch1;
		int * ch2;
		int time1;
		int time2;

	 	for(i=0;i<(ps/2);i++){
	 		if((rand() % 100) > pc) {

	 			ch1 = Crossover(pop[i],pop[ps/2+i]); 
	 			ch2 = Crossover(pop[ps/2+i],pop[i]); 
	 			
	 			if((rand() % 100) > pm){
					Mutation(ch1);
				 }

				if((rand() % 100) > pm) {
					Mutation(ch2);
				}
				
	 			time1 = ComputeStartTimes(ch1);
	 			time2 = ComputeStartTimes(ch2);
	 			
	 			sortpop(eachtime,pop);
	 			int replace1 = 1;
	 			int replace2 = 1;
	 			for(j=0;j<ps;j++){
	 				if(time1 < eachtime[j] && replace1){
	 					eachtime[j] = time1;
	 					pop[j] = ch1;
	 					replace1 = 0;
					 }
					 if(time2 < eachtime[j] && replace2){
	 					eachtime[j] = time2;
	 					pop[j] = ch2;
	 					replace2 = 0;
					 }
				 }
				 shufflepop(eachtime,pop);
			 }
		 }
		 
		sortpop(eachtime,pop);
		
		/***********测试用**********/
//		printf("迭代第%d次的答案:%d\n",it,eachtime[0]); //删去最大加速 
//		printf("%d  ",eachtime[0]); 
//		displaypop(pop,eachtime);//删去加速 
		
	 }
	 sortpop(eachtime,pop);
	 return eachtime[0];
}
