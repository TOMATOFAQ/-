#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//��Ҫ��Ϊ���ε���: 
#define ps 5 //��Ⱥ��С population size
#define mit 3 //�������� mount for iteratior
#define pc 0 //������� probability for crossover
#define pm 0 //ͻ����� probability for mutation

int *** I;//¼����Ϣ 
int n, m;//������,������ 
int length = 0; //Ⱦɫ�峤�� 

typedef struct GRAPH{//����ͼ�Ľṹ֮�ڵ�
    struct NODE * children;
}graph;
typedef struct NODE{//����ͼ�Ľṹ֮�ڵ�
    int * son;
}node;
graph G;
int * st;//һ�����ҵ�Ⱦɫ�� 

int * Ct;//���ÿ������ʼ�ӹ���ʱ��

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
void FormatSolution_and_display(int * s,int * C){//���ﲻӦ����ȫ�ֱ���

    int i,j,k;
    int * T = (int *)malloc(sizeof(int)*n);
    memset(T,0,sizeof(int)*n);

    int ** S = (int **)malloc(sizeof(int *)*n);
    for(i=0;i<n;i++){
        S[i] = (int *)malloc(sizeof(int)*m);
        memset(S[i],0,sizeof(int)*m);
    }

    for(i=0;i<length;i++){
        int j = s[i];//���ʵ�ʹ�����
        int t = T[j-1];//������ǵ�j�������ĵ�t������
        S[j-1][t] = Ct[i]; //��j-1�������ĵ�t������Ŀ�ʼʱ��
        T[j-1] = T[j-1] + 1
		;
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

	//չʾԭ����s
	printf("s:");
	for(i=0;i<length;i++){
		printf("%d ",s[i]);
	} printf("\n");

	//չʾԭ����Ct
	printf("Ct:");
	for(i=0;i<=length;i++){
		printf("%d ",C[i]);
	} printf("\n");
	
	//չʾԭ����S
		printf("S:\n");
	for(i=0;i<n;i++){
		for(j=0;j<m;j++) printf("%d ",S[i][j]);
		printf("\n");
	} printf("\n");

	//��ô,������,
	for(i=0;i<n;i++){ //��i������
		for(j=0;j<m;j++){ //��i�������ĵ�j������
			// S[i][j];//��i�������ĵ�j����������ʼ��ʱ��
			for(k=0;k<m;k++){//�ҵ���k����λ
				if(result[I[i][j][0]-1][k][0]==-1) break;
			}
			result[ I[i][j][0] -1][k][0] = i;
			result[ I[i][j][0] -1][k][1] = j;
			result[ I[i][j][0] -1][k][2] = S[i][j];
			result[ I[i][j][0] -1][k][3] = S[i][j] + I[i][j][1];
		}
	}

	//�������.��ÿһ������������һ������
	for(i=0;i<m;i++){
		//ð������
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
	//1-������Ϣ
	LoadInstance();
	printInformation();

	//2-��ʼ��
	int * eachtime = (int *)malloc(sizeof(int)*ps);
	int ** pop = InitPopulation();

	int i;
	for(i=0;i<ps;i++){
		 eachtime[i] = ComputeStartTimes(pop[i]) ;
		 printf("%d\n",ComputeStartTimes(pop[i]) );
	}
	
//	sortpop(eachtime,pop);
	displaypop(pop,eachtime);
	
	//����	 
	 printf("\n");
	 for(i=0;i<ps;i++){
		 printf("%d\n",ComputeStartTimes(pop[i]));
	}
	
	
	
	//3-����
	iterator(pop,eachtime);

	//4-������
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
//	 			
//	 			printf("����ǰ�ĵ�%d���͵�%d��Ⱦɫ��:\n",i,ps/2+i);
//	 			printfparent(pop[i]);
//	 			printfparent(pop[ps/2+i]);
	 			
	 			ch1 = Crossover(pop[i],pop[ps/2+i]); 
	 			ch2 = Crossover(pop[ps/2+i],pop[i]); 
	 			
//	 			printf("�����ĵ�%d���͵�%d��Ⱦɫ��:\n",i,ps/2+i);
//	 			printfparent(ch1);
//	 			printfparent(ch2);
	 			
	 			if((rand() % 100) > pm){
					Mutation(ch1);
//					printf("�����ĵ�%d��Ⱦɫ��:\n",i);
//		 			printfparent(ch1);
				 }

				if((rand() % 100) > pm) {

					Mutation(ch2);
//					printf("�����ĵ�%d��Ⱦɫ��:\n",ps/2 + i);
//	 				printfparent(ch2);
				}
				
	 			time1 = ComputeStartTimes(ch1);
	 			time2 = ComputeStartTimes(ch2);
	 			
//	 			printf("%d %d\n",time1,time2);
	 			
				//����д���˰�......ʲô��sort�����ǴӴ���С�Ű�,�ǵ�����ʲô�ط������
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
		printf("������%d�εĴ�:%d\n",it,eachtime[0]); 
//		printf("%d  ",eachtime[0]); 
		displaypop(pop,eachtime);//���� 
		
	//����	 
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


void shuffle(int * gene) {//ϴ�ƺ���.���������������ϴ��length/2��.̫��Ҳ�����˷�ʱ��.���ϴ�ƴ���Ӧ���ǿ����ɾ���ĳ�����.
	int i;
	for (i = 0; i<length; i++) {//******************************************�������ʲô��� 
		int loc1 = rand() % length;
		int temp = gene[i];
		gene[i] = gene[loc1];
		gene[loc1] = temp;
	}
}

int ** InitPopulation(){//ԭ����������������,ps��I,������ȡ��
//ȷ��һ��Ⱦɫ��ĳ���,��ȷ�����������,����ȷ�����������
	int i = 0, j = 0, k = 0;
	for (i = 0;i<n; i++) {
		for (j = 0;j<n&&I[i][j][0]!=0; j++) {
				length++;
		}
	}

	int * gene = (int *)malloc(sizeof(int)*length);
	for (j = 1, i = 0;j<=n; j++) {//j��ʾ�Թ����ı���,i��ʾ�����ϵ�i��λ��
		int t;//t��ʾ��ָ���Ĺ����Ĺ���ı���
		for (t = 0;t<m&&I[j-1][t][0]!=0; t++, i++) {
			gene[i] = j;
		}
	}

	int ** population = (int **)malloc(sizeof(int*)*ps);
	for (i = 0; i<ps; i++) {
		shuffle(gene);
		population[i] = (int *)malloc(sizeof(int)*length);         //��ps������ƾ����һ��Ⱦɫ����
		for (j = 0; j<length; j++) {
			population[i][j] = gene[j];
		}
	}
	return population;
}

void displaypop(int ** pop,int * eachtime){//����������Ⱥ�����õ� 
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
void ComputeDAG(int * s){//��������ͼ,sΪȾɫ�����������
	int i,j,k;//ѭ���ĸ�������

    G.children = (node*)malloc(sizeof(node)*(length+1));//G.children[length]��Ϊ��ֹ�ڵ� 
	for(i=0;i<length+1;i++){
		G.children[i].son = (int *)malloc(sizeof(int)*(length)); // ��5������ϰ���Ϸ�ֹ����� 
		memset(G.children[i].son,0,sizeof(G.children[i].son));
	}

//    ��ʼ��һ������Ϊ��������ȫ0list
    int * T = (int *)malloc(sizeof(int)*n);
    memset(T,0,sizeof(int)*n);

    //����洢ÿ��������һ�μ���ͼ�Ĺ���ţ���ʼ��Ϊ-1
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
    	
        int j = s[i] - 1;//��ȡ���Ϊi�Ľڵ��Ӧ�Ĺ�����
        int t = T[j];//��������,��¼���i�Ľڵ���j�����ڵĵڼ�������

        st[i] = t;//��t����st
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
		
        int r = I[j][t][0] - 1;//��j�����ĵ�t�������Ӧ����ż�еĵ�һ�����֣����ù���Ļ����Ŵ������r

        //�ж�j����Թ���ŵļ�����t��ֵ�Ƿ����j�Ĺ��������������j�����һ������ָ����ֹ�ڵ�
		int gongxushu_j;
		for(gongxushu_j=0;gongxushu_j<m && I[j][gongxushu_j][0]!=0;gongxushu_j++);
        if(t+1 == gongxushu_j) {
        	int tempnum;
        	for(tempnum = 0;G.children[length].son[tempnum]!=0;tempnum++);
        	
        	G.children[length].son[tempnum] = i;
		}
		 
        //������j�ĵ�һ�������򽫸ù����ǰһ������ָ�����ǰ����Ľڵ�i

        if(t>0) {
        	int tempnum;
        	for(tempnum = 0;G.children[length].son[tempnum]!=0;tempnum++);
        	G.children[i].son[tempnum] = last_task_job[j];
		}

        //���֮ǰ����Ľڵ�Ҳ��ʹ�ú�j�ĵ�t��������ͬ�Ļ���r,����Щ�ڵ���ָ��ǰ����ڵ�i
        int j2;
        for(j2=0;j2<n;j2++){
            if(j2!=j && tasks_resource[r][j2] > -1){
                int tempnum;
        		for(tempnum = 0;G.children[length].son[tempnum]!=0;tempnum++);
        		G.children[i].son[tempnum] = tasks_resource[r][j2];
            }
        }

        T[j] = T[j] + 1;	//j�����Ĺ�����ۼ�����1
        last_task_job[j] = i;	//��j��������һ������Ľڵ�����Ϊ��ǰ�ڵ�ı��i
        tasks_resource[r][j] = i;	//��j����������ʹ�û���r�Ĺ����Ӧ�Ľڵ�����Ϊi
    }
}

int ComputeStartTimes(int * s){//�����Ⱦɫ��������Ҫ��ʱ��
    ComputeDAG(s);//�õ���һ��ͼ,�õ��˽ڵ�����Ӧ�ĵ� n ������
    
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
//����Ϊʲô�����д����ô�鷳,��Ϊ�˲��ƻ��ڵ�һ�����Ѿ�ȷ�����˹����ϵ 
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b)) 

int ** Index(int * p){//����һ����Ԫ�Ե�����,ÿ����Ԫ���е�һ����ʾ���ǹ���,�ڶ�����ʾ����˳��.
//ֻ���൱�ڸ߼�һ��Ľṹ���� 
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

void printfIndex(int ** idx){//��������,���ڹ۲��ڲ���� 
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

int * Crossover(int * p1,int *p2){//p1,p2�������ȴ������Ⱦɫ��,pΪparent����д
    int i,j,k;//����ѭ���ĸ�������
    int ** idx_p1 = Index(p1);
    int ** idx_p2 = Index(p2);

//	printfIndex(idx_p1);
//	printfIndex(idx_p2);

    int randi = (rand()%(length-1))+1;//����[1,nt)�������,��ʾ��Ƕ��ı���Ƭ��֮�� 
    int randj = rand()%(length-randi);//����[0,nt-randi-1)�������, ��ʾ����Ƭ����parent1�е���ʼλ�� 
    int randk = rand()%length;//����[0,nt)�������,��ʾ����Ƭ����parent2�е���ʼλ�� 

//	printf("%d %d %d \n",randi,randj,randk);

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
	printf("������Ϣ��(�����ţ�����ʱ��)\n");
	for (i = 0; i<n; i++) {
		printf("����%2d ���蹤��:", i+1);
		for (j = 0; j<m; j++)    printf("(%2d,%2d),", I[i][j][0], I[i][j][1]);
		printf("\n");
	}
}

void LoadInstance() {
	/*�����I��˵����
	I[i][j][0] ��ʾ��i��������j����������Ҫ�Ļ���,��ֵΪ0��˵���������������
	I[i][j][1] ��ʾ��i��������j����������Ҫ��ʱ��,��ֵΪ0��˵���������������
	*/
	freopen("input.txt", "r", stdin);//Ĭ���ض���������input.txt�������Ҫ�ֶ�������ע�͸��л����޸������ļ�
	int i, j, k;//ѭ����������
	scanf("%d %d", &n, &m);

	I = (int ***)malloc(sizeof(int**)*n);
	for (i = 0; i<n; i++) {
		I[i] = (int **)malloc(sizeof(int*)*m);
		for (j = 0; j<m; j++) {
			I[i][j] = (int *)malloc(sizeof(int) * 2);
			memset(I[i][j], 0, sizeof(I[i][j]));//0��ʾ�������������
		}
	}

	//��������n��mʱ�����Ŀո�ͻس�����
	char ch = getchar();
	ch = getchar();
	ch = getchar();

	for (i = 0; i<n; i++) {//��i������
		ch = '~';//��ʼ��
		j = 0;//��j������
		while (ch != '\n') {
			ch = getchar();//ɾ���޹ص��ַ�
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
