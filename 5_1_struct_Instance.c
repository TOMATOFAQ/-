//等等，这个函数很像不用写？
//这段分工有问题，这个 类 不必要，直接全局变量即可。要写的话下面这个就是了。要做的只是按照规范的形式读到I就可以了
struct Instance{
    int *** I;
    int n;
    int m;
};

/*
Python源代码：
class Instance:
    """Class representing an instance of the JSP."""
    def __init__(self, jobs, m):
        self.jobs = jobs
        self.n = len(jobs) # number of jobs
        self.m = m         # number of machines

    def __getitem__(self, i):
        return self.jobs[i]

    def __len__(self):
        return len(self.jobs)
*/

/*
解读：
对于一个类，中含有工程的全部信息，也含有工件的数目，机器的数目。
1、jobs=[[(1,3),(2,2)],[(2,5),(1,1)]]（list中又嵌套两个子list,每个子list对应一个工件的信息，子list中的一个有序偶对应表格中的一列）
2、n=2 (工件的个数)
3、m=3（机器的个数）

事实上，知道I的规范表达就已经可以得出n与m了。
只需以三维数组[[(1,3),(2,2)],[(2,5),(1,1)]]形式读入I后，通过I的下标最大值就可以了得到n和m.
*/