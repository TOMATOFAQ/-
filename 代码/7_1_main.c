#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ps 10 //��Ⱥ��С 
#define mit 10 //�������� 
int *** I;
int n, m;
void printInformation();
void LoadInstance();

int findlen(int * a){
	return sizeof(a)/sizeof(int);
}

/********************************* InitPopulation *************************************/ 
int length = 0; //Ⱦɫ�峤�� 

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

void displaypop(int ** pop){//����������Ⱥ�����õ� 
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
struct GRAPH{//����ͼ�Ľṹ֮�ڵ�
    struct NODE * children;
};

struct NODE{//����ͼ�Ľṹ֮�ڵ�
    int * son;
};

typedef struct NODE node;
typedef struct GRAPH graph;

graph G;
int * st;

/*
sΪȾɫ����������У�IΪ������Ϣ;�������������Ƴ���Ҫ������������������,���ڴ˴�ָ��.����Ĳ���˵��:
���������ж������G,st.Ȼ��ȡ��ַ��������.��ComputDAG(s,I,&G,&st);����. 
*/
 
void ComputeDAG(int * s){//��������ͼ
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
//		for(temp = 0;temp<length;temp++) printf("%d  ",s[temp]);
//		printf("\n");
//		for(temp = 0;temp<n;temp++) printf("%d  ",T[temp]);
//		printf("\n");
//		for(temp = 0;temp<length;temp++) printf("%d  ",st[temp]);
//		printf("\n");
		
		
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

int * Crossover(int * p1,int *p2){//p1,p2�������ȴ������Ⱦɫ��,pΪparent����д
    int i,j,k;//����ѭ���ĸ�������
    int ** idx_p1 = Index(p1);
    int ** idx_p2 = Index(p2);

    int nt = findlen(p1);
    int randi = (rand()%(nt-1))+1;//����[1,nt)�������
    int randj = rand()%(nt-1);//����[0,nt-1)�������
    int randk = rand()%nt;//����[0,nt)�������

    //implant �൱������ġ�(A,2)(C,1)(A,3)(B,3)������Parent2��ȡ��Ƭ�� 
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

    //�����ǲ��ǿ�����sizeof(implat)������������ֱ��������İ�
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
	srand((unsigned)time(NULL));    //��ʱ������
	int i,j,k;
	
	LoadInstance();
	printInformation();

	int * eachpopulationtime = (int *)malloc(sizeof(int)*ps);
	int ** pop = InitPopulation();
	
	displaypop(pop);	//������
	
	int max = 0;
	for(i=0;i<ps;i++){
		 eachpopulationtime[i] = ComputeStartTimes(pop[i]);
		 if(eachpopulationtime[i]>max)  max = eachpopulationtime[i];
	}

//	for(i=0;i<ps;i++) printf("%2d:%2d\n",i,eachpopulationtime[i]);
	printf("%d\n",max);


	 int it;	//iterator ������
	 for(it=1;it<=mit;it++){
		shufflepop(pop);
		int hpop = ps/2;
	 	 for(i=0;i<(ps/2);i++){
	 		int pc = 60;		//�������,����������� 
	 		int p1 = rand() % 100;
	 		if(p1 > pc) {
	 			
	 			
	 			
	 			
	 			
	 			int pm = 60;	//ͻ�����,���������ͻ�� 
			 }
		 }
	 }
	 
	 return 0;
}

/********************************* MAIN *************************************/ 

/*
def main():
""" pop����Ⱥ����Ⱥ�е�ÿ���������ʽ�ǣ���makespan, Ⱦɫ�壩"""     
    pop = [(ComputeStartTimes(g, I)[-1], g) for g in InitPopulation(ps, I)]
    for it in xrange(1, mit+1):#""" mit�ǵ�������"""
        # Random ordering of the population
        shuffle(pop)#"""��pop�и��������˳����� """
        hpop = len(pop) / 2#""" hpop����Ⱥ��һ��"""
        for i in xrange(hpop):#""" ������Ⱥ��ǰ�벿�ַ�"""
            if random() < pc:#"""��[0,1]֮�������� < pc """
                # Create two new elements
                ch1 = Crossover(pop[i][1], pop[hpop + i][1], I)#""" ͨ������������һ��"""
                ch2 = Crossover(pop[hpop + i][1], pop[i][1], I)#""" ͨ������������һ��"""
                if random() < pm:#"""��[0,1]֮�������� < pm """
                    ch1 = Mutation(ch1)#""" ��ch1���б���"""
                if random() < pm:#"""��[0,1]֮�������� < pm """
                    ch2 = Mutation(ch2)#"""��ch2���б���"""
                pop.append((ComputeStartTimes(ch1, I)[-1], ch1))#""" ���������Ⱦɫ��Ż���Ⱥ"""
                pop.append((ComputeStartTimes(ch2, I)[-1], ch2))
        # Sort individuals in increasing timespan order and
        # select only the best ones for the next iteration
        pop.sort()#""" ��pop�е�Ⱦɫ�尴makespan����"""
        pop = pop[:ps]#""" ��ȡpop�е�ǰps��Ⱦɫ��"""
    return pop[0]#"""����makespan����Ⱦɫ�� """
*/

void printInformation() {
	int i, j;
	printf("������Ϣ��(�����ţ�����ʱ��)\n");
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
				j++;
			}
		}
	}
	
	/*
	�����ʽΪ:

	I[0][0][0] Ϊ��һ�������ĵ�һ������� 
	 
	...Ҫ��Ҫ�޸�ΪI[1][1][0]�ǵ�һ�������ĵ�һ���������ڵĻ�����?�о��ǳ��Ļ���.����޸ĵĻ��ƺ�Ҫ�Ӻܶ�ܶ�����ںż�����Ҫ����д��. 
	*/
}





