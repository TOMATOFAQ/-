#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ps 10 //种群大小 
#define mit 10 //迭代次数 
int *** I;
int n, m;
void printInformation();
void LoadInstance();

int findlen(int * a){
	return sizeof(a)/sizeof(int);
}

/********************************* InitPopulation *************************************/ 
int length = 0; //染色体长度 

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

void displaypop(int ** pop){//测试生成种群函数用的 
	int i,j;
	for(i=0;i<ps;i++){
		printf("%2d:",i+1);
		for(j=0;j<length;j++){
			printf("%2d",pop[i][j]);
		}
		printf("\n");
	}
}
/********************************* InitPopulation *************************************/ 
/********************************* ComputDAG *************************************/ 
struct GRAPH{//有向图的结构之节点
    struct NODE * children;
};

struct NODE{//有向图的结构之节点
    int * son;
};

typedef struct NODE node;
typedef struct GRAPH graph;

graph G;
int * st;

/*
s为染色体的乱序排列，I为输入信息;由于这个函数设计成需要传两个变量回主函数,故在此传指针.具体的操作说明:
在主函数中定义变量G,st.然后取地址传入其中.即ComputDAG(s,I,&G,&st);即可. 
*/
 
void ComputeDAG(int * s){//构建有向图
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
//		for(temp = 0;temp<length;temp++) printf("%d  ",s[temp]);
//		printf("\n");
//		for(temp = 0;temp<n;temp++) printf("%d  ",T[temp]);
//		printf("\n");
//		for(temp = 0;temp<length;temp++) printf("%d  ",st[temp]);
//		printf("\n");
		
		
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
    return C[length];
}

/********************************* ComputDAG *************************************/ 
/********************************* Crossover *************************************/ 

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b)) 
 
void Remove(int ** child,int i){
    for(;findlen(child[i]);i++) child[i] = child[i+1];
}

int ** Index(int * p1){
    int * ct = (int *)malloc(sizeof(int)*n);
    memset(ct,0,sizeof(int)*n);

    int ** s = (int **)malloc(sizeof(int*)*findlen(p1));
    int i,j;
    for(i=0;i<length;i++) s[i] = (int*)malloc(sizeof(int)*2);
    
    for(i=0,j=0;i<length;i++){
        s[i][0] = p1[i];
        s[i][1] = ct[p1[i]]; 
        ct[p1[i]]++;
    }
    
    return s;
}

int * Crossover(int * p1,int *p2){//p1,p2是两条等待交叉的染色体,p为parent的缩写
    int i,j,k;//用于循环的辅助变量
    int ** idx_p1 = Index(p1);
    int ** idx_p2 = Index(p2);

    int nt = findlen(p1);
    int randi = (rand()%(nt-1))+1;//生成[1,nt)的随机数
    int randj = rand()%(nt-1);//生成[0,nt-1)的随机数
    int randk = rand()%nt;//生成[0,nt)的随机数

    //implant 相当于上面的“(A,2)(C,1)(A,3)(B,3)”即从Parent2抽取的片段 
    int *** implant =  (int ***)malloc(sizeof(int**)*(MIN(randi+randj,nt)+MIN(randj+randi,nt)));
    
    j=0;
    for(i=randj;i<MIN(randj+randi,nt);i++,j++){
        implant[j] = &idx_p1[i];
    }
    for(i=0;i<(randi-MIN(randj+randi,nt)+randj);i++,j++){
        implant[j] = &idx_p1[i];
    }

    int ** lft_child = (int **)malloc(sizeof(int *)*randk);
    for(i=0;i<randk;i++){
        lft_child[i] = idx_p2[i];
    }
    
    int ** rgt_child = (int **)malloc(sizeof(int *)*(nt-randk));
    for(i=0,j=randk;i<nt-randk;i++,j++){
        rgt_child[i] = idx_p2[j];
    }

    for(i=0;i;i++){
        for(j=0;j;j++){
            if(implant[i] == lft_child[j]){
                Remove(lft_child,j);
            }
            if(implant[i] == rgt_child[j]){
                Remove(rgt_child,j);
            }
        }
    }

    //这里是不是可以用sizeof(implat)后用四则运算直接求出来的啊
    int pingjiesize = 0;
    for(i=0;findlen(lft_child[i]);i++){
        pingjiesize ++;
    }
    for(i=0;findlen(rgt_child[i]);i++){
        pingjiesize ++;
    }
    for(i=0;findlen(implant[i]);i++){
        pingjiesize ++;
    }

    int * child = (int *)malloc(sizeof(int)*pingjiesize);
    
    j=0;
    for(i=0;findlen(lft_child[i]);i++,j++){
        child[j] =lft_child[i][0];
    }
    for(i=0;findlen(rgt_child[i]);i++,j++){
        child[j] =rgt_child[i][0];
    }
    for(i=0;findlen(implant[i]);i++,j++){
        child[j] =implant[i][0];
    }

    return child; 
}


