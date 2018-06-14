#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *** I;
int n,m;
int printInformation();
int LoadInstance();
/********************新代码********************/


int main(){
    LoadInstance();
    printInformation();
}


/********************新代码********************/
int printInformation(){
    int i,j;
    printf("工件信息：(机器号，所需时间)\n");    
    for(i=0;i<n;i++){
        printf("工件%2d 所需工序:",i);
        for(j=0;j<m;j++)    if(I[i][j][0]) printf("(%2d,%2d),",I[i][j][0],I[i][j][1]);
        printf("\n");
    }
}


int LoadInstance(){
    /*补充对I的说明：
    I[i][j][0] 表示第i个工件第j个工序所需要的机器,若值为0，说明不存在这个工序
    I[i][j][1] 表示第i个工件第j个工序所需要的时间,若值为0，说明不存在这个工序
    */
    
    freopen("input.txt","r",stdin);//默认重定向输入至input.txt，如果需要手动输入请注释改行或者修改输入文件
    int i,j,k;//循环辅助变量
    scanf("%d %d",&n,&m);

    I = (int ***)malloc(sizeof(int**)*n);
    for(i=0;i<n;i++){
        I[i] = (int **)malloc(sizeof(int*)*m);
        for(j=0;j<m;j++){
            I[i][j] = (int *)malloc(sizeof(int)*2);
            memset(I[i][j],0,sizeof(I[i][j]));//0表示不存在这个工序
        }
    }

    //处理输入n，m时残留的空格和回车符号
    char ch = getchar();
    ch = getchar();
    ch = getchar();

    for(i=0;i<n;i++){//第i个工件
        ch = '~';//初始化
        j=0;//第j个工序
        while(ch!='\n'){
            ch = getchar();//删除无关的字符
            if(ch=='('){
                scanf("%d",&I[i][j][1]);
                getchar();
                scanf("%d",&I[i][j][0]);                
                j++;
            }
        }
    }
}
