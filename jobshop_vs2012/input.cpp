#include "jobshop.h"

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