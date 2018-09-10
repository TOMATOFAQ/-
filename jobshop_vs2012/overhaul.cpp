#include "jobshop.h"

DWORD WINAPI overhaul_output(LPVOID pPararneter);
DWORD WINAPI overhaul_input(LPVOID pPararneter);

void overhaulrun(){
	int i,j;
	//printf("The result had been outputed into output.txt.Please enter num 1 to overhaul.\n");//可能验收的时候这里要暂停 
	overhaul_list = (int **)malloc(sizeof(int*)*(n*m));//假设一共n*m条检修命令
	for(i = 0;i<n*m;i++){
		overhaul_list[i] = (int*)malloc(sizeof(int)*3);//每条检修命令储存三个信息。该检修命令的机器、开始时间，结束时间
		memset(overhaul_list[i],-1,sizeof(int)*3);//-1表示不存在这个要求
	}
	
	HANDLE OVERHAUL_INPUT,OVERHAUL_OUTPUT;
	OVERHAUL_INPUT = CreateThread(NULL,0,overhaul_input,NULL,0,NULL);
	OVERHAUL_OUTPUT = CreateThread(NULL,0,overhaul_output,NULL,0,NULL);

	while(TIME != -1)Sleep(1000);//检修是否结束

	//输出检修后结果
	freopen("output2.txt", "w", stdout);//重定向
		for(i=0;i<m;i++){//第i台机器
			printf("M%d",i);
			for(j=0;j<n;j++){//第j个工序
				//检查该工序与上一个工序之间是否存在检修命令，若是，插入检修信息
				insert_overhaul(i,j);
				printf(" (%d,%d-%d,%d)",result[i][j][2],result[i][j][0],result[i][j][1],result[i][j][3]);
			}
			printf("\n");
		}		
	printf("Time Used:%.3lf\n",duration);
	printf("End Time:%d\n",final_result);
	freopen("CON", "w", stdout);//回到控制台
	printf("The result had been outputed into output2.txt.\n");
	
	//在控制台检验结果
		for(i=0;i<m;i++){//第i台机器
			printf("M%d",i);
			for(j=0;j<n;j++){//第j个工序

				// 测试代码：
				 if(j>0){//临时测试：检验有无冲突
				 	if(result[i][j][2]<result[i][j-1][3]) printf("██ERROR██");
				 }
				//检查该工序与上一个工序之间是否存在检修命令，若是，插入检修信息
				insert_overhaul(i,j);
				printf(" (%d,%d-%d,%d)",result[i][j][2],result[i][j][0],result[i][j][1],result[i][j][3]);
			}
			printf("\n");
		}		
	printf("Time Used:%.3lf\n",duration);
	printf("End Time:%d\n",final_result);
}

int checkstate(int aimm,int time){//返回机器在time时刻的状态.0表示空闲，1表示工作，2表示检修
	int i,j,k;
	//检查是否在检修状态：遍历overhaul_list
	for(i=0;i<m*n;i++){
		if(overhaul_list[i][0] == -1) break;
		if(overhaul_list[i][0] == aimm){
			if(time>=overhaul_list[i][1] && time<=overhaul_list[i][2]) return 2;
		}		
	}
	
	//检查是否在工作状态
	for(j=0;j<n;j++){
		if(time >= result[aimm][j][2] && time <= result[aimm][j][3]) return 1;
	}
	
	return 0;
}

 void putforward(int aimi,int aimj){
	int i,j;
	//检查前方有无空闲时间

	if(aimj!=0 && result[aimi][aimj][2] > result[aimi][aimj-1][3] ){
		
		//检查前方空闲时间是否是检修时间
		for(i=0;i<n*m;i++){//此处可优化，但意义不大了。如果遇到有检修要求，直接跳过不前推
			if(overhaul_list[i][0] == aimi && overhaul_list[i][1] >= result[aimi][aimj-1][3] && overhaul_list[i][2] <= result[aimi][aimj][2]) return;
		}

		//既然中间段没有检修需求，那么，考察该工件的上一个工序的结束时间
		int name = result[aimi][aimj][0];
		int order = result[aimi][aimj][1];

		for(i=0;i<m;i++){
			for(j=0;j<n;j++){
				if(result[i][j][0] == name && result[i][j][1] == (order-1)){//遍历找到他的上一个工件
					//对齐该机器上一个工序的结束时间和该工件上一个工序的结束时间的最大值
					int max;
					if(result[i][j][3] > result[aimi][aimj-1][3]) max = result[i][j][3];
					else max = result[aimi][aimj-1][3];

					//计算前推时长
					int duration = result[aimi][aimj][2] -  max;
					result[aimi][aimj][2] -= duration;
					result[aimi][aimj][3] -= duration;
					
					break;
				}
			}
		}
	}
}


