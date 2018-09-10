#include "jobshop.h"

int ComputeStartTimes(int * s,int * Ct){//�����Ⱦɫ��������Ҫ��ʱ��

	//computeDAG����
    int i;
	graph G;

    G.nodes = (node*)malloc(sizeof(node)*(length+1));//��length+1Ϊ��ֹ�ڵ�,G.nodes[length]��Ϊ��ֹ�ڵ� 
	for(i=0;i<length+1;i++){
		G.nodes[i].fathers = (int *)malloc(sizeof(int)*(length)); 
		memset(G.nodes[i].fathers,-1,sizeof(int)*length);
	}

    // ��ʼ��һ������Ϊ��������ȫ0list
    int * T = (int *)malloc(sizeof(int)*n);
    memset(T,0,sizeof(int)*n);

    // ����洢ÿ��������һ�μ���ͼ�Ĺ���ţ���ʼ��Ϊ-1
    int * last_task_job = (int *)malloc(sizeof(int)*n);
    memset(last_task_job,-1,sizeof(int)*n);

    int ** tasks_resource = (int **)malloc(sizeof(int*)*m);//tasks_resource �ĵ�һ������С�ڻ�����,�ڶ�������С�ڹ�����
    for(i=0;i<m;i++){
        tasks_resource[i] = (int *)malloc(sizeof(int)*n);
        memset(tasks_resource[i],-1,sizeof(int)*n);
    }

    int * st = (int *)malloc(sizeof(int)*length);
    memset(st,0,sizeof(int)*length);
    
    for(i=0;i<length;i++){
        int j = s[i];//��ȡ���Ϊi�Ľڵ��Ӧ�Ĺ�����
        int t = T[j];//��������,��¼���i�Ľڵ���j�����ڵĵڼ�������

        st[i] = t;//��t����st

		
        int r = I[j][t][0];//��j�����ĵ�t�������Ӧ����ż�еĵ�һ�����֣����ù���Ļ����Ŵ������r

        //�ж�j����Թ���ŵļ�����t��ֵ�Ƿ����j�Ĺ��������������j�����һ������ָ����ֹ�ڵ�
		int gongxushu_j = m;//�����������������,���л�����������m̨������,�ʼ���ÿ̨������ֻ��m̨����
   
        if(t+1 == gongxushu_j) {//0,1,2,3,4,��t=4ʱ,�������һ��������
        	int tempnum;
        	for(tempnum = 0;G.nodes[length].fathers[tempnum]!=-1;tempnum++);//����ֹ�ڵ�ĵ�һ���յ�λ��
        	
        	G.nodes[length].fathers[tempnum] = i;//����ָ����ֹ�ڵ�Ľڵ���Ⱦɫ���ϵ�λ��
		}
		 
        //������j�ĵ�һ�������򽫸ù����ǰһ������ָ�����ǰ����Ľڵ�i
        if(t>0) {
        	int tempnum;
        	for(tempnum = 0;G.nodes[i].fathers[tempnum]>-1;tempnum++);//�ҵ�
        	G.nodes[i].fathers[tempnum] = last_task_job[j];
		}

        //���֮ǰ����Ľڵ�Ҳ��ʹ�ú�j�ĵ�t��������ͬ�Ļ���r,����Щ�ڵ���ָ��ǰ����ڵ�i
        
        int j2;
        for(j2=0;j2<n;j2++){
            if(j2!=j && (tasks_resource[ r ][ j2 ] > -1)){
			//tasks_resource �ĵ�һ������С�ڻ�����,�ڶ�������С�ڹ�����
                int tempnum;
        		for(tempnum = 0;G.nodes[i].fathers[tempnum]!=-1;tempnum++);
        		G.nodes[i].fathers[tempnum] = tasks_resource[r][j2];
            }
        }

        T[j] = T[j] + 1;	//j�����Ĺ�����ۼ�����1
        last_task_job[j] = i;	//��j��������һ������Ľڵ�����Ϊ��ǰ�ڵ�ı��i
        tasks_resource[r][j] = i;	//��j����������ʹ�û���r�Ĺ����Ӧ�Ľڵ�����Ϊi
    }


	//�ڴ��ͷ�
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

	//�ڴ��ͷ�
	for(i=0;i<=length;i++) free(G.nodes[i].fathers);
	free(G.nodes);
	free(st);
	free(C);
	
    return Ct[length];
}