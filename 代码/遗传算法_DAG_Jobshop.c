#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

//��Ҫ��Ϊ���ε���: 
#define ps 100 //��Ⱥ��С population size
#define mit 300 //�������� mount for iteratior
#define pc 0 //������� probability for crossover
#define pm 0 //ͻ����� probability for mutation


typedef struct GRAPH{//����ͼ�Ľṹ֮�ڵ� 
    struct NODE * nodes;
}graph;
typedef struct NODE{//����ͼ�Ľṹ֮�ڵ�ָ֮�򱾽ڵ����һ���ڵ��� 
    int * fathers;
}node;


int *** I;	//¼����Ϣ 
int n, m;	//������,������ 
int length = 0; //Ⱦɫ�峤�� 
graph G;
int * st;//һ�����ҵ�Ⱦɫ�� 
int * Ct;//���ÿ������ʼ�ӹ���ʱ��


/********************************* 1-Input *************************************/ 
void printInformation();
void LoadInstance();//Ĭ���ض���������input.txt�������Ҫ�ֶ�������ע�͸��л����޸������ļ�
/********************************* 2-InitPopulation ********************************/ 
void shuffle(int * gene);
int ** InitPopulation(); 
void displaypop(int ** pop,int * eachtime);
/********************************* 3-ComputDAG *************************************/ 
void ComputeDAG(int * s);
int ComputeStartTimes(int * s);
void printfgraph();
/********************************* 4-Crossover *************************************/  
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
void FormatSolution_and_display(int * s);



/********************************* 0-MAIN *************************************/ 
int main(){
	//0-��ʼ��ʱ�����
	srand((unsigned)time(NULL));

	//1-������Ϣ
	LoadInstance();
	printInformation();

	//2-��ʼ�����ݽṹ
	int * eachtime = (int *)malloc(sizeof(int)*ps);
	memset(eachtime,0,sizeof(int)*ps);
	int ** pop = InitPopulation();

	int i;
	for(i=0;i<ps;i++){
		 eachtime[i] = ComputeStartTimes(pop[i]);
		 eachtime[i] = ComputeStartTimes(pop[i]);
	}

	//3-����
	iterator(pop,eachtime);

	//4-������
	FormatSolution_and_display(pop[0]);
	return 0;
}
/********************************* 0-MAIN-END *************************************/ 







/********************************* 1-INPUT *************************************/ 
void printInformation() {
	int i, j;
	printf("%d������.%d̨����,������Ϣ��(�����ţ�����ʱ��)\n",n,m);
	for (i = 0; i<n; i++) {
		printf("����%2d ���蹤��:", i);
		for (j = 0; j<m; j++)    printf("(%2d,%2d),", I[i][j][0], I[i][j][1]);
		printf("\n");
	}
}

void LoadInstance() {
	
	/*�����I��˵����
	I[i][j][0] ��ʾ��i��������j����������Ҫ�Ļ���,��ֵΪ0��˵���������������
	I[i][j][1] ��ʾ��i��������j����������Ҫ��ʱ��,��ֵΪ0��˵���������������
	i,j����ͷ��ʼ���� 
	*/
	freopen("input.txt", "r", stdin);//Ĭ���ض���������input.txt�������Ҫ�ֶ�������ע�͸��л����޸������ļ�
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
	return population;
}

void displaypop(int ** pop,int * eachtime){/***********������**********/
	int i,j;
	for(i=0;i<ps;i++){ 
		printf("The %2d one use %2d:",i+1,eachtime[i]);
		for(j=0;j<length;j++){
			printf("%2d",pop[i][j]);
		}
		printf("\n");
	}
}
/********************************* 2-InitPopulation-END *************************************/ 



/********************************* 3-ComputDAG *************************************/ 
void printfgraph(){/***********������**********/
	int i,j,k;
	for(i=0;i<length+1;i++){
		printf("Node %2d:",i);
		for(j=0;j<length;j++){
			printf("%2d ",G.nodes[i].fathers[j]);
		}
		printf("\n");
	}
}

void ComputeDAG(int * s){/***********��������ͼ**********/
	int i,j,k;

    G.nodes = (node*)malloc(sizeof(node)*(length+1));//��length+1Ϊ��ֹ�ڵ�,G.nodes[length]��Ϊ��ֹ�ڵ� 
	for(i=0;i<length+1;i++){
		G.nodes[i].fathers = (int *)malloc(sizeof(int)*(length)); 
		memset(G.nodes[i].fathers,-1,sizeof(int)*length);
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
}

int ComputeStartTimes(int * s){//�����Ⱦɫ��������Ҫ��ʱ��
    ComputeDAG(s);//����һ��ͼ,����һ���õ��˽ڵ�����Ӧ�ĵ� n �����������st
    
    int nodenum = length+1;
    int * C = (int *)malloc(sizeof(int)*nodenum);
    memset(C,0,sizeof(C));

    int i;
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
	Ct = C;

	for(i=0;i<=length;i++) free(G.nodes[i].fathers);
	free(G.nodes);
    return C[length];
}
/********************************* 3-ComputDAG-END *************************************/ 







/********************************* 4-Crossover *************************************/ 
//����Ϊʲô�����д����ô�鷳,��Ϊ�˲��ƻ��ڵ�һ�����Ѿ�ȷ�����˹����ϵ 
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b)) 

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
    return s;
}

