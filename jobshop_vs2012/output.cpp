#include "jobshop.h"

void FormatSoverhaul_listution_and_display(int * s,int * Ct,int choice){//choiceΪ0:���������.choiceΪ1,�������

	ComputeStartTimes(s,Ct);//����һ��,ˢ��Ct.�������ʧ��,��Ӧ����ȫ�ֱ�����.�������ǵ�һ�γ���ֻ��һ��,���Բ�����. //ԭ��δ֪��bug

    int i,j,k;
    int * T = (int *)malloc(sizeof(int)*n);
    memset(T,0,sizeof(int)*n);
	ganttorignal = (int ***)malloc(sizeof(int**)*m);
	for(i=0;i<m;i++){//��į����
		ganttorignal[i] = (int **)malloc(sizeof(int*)*n);
		for(j=0;j<n;j++){//��į�����ĵ�j������
			ganttorignal[i][j] = (int *)malloc(sizeof(int)*4);//��į�����ĵ�j����������Ӧ���������,��������������ڹ���,�Լ���ʼʱ��,��ֹʱ��
		}
	}
	
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
		for(i=0;i<m;i++){
			printf("M%d ",i);
			for(j=0;j<n;j++){
				
				// ���Դ��룺
				// if(j>0){//��ʱ���ԣ��������޳�ͻ
				// 	if(result[i][j][2]<result[i][j-1][3]) printf("����ERROR����");
				// }
				ganttorignal[i][j][0] = result[i][j][0];
				ganttorignal[i][j][1] = result[i][j][1];
				ganttorignal[i][j][2] = result[i][j][2];
				ganttorignal[i][j][3] = result[i][j][3];

				printf("(%d,%d-%d,%d) ",result[i][j][2],result[i][j][0],result[i][j][1],result[i][j][3]);

			}
			printf("\n");
		}		
	}


	//�ڴ��ͷ�
	free(T);
	
    for(i=0;i<n;i++) free(S[i]);
    free(S);
} 
