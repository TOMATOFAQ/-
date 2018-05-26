#include "jobshop.h"

void printfgraph(){/***********������**********/
	int i,j,k;
	for(i=0;i<length+1;i++){
		printf("Node %2d:",i);
		for(j=0;j<length;j++){
			printf("%2d ",G.children[i].son[j]);
		}
		printf("\n");
	}
}

void ComputeDAG(int * s){/***********��������ͼ**********/
	int i,j,k;

    G.children = (node*)malloc(sizeof(node)*(length+1));//��length+1Ϊ��ֹ�ڵ�,G.children[length]��Ϊ��ֹ�ڵ� 
	for(i=0;i<length+1;i++){
		G.children[i].son = (int *)malloc(sizeof(int)*(length)); 
		memset(G.children[i].son,0,sizeof(int)*length);
	}

//    ��ʼ��һ������Ϊ��������ȫ0list
    int * T = (int *)malloc(sizeof(int)*n);
    memset(T,0,sizeof(int)*n);

//    ����洢ÿ��������һ�μ���ͼ�Ĺ���ţ���ʼ��Ϊ-1
    int * last_task_job = (int *)malloc(sizeof(int)*n);
    memset(last_task_job,-1,sizeof(int)*n);

    int ** tasks_resource = (int **)malloc(sizeof(int*)*m);//tasks_resource �ĵ�һ������С�ڻ�����,�ڶ�������С�ڹ�����
    for(i=0;i<m;i++){
        tasks_resource[i] = (int *)malloc(sizeof(int)*n);
        memset(tasks_resource[i],-1,sizeof(int)*n);
    }

    st = (int *)malloc(sizeof(int)*length);
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
        	for(tempnum = 0;G.children[length].son[tempnum]!=0;tempnum++);//����ֹ�ڵ�ĵ�һ���յ�λ��
        	
        	G.children[length].son[tempnum] = i;//����ָ����ֹ�ڵ�Ľڵ���Ⱦɫ���ϵ�λ��
		}
		 
        //������j�ĵ�һ�������򽫸ù����ǰһ������ָ�����ǰ����Ľڵ�i
        if(t>0) {
        	int tempnum;
        	for(tempnum = 0;G.children[i].son[tempnum];tempnum++);//�ҵ�
        	G.children[i].son[tempnum] = last_task_job[j];
		}

        //���֮ǰ����Ľڵ�Ҳ��ʹ�ú�j�ĵ�t��������ͬ�Ļ���r,����Щ�ڵ���ָ��ǰ����ڵ�i
        
        int j2;
        for(j2=0;j2<n;j2++){
            if(j2!=j && (tasks_resource[ r ][ j2 ] > -1)){
			//tasks_resource �ĵ�һ������С�ڻ�����,�ڶ�������С�ڹ�����
                int tempnum;
        		for(tempnum = 0;G.children[i].son[tempnum]!=0;tempnum++);
        		G.children[i].son[tempnum] = tasks_resource[r][j2];
            }
        }

        T[j] = T[j] + 1;	//j�����Ĺ�����ۼ�����1
        last_task_job[j] = i;	//��j��������һ������Ľڵ�����Ϊ��ǰ�ڵ�ı��i
        tasks_resource[r][j] = i;	//��j����������ʹ�û���r�Ĺ����Ӧ�Ľڵ�����Ϊi
    }
}

int ComputeStartTimes(int * s){//�����Ⱦɫ��������Ҫ��ʱ��
    ComputeDAG(s);//����һ��ͼ,����һ��st,�õ��˽ڵ�����Ӧ�ĵ� n ������
    
    int nodenum = length+1;
    int * C = (int *)malloc(sizeof(int)*nodenum);
    memset(C,0,sizeof(C));

    int i;
    for(i=0;i<nodenum;i++){
    	
    	int length_of_G_children_son_i;
    	for(length_of_G_children_son_i=0;length_of_G_children_son_i<length && G.children[i].son[length_of_G_children_son_i]!=0;length_of_G_children_son_i++);
    	
        if(length_of_G_children_son_i == 0 ) C[i] = 0;
        else{
            int max = 0;
			int k;
            for(k=0;k<length_of_G_children_son_i;k++){
				int l = G.children[i].son[k];
                if((C[l] + I[s[l]][st[l]][1]) > max) max = C[l] + I[s[l]][st[l]][1];				
            }
            C[i] = max;
        }
    }
	Ct = C;

	for(i=0;i<=length;i++) free(G.children[i].son);
	free(G.children);
    return C[length];
}
