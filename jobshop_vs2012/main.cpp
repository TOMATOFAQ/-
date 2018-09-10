#include "jobshop.h"
#include "graphics.h"

int *** I;	//¼����Ϣ 
int *** result;
int *** ganttorignal;
int n, m;	//������,������ 
int length = 0; 	//Ⱦɫ�峤�� 
double duration;
double duraorignal;

//���ڶ��̵߳��ĸ�ȫ�ֱ���
int final_result = ((unsigned)1<<31)-1; //ʵʱ���´�����ʱ��
int finalorignal;
int * final_s;
int * final_Ct;
HANDLE hMutex;//���ڷ�ֹ���߳�֮����໥����

int ** overhaul_list;
int TIME;//����ͬ������������������е�ʱ��

int once(){//һ���Ŵ��㷨

	//2-��ʼ�����ݽṹ
	int * eachtime = (int *)malloc(sizeof(int)*ps);
	memset(eachtime,0,sizeof(int)*ps);
	int ** pop = InitPopulation();

	int * Ct = (int *)malloc(sizeof(int)*(length+1));

	int i;
	for(i=0;i<ps;i++){
		 eachtime[i] = ComputeStartTimes(pop[i],Ct);
		 eachtime[i] = ComputeStartTimes(pop[i],Ct);
	}

	//3-����
	iterator(pop,eachtime,Ct);
	
	//4-������
	// FormatSolution_and_display(pop[0],Ct,1); //��ʾÿһ�ε����ľ�����
	ComputeStartTimes(pop[0],Ct);
	
	// printf("Time Used:%.3lf\n",Ct[0]);
	printf("End Time:%d\n",final_result);

	//�޸�ȫ�ֱ���
	WaitForSingleObject(hMutex,INFINITE);
	if(Ct[length] < final_result){
		final_result = Ct[length];
		for(i=0;i<length;i++) final_s[i] = pop[0][i];
		for(i=0;i<=length;i++) final_Ct[i] = Ct[i];
	}
	ReleaseMutex(hMutex);

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

int main(int argc, char**argv){
	int i,j;
	setinitmode(0,0,0);
	initgraph(1280,716);
	PIMAGE waiting;
	waiting=newimage();
	getimage (waiting,"waiting.jpg",0,0);
	putimage(0,0,waiting);
	//Ĭ���ض���������input.txt�������Ҫ�ֶ�������ע�͸��л����޸������ļ�
	//freopen("output.txt","w",stdout);

	LoadInstance();
	
	clock_t start,finish;
	start = clock();
	
	//��ʼ�������ĸ��̴߳𰸵ĵ�ȫ�ֱ���
	length = n*m;
	final_s = (int *)malloc(sizeof(int)*length);
	final_Ct = (int *)malloc(sizeof(int)*(length+1));

	HANDLE hThread1,hThread2,hThread3,hThread4;
	hMutex=CreateMutex(NULL,FALSE,NULL);
	hThread1=CreateThread(NULL,0,original_main,NULL ,0,NULL);
	Sleep(1000);//�ı���Ϊ��������ӵ�ʱ�� 
	hThread2=CreateThread(NULL,0,original_main,NULL ,0,NULL);
	Sleep(1000);
	hThread3=CreateThread(NULL,0,original_main,NULL ,0,NULL);
	Sleep(1000);
	hThread4=CreateThread(NULL,0,original_main,NULL ,0,NULL);
	
	CloseHandle(hThread1);
	CloseHandle(hThread2);
	CloseHandle(hThread3);
	CloseHandle(hThread4);
	
	Sleep(programmeTime*1000);

	finish = clock();
	duration = (double)(finish-start)/CLOCKS_PER_SEC;
	
	//�������ǰ���
	freopen("output.txt", "w", stdout);//�ض���
	FormatSoverhaul_listution_and_display(final_s,final_Ct,1);
	printf("Time Used:%.3lf\n",duration);
	duraorignal=duration;
	printf("End Time:%d\n",final_result);
	finalorignal=final_result;
	freopen("CON", "w", stdout);//�ص�����̨
	
	
	//�ڿ���̨������
	FormatSoverhaul_listution_and_display(final_s,final_Ct,1);
	printf("Time Used:%.3lf\n",duration);
	printf("End Time:%d\n",final_result);
	 
	image_ganttorignal();
	image_list();

	//�ڴ��ͷ�
	for (i = 0; i<n; i++) {
		for (j = 0; j<m; j++) free(I[i][j]);
		free(I[i]);
	}
	free(final_s);
	free(final_Ct);
	free(I);

	
	for(i=0;i<m;i++){
		for(j=0;j<n;j++){
			free(result[i][j]);
			free(ganttorignal[i][j]);
		}
		free(result[i]);
		free(ganttorignal[i]);
	}
	free(result);
	free(ganttorignal);

	return 0;
}
