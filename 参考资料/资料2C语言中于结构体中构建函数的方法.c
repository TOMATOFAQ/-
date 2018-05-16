C语言中于结构体中构建函数
    //法一：传入函数的地址
#include <stdio.h>
//几个用于测试的函数 
int max(int a, int b)
{
    return a>b?a:b; 
}
 
int min(int a, int b)
{
    return a<b?a:b; 
} 
 
//结构体 
struct func
{
  int (*max)(int, int);//函数指针 
  int (*min)(int, int);
};
 
typedef struct func func; //添加别名 
 
void init(func *data)
{
    data->max = max;//初始化函数指针 
    data->min = min; 
} 
 
int main()
{
    int a, b; 
    func test;
     
    init(&test); //初始化，你可以说它是构造函数 
     
    a = test.max(100, 215);
    b = test.min(64, 42); 
     
    printf("result:\nmax: %d\nmin: %d\n", a, b); 
    return 0; 
} 