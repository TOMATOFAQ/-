
struct GRAPH{//����ͼ�Ľṹ֮�ڵ�
    struct NODE * children;
};

struct NODE{//����ͼ�Ľṹ֮�ڵ�
    int * son;
};

typedef struct NODE node;
typedef struct GRAPH graph;

/*
sΪȾɫ����������У�IΪ������Ϣ;�������������Ƴ���Ҫ������������������,���ڴ˴�ָ��.����Ĳ���˵��:
���������ж������G,st.Ȼ��ȡ��ַ��������.��ComputDAG(s,I,&G,&st);����. 
*/
 
void ComputeDAG(int * s,int *** I,node * aimG,int ** aimst){

    int i,j,k;//ѭ���ĸ�������
    graph G;//"����һ����list���洢ͼ"

    G.children = (node*)malloc(sizeof(node)*(strlen(s)+1 +5);//G.children[strlen(s)]��Ϊ��ֹ�ڵ� 
	for(i=0;i<strlen(s)+1;i++){
		G.children[i].son = (node)malloc(sizeof(int)*(strlen(s)+5); // ��5������ϰ���Ϸ�ֹ����� 
		memset(G.children[i].son,0,sizeof(G.children[i].son)+5);
	}

    //��ʼ��һ������Ϊ��������ȫ0list
    int * T = (int *)malloc(sizeof(int)*n);
    memset(T,0,sizeof(T));

    //����洢ÿ��������һ�μ���ͼ�Ĺ���ţ���ʼ��Ϊ-1
    int * last_task_job = (int *)malloc(sizeof(int)*n);
    memset(last_task_job,-1,sizeof(last_task_job));

    int ** tasks_resource = (int **)malloc(sizeof(int*)*m);
    for(i=0;i<n;i++){
        tasks_resource[i] = (int *)malloc(sizeof(int)*n);
        memset(tasks_resource[i],-1,sizeof(tasks_resource));
    }

    int * st = (int *)malloc(sizeof(int)*strlen(s));
    for(i=0;i<strlen(s);i++){
        int j = s[i];//��ȡ���Ϊi�Ľڵ��Ӧ�Ĺ�����
        int t = T[j];//��������,��¼���i�Ľڵ���j�����ڵĵڼ�������

        st[i] = t;//��t����st

        r = I[j][t][0];//��j�����ĵ�t�������Ӧ����ż�еĵ�һ�����֣����ù���Ļ����Ŵ������r

        //�ж�j����Թ���ŵļ�����t��ֵ�Ƿ����j�Ĺ��������������j�����һ������ָ����ֹ�ڵ�
        
        
        
        if(t+1 == strlen(I[j])) {
        	int tempnum;
        	for(tempnum = 0;G.children[strlen(s)].son[tempnum]!=0;tempnum++);
        	
        	G.children[strlen(s)].son[tempnum] = i;
		}
		 
        //������j�ĵ�һ�������򽫸ù����ǰһ������ָ�����ǰ����Ľڵ�i

        if(t>0) {
        	int tempnum;
        	for(tempnum = 0;G.children[strlen(s)].son[tempnum]!=0;tempnum++);
        	G.children[i].son[tempnum] = last_task_job[j];
		}

        //���֮ǰ����Ľڵ�Ҳ��ʹ�ú�j�ĵ�t��������ͬ�Ļ���r,����Щ�ڵ���ָ��ǰ����ڵ�i
        int j2;
        for(j2=0;j2<n;j2++){
            if(j2!=j&&tasks_resouce[r][j2] > -1){
                int tempnum;
        		for(tempnum = 0;G.children[strlen(s)].son[tempnum]!=0;tempnum++);
        		G.children[i].son[tempnum] = tasks_resource[r][j2];
            }
        }

        T[j] = T[j] + 1;	//j�����Ĺ�����ۼ�����1
        last_task_job[j] = i;	//��j��������һ������Ľڵ�����Ϊ��ǰ�ڵ�ı��i
        tasks_resouce[r][j] = i;	//��j����������ʹ�û���r�Ĺ����Ӧ�Ľڵ�����Ϊi
    }
    
    *aimG = G;
    *aimst = st;
}

int main(){
    
    node G;
    int * st;
    ComputDAG(s,I,G,st);
    
}

