#include <stdlib.h>
#include <time.h>

int * InitPopulation(int ps,int I){
    //确定一个染色体的长度,即确定工序的总数,下面确定工序的总数
    int length = 0;
    int i=0,j=0,k=0;
    for(i=0;I[i][j][k];i++){
        for(j=0;I[i][j][k];j++){
            for(k=0;I[i][j][k];k++){
                length++;
            }`
        }
    }

    int * gene = (int *)malloc(sizeof(int)*length);
    for(j=0,i=0;j<n;j++){//j表示对工件的遍历,i表示基因上第i个位置
        int t;//t表示对指定的工件的工序的遍历
        for(t=0;I[j][t][0];t++,i++){
            gene[i] = j;
        }
    }

    int * popilation = (int *)malloc(sizeof(int)*length*ps);
    for(i=0,j=0;i<ps;i++){
        shuffle(gene);
        for(k=0;k<length;k++) popilation[j] = gene[k];        //将ps个基因凭借在一条染色体上
    }
    return population;
}

void shuffle(int * gene){//洗牌函数.这里对这个基因随便洗牌length/2次.太多也不好浪费时间.这个洗牌次数应该是可以由经验的出来的.
    srand((unsigned)time(NULL));    //以时间做种
    length = strlen(gene);

    int i;
    for(i=0;i<(length/2));i++{  //随机取两个位置交换,交换length/2次
        int loc1 = rand()%length;
        int loc2 = rand()%length;

        int temp = gene[loc1];
        gene[loc1] = gene[loc2];
        gene[loc2] = temp;
    }
}
