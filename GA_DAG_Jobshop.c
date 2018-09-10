#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define ps 20//��Ⱥ��С
#define mit 100 //��������
#define pc 40 //�������
#define pm 90 //ͻ�����
#define totalnum 100 //δ����ʱ��֮ǰ���Ŵ�����.����ʱ��֮��Ͳ����ƴ�����,ֱ���ܵ��涨ʱ��Ϊֹ
#define ThreadTime 1//�����Ŵ����е�ʱ��
#define programmeTime 2 //����������ʱ��

typedef struct GRAPH{//����ͼ�Ľṹ֮�ڵ� 
    struct NODE * nodes;
}graph;

typedef struct NODE{//����ͼ�Ľṹ֮�ڵ�ָ֮�򱾽ڵ����һ���ڵ��� 
    int * fathers;
}node;

int *** I;	//¼����Ϣ 
int n, m;	//������,������ 
int length = 0; 	//Ⱦɫ�峤�� 

//���ڶ��̵߳��ĸ�ȫ�ֱ���
int final_result = ((unsigned)1<<31)-1; //ʵʱ���´�����ʱ��
int * final_s;
int * final_Ct;
HANDLE hMutex;//���ڷ�ֹ���߳�֮����໥����

//������������޵ĵ�ȫ�ֱ���
int *** result;
int ** overhaul_list;
int TIME;//����ͬ������������������е�ʱ��
//ʩ����

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
/********************************* 7-Observer-ɾȥ��Ӱ�캯������ *************************************/ 
void printInformation();
void printfpop(int ** pop,int * eachtime);
void printfgraph();
void printfIndex(int ** idx);
void printfparent(int * p);
/********************************* 0-MAIN *************************************/ 
int once(){//һ���Ŵ��㷨

	//2-��ʼ�����ݽṹ
	int * eachtime = (int *)malloc(sizeof(int)*ps);
	memset(eachtime,0,sizeof(int)*ps);
	int ** pop = InitPopulation();

	int * Ct = (int *)malloc(sizeof(int)*(length+1));

	int i,j;
	for(i=0;i<ps;i++){
		 eachtime[i] = ComputeStartTimes(pop[i],Ct);
		 eachtime[i] = ComputeStartTimes(pop[i],Ct);
	}

	//3-����
	iterator(pop,eachtime,Ct);
	
	//4-������
	// FormatSovelution_and_display(pop[0],Ct,1); //��ʾÿһ�ε����ľ�����
	ComputeStartTimes(pop[0],Ct);
	
	//�޸�ȫ�ֱ���
	WaitForSingleObject(hMutex,INFINITE);
	if(Ct[length] < final_result){
		final_result = Ct[length];
		for(i=0;i<length;i++) final_s[i] = pop[0][i];
		for(i=0;i<=length;i++) final_Ct[i] = Ct[i];
	}
	ReleaseMutex(hMutex);

	printf("End Time:%d\n",final_result);

	//�ڴ��ͷż��������Ž�
	int result = Ct[length];
	free(eachtime);
	free(Ct);
	for(i=0;i<ps;i++) free(pop[i]);
	free(pop);

	return result;
}

DWORD WINAPI original_main(LPVOID pPararneter){//ԭ�ȵ�һ��main����,���ڸ�Ϊһ�߳� 
	//��ʼ��ʱ�������Ϣ
	srand((unsigned)time(NULL));

	clock_t start,finish;
	double duration;
	start = clock();

	//���ж���Ŵ��㷨
	int min = ((unsigned)1<<31)-1; 
	int i,j;	
	for(i=0;;i++) {//ֱ���ܵ�ʱ�����
		int result = once();
		if(result<min) min = result;
		// printf("��С:%-4d,��%d���Ŵ��㷨:%4d,time:%d\n",min,i,result,(int)((clock()-start)/CLOCKS_PER_SEC));
		if((double)((clock()-start)/CLOCKS_PER_SEC) > ThreadTime) break;
	}

	//������
	finish = clock();
	duration = (double)(finish-start)/CLOCKS_PER_SEC;
	// printf("%f seconds.\n",duration);
	
	if(min<final_result) final_result = min;
	
	return 0;
}