DWORD WINAPI overhaul_output(LPVOID pPararneter){//负责显示与时间
	int i,j,k;

	clock_t start,finish;
	start = clock();

	int mutiple = 10;//制定加工时间比


	while(  ((int)(clock()-start)/CLOCKS_PER_SEC)*mutiple  <final_result){
		TIME = ((int)(clock()-start)/CLOCKS_PER_SEC)*mutiple;

		for(i=0;i<m;i++){
			//找到当前TIME所对应的j
			for(j=0;j<n;j++){
				if( result[i][j][2] >= TIME ) break;
			} 
			
			if(j>0) j--;
			printf("%d %d ",TIME,i);
			
			int state = checkstate(i,TIME);

			if(state==2) printf("检修\n");//在检修状态
			else{
				if(state==1) {
					printf("加工 ");//加工状态 
					printf("%d-%d %d %d\n",result[i][j][0],result[i][j][1],TIME-result[i][j][2],result[i][j][3]-result[i][j][2]);
				}
				else printf("空闲\n");//空闲状态
			}
		}

		Sleep(2*1000);//停4秒，不然你以为你是手速侠么 
		printf("\n");
			
	}
	TIME = -1;//设置退出 

	return 0;
}

DWORD WINAPI overhaul_input(LPVOID pPararneter){//不仅仅是输入，功能还包括生成检修序列、后退、前推等一系列输出没有的功能
	int i,j,k,t=0;//其中,t特别用来记录这是第t个检修要求
	int aimm,lasttime;

	while(TIME<final_result){
		scanf("%d",&aimm);
		scanf("%d",&lasttime);
		printf("检修要求录入成功！\n"); 
		overhaul_list[t][0] = aimm;

		//检查目标机器的状态，生成检修要求
		int state = checkstate(aimm,TIME);
		if(state==2){//检修状态追加到检修结束时间
			for(i=0;i<t;i++){
				if(overhaul_list[i][0] == aimm && TIME >= overhaul_list[i][1] && TIME <= overhaul_list[i][2]){//多加判断，可能存在一个机器多次检修的例子
					overhaul_list[t][1] = overhaul_list[i][2];
					overhaul_list[t][2] = overhaul_list[i][2] + lasttime;
					break;
				}
			}
		}
		
		else if(state==1){//工作状态将检修过程追加到工作结束时间
			//寻找工件结束时间
			for(j=0;j<n;j++){
				if(TIME >= result[aimm][j][2] && TIME <= result[aimm][j][3]){
					overhaul_list[t][1] = result[aimm][j][3];
					overhaul_list[t][2] = result[aimm][j][3] + lasttime;
					break;
				}
			}
		}
		else{//空闲状态直接在当前状态进入检修过程
			overhaul_list[t][1] = TIME;
			overhaul_list[t][2] = TIME + lasttime; 	
		}

		//检修列生成完毕，现在进行全体后退。所有开始时间落后于检修时间开始时间的工件统一，统一加上检修时长
		for(i=0;i<m;i++){
			for(j=0;j<n;j++){
				if(result[i][j][2] >= overhaul_list[t][1]){
					result[i][j][2] += lasttime;
					result[i][j][3] += lasttime;
				}
			}
		}
		
		//全体后退完毕，现在进行前推
		for(k=0;k<n*m*2;k++){//循环多次，因为会出现当前本可以前推的工件受还未前推的工件的制约。至于为什么是n*m*2次，乱写的。
			for(i=0;i<m;i++){
				for(j=0;j<n;j++){
					putforward(i,j);
				}
			}
		}

		//动态刷新final_result
		for(i=0;i<m;i++) if(result[i][n-1][3] > final_result) final_result = result[i][n-1][3];

		
		
		//至此，为加入当前检修要求下的最优解
		t++;
	}
	 
	return 0;
}

void insert_overhaul(int aimi,int aimj){
	//先获取该工序的开始时间与上一个工序的结束时间,分别记为区间的开始与结束
	int start,end;
	if(aimj==0){
		start = 0;
		end = result[aimi][aimj][2];
	}
	else{
		start = result[aimi][aimj-1][3];
		end = result[aimi][aimj][2];
	}

	//遍历检修请求列表,查看有无落于该区间的检修请求，输出该区间内的所有检修请求
	int i,j,k;
	for(i=0;i<n*m;i++){
		if(overhaul_list[i][0]==-1)break;
		if(overhaul_list[i][1]>=start && overhaul_list[i][2]<= end && overhaul_list[i][0] == aimi) printf(" (%d,检修,%d)",overhaul_list[i][1],overhaul_list[i][2]);
	}

}