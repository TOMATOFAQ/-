#include "jobshop.h"

void printInformation() {
	int i, j;
	printf("%d件工件.%d台机器,工件信息：(机器号，所需时间)\n",n,m);
	for (i = 0; i<n; i++) {
		printf("工件%2d 所需工序:", i);
		for (j = 0; j<m; j++)    printf("(%2d,%2d),", I[i][j][0], I[i][j][1]);
		printf("\n");
	}
}

void LoadInstance() {
	
	/*补充对I的说明：
	I[i][j][0] 表示第i个工件第j个工序所需要的机器,若值为0，说明不存在这个工序
	I[i][j][1] 表示第i个工件第j个工序所需要的时间,若值为0，说明不存在这个工序
	i,j都从头开始计算 
	*/
	freopen("input.txt", "r", stdin);//默认重定向输入至input.txt，如果需要手动输入请注释改行或者修改输入文件
	int i, j, k;//循环辅助变量
	scanf("%d %d", &n, &m);

	I = (int ***)malloc(sizeof(int**)*n);
	for (i = 0; i<n; i++) {
		I[i] = (int **)malloc(sizeof(int*)*m);
		for (j = 0; j<m; j++) {
			I[i][j] = (int *)malloc(sizeof(int) * 2);
			memset(I[i][j], 0, sizeof(I[i][j]));
		}
	}

	for (i = 0; i<n; i++) {//第i个工件
		for(j=0;j<m;j++){
			scanf("%d",&I[i][j][0]);
			scanf("%d",&I[i][j][1]);
			}	
		}
} 
