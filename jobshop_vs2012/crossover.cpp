#include "jobshop.h"

int ** Index(int * p){//返回一个二元对的数组,每个二元对中第一个表示的是工件,第二个表示的是顺序.P.S.这么写纯粹是为了看起来像"高级语言 
    int * ct = (int *)malloc(sizeof(int)*n);
    memset(ct,0,sizeof(int)*n);

    int ** s = (int **)malloc(sizeof(int*)*length);	 
    int i,j;
    for(i=0;i<length;i++) s[i] = (int*)malloc(sizeof(int)*2);
    
    for(i=0,j=0;i<length;i++){
        s[i][0] = p[i];
        s[i][1] = ct[p[i]]; 
        ct[p[i]] = ct[p[i]] + 1;
    }

	//内存释放
	//此函数中的s在Crossover中free
	free(ct);

    return s;
}

int * Crossover(int * p1,int *p2){//p1,p2是两条等待交叉的染色体,p为parent的缩写
    int i,j,k;//用于循环的辅助变量
    int ** idx_p1 = Index(p1);
    int ** idx_p2 = Index(p2);

    int randi = (rand()%(length-1))+1;//生成[1,nt)的随机数,表示将嵌入的变异片段之长 
    int randj = rand()%(length-randi);//生成[0,nt-randi-1)的随机数, 表示变异片段在parent1中的起始位置 
    int randk = rand()%length;//生成[0,nt)的随机数,表示变异片段在parent2中的起始位置 

    //implant 意为嵌入 .生成将交叉片段. 
    int ** implant =  (int **)malloc(sizeof(int*)*(randi));	 
    for(i=0;i<randi;i++){
    	implant[i] = (int *)malloc(sizeof(int)*2);
		implant[i][0] = idx_p1[randj+i][0];
		implant[i][1] = idx_p1[randj+i][1];
	}
	
	//同时检测parent2中与嵌入片段重复的内容,并将其删除 
	int * aftermutation = (int *)malloc(sizeof(int)*length);
	for(i=0;i<length;i++){
			for(j=0;j<randi;j++){
				if(idx_p2[i][0] == implant[j][0] && idx_p2[i][1] == implant[j][1]) idx_p2[i][0] = -1;//做上标记 
			}
	}

	for(i=0,k=0;i<length;k++){
		if(k==randk){
			for(j=0;j<randi;j++,i++){
				aftermutation[i] = implant[j][0];
			}
		}
		
		if(idx_p2[k][0]!=-1){
			aftermutation[i] = idx_p2[k][0];
			i++;
		}		
	}
	
	 //内存释放
	for(i=0;i<randi;i++) free(implant[i]);
	free(implant);
	for(i=0;i<length;i++) { free(idx_p1[i]); free(idx_p2[i]); }
	free(idx_p1);
	free(idx_p2); 

	return aftermutation;
}

void Mutation(int * p){
    int ranki = rand()%length;
    int rankj = rand()%length;

    int temp;
    temp = p[ranki];
    p[ranki] = p[rankj];
    p[rankj] = temp;
}