/********************************* Crossover END*************************************/ 
void shufflepop(int ** pop){
	int i;
	for(i=0;i<(ps/2);i++){
		int loc = rand()%(ps);
		int * temp = pop[i];
		pop[i] = pop[loc];
		pop[loc] = temp;
	}
}
/********************************* MAIN *************************************/ 
int main(){
	srand((unsigned)time(NULL));    //以时间做种
	int i,j,k;
	
	LoadInstance();
	printInformation();

	int * eachpopulationtime = (int *)malloc(sizeof(int)*ps);
	int ** pop = InitPopulation();
	
	displaypop(pop);	//测试用
	
	int max = 0;
	for(i=0;i<ps;i++){
		 eachpopulationtime[i] = ComputeStartTimes(pop[i]);
		 if(eachpopulationtime[i]>max)  max = eachpopulationtime[i];
	}

//	for(i=0;i<ps;i++) printf("%2d:%2d\n",i,eachpopulationtime[i]);
	printf("%d\n",max);


	 int it;	//iterator 迭代器
	 for(it=1;it<=mit;it++){
		shufflepop(pop);
		int hpop = ps/2;
	 	 for(i=0;i<(ps/2);i++){
	 		int pc = 60;		//交配概率,如果大于则交配 
	 		int p1 = rand() % 100;
	 		if(p1 > pc) {
	 			
	 			
	 			
	 			
	 			
	 			int pm = 60;	//突变概率,如果大于则突变 
			 }
		 }
	 }
	 
	 return 0;
}

/********************************* MAIN *************************************/ 

/*
def main():
""" pop是种群，种群中的每个个体的形式是，（makespan, 染色体）"""     
    pop = [(ComputeStartTimes(g, I)[-1], g) for g in InitPopulation(ps, I)]
    for it in xrange(1, mit+1):#""" mit是迭代次数"""
        # Random ordering of the population
        shuffle(pop)#"""把pop中各个个体的顺序打乱 """
        hpop = len(pop) / 2#""" hpop是种群的一半"""
        for i in xrange(hpop):#""" 遍历种群的前半部分份"""
            if random() < pc:#"""若[0,1]之间的随机数 < pc """
                # Create two new elements
                ch1 = Crossover(pop[i][1], pop[hpop + i][1], I)#""" 通过交叉生成下一代"""
                ch2 = Crossover(pop[hpop + i][1], pop[i][1], I)#""" 通过交叉生成下一代"""
                if random() < pm:#"""若[0,1]之间的随机数 < pm """
                    ch1 = Mutation(ch1)#""" 对ch1进行变异"""
                if random() < pm:#"""若[0,1]之间的随机数 < pm """
                    ch2 = Mutation(ch2)#"""对ch2进行变异"""
                pop.append((ComputeStartTimes(ch1, I)[-1], ch1))#""" 将进化后的染色体放回种群"""
                pop.append((ComputeStartTimes(ch2, I)[-1], ch2))
        # Sort individuals in increasing timespan order and
        # select only the best ones for the next iteration
        pop.sort()#""" 将pop中的染色体按makespan排序"""
        pop = pop[:ps]#""" 提取pop中的前ps个染色体"""
    return pop[0]#"""返回makespan最大的染色体 """
*/

void printInformation() {
	int i, j;
	printf("工件信息：(机器号，所需时间)\n");
	for (i = 0; i<n; i++) {
		printf("工件%2d 所需工序:", i);
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
				j++;
			}
		}
	}
	
	/*
	储存格式为:

	I[0][0][0] 为第一个工件的第一个工序的 
	 
	...要不要修改为I[1][1][0]是第一个工件的第一个工序所在的机器呢?感觉非常的混乱.如果修改的话似乎要加很多很多个等于号几乎是要重新写了. 
	*/
}





