#include "jobshop.h"

DWORD WINAPI overhaul_output(LPVOID pPararneter);
DWORD WINAPI overhaul_input(LPVOID pPararneter);

void overhaulrun(){
	int i,j;
	//printf("The result had been outputed into output.txt.Please enter num 1 to overhaul.\n");//�������յ�ʱ������Ҫ��ͣ 
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
				//���ù�������һ������֮���Ƿ���ڼ���������ǣ����������Ϣ
				insert_overhaul(i,j);
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
}

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

	if(aimj!=0 && result[aimi][aimj][2] > result[aimi][aimj-1][3] ){
		
		//���ǰ������ʱ���Ƿ��Ǽ���ʱ��
		for(i=0;i<n*m;i++){//�˴����Ż��������岻���ˡ���������м���Ҫ��ֱ��������ǰ��
			if(overhaul_list[i][0] == aimi && overhaul_list[i][1] >= result[aimi][aimj-1][3] && overhaul_list[i][2] <= result[aimi][aimj][2]) return;
		}

		//��Ȼ�м��û�м���������ô������ù�������һ������Ľ���ʱ��
		int name = result[aimi][aimj][0];
		int order = result[aimi][aimj][1];

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

	int mutiple = 10;//�ƶ��ӹ�ʱ���


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

		Sleep(2*1000);//ͣ4�룬��Ȼ����Ϊ����������ô 
		printf("\n");
			
	}
	TIME = -1;//�����˳� 

	return 0;
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
			for(i=0;i<t;i++){
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
	 
	return 0;
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