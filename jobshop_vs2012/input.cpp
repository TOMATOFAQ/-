#include "jobshop.h"

void LoadInstance(){
	/*补充对I的说明：
	I[i][j][0] 表示第i个工件第j个工序所需要的机器,若值为0，说明不存在这个工序
	I[i][j][1] 表示第i个工件第j个工序所需要的时间,若值为0，说明不存在这个工序
	i,j都从头开始计算 
	*/

	freopen("input.txt", "r", stdin);//重定向

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

	freopen("CON", "r", stdin);//回到控制台
	//内存释放:I在主函数free
}