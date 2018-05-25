#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//需要人为调参的量: 
#define ps 5 //种群大小 population size
#define mit 3 //迭代次数 mount for iteratior
#define pc 0 //交配概率 probability for crossover
#define pm 0 //突变概率 probability for mutation

int *** I;//录入信息 
int n, m;//工件数,机器数 
int length = 0; //染色体长度 

typedef struct GRAPH{//有向图的结构之节点
    struct NODE * children;
}graph;
typedef struct NODE{//有向图的结构之节点
    int * son;
}node;
graph G;
int * st;//一条打乱的染色体 

int * Ct;//存放每道工序开始加工的时间

/********************************* 1-Input *************************************/ 
void printInformation();
void LoadInstance();
/********************************* 2-InitPopulation ********************************/ 
void shuffle(int * gene);
int ** InitPopulation(); 
void displaypop(int ** pop,int * eachtime);
/********************************* 3-ComputDAG *************************************/ 
void ComputeDAG(int * s);
int ComputeStartTimes(int * s);
/********************************* 4-Crossover *************************************/ 
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b)) 
int ** Index(int * p);
void printfIndex(int ** idx);
void printfparent(int * p);
int * Crossover(int * p1,int *p2);
void * Mutation(int * p);
/********************************* 5-iterator *************************************/ 
void shufflepop(int * eachtime,int ** pop);
void sortpop(int * eachtime,int ** pop);
int iterator(int ** pop,int * eachtime);
/********************************* 6-Output *************************************/ 
void FormatSolution_and_display(int * s,int * C){//这里不应该用全局变量

    int i,j,k;
    int * T = (int *)malloc(sizeof(int)*n);
    memset(T,0,sizeof(int)*n);

    int ** S = (int **)malloc(sizeof(int *)*n);
    for(i=0;i<n;i++){
        S[i] = (int *)malloc(sizeof(int)*m);
        memset(S[i],0,sizeof(int)*m);
    }

    for(i=0;i<length;i++){
        int j = s[i];//获得实际工件号
        int t = T[j-1];//获得这是第j个工件的第t个工序
        S[j-1][t] = Ct[i]; //第j-1个工件的第t个工序的开始时间
        T[j-1] = T[j-1] + 1
		;
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

	//展示原数据s
	printf("s:");
	for(i=0;i<length;i++){
		printf("%d ",s[i]);
	} printf("\n");

	//展示原数据Ct
	printf("Ct:");
	for(i=0;i<=length;i++){
		printf("%d ",C[i]);
	} printf("\n");
	
	//展示原数据S
		printf("S:\n");
	for(i=0;i<n;i++){
		for(j=0;j<m;j++) printf("%d ",S[i][j]);
		printf("\n");
	} printf("\n");

	//那么,先生成,
	for(i=0;i<n;i++){ //第i个工件
		for(j=0;j<m;j++){ //第i个工件的第j个工序
			// S[i][j];//第i个工件的第j个工序所开始的时间
			for(k=0;k<m;k++){//找到第k个空位
				if(result[I[i][j][0]-1][k][0]==-1) break;
			}
			result[ I[i][j][0] -1][k][0] = i;
			result[ I[i][j][0] -1][k][1] = j;
			result[ I[i][j][0] -1][k][2] = S[i][j];
			result[ I[i][j][0] -1][k][3] = S[i][j] + I[i][j][1];
		}
	}

	//再排序吧.对每一个机器都进行一次排序
	for(i=0;i<m;i++){
		//冒泡排序
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
/********************************* 6-Output-END *************************************/ 
/********************************* 0-MAIN *************************************/ 
int main(){
	srand((unsigned)time(NULL));
	//1-读入信息
	LoadInstance();
	printInformation();

	//2-初始化
	int * eachtime = (int *)malloc(sizeof(int)*ps);
	int ** pop = InitPopulation();

	int i;
	for(i=0;i<ps;i++){
		 eachtime[i] = ComputeStartTimes(pop[i]) ;
		 printf("%d\n",ComputeStartTimes(pop[i]) );
	}
	
//	sortpop(eachtime,pop);
	displaypop(pop,eachtime);
	
	//测试	 
	 printf("\n");
	 for(i=0;i<ps;i++){
		 printf("%d\n",ComputeStartTimes(pop[i]));
	}
	
	
	
	//3-迭代
	iterator(pop,eachtime);

	//4-输出结果
	FormatSolution_and_display(pop[0],Ct);
	return 0;
}

/********************************* 0-MAIN-END *************************************/ 
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

void sortpop(int * eachtime,int ** pop){
	int i,j;
	for(i=0;i<ps;i++){
		for(j=0;j<ps-1;j++){
			if(eachtime[j]<eachtime[j+1]){
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
//	 			
//	 			printf("交叉前的第%d条和第%d条染色体:\n",i,ps/2+i);
//	 			printfparent(pop[i]);
//	 			printfparent(pop[ps/2+i]);
	 			
	 			ch1 = Crossover(pop[i],pop[ps/2+i]); 
	 			ch2 = Crossover(pop[ps/2+i],pop[i]); 
	 			
//	 			printf("交叉后的第%d条和第%d条染色体:\n",i,ps/2+i);
//	 			printfparent(ch1);
//	 			printfparent(ch2);
	 			
	 			if((rand() % 100) > pm){
					Mutation(ch1);
//					printf("变异后的第%d条染色体:\n",i);
//		 			printfparent(ch1);
				 }

				if((rand() % 100) > pm) {

					Mutation(ch2);
//					printf("变异后的第%d条染色体:\n",ps/2 + i);
//	 				printfparent(ch2);
				}
				
	 			time1 = ComputeStartTimes(ch1);
	 			time2 = ComputeStartTimes(ch2);
	 			
//	 			printf("%d %d\n",time1,time2);
	 			
				//这里写反了啊......什么鬼啊sort函数是从大往小排啊,那到底是什么地方搞错了
	 			sortpop(eachtime,pop);
	 			int replace1 = 1;
	 			int replace2 = 1;
	 			for(j=0;j<ps;j++){
	 				
	 				if(time1 >= eachtime[j] && replace1){
	 					eachtime[j] = time1;
	 					pop[j] = ch1;
	 					replace1 = 0;
					 }
					 
					 if(time2 >= eachtime[j] && replace2){
	 					eachtime[j] = time2;
	 					pop[j] = ch2;
	 					replace2 = 0;
					 }
				 }
				 
				 shufflepop(eachtime,pop);
			 }
		 }
		 
		sortpop(eachtime,pop);
		printf("迭代第%d次的答案:%d\n",it,eachtime[0]); 
//		printf("%d  ",eachtime[0]); 
		displaypop(pop,eachtime);//加速 
		
	//测试	 
	 printf("\n");
	 for(i=0;i<ps;i++){
		 printf("%d\n",ComputeStartTimes(pop[i]));
	 	}
	 }
	 
	 sortpop(eachtime,pop);

	 

	 return eachtime[0];
}
/********************************* 5-iterator-END *************************************/ 













/********************************* 2-InitPopulation *************************************/ 


void shuffle(int * gene) {//洗牌函数.这里对这个基因随便洗牌length/2次.太多也不好浪费时间.这个洗牌次数应该是可以由经验的出来的.
	int i;
	for (i = 0; i<length; i++) {//******************************************随机数是什么情况 
		int loc1 = rand() % length;
		int temp = gene[i];
		gene[i] = gene[loc1];
		gene[loc1] = temp;
	}
}

int ** InitPopulation(){//原本将输入两个变量,ps和I,这里先取消
//确定一个染色体的长度,即确定工序的总数,下面确定工序的总数
	int i = 0, j = 0, k = 0;
	for (i = 0;i<n; i++) {
		for (j = 0;j<n&&I[i][j][0]!=0; j++) {
				length++;
		}
	}

	int * gene = (int *)malloc(sizeof(int)*length);
	for (j = 1, i = 0;j<=n; j++) {//j表示对工件的遍历,i表示基因上第i个位置
		int t;//t表示对指定的工件的工序的遍历
		for (t = 0;t<m&&I[j-1][t][0]!=0; t++, i++) {
			gene[i] = j;
		}
	}

	int ** population = (int **)malloc(sizeof(int*)*ps);
	for (i = 0; i<ps; i++) {
		shuffle(gene);
		population[i] = (int *)malloc(sizeof(int)*length);         //将ps个基因凭借在一条染色体上
		for (j = 0; j<length; j++) {
			population[i][j] = gene[j];
		}
	}
	return population;
}

void displaypop(int ** pop,int * eachtime){//测试生成种群函数用的 
	int i,j;
	for(i=0;i<ps;i++){
		printf("The %2d one use %2d:",i+1,eachtime[i]);
		for(j=0;j<length;j++){
			printf("%3d",pop[i][j]);
		}
		printf("\n");
	}
}
/********************************* 2-InitPopulation-END *************************************/ 













/********************************* 3-ComputDAG *************************************/ 
void ComputeDAG(int * s){//构建有向图,s为染色体的乱序排列
	int i,j,k;//循环的辅助变量

    G.children = (node*)malloc(sizeof(node)*(length+1));//G.children[length]即为终止节点 
	for(i=0;i<length+1;i++){
		G.children[i].son = (int *)malloc(sizeof(int)*(length)); // 加5仅仅是习惯上防止溢出尔 
		memset(G.children[i].son,0,sizeof(G.children[i].son));
	}

//    初始化一个长度为工件数的全0list
    int * T = (int *)malloc(sizeof(int)*n);
    memset(T,0,sizeof(int)*n);

    //构造存储每个工件上一次加入图的工序号，初始化为-1
    int * last_task_job = (int *)malloc(sizeof(int)*n);
    memset(last_task_job,-1,sizeof(last_task_job));

    int ** tasks_resource = (int **)malloc(sizeof(int*)*m);
    for(i=0;i<n;i++){
        tasks_resource[i] = (int *)malloc(sizeof(int)*n);
        memset(tasks_resource[i],-1,sizeof(tasks_resource));
    }

    st = (int *)malloc(sizeof(int)*length);
    memset(st,0,sizeof(int)*length);
    
    for(i=0;i<length;i++){
    	
        int j = s[i] - 1;//提取编号为i的节点对应的工件号
        int t = T[j];//辅助变量,记录编号i的节点是j工件内的第几道工序

        st[i] = t;//将t存入st
//        
//        printf("j and t : %d %d\n",j,t);
//        int temp;
////        printf("s:");
////		for(temp = 0;temp<length;temp++) printf("%d ",s[temp]);
////		printf("\n");
//		printf("T:");
//		for(temp = 0;temp<n;temp++) printf("%d ",T[temp]);
//		printf("\n");
//		printf("st:");
//		for(temp = 0;temp<length;temp++) printf("%d ",st[temp]);
//		printf("\n\n");
		
        int r = I[j][t][0] - 1;//将j工件的第t道工序对应有序偶中的第一个数字，即该工序的机器号存入变量r

        //判断j的相对工序号的计数器t的值是否等于j的工序数，若相等则将j的最后一道工序指向终止节点
		int gongxushu_j;
		for(gongxushu_j=0;gongxushu_j<m && I[j][gongxushu_j][0]!=0;gongxushu_j++);
        if(t+1 == gongxushu_j) {
        	int tempnum;
        	for(tempnum = 0;G.children[length].son[tempnum]!=0;tempnum++);
        	
        	G.children[length].son[tempnum] = i;
		}
		 
        //若不是j的第一道工序则将该工序的前一道工序指向代表当前工序的节点i

        if(t>0) {
        	int tempnum;
        	for(tempnum = 0;G.children[length].son[tempnum]!=0;tempnum++);
        	G.children[i].son[tempnum] = last_task_job[j];
		}

        //如果之前加入的节点也在使用和j的第t道工序相同的机器r,则将这些节点编号指向当前处理节点i
        int j2;
        for(j2=0;j2<n;j2++){
            if(j2!=j && tasks_resource[r][j2] > -1){
                int tempnum;
        		for(tempnum = 0;G.children[length].son[tempnum]!=0;tempnum++);
        		G.children[i].son[tempnum] = tasks_resource[r][j2];
            }
        }

        T[j] = T[j] + 1;	//j工件的工序号累加器加1
        last_task_job[j] = i;	//将j工件的上一道工序的节点编号置为当前节点的编号i
        tasks_resource[r][j] = i;	//将j工件中正在使用机器r的工序对应的节点编号置为i
    }
}

int ComputeStartTimes(int * s){//计算该染色体下所需要的时间
    ComputeDAG(s);//得到了一个图,得到了节点所对应的第 n 个工序
    
    int nodenum = length+1;
    int * C = (int *)malloc(sizeof(int)*nodenum);
    memset(C,0,sizeof(C));

    int i;
    for(i=0;i<nodenum;i++){
    	
    	int length_of_G_children_son_i;
    	for(length_of_G_children_son_i=0;length_of_G_children_son_i<length && G.children[i].son[length_of_G_children_son_i]!=0;length_of_G_children_son_i++);
    	
        if(length_of_G_children_son_i ==0 )C[i] = 0;
        else{
            int max = 0;
            int k;
            for(k=0;k<length_of_G_children_son_i;k++){
                if((C[k] + I[s[k] - 1][st[k]][1]) > max) max = C[k] + I[s[k]-1][st[k]][1];
            }
            C[i] = max;
        }
    }
	Ct = C;
    return C[length];
}
/********************************* 3-ComputDAG-END *************************************/ 











/********************************* 4-Crossover *************************************/ 
//至于为什么这里会写得那么麻烦,是为了不破坏在第一部中已经确立好了工序关系 
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b)) 

int ** Index(int * p){//返回一个二元对的数组,每个二元对中第一个表示的是工件,第二个表示的是顺序.
//只是相当于高级一点的结构而已 
    int * ct = (int *)malloc(sizeof(int)*n);
    memset(ct,0,sizeof(int)*n);

    int ** s = (int **)malloc(sizeof(int*)*length);
    int i,j;
    for(i=0;i<length;i++) s[i] = (int*)malloc(sizeof(int)*2);
    
    for(i=0,j=0;i<length;i++){
        s[i][0] = p[i];
        s[i][1] = ct[p[i]-1]; 
        ct[p[i]-1] = ct[p[i]-1] + 1;
    }
    return s;
}

void printfIndex(int ** idx){//辅助函数,用于观察内部情况 
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

int * Crossover(int * p1,int *p2){//p1,p2是两条等待交叉的染色体,p为parent的缩写
    int i,j,k;//用于循环的辅助变量
    int ** idx_p1 = Index(p1);
    int ** idx_p2 = Index(p2);

//	printfIndex(idx_p1);
//	printfIndex(idx_p2);

    int randi = (rand()%(length-1))+1;//生成[1,nt)的随机数,表示将嵌入的变异片段之长 
    int randj = rand()%(length-randi);//生成[0,nt-randi-1)的随机数, 表示变异片段在parent1中的起始位置 
    int randk = rand()%length;//生成[0,nt)的随机数,表示变异片段在parent2中的起始位置 

//	printf("%d %d %d \n",randi,randj,randk);

    //implant 意为嵌入 .生成将变异片段. 
    int ** implant =  (int **)malloc(sizeof(int*)*(randi));
    for(i=0;i<randi;i++){
    	implant[i] = (int *)malloc(sizeof(int)*2);
		implant[i][0] = idx_p1[randj+i][0];
		implant[i][1] = idx_p1[randj+i][1];
	}
	
	//同时检测parent2中与嵌入片段重复的内容,并将其删除 
	int * aftermutation = (int *)malloc(sizeof(int)*length) ;
	for(i=0;i<length;i++){
			for(j=0;j<randi;j++){
				if(idx_p2[i][0] == implant[j][0] && idx_p2[i][1] == implant[j][1]) idx_p2[i][0] = -1;//做上标记 
			}
	}
	
//	for(i=0;i<randi;i++) printf("%d ",implant[i][0]);
//	printf("\n");
//	
//	for(i=0;i<length;i++) printf("%d ",idx_p2[i][0]);
//	printf("\n");

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
	
//	printfparent(aftermutation);
	return aftermutation;
}

void * Mutation(int * p){
    int nt = length;        
    int ranki = rand()%nt;
    int rankj = rand()%nt;

    int temp;
    temp = p[ranki];
    p[ranki] = p[rankj];
    p[rankj] = temp;
}
/********************************* 4-Crossover-END*************************************/ 














/********************************* 1-INPUT *************************************/ 
void printInformation() {
	int i, j;
	printf("工件信息：(机器号，所需时间)\n");
	for (i = 0; i<n; i++) {
		printf("工件%2d 所需工序:", i+1);
		for (j = 0; j<m; j++)    printf("(%2d,%2d),", I[i][j][0], I[i][j][1]);
		printf("\n");
	}
}

void LoadInstance() {
	/*补充对I的说明：
	I[i][j][0] 表示第i个工件第j个工序所需要的机器,若值为0，说明不存在这个工序
	I[i][j][1] 表示第i个工件第j个工序所需要的时间,若值为0，说明不存在这个工序
	*/
	freopen("input.txt", "r", stdin);//默认重定向输入至input.txt，如果需要手动输入请注释改行或者修改输入文件
	int i, j, k;//循环辅助变量
	scanf("%d %d", &n, &m);

	I = (int ***)malloc(sizeof(int**)*n);
	for (i = 0; i<n; i++) {
		I[i] = (int **)malloc(sizeof(int*)*m);
		for (j = 0; j<m; j++) {
			I[i][j] = (int *)malloc(sizeof(int) * 2);
			memset(I[i][j], 0, sizeof(I[i][j]));//0表示不存在这个工序
		}
	}

	//处理输入n，m时残留的空格和回车符号
	char ch = getchar();
	ch = getchar();
	ch = getchar();

	for (i = 0; i<n; i++) {//第i个工件
		ch = '~';//初始化
		j = 0;//第j个工序
		while (ch != '\n') {
			ch = getchar();//删除无关的字符
			if (ch == '(') {
				scanf("%d", &I[i][j][1]);
				getchar();
				scanf("%d", &I[i][j][0]);
				/////
				I[i][j][0] = I[i][j][0] +1;
				j++;
			}
		}
	}
}

/********************************* 1-INPUT-END *************************************/
