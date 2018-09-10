#include "jobshop.h"

void printfIndex(int ** idx){
	int i;
	printf("Parent:");
	for(i=0;i<length;i++) printf("%2d",idx[i][0]);
	printf("\n");
	
	printf("Index :");
	for(i=0;i<length;i++) printf("%2d",idx[i][1]);
	printf("\n");
} 

void printfparent(int * p){
	int i;
	for(i=0;i<length;i++) printf("%d ",p[i]);
	printf("\n");
}

void printfgraph(graph G){
	int i,j;
	for(i=0;i<length+1;i++){
		printf("Node %2d:",i);
		for(j=0;j<length;j++){
			printf("%2d ",G.nodes[i].fathers[j]);
		}
		printf("\n");
	}
}

void printInformation() {
	int i, j;
	printf("%d件工件.%d台机器,工件信息：(机器号，所需时间)\n",n,m);
	for (i = 0; i<n; i++) {
		printf("工件%2d 所需工序:", i);
		for (j = 0; j<m; j++)    printf("(%2d,%2d),", I[i][j][0], I[i][j][1]);
		printf("\n");
	}
}

void printfpop(int ** pop,int * eachtime){
	int i,j;
	for(i=0;i<ps;i++){ 
		printf("The %2d one use %2d:",i+1,eachtime[i]);
		for(j=0;j<length;j++){
			printf("%2d",pop[i][j]);
		}
		printf("\n");
	}
}
