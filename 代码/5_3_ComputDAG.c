
struct GRAPH{//有向图的结构之节点
    struct NODE * children;
};

struct NODE{//有向图的结构之节点
    int * son;
};

typedef struct NODE node;
typedef struct GRAPH graph;

/*
s为染色体的乱序排列，I为输入信息;由于这个函数设计成需要传两个变量回主函数,故在此传指针.具体的操作说明:
在主函数中定义变量G,st.然后取地址传入其中.即ComputDAG(s,I,&G,&st);即可. 
*/
 
void ComputeDAG(int * s,int *** I,node * aimG,int ** aimst){

    int i,j,k;//循环的辅助变量
    graph G;//"构造一个空list来存储图"

    G.children = (node*)malloc(sizeof(node)*(strlen(s)+1 +5);//G.children[strlen(s)]即为终止节点 
	for(i=0;i<strlen(s)+1;i++){
		G.children[i].son = (node)malloc(sizeof(int)*(strlen(s)+5); // 加5仅仅是习惯上防止溢出尔 
		memset(G.children[i].son,0,sizeof(G.children[i].son)+5);
	}

    //初始化一个长度为工件数的全0list
    int * T = (int *)malloc(sizeof(int)*n);
    memset(T,0,sizeof(T));

    //构造存储每个工件上一次加入图的工序号，初始化为-1
    int * last_task_job = (int *)malloc(sizeof(int)*n);
    memset(last_task_job,-1,sizeof(last_task_job));

    int ** tasks_resource = (int **)malloc(sizeof(int*)*m);
    for(i=0;i<n;i++){
        tasks_resource[i] = (int *)malloc(sizeof(int)*n);
        memset(tasks_resource[i],-1,sizeof(tasks_resource));
    }

    int * st = (int *)malloc(sizeof(int)*strlen(s));
    for(i=0;i<strlen(s);i++){
        int j = s[i];//提取编号为i的节点对应的工件号
        int t = T[j];//辅助变量,记录编号i的节点是j工件内的第几道工序

        st[i] = t;//将t存入st

        r = I[j][t][0];//将j工件的第t道工序对应有序偶中的第一个数字，即该工序的机器号存入变量r

        //判断j的相对工序号的计数器t的值是否等于j的工序数，若相等则将j的最后一道工序指向终止节点
        
        
        
        if(t+1 == strlen(I[j])) {
        	int tempnum;
        	for(tempnum = 0;G.children[strlen(s)].son[tempnum]!=0;tempnum++);
        	
        	G.children[strlen(s)].son[tempnum] = i;
		}
		 
        //若不是j的第一道工序则将该工序的前一道工序指向代表当前工序的节点i

        if(t>0) {
        	int tempnum;
        	for(tempnum = 0;G.children[strlen(s)].son[tempnum]!=0;tempnum++);
        	G.children[i].son[tempnum] = last_task_job[j];
		}

        //如果之前加入的节点也在使用和j的第t道工序相同的机器r,则将这些节点编号指向当前处理节点i
        int j2;
        for(j2=0;j2<n;j2++){
            if(j2!=j&&tasks_resouce[r][j2] > -1){
                int tempnum;
        		for(tempnum = 0;G.children[strlen(s)].son[tempnum]!=0;tempnum++);
        		G.children[i].son[tempnum] = tasks_resource[r][j2];
            }
        }

        T[j] = T[j] + 1;	//j工件的工序号累加器加1
        last_task_job[j] = i;	//将j工件的上一道工序的节点编号置为当前节点的编号i
        tasks_resouce[r][j] = i;	//将j工件中正在使用机器r的工序对应的节点编号置为i
    }
    
    *aimG = G;
    *aimst = st;
}

int main(){
    
    node G;
    int * st;
    ComputDAG(s,I,G,st);
    
}

