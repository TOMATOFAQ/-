/*part1基本原理介绍
遗传算法是一种随机搜索算法，它主要分为六大功能模块：编码、交叉、变异、解码、评价、选择。
*/



/*各种定义的说明——按照顺序*/
int *** I;
/*
information.三维数组。存放每个工件下每道工序使用的机器号和在该机器上加工的时间。如I=[[(1,3),(2,2)],[(2,5),(1,1)]]。I 下面又分别有两个list, 它们是I[0]=[(1,3),(2,2)] 和I[1]=[(2,5),(1,1)]。
I[i]表示第i个工件对应的信息。I[i][j]表示第i个工件的第j个工序所需要的机器。I[i][j][k]表示第i个工件在第j个指定的机器上所需要的时间。
PS:看起来是有点复杂，但先优先按照已有的说明来操作，后续再优化吧。
*/
int ps;     /*表示种群的大小*/
int ** population;      /*存放各种生成的染色体*/
int * p;        /*染色体*/
int * child;    /*新交叉的染色体*/
int * m;    /*操纵交叉所得的染色体后所得的变异染色体*/
int n;      /*工件数量*/
int m;      /*机器数量*/
struct * G ;    /*图*/
int *s;     /*s是染色体即工件号的乱序排列*/
int * st;      /*记录每个节点对应的工序在其工件内是第几道工序,由s生成*/
int * C;    /*用来存放图G中每个节点的开始时间，长度为G的节点个数*/
int ** S;   /*S中存放的是每道工序开始加工的时间，它的形式为：[[a,b,c],[d,e,f],[g,h,i]],每个子list代表一个工件的信息，子list中的字母代表这个工件下面每道工序开始加工的时间。*/




/*
part2编码:生成初始种群
*///简单
int ** InitPopulation(int ps,int *** I){
    int ** population;      /*存放各种生成的染色体*/

    return population;
}



/*
part3、交叉：从两条染色体中各自取出一部分组合成新的染色体
*///简单，中间的嵌套函数灵活处理即可
int * Crossover(int * p1,int * p2,int ** I){
    int * child;    /*新交叉的染色体*/

    return child;
}



/*
part4、变异：过随机交换染色体的两个位置上的值来得到变异后的染色体。
*///简单
int * Mutation(int *p){
    int * m;    /*操纵交叉所得的染色体后所得的变异染色体*/

    return m;
}



/*
part5、解码：解码包含了两步，一步时实际上的第一步即转换input.txt数据到可操作的形态；第二步时便是整个程序中求出答案的核心部分
*/
//5-1定义结构体  
//中等  
struct Instance{
    void (*__init__)(self,jobs,m);
    int (*__getitem)(self,i);
    int (*__len__)(self);
};

//5-2将input.txt内信息读到成员变量中
//简单
int *** loadInstance(filename){

    // return Instance(int ** I,int m); //此处为返回一个类。不必，直接返回I即可。m作为全局变量来控制。PS：I也做全局变量吧，不然太麻烦了。写了再说。
    return I;
}

//5-3 构建有向无环图
//难
int * ComputeDAG(int * s,int ** I){

    return G,st;//多参数调回需要在传参时用指针，具体操作时再做修改吧
}

//5-4 对图用动态规划求每个解的适应度
//难
int ComputeStartTimes(int * s,int ** I){

    return C;
}



//part6结果显示:
//简单
int ** FormatSolution(int * s,int C,int ** I){

    return S;
}



//part7整体流程
//这个很像也挺难的
int main(){

}