int main(){
	//Ĭ���ض���������input.txt�������Ҫ�ֶ�������ע�͸��л����޸������ļ�
	// freopen("output.txt","w",stdout);
	int i,j,k;
	LoadInstance();
	
	clock_t start,finish;
	double duration;
	start = clock();
	
	//��ʼ�������ĸ��̴߳𰸵ĵ�ȫ�ֱ���
	length = n*m;
	final_s = (int *)malloc(sizeof(int)*length);
	final_Ct = (int *)malloc(sizeof(int)*(length+1));

	HANDLE hThread1,hThread2,hThread3,hThread4;
	hMutex=CreateMutex(NULL,FALSE,NULL);
	hThread1=CreateThread(NULL,0,original_main,NULL ,0,NULL);
	Sleep(1000);//�ı���Ϊ��������ӵ�ʱ�� 
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
	
	//�������ǰ���
	freopen("output.txt", "w", stdout);//�ض���
	FormatSovelution_and_display(final_s,final_Ct,1);
	printf("Time Used:%.3lf\n",duration);
	printf("End Time:%d\n",final_result);
	freopen("CON", "w", stdout);//�ص�����̨
	
	
	//�ڿ���̨������
	FormatSovelution_and_display(final_s,final_Ct,1);
	printf("Time Used:%.3lf\n",duration);
	printf("End Time:%d\n",final_result);
	 
	
	//���޹���
	printf("The result had been outputed into output.txt.Please enter num 1 to overhaul.\n");//�������յ�ʱ������Ҫ��ͣ 
	overhaul_list = (int **)malloc(sizeof(int*)*(n*m));//����һ��n*m����������
	for(i = 0;i<n*m;i++){
		overhaul_list[i] = (int*)malloc(sizeof(int)*3);//ÿ�����������������Ϣ���ü�������Ļ�������ʼʱ�䣬����ʱ��
		memset(overhaul_list[i],-1,sizeof(int)*3);//-1��ʾ���������Ҫ��
	}
	
	HANDLE OVERHAUL_INPUT,OVERHAUL_OUTPUT;
	OVERHAUL_INPUT = CreateThread(NULL,0,overhaul_input,NULL,0,NULL);
	OVERHAUL_OUTPUT = CreateThread(NULL,0,overhaul_output,NULL,0,NULL);

	while(TIME != -1)Sleep(1000);//�����Ƿ����

	//������޺���
	freopen("output2.txt", "w", stdout);//�ض���
		for(i=0;i<m;i++){//��į����
			printf("M%d",i);
			for(j=0;j<n;j++){//��j������
				printf(" (%d,%d-%d,%d)",result[i][j][2],result[i][j][0],result[i][j][1],result[i][j][3]);
			}
			printf("\n");
		}		
	printf("Time Used:%.3lf\n",duration);
	printf("End Time:%d\n",final_result);
	freopen("CON", "w", stdout);//�ص�����̨
	printf("The result had been outputed into output2.txt.\n");
	
	//�ڿ���̨������
		for(i=0;i<m;i++){//��į����
			printf("M%d",i);
			for(j=0;j<n;j++){//��j������

				// ���Դ��룺
				 if(j>0){//��ʱ���ԣ��������޳�ͻ
				 	if(result[i][j][2]<result[i][j-1][3]) printf("����ERROR����");
				 }
				//���ù�������һ������֮���Ƿ���ڼ���������ǣ����������Ϣ
				insert_overhaul(i,j);
				printf(" (%d,%d-%d,%d)",result[i][j][2],result[i][j][0],result[i][j][1],result[i][j][3]);
			}
			printf("\n");
		}		
	printf("Time Used:%.3lf\n",duration);
	printf("End Time:%d\n",final_result);
	 
	
	
	//�ڴ��ͷ�
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
	/*�����I��˵����
	I[i][j][0] ��ʾ��i��������j����������Ҫ�Ļ���,��ֵΪ0��˵���������������
	I[i][j][1] ��ʾ��i��������j����������Ҫ��ʱ��,��ֵΪ0��˵���������������
	i,j����ͷ��ʼ���� 
	*/

	freopen("input.txt", "r", stdin);//�ض���

	int i, j, k;//ѭ����������
	scanf("%d %d", &n, &m);

	I = (int ***)malloc(sizeof(int**)*n);
	for (i = 0; i<n; i++) {
		I[i] = (int **)malloc(sizeof(int*)*m);
		for (j = 0; j<m; j++) {
			I[i][j] = (int *)malloc(sizeof(int) * 2);
			memset(I[i][j], 0, sizeof(I[i][j]));
		}
	}

	for (i = 0; i<n; i++) {//��i������
		for(j=0;j<m;j++){
			scanf("%d",&I[i][j][0]);
			scanf("%d",&I[i][j][1]);
			}	
	}

	freopen("CON", "r", stdin);//�ص�����̨
	//�ڴ��ͷ�:I��������free
}
/********************************* 1-INPUT-END *************************************/
/********************************* 2-InitPopulation *************************************/ 

