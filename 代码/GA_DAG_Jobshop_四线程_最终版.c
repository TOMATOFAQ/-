#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define ps 20//种群大小
#define mit 100 //迭代次数
#define pc 40 //交配概率
#define pm 90 //突变概率
#define totalnum 100 //未限制时间之前的遗传次数.限制时间之后就不限制次数了,直接跑到规定时间为止
#define ThreadTime 1//单次遗传运行的时间
#define programmeTime 2 //程序总运行时间

typedef struct GRAPH{//有向图的结构之节点 
    struct NODE * nodes;
}graph;

typedef struct NODE{//有向图的结构之节点之指向本节点的上一个节点们 
    int * fathers;
}node;

int *** I;	//录入信息 
int n, m;	//工件数,机器数 
int length = 0; 	//染色体长度 

//用于多线程的四个全局变量
int final_result = ((unsigned)1<<31)-1; //实时更新存放最短时间
int * final_s;
int * final_Ct;
HANDLE hMutex;//用于防止各线程之间的相互干扰

//用于输出及检修的的全局变量
int *** result;
int ** overhaul_list;
int TIME;//用来同步与输入与输出过程中的时间
//施工中

/********************************* 1-Input *************************************/ 
void LoadInstance();
/********************************* 2-InitPopulation ********************************/ 
void shuffle(int * gene);
int ** InitPopulation(); 
/********************************* 3-ComputDAG *************************************/ 
int ComputeStartTimes(int * s,int * Ct);
/********************************* 4-Crossover *************************************/  
int ** Index(int * p);
int * Crossover(int * p1,int *p2);
void Mutation(int * p);
/********************************* 5-iterator *************************************/ 
void shufflepop(int * eachtime,int ** pop);
void sortpop(int * eachtime,int ** pop);
int iterator(int ** pop,int * eachtime,int * Ct);
/********************************* 6-Output *************************************/ 
void FormatSovelution_and_display(int * s,int * Ct,int choice);

