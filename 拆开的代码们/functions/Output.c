#include "jobshop.h"

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
	for(i=0;i<m;i++){
		printf("The machine %d :",i);
		for(j=0;j<n;j++){
			printf("(%d,%d-%d,%d),",result[i][j][2],result[i][j][0]+1,result[i][j][1]+1,result[i][j][3]);
		}
		printf("\n");
	}
}
