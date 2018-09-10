#include "jobshop.h"
#include "graphics.h"

int *** I;	//录入信息 
int *** result;
int *** ganttorignal;
int n, m;	//工件数,机器数 
int length = 0; 	//染色体长度 
double duration;
double duraorignal;

//用于多线程的四个全局变量
int final_result = ((unsigned)1<<31)-1; //实时更新存放最短时间
int finalorignal;
int * final_s;
int * final_Ct;
HANDLE hMutex;//用于防止各线程之间的相互干扰

int ** overhaul_list;
int TIME;//用来同步与输入与输出过程中的时间

int once(){//一次遗传算法

	//2-初始化数据结构
	int * eachtime = (int *)malloc(sizeof(int)*ps);
	memset(eachtime,0,sizeof(int)*ps);
	int ** pop = InitPopulation();

	int * Ct = (int *)malloc(sizeof(int)*(length+1));

	int i;
	for(i=0;i<ps;i++){
		 eachtime[i] = ComputeStartTimes(pop[i],Ct);
		 eachtime[i] = ComputeStartTimes(pop[i],Ct);
	}

	//3-迭代
	iterator(pop,eachtime,Ct);
	
	//4-输出结果
	// FormatSolution_and_display(pop[0],Ct,1); //显示每一次迭代的具体结果
	ComputeStartTimes(pop[0],Ct);
	
	// printf("Time Used:%.3lf\n",Ct[0]);
	printf("End Time:%d\n",final_result);

	//修改全局变量
	WaitForSingleObject(hMutex,INFINITE);
	if(Ct[length] < final_result){
		final_result = Ct[length];
		for(i=0;i<length;i++) final_s[i] = pop[0][i];
		for(i=0;i<=length;i++) final_Ct[i] = Ct[i];
	}
	ReleaseMutex(hMutex);

	//内粗释放及返回最优解
	int result = Ct[length];
	free(eachtime);
	free(Ct);
	for(i=0;i<ps;i++) free(pop[i]);
	free(pop);

	return result;
}


DWORD WINAPI original_main(LPVOID pPararneter){//原先的一次main函数,现在改为一线程 
	//初始化时间相关信息
	srand((unsigned)time(NULL));

	clock_t start,finish;
	double duration;
	start = clock();

	//运行多次遗传算法
	int min = ((unsigned)1<<31)-1; 
	int i,j;	
	for(i=0;;i++) {//直接跑到时间结束
		int result = once();
		if(result<min) min = result;
		// printf("最小:%-4d,第%d次遗传算法:%4d,time:%d\n",min,i,result,(int)((clock()-start)/CLOCKS_PER_SEC));
		if((double)((clock()-start)/CLOCKS_PER_SEC) > ThreadTime) break;
	}

	//输出结果
	finish = clock();
	duration = (double)(finish-start)/CLOCKS_PER_SEC;
	// printf("%f seconds.\n",duration);
	
	if(min<final_result) final_result = min;
	
	return 0;
}

int main(int argc, char**argv){
	int i,j;
	setinitmode(0,0,0);
	initgraph(1280,716);
	PIMAGE waiting;
	waiting=newimage();
	getimage (waiting,"waiting.jpg",0,0);
	putimage(0,0,waiting);
	//默认重定向输入至input.txt，如果需要手动输入请注释该行或者修改输入文件
	//freopen("output.txt","w",stdout);

	LoadInstance();
	
	clock_t start,finish;
	start = clock();
	
	//初始化储存四个线程答案的的全局变量
	length = n*m;
	final_s = (int *)malloc(sizeof(int)*length);
	final_Ct = (int *)malloc(sizeof(int)*(length+1));

	HANDLE hThread1,hThread2,hThread3,hThread4;
	hMutex=CreateMutex(NULL,FALSE,NULL);
	hThread1=CreateThread(NULL,0,original_main,NULL ,0,NULL);
	Sleep(1000);//改变作为随机数种子的时间 
	hThread2=CreateThread(NULL,0,original_main,NULL ,0,NULL);
	Sleep(1000);
	hThread3=CreateThread(NULL,0,original_main,NULL ,0,NULL);
	Sleep(1000);
	hThread4=CreateThread(NULL,0,original_main,NULL ,0,NULL);
	
	CloseHandle(hThread1);
	CloseHandle(hThread2);
	CloseHandle(hThread3);
	CloseHandle(hThread4);
	
	Sleep(programmeTime*1000);

	finish = clock();
	duration = (double)(finish-start)/CLOCKS_PER_SEC;
	
	//输出检修前结果
	freopen("output.txt", "w", stdout);//重定向
	FormatSoverhaul_listution_and_display(final_s,final_Ct,1);
	printf("Time Used:%.3lf\n",duration);
	duraorignal=duration;
	printf("End Time:%d\n",final_result);
	finalorignal=final_result;
	freopen("CON", "w", stdout);//回到控制台
	
	
	//在控制台检验结果
	FormatSoverhaul_listution_and_display(final_s,final_Ct,1);
	printf("Time Used:%.3lf\n",duration);
	printf("End Time:%d\n",final_result);
	 
	image_ganttorignal();
	image_list();

	//内存释放
	for (i = 0; i<n; i++) {
		for (j = 0; j<m; j++) free(I[i][j]);
		free(I[i]);
	}
	free(final_s);
	free(final_Ct);
	free(I);

	
	for(i=0;i<m;i++){
		for(j=0;j<n;j++){
			free(result[i][j]);
			free(ganttorignal[i][j]);
		}
		free(result[i]);
		free(ganttorignal[i]);
	}
	free(result);
	free(ganttorignal);

	return 0;
}