/********************************* 7-Overhaul *************************************/ 
int checkstate(int aimm,int time);
void putforward(int aimi,int aimj);
DWORD WINAPI overhaul_output(LPVOID pPararneter);
DWORD WINAPI overhaul_input(LPVOID pPararneter);
void insert_overhaul(int aimi,int aimj);
/********************************* 7-Observer-删去不影响函数功能 *************************************/ 
void printInformation();
void printfpop(int ** pop,int * eachtime);
void printfgraph();
void printfIndex(int ** idx);
void printfparent(int * p);
/********************************* 0-MAIN *************************************/ 
int once(){//一次遗传算法

	//2-初始化数据结构
	int * eachtime = (int *)malloc(sizeof(int)*ps);
	memset(eachtime,0,sizeof(int)*ps);
	int ** pop = InitPopulation();

	int * Ct = (int *)malloc(sizeof(int)*(length+1));

	int i,j;
	for(i=0;i<ps;i++){
		 eachtime[i] = ComputeStartTimes(pop[i],Ct);
		 eachtime[i] = ComputeStartTimes(pop[i],Ct);
	}

	//3-迭代
	iterator(pop,eachtime,Ct);
	
	//4-输出结果
	// FormatSovelution_and_display(pop[0],Ct,1); //显示每一次迭代的具体结果
	ComputeStartTimes(pop[0],Ct);
	
	//修改全局变量
	WaitForSingleObject(hMutex,INFINITE);
	if(Ct[length] < final_result){
		final_result = Ct[length];
		for(i=0;i<length;i++) final_s[i] = pop[0][i];
		for(i=0;i<=length;i++) final_Ct[i] = Ct[i];
	}
	ReleaseMutex(hMutex);

	printf("End Time:%d\n",final_result);

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

int main(){
	//默认重定向输入至input.txt，如果需要手动输入请注释该行或者修改输入文件
	// freopen("output.txt","w",stdout);
	int i,j,k;
	LoadInstance();
	
	clock_t start,finish;
	double duration;
	start = clock();
	
	//初始化储存四个线程答案的的全局变量
	length = n*m;
	final_s = (int *)malloc(sizeof(int)*length);
	final_Ct = (int *)malloc(sizeof(int)*(length+1));

	HANDLE hThread1,hThread2,hThread3,hThread4;
	hMutex=CreateMutex(NULL,FALSE,NULL);
	hThread1=CreateThread(NULL,0,original_main,NULL ,0,NULL);
	Sleep(1000);//改变作为随机数种子的时间 
	// hThread2=CreateThread(NULL,0,original_main,NULL ,0,NULL);
	// Sleep(1000);
	// hThread3=CreateThread(NULL,0,original_main,NULL ,0,NULL);
	// Sleep(1000);
	// hThread4=CreateThread(NULL,0,original_main,NULL ,0,NULL);
	
	CloseHandle(hThread1);
	// CloseHandle(hThread2);
	// CloseHandle(hThread3);
	// CloseHandle(hThread4);
	
	Sleep(programmeTime*1000);

	finish = clock();
	duration = (double)(finish-start)/CLOCKS_PER_SEC;
	
	//输出检修前结果
	freopen("output.txt", "w", stdout);//重定向
	FormatSovelution_and_display(final_s,final_Ct,1);
	printf("Time Used:%.3lf\n",duration);
	printf("End Time:%d\n",final_result);
	freopen("CON", "w", stdout);//回到控制台
	
	
	//在控制台检验结果
	FormatSovelution_and_display(final_s,final_Ct,1);
	printf("Time Used:%.3lf\n",duration);
	printf("End Time:%d\n",final_result);
	 
	
	//检修过程
	printf("The result had been outputed into output.txt.Please enter num 1 to overhaul.\n");//可能验收的时候这里要暂停 
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
	 
	
	
	//内存释放
	for (i = 0; i<n; i++) {
		for (j = 0; j<m; j++) free(I[i][j]);
		free(I[i]);
	}
	free(final_s);
	free(final_Ct);
	free(I);

	
	for(i=0;i<m;i++){
		for(j=0;j<n;j++) free(result[i][j]);
		free(result[i]);
	}
	free(result);

	return 0;
}

/********************************* 0-MAIN-END *************************************/ 
/********************************* 1-INPUT *************************************/ 
void LoadInstance(){
	/*补充对I的说明：
	I[i][j][0] 表示第i个工件第j个工序所需要的机器,若值为0，说明不存在这个工序
	I[i][j][1] 表示第i个工件第j个工序所需要的时间,若值为0，说明不存在这个工序
	i,j都从头开始计算 
	*/

	freopen("input.txt", "r", stdin);//重定向

	int i, j, k;//循环辅助变量
	scanf("%d %d", &n, &m);

	I = (int ***)malloc(sizeof(int**)*n);
	for (i = 0; i<n; i++) {
		I[i] = (int **)malloc(sizeof(int*)*m);
		for (j = 0; j<m; j++) {
			I[i][j] = (int *)malloc(sizeof(int) * 2);
			memset(I[i][j], 0, sizeof(I[i][j]));
		}
	}

	for (i = 0; i<n; i++) {//第i个工件
		for(j=0;j<m;j++){
			scanf("%d",&I[i][j][0]);
			scanf("%d",&I[i][j][1]);
			}	
	}

	freopen("CON", "r", stdin);//回到控制台
	//内存释放:I在主函数free
}
/********************************* 1-INPUT-END *************************************/
/********************************* 2-InitPopulation *************************************/ 

void shuffle(int * gene) {//洗牌函数,打乱基因顺序
	int i;
	for (i = 0; i<length; i++) {
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

	free(gene);

	return population;
}


/********************************* 2-InitPopulation-END *************************************/ 
/********************************* 3-ComputDAG *************************************/ 
int ComputeStartTimes(int * s,int * Ct){//计算该染色体下所需要的时间

	//computeDAG部分
    int i,j,k;
	graph G;

    G.nodes = (node*)malloc(sizeof(node)*(length+1));//第length+1为终止节点,G.nodes[length]即为终止节点 
	for(i=0;i<length+1;i++){
		G.nodes[i].fathers = (int *)malloc(sizeof(int)*(length)); 
		memset(G.nodes[i].fathers,-1,sizeof(int)*length);
	}

    // 初始化一个长度为工件数的全0list
    int * T = (int *)malloc(sizeof(int)*n);
    memset(T,0,sizeof(int)*n);

    // 构造存储每个工件上一次加入图的工序号，初始化为-1
    int * last_task_job = (int *)malloc(sizeof(int)*n);
    memset(last_task_job,-1,sizeof(int)*n);

    int ** tasks_resource = (int **)malloc(sizeof(int*)*m);//tasks_resource 的第一个坐标小于机器数,第二个坐标小于工件数
    for(i=0;i<m;i++){
        tasks_resource[i] = (int *)malloc(sizeof(int)*n);
        memset(tasks_resource[i],-1,sizeof(int)*n);
    }

    int * st = (int *)malloc(sizeof(int)*length);
    memset(st,0,sizeof(int)*length);
    
    for(i=0;i<length;i++){
        int j = s[i];//提取编号为i的节点对应的工件号
        int t = T[j];//辅助变量,记录编号i的节点是j工件内的第几道工序

        st[i] = t;//将t存入st

		
        int r = I[j][t][0];//将j工件的第t道工序对应有序偶中的第一个数字，即该工序的机器号存入变量r

        //判断j的相对工序号的计数器t的值是否等于j的工序数，若相等则将j的最后一道工序指向终止节点
		int gongxushu_j = m;//根据输入的数据来看,所有机器都是做满m台机器的,故假设每台机器都只是m台工序
   
        if(t+1 == gongxushu_j) {//0,1,2,3,4,当t=4时,就是最后一个工序了
        	int tempnum;
        	for(tempnum = 0;G.nodes[length].fathers[tempnum]!=-1;tempnum++);//找终止节点的第一个空的位置
        	
        	G.nodes[length].fathers[tempnum] = i;//存入指向终止节点的节点在染色体上的位置
		}
		 
        //若不是j的第一道工序则将该工序的前一道工序指向代表当前工序的节点i
        if(t>0) {
        	int tempnum;
        	for(tempnum = 0;G.nodes[i].fathers[tempnum]>-1;tempnum++);//找到
        	G.nodes[i].fathers[tempnum] = last_task_job[j];
		}

        //如果之前加入的节点也在使用和j的第t道工序相同的机器r,则将这些节点编号指向当前处理节点i
        
        int j2;
        for(j2=0;j2<n;j2++){
            if(j2!=j && (tasks_resource[ r ][ j2 ] > -1)){
			//tasks_resource 的第一个坐标小于机器数,第二个坐标小于工件数
                int tempnum;
        		for(tempnum = 0;G.nodes[i].fathers[tempnum]!=-1;tempnum++);
        		G.nodes[i].fathers[tempnum] = tasks_resource[r][j2];
            }
        }

        T[j] = T[j] + 1;	//j工件的工序号累加器加1
        last_task_job[j] = i;	//将j工件的上一道工序的节点编号置为当前节点的编号i
        tasks_resource[r][j] = i;	//将j工件中正在使用机器r的工序对应的节点编号置为i
    }


	//内存释放
	free(T);
	free(last_task_job);
	for(i=0;i<m;i++) free(tasks_resource[i]);
	free(tasks_resource);

    int nodenum = length+1;
    int * C = (int *)malloc(sizeof(int)*nodenum);
    memset(C,0,sizeof(C));

    for(i=0;i<nodenum;i++){
    	
    	int length_of_G_nodes_fathers_i;
    	for(length_of_G_nodes_fathers_i=0;length_of_G_nodes_fathers_i<length && G.nodes[i].fathers[length_of_G_nodes_fathers_i]!=-1;length_of_G_nodes_fathers_i++);
    	
        if(length_of_G_nodes_fathers_i == 0 ) C[i] = 0;
        else{
            int max = 0;
			int k;
            for(k=0;k<length_of_G_nodes_fathers_i;k++){
				int l = G.nodes[i].fathers[k];
                if((C[l] + I[s[l]][st[l]][1]) > max) max = C[l] + I[s[l]][st[l]][1];				
            }
            C[i] = max;
        }
    }

	for(i=0;i<nodenum;i++) Ct[i] = C[i];

	//内存释放
	for(i=0;i<=length;i++) free(G.nodes[i].fathers);
	free(G.nodes);
	free(st);
	free(C);
	
    return Ct[length];
}
/********************************* 3-ComputDAG-END *************************************/ 



/********************************* 4-Crossover *************************************/ 
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

/********************************* 4-Crossover-END*************************************/ 
/********************************* 5-iterator *************************************/ 
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

/********************************* 5-iterator-END *************************************/ 
/********************************* 6-Output *************************************/ 
void FormatSovelution_and_display(int * s,int * Ct,int choice){//choice为0:不输出工序.choice为1,输出工序

	ComputeStartTimes(s,Ct);//再算一次,刷新Ct.函数设计失误,不应该用全局变量的.不过考虑到一次程序只算一次,忽略不计了. //原因未知的bug

    int i,j,k;
    int * T = (int *)malloc(sizeof(int)*n);
    memset(T,0,sizeof(int)*n);
	
	//S中存放的是每道工序开始加工的时间，它的形式为：[[a,b,c],[d,e,f],[g,h,i]],每个子list代表一个工件的信息，子list中的字母代表这个工件下面每道工序开始加工的时间。
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
	
	result = (int ***)malloc(sizeof(int**)*m);
	for(i=0;i<m;i++){//第i台机器
		result[i] = (int **)malloc(sizeof(int*)*n);
		for(j=0;j<n;j++){//第i台机器的第j个工序
			result[i][j] = (int *)malloc(sizeof(int)*4);//第i台机器的第j个工序所对应工件的序号,和这个工件的所在工序,以及开始时间,终止时间
			memset(result[i][j],-1,sizeof(int)*4);//-1表示没访问过
		}
	}
	//result这个数组里面,对于每一台机器.需要知道占用的时间.被占用的机器.如果机器是只在加工时间里加工一次的话,那么就不需要知道是第几次了,搜索即可.//算了,还是顺便存上好了.


	//生成
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

	//排序.对每一个机器都进行一次排序
	for(i=0;i<m;i++){//排序每个工序的开始时间
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


	if(choice){
		for(i=0;i<m;i++){//第i台机器
			printf("M%d",i);
			for(j=0;j<n;j++)//第j个工序
				printf(" (%d,%d-%d,%d)",result[i][j][2],result[i][j][0],result[i][j][1],result[i][j][3]);
			printf("\n");
		}		
	}

	//内存释放
	free(T);
	
    for(i=0;i<n;i++) free(S[i]);
    free(S);
} 
/********************************* 6-Output-END *************************************/ 
/********************************* 7-overhaul *************************************/ 

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
	if((aimj!=0 && result[aimi][aimj][2] > result[aimi][aimj-1][3] )|| aimj==0){

		int name = result[aimi][aimj][0];
		int order = result[aimi][aimj][1];

		int duration;
		
		//两个特判 
		if(aimj==0){//是整个机器的第0个工序，但不是该工件的第0个工序 
			if(result[aimi][aimj][1]!=0) return;
			
			duration = result[aimi][aimj][2];
			result[aimi][aimj][2] -= duration;
			result[aimi][aimj][3] -= duration;
			return;
		}

		if(order==0){//是整个工件的第0个工序//我为什么要写这段？ 
			duration = result[aimi][aimj][2]-result[aimi][aimj-1][3];
			result[aimi][aimj][2] -= duration;
			result[aimi][aimj][3] -= duration;
			return;
		}
			

		//检查前方空闲时间是否是检修时间
		for(i=0;i<n*m;i++){//此处可优化，但意义不大了。如果遇到有检修要求，直接跳过不前推
			if(overhaul_list[i][0] == aimi && overhaul_list[i][1] >= result[aimi][aimj-1][3] && overhaul_list[i][2] <= result[aimi][aimj][2]) return;
		}

		//既然中间段没有检修需求，那么，考察该工件的上一个工序的结束时间
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

	int mutiple = 100;//制定加工时间比


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

		Sleep(3*1000);//停4秒，不然你以为你是手速侠么 
		printf("\n");
			
	}
	TIME = -1;//设置退出 
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
			for(i<0;i<t;i++){
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
/********************************* 7-overhaul-end*************************************/ 
/********************************* 7-observer-(删去不影响代码功能) *************************************/ 
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
	int i,j,k;
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

/********************************* 7-observer-END *************************************/ 