void printfIndex(int ** idx){/***********������**********/
	int i;
	printf("Parent:");
	for(i=0;i<length;i++) printf("%2d",idx[i][0]);
	printf("\n");
	
	printf("Index :");
	for(i=0;i<length;i++) printf("%2d",idx[i][1]);
	printf("\n");
} 

void printfparent(int * p){/***********������**********/
	int i;
	for(i=0;i<length;i++) printf("%d ",p[i]);
	printf("\n");
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
	int * aftermutation = (int *)malloc(sizeof(int)*length) ;
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

int iterator(int ** pop,int * eachtime){
	 int i,j; //ѭ���������� 
	 int it;	//iterator ������

	 for(it=1;it<=mit;it++){
//	 	printf("----------��%d�ε���----------\n",it);
		int * ch1;
		int * ch2;
		int time1;
		int time2;

	 	for(i=0;i<(ps/2);i++){
	 		if((rand() % 100) > pc) {

	 			ch1 = Crossover(pop[i],pop[ps/2+i]); 
	 			ch2 = Crossover(pop[ps/2+i],pop[i]); 
	 			
	 			if((rand() % 100) > pm){
					Mutation(ch1);
				 }

				if((rand() % 100) > pm) {
					Mutation(ch2);
				}
				
	 			time1 = ComputeStartTimes(ch1);
	 			time2 = ComputeStartTimes(ch2);
	 			
	 			sortpop(eachtime,pop);
	 			int replace1 = 1;
	 			int replace2 = 1;
	 			for(j=0;j<ps;j++){
	 				if(time1 < eachtime[j] && replace1){
	 					eachtime[j] = time1;
	 					pop[j] = ch1;
	 					replace1 = 0;
					 }
					 if(time2 < eachtime[j] && replace2){
	 					eachtime[j] = time2;
	 					pop[j] = ch2;
	 					replace2 = 0;
					 }
				 }
				 shufflepop(eachtime,pop);
			 }
		 }
		 
		sortpop(eachtime,pop);
		
		/***********������**********/
		// printf("������%d�εĴ�:%d\n",it,eachtime[0]); //ɾȥ������ 
//		printf("%d  ",eachtime[0]); 
//		displaypop(pop,eachtime);//ɾȥ���� 
		
	 }
	 sortpop(eachtime,pop);
	 return eachtime[0];
}
/********************************* 5-iterator-END *************************************/ 




/********************************* 6-Output *************************************/ 
void FormatSolution_and_display(int * s){
	
	ComputeStartTimes(s);//����һ��,ˢ��Ct.�������ʧ��,��Ӧ����ȫ�ֱ�����.�������ǵ�һ�γ���ֻ��һ��,���Բ�����.

    int i,j,k;
    int * T = (int *)malloc(sizeof(int)*n);
    memset(T,0,sizeof(int)*n);
	
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
	
    //S�д�ŵ���ÿ������ʼ�ӹ���ʱ�䣬������ʽΪ��[[a,b,c],[d,e,f],[g,h,i]],ÿ����list����һ����������Ϣ����list�е���ĸ���������������ÿ������ʼ�ӹ���ʱ�䡣 

	int *** result = (int ***)malloc(sizeof(int**)*m);//��į����
	for(i=0;i<m;i++){
		result[i] = (int **)malloc(sizeof(int*)*n);//��į�����ĵ�j������
		for(j=0;j<n;j++){
			result[i][j] = (int *)malloc(sizeof(int)*4);//��į�����ĵ�j����������Ӧ���������,��������������ڹ���,�Լ���ʼʱ��,��ֹʱ��
			memset(result[i][j],-1,sizeof(int)*4);//-1��ʾû���ʹ�
		}
	}
	//result�����������,����ÿһ̨����.��Ҫ֪��ռ�õ�ʱ��.��ռ�õĻ���.���������ֻ�ڼӹ�ʱ����ӹ�һ�εĻ�,��ô�Ͳ���Ҫ֪���ǵڼ�����,��������.//����,����˳����Ϻ���.

	printf("/***************���չʾ**************/\n");
	/***********������**********/
	// //չʾԭ����s
	// printf("s:");
	// for(i=0;i<length;i++){
	// 	printf("%d ",s[i]);
	// } printf("\n");

	// //չʾԭ����Ct
	// printf("Ct:");
	// for(i=0;i<=length;i++){
	// 	printf("%d ",Ct[i]);
	// } printf("\n");
	
	// //չʾԭ����S
	// 	printf("S:\n");
	// for(i=0;i<n;i++){
	// 	for(j=0;j<m;j++) printf("%d ",S[i][j]);
	// 	printf("\n");
	// } printf("\n");

	//������
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

	// ������.��ÿһ������������һ������
	for(i=0;i<m;i++){//ð������

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

	//���
    printf("The result is : %d\n",Ct[length]);
	for(i=0;i<m;i++){
		printf("The machine %d :",i);
		for(j=0;j<n;j++){
			printf("(%d,%d-%d,%d),",result[i][j][2],result[i][j][0]+1,result[i][j][1]+1,result[i][j][3]);
		}
		printf("\n");
	}
}
/********************************* 6-Output-END *************************************/ 
