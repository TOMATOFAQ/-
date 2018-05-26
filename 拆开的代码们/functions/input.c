#include "jobshop.h"

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