void shuffle(int * gene) {//ϴ�ƺ���,���һ���˳��
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
	//ȷ��һ��Ⱦɫ��ĳ���,��ȷ�����������,����ȷ�����������//�������в����������涼��n*m,���������û�������� 
	length	= n*m;
	
		 
	int * gene = (int *)malloc(sizeof(int)*length);

	 i = 0;
	for (j = 0;j<n; j++) {//j��ʾ�Թ����ı���,i��ʾ�����ϵ�i��λ��
		int t;//t��ʾ��ָ���Ĺ����Ĺ���ı���
		for (t = 0;t<m; t++, i++) {
			gene[i] = j;
		}
	}

	int ** population = (int **)malloc(sizeof(int*)*ps);
	for (i = 0; i<ps; i++) {
		shuffle(gene);
		population[i] = (int *)malloc(sizeof(int)*length);//��ps������ƴ����һ��Ⱦɫ����
		for (j = 0; j<length; j++) {
			population[i][j] = gene[j];
		}
	}

	free(gene);

	return population;
}


/********************************* 2-InitPopulation-END *************************************/ 
/********************************* 3-ComputDAG *************************************/ 
int ComputeStartTimes(int * s,int * Ct){//�����Ⱦɫ��������Ҫ��ʱ��

	//computeDAG����
    int i,j,k;
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
/********************************* 3-ComputDAG-END *************************************/ 



/********************************* 4-Crossover *************************************/ 
int ** Index(int * p){//����һ����Ԫ�Ե�����,ÿ����Ԫ���е�һ����ʾ���ǹ���,�ڶ�����ʾ����˳��.P.S.��ôд������Ϊ�˿�������"�߼����� 
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

	//�ڴ��ͷ�
	//�˺����е�s��Crossover��free
	free(ct);

    return s;
}

int * Crossover(int * p1,int *p2){//p1,p2�������ȴ������Ⱦɫ��,pΪparent����д
    int i,j,k;//����ѭ���ĸ�������
    int ** idx_p1 = Index(p1);
    int ** idx_p2 = Index(p2);

    int randi = (rand()%(length-1))+1;//����[1,nt)�������,��ʾ��Ƕ��ı���Ƭ��֮�� 
    int randj = rand()%(length-randi);//����[0,nt-randi-1)�������, ��ʾ����Ƭ����parent1�е���ʼλ�� 
    int randk = rand()%length;//����[0,nt)�������,��ʾ����Ƭ����parent2�е���ʼλ�� 

    //implant ��ΪǶ�� .���ɽ�����Ƭ��. 
    int ** implant =  (int **)malloc(sizeof(int*)*(randi));	 
    for(i=0;i<randi;i++){
    	implant[i] = (int *)malloc(sizeof(int)*2);
		implant[i][0] = idx_p1[randj+i][0];
		implant[i][1] = idx_p1[randj+i][1];
	}
	
	//ͬʱ���parent2����Ƕ��Ƭ���ظ�������,������ɾ�� 
	int * aftermutation = (int *)malloc(sizeof(int)*length);
	for(i=0;i<length;i++){
			for(j=0;j<randi;j++){
				if(idx_p2[i][0] == implant[j][0] && idx_p2[i][1] == implant[j][1]) idx_p2[i][0] = -1;//���ϱ�� 
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
	
	 //�ڴ��ͷ�
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

void sortpop(int * eachtime,int ** pop){//�������� 
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


//��ֹ���װ�
int iterator(int ** pop,int * eachtime,int * Ct){
	 int i,j,k; //ѭ���������� 
	 int it;	//iterator ������

	//Ϊ������һ�ε�����,Ⱦɫ��ֻ��ͬ����Ⱦɫ���ӽ�,��Ҫ���³����ɵ������Ӵ�����������.�ȵ������ӽ����֮��,�ټ�����̭�����Ӵ�
	int ** temp = (int **)malloc(sizeof(int *)*ps);//��Ⱦɫ��˳��
	for(i=0;i<ps;i++){
		temp[i] = (int *)malloc(sizeof(int)*length);
		memset(temp[i],-1,sizeof(int)*length);//δ���������Ӵ��Ĳ�������Ϊ��һ,�������
	}
	int * temptime = (int *)malloc(sizeof(int)*ps);//��Ⱦɫ���Ӧ��ʱ��
	memset(temptime,-1,sizeof(int)*ps);


	for(it=1;it<=mit;it++){
		int * ch1;
		int * ch2;
		int time1;
		int time2;

		int son = 0;//���ڼ�¼�����,ָ��temp�������С�Ŀ�λ.��¼���ɵ������Ӵ�,���ڲ����temp�еĿ�λ��

	 	for(i=0;i<(ps/2);i++){//���н��滥��
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
	 			
				//�洢
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
		 
		//�Ӵ��滻����,˳�����
		for(i=0;i<son;i++){
			for(j=0;j<ps;j++){
				if(temptime[i] < eachtime[j]){
					eachtime[j] = temptime[i];
					for(k=0;k<length;k++){
						pop[j][k] = temp[i][k];
					}
					break;	//ֻҪ�滻��һ�ξ��˳�,��ֹ�ظ�
				}
			}
			memset(temp[i],-1,sizeof(int)*length);
			temptime[i] = -1;
		}
		
		shufflepop(eachtime,pop);
		///***********������*********
		//  printf("������%d�εĴ�:%d\n",it,eachtime[0]); //ɾȥ������ 
		// printf("%d  ",eachtime[0]); 
		// printfpop(pop,eachtime);//ɾȥ���� 
	 }

	//�ڴ��ͷ�
	free(temptime);
	for(i=0;i<ps;i++) free(temp[i]);
	free(temp);

	//���򲢷��ش�(����Ҫ)
	sortpop(eachtime,pop);
	return eachtime[0];
}

/********************************* 5-iterator-END *************************************/ 
/********************************* 6-Output *************************************/ 
void FormatSovelution_and_display(int * s,int * Ct,int choice){//choiceΪ0:���������.choiceΪ1,�������

	ComputeStartTimes(s,Ct);//����һ��,ˢ��Ct.�������ʧ��,��Ӧ����ȫ�ֱ�����.�������ǵ�һ�γ���ֻ��һ��,���Բ�����. //ԭ��δ֪��bug

    int i,j,k;
    int * T = (int *)malloc(sizeof(int)*n);
    memset(T,0,sizeof(int)*n);
	
	//S�д�ŵ���ÿ������ʼ�ӹ���ʱ�䣬������ʽΪ��[[a,b,c],[d,e,f],[g,h,i]],ÿ����list����һ����������Ϣ����list�е���ĸ���������������ÿ������ʼ�ӹ���ʱ�䡣
    int ** S = (int **)malloc(sizeof(int *)*n);
    for(i=0;i<n;i++){
        S[i] = (int *)malloc(sizeof(int)*m);
        memset(S[i],0,sizeof(int)*m);
    }

    for(i=0;i<length;i++){
        int j = s[i];//��ù�����
        int t = T[j];//������ǵ�j�������ĵ�t������
        S[j][t] = Ct[i]; //��j�������ĵ�t������Ŀ�ʼʱ��
        T[j] = T[j] + 1;
    }
	
	result = (int ***)malloc(sizeof(int**)*m);
	for(i=0;i<m;i++){//��į����
		result[i] = (int **)malloc(sizeof(int*)*n);
		for(j=0;j<n;j++){//��į�����ĵ�j������
			result[i][j] = (int *)malloc(sizeof(int)*4);//��į�����ĵ�j����������Ӧ���������,��������������ڹ���,�Լ���ʼʱ��,��ֹʱ��
			memset(result[i][j],-1,sizeof(int)*4);//-1��ʾû���ʹ�
		}
	}
	//result�����������,����ÿһ̨����.��Ҫ֪��ռ�õ�ʱ��.��ռ�õĻ���.���������ֻ�ڼӹ�ʱ����ӹ�һ�εĻ�,��ô�Ͳ���Ҫ֪���ǵڼ�����,��������.//����,����˳����Ϻ���.


	//����
	for(i=0;i<n;i++){ //��i������
		for(j=0;j<m;j++){ //��i�������ĵ�j������
			// S[i][j];//��i�������ĵ�j����������ʼ��ʱ��
			for(k=0;k<n;k++){//�ҵ���k����λ
				if(result[ I[i][j][0] ][k][0]==-1) break;
			}
			result[ I[i][j][0] ][k][0] = i;
			result[ I[i][j][0] ][k][1] = j;
			result[ I[i][j][0] ][k][2] = S[i][j];
			result[ I[i][j][0] ][k][3] = S[i][j] + I[i][j][1];
		}
	}

	//����.��ÿһ������������һ������
	for(i=0;i<m;i++){//����ÿ������Ŀ�ʼʱ��
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
		for(i=0;i<m;i++){//��į����
			printf("M%d",i);
			for(j=0;j<n;j++)//��j������
				printf(" (%d,%d-%d,%d)",result[i][j][2],result[i][j][0],result[i][j][1],result[i][j][3]);
			printf("\n");
		}		
	}

	//�ڴ��ͷ�
	free(T);
	
    for(i=0;i<n;i++) free(S[i]);
    free(S);
} 
/********************************* 6-Output-END *************************************/ 
/********************************* 7-overhaul *************************************/ 

int checkstate(int aimm,int time){//���ػ�����timeʱ�̵�״̬.0��ʾ���У�1��ʾ������2��ʾ����
	int i,j,k;
	//����Ƿ��ڼ���״̬������overhaul_list
	for(i=0;i<m*n;i++){
		if(overhaul_list[i][0] == -1) break;
		if(overhaul_list[i][0] == aimm){
			if(time>=overhaul_list[i][1] && time<=overhaul_list[i][2]) return 2;
		}		
	}
	
	//����Ƿ��ڹ���״̬
	for(j=0;j<n;j++){
		if(time >= result[aimm][j][2] && time <= result[aimm][j][3]) return 1;
	}
	
	return 0;
}

void putforward(int aimi,int aimj){
	int i,j;
	//���ǰ�����޿���ʱ��
	if((aimj!=0 && result[aimi][aimj][2] > result[aimi][aimj-1][3] )|| aimj==0){

		int name = result[aimi][aimj][0];
		int order = result[aimi][aimj][1];

		int duration;
		
		//�������� 
		if(aimj==0){//�����������ĵ�0�����򣬵����Ǹù����ĵ�0������ 
			if(result[aimi][aimj][1]!=0) return;
			
			duration = result[aimi][aimj][2];
			result[aimi][aimj][2] -= duration;
			result[aimi][aimj][3] -= duration;
			return;
		}

		if(order==0){//�����������ĵ�0������//��ΪʲôҪд��Σ� 
			duration = result[aimi][aimj][2]-result[aimi][aimj-1][3];
			result[aimi][aimj][2] -= duration;
			result[aimi][aimj][3] -= duration;
			return;
		}
			

		//���ǰ������ʱ���Ƿ��Ǽ���ʱ��
		for(i=0;i<n*m;i++){//�˴����Ż��������岻���ˡ���������м���Ҫ��ֱ��������ǰ��
			if(overhaul_list[i][0] == aimi && overhaul_list[i][1] >= result[aimi][aimj-1][3] && overhaul_list[i][2] <= result[aimi][aimj][2]) return;
		}

		//��Ȼ�м��û�м���������ô������ù�������һ������Ľ���ʱ��
		for(i=0;i<m;i++){
			for(j=0;j<n;j++){
				if(result[i][j][0] == name && result[i][j][1] == (order-1)){//�����ҵ�������һ������
					//����û�����һ������Ľ���ʱ��͸ù�����һ������Ľ���ʱ������ֵ
					int max;
					if(result[i][j][3] > result[aimi][aimj-1][3]) max = result[i][j][3];
					else max = result[aimi][aimj-1][3];

					//����ǰ��ʱ��
					int duration = result[aimi][aimj][2] -  max;
					result[aimi][aimj][2] -= duration;
					result[aimi][aimj][3] -= duration;
					
					break;
				}
			}
		}
	}
}

DWORD WINAPI overhaul_output(LPVOID pPararneter){//������ʾ��ʱ��
	int i,j,k;

	clock_t start,finish;
	start = clock();

	int mutiple = 100;//�ƶ��ӹ�ʱ���


	while(  ((int)(clock()-start)/CLOCKS_PER_SEC)*mutiple  <final_result){
		TIME = ((int)(clock()-start)/CLOCKS_PER_SEC)*mutiple;

		for(i=0;i<m;i++){
			//�ҵ���ǰTIME����Ӧ��j
			for(j=0;j<n;j++){
				if( result[i][j][2] >= TIME ) break;
			} 
			
			if(j>0) j--;
			printf("%d %d ",TIME,i);
			
			int state = checkstate(i,TIME);

			if(state==2) printf("����\n");//�ڼ���״̬
			else{
				if(state==1) {
					printf("�ӹ� ");//�ӹ�״̬ 
					printf("%d-%d %d %d\n",result[i][j][0],result[i][j][1],TIME-result[i][j][2],result[i][j][3]-result[i][j][2]);
				}
				else printf("����\n");//����״̬
			}
		}

		Sleep(3*1000);//ͣ4�룬��Ȼ����Ϊ����������ô 
		printf("\n");
			
	}
	TIME = -1;//�����˳� 
}

DWORD WINAPI overhaul_input(LPVOID pPararneter){//�����������룬���ܻ��������ɼ������С����ˡ�ǰ�Ƶ�һϵ�����û�еĹ���
	int i,j,k,t=0;//����,t�ر�������¼���ǵ�t������Ҫ��
	int aimm,lasttime;

	while(TIME<final_result){
		scanf("%d",&aimm);
		scanf("%d",&lasttime);
		printf("����Ҫ��¼��ɹ���\n"); 
		overhaul_list[t][0] = aimm;

		//���Ŀ�������״̬�����ɼ���Ҫ��
		int state = checkstate(aimm,TIME);
		if(state==2){//����״̬׷�ӵ����޽���ʱ��
			for(i<0;i<t;i++){
				if(overhaul_list[i][0] == aimm && TIME >= overhaul_list[i][1] && TIME <= overhaul_list[i][2]){//����жϣ����ܴ���һ��������μ��޵�����
					overhaul_list[t][1] = overhaul_list[i][2];
					overhaul_list[t][2] = overhaul_list[i][2] + lasttime;
					break;
				}
			}
		}
		
		else if(state==1){//����״̬�����޹���׷�ӵ���������ʱ��
			//Ѱ�ҹ�������ʱ��
			for(j=0;j<n;j++){
				if(TIME >= result[aimm][j][2] && TIME <= result[aimm][j][3]){
					overhaul_list[t][1] = result[aimm][j][3];
					overhaul_list[t][2] = result[aimm][j][3] + lasttime;
					break;
				}
			}
		}
		else{//����״ֱ̬���ڵ�ǰ״̬������޹���
			overhaul_list[t][1] = TIME;
			overhaul_list[t][2] = TIME + lasttime; 	
		}

		//������������ϣ����ڽ���ȫ����ˡ����п�ʼʱ������ڼ���ʱ�俪ʼʱ��Ĺ���ͳһ��ͳһ���ϼ���ʱ��
		for(i=0;i<m;i++){
			for(j=0;j<n;j++){
				if(result[i][j][2] >= overhaul_list[t][1]){
					result[i][j][2] += lasttime;
					result[i][j][3] += lasttime;
				}
			}
		}
		
		//ȫ�������ϣ����ڽ���ǰ��
		for(k=0;k<n*m*2;k++){//ѭ����Σ���Ϊ����ֵ�ǰ������ǰ�ƵĹ����ܻ�δǰ�ƵĹ�������Լ������Ϊʲô��n*m*2�Σ���д�ġ�
			for(i=0;i<m;i++){
				for(j=0;j<n;j++){
					putforward(i,j);
				}
			}
		}

		//��̬ˢ��final_result
		for(i=0;i<m;i++) if(result[i][n-1][3] > final_result) final_result = result[i][n-1][3];

		
		
		//���ˣ�Ϊ���뵱ǰ����Ҫ���µ����Ž�
		t++;
	}
}
void insert_overhaul(int aimi,int aimj){
	//�Ȼ�ȡ�ù���Ŀ�ʼʱ������һ������Ľ���ʱ��,�ֱ��Ϊ����Ŀ�ʼ�����
	int start,end;
	if(aimj==0){
		start = 0;
		end = result[aimi][aimj][2];
	}
	else{
		start = result[aimi][aimj-1][3];
		end = result[aimi][aimj][2];
	}

	//�������������б�,�鿴�������ڸ�����ļ�����������������ڵ����м�������
	int i,j,k;
	for(i=0;i<n*m;i++){
		if(overhaul_list[i][0]==-1)break;
		if(overhaul_list[i][1]>=start && overhaul_list[i][2]<= end && overhaul_list[i][0] == aimi) printf(" (%d,����,%d)",overhaul_list[i][1],overhaul_list[i][2]);
	}

}
/********************************* 7-overhaul-end*************************************/ 
/********************************* 7-observer-(ɾȥ��Ӱ����빦��) *************************************/ 
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
	printf("%d������.%d̨����,������Ϣ��(�����ţ�����ʱ��)\n",n,m);
	for (i = 0; i<n; i++) {
		printf("����%2d ���蹤��:", i);
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


