#include "jobshop.h"

void FormatSolution_and_display(int * s){
	
	ComputeStartTimes(s);//再算一次,刷新Ct.函数设计失误,不应该用全局变量的.不过考虑到一次程序只算一次,忽略不计了.

    int i,j,k;
    int * T = (int *)malloc(sizeof(int)*n);
    memset(T,0,sizeof(int)*n);
	
    int ** S = (int **)malloc(sizeof(int *)*n);
    for(i=0;i<n;i++){
        S[i] = (int *)malloc(sizeof(int)*m);
        memset(S[i],0,sizeof(int)*m);
    }

    for(i=0;i<length;i++){
        int j = s[i];//获得工件号
        int t = T[j];//获得这是第j个工件的第t个工序
        S[j][t] = Ct[i]; //第j个工件的第t个工序的开始时间
        T[j] = T[j] + 1;
    }
	
    //S中存放的是每道工序开始加工的时间，它的形式为：[[a,b,c],[d,e,f],[g,h,i]],每个子list代表一个工件的信息，子list中的字母代表这个工件下面每道工序开始加工的时间。 

	int *** result = (int ***)malloc(sizeof(int**)*m);//第i台机器
	for(i=0;i<m;i++){
		result[i] = (int **)malloc(sizeof(int*)*n);//第i台机器的第j个工序
		for(j=0;j<n;j++){
			result[i][j] = (int *)malloc(sizeof(int)*4);//第i台机器的第j个工序所对应工件的序号,和这个工件的所在工序,以及开始时间,终止时间
			memset(result[i][j],-1,sizeof(int)*4);//-1表示没访问过
		}
	}
	//result这个数组里面,对于每一台机器.需要知道占用的时间.被占用的机器.如果机器是只在加工时间里加工一次的话,那么就不需要知道是第几次了,搜索即可.//算了,还是顺便存上好了.

	printf("/***************结果展示**************/\n");
	/***********测试用**********/
	// //展示原数据s
	// printf("s:");
	// for(i=0;i<length;i++){
	// 	printf("%d ",s[i]);
	// } printf("\n");

	// //展示原数据Ct
	// printf("Ct:");
	// for(i=0;i<=length;i++){
	// 	printf("%d ",Ct[i]);
	// } printf("\n");
	
	// //展示原数据S
	// 	printf("S:\n");
	// for(i=0;i<n;i++){
	// 	for(j=0;j<m;j++) printf("%d ",S[i][j]);
	// 	printf("\n");
	// } printf("\n");

	//先生成
	for(i=0;i<n;i++){ //第i个工件
		for(j=0;j<m;j++){ //第i个工件的第j个工序
			// S[i][j];//第i个工件的第j个工序所开始的时间
			for(k=0;k<n;k++){//找到第k个空位
				if(result[ I[i][j][0] ][k][0]==-1) break;
			}
			
			result[ I[i][j][0] ][k][0] = i;
			result[ I[i][j][0] ][k][1] = j;
			result[ I[i][j][0] ][k][2] = S[i][j];
			result[ I[i][j][0] ][k][3] = S[i][j] + I[i][j][1];
		}
	}

	// 再排序.对每一个机器都进行一次排序
	for(i=0;i<m;i++){//冒泡排序

		for(j=0;j<n;j++){
			for(k=0;k<n-1;k++){
				if(result[i][k][2]>result[i][k+1][2]){
					int * temp = result[i][k+1];
					result[i][k+1] = result[i][k];
					result[i][k] = temp;
				}
			}
		}
	}

	//输出
	for(i=0;i<m;i++){
		printf("The machine %d :",i);
		for(j=0;j<n;j++){
			printf("(%d,%d-%d,%d),",result[i][j][2],result[i][j][0]+1,result[i][j][1]+1,result[i][j][3]);
		}
		printf("\n");
	}
}
