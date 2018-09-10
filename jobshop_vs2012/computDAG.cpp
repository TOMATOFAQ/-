#include "jobshop.h"

int ComputeStartTimes(int * s,int * Ct){//计算该染色体下所需要的时间

	//computeDAG部分
    int i;
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