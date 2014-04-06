#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;
 
int main()
{
    double random(double,double);
    srand(unsigned(time(0)));
    for(int icnt = 0; icnt != 3; ++icnt)
        cout << "No." << icnt+1 << ": " << int(random(0,512))<< endl; 
	getchar();
    return 0;
}
double random(double start, double end)
{ return start+(end-start)*rand()/(RAND_MAX+ 1.0);}



<一>
C/C++怎样产生随机数：这里要用到的是rand()函数, srand()函数，C语言/C++里没有自带的random(int number)函数。
(1)  如果你只要产生随机数而不需要设定范围的话，你只要用rand()就可以了：rand()会返回一随机数值, 范围在0至RAND_MAX 间。RAND_MAX定义在stdlib.h, 其值为2147483647。
例如：
#include<stdio.h>
#include<stdlib.h>
void main()
{
       for(int i=0;i<10;i+)
             printf("%d/n",rand());
}
(2)  如果你要随机生成一个在一定范围的数，你可以在宏定义中定义一个random(int number)函数，然后在main()里面直接调用random()函数：
例如：随机生成10个0~100的数：
#include<stdio.h>
#include<stdlib.h>
#define random(x) (rand()%x)

void main()
{
     for(int x=0;x<10;x++)
           printf("%d/n",random(100));
}
 
(3)但是上面两个例子所生成的随机数都只能是一次性的，如果你第二次运行的时候输出结果仍和第一次一样。这与srand()函数有关。srand()用来设置rand()产生随机数时的随机数种子。在调用rand()函数产生随机数前，必须先利用srand()设好随机数种子（seed）, 如果未设随机数种子, rand()在调用时会自动设随机数种子为1。上面的两个例子就是因为没有设置随机数种子，每次随机数种子都自动设成相同值1 ，进而导致rand()所产生的随机数值都一样。
srand()函数定义 ： void srand (unsigned int seed); 
通常可以利用geypid()或time(0)的返回值来当做seed
如果你用time(0)的话，要加入头文件#include<time.h>
例如：
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define random(x) (rand()%x)

void main()
{
     srand((int)time(0));
     for(int x=0;x<10;x++)
           printf("%d/n",random(100));
}
这样两次运行的结果就会不一样了！！
 
<二>
标准C库中函数rand（）可以生成0~RAND_MAX之间的一个随机数，其中RAND_MAX 是stdlib.h 中定义的一个整数，它与系统有关。
rand（）函数没有输入参数，直接通过表达式rand（）来引用；例如可以用下面的语句来打印两个随机数：
printf("Random numbers are: %i %i/n",rand(),rand());
因为rand（）函数是按指定的顺序来产生整数，因此每次执行上面的语句都打印相同的两个值，所以说C语言的随即并不是正真意义上的随机。
为了时程序在每次执行时都能生成一个新序列的随机值，我们通常通过为随机数生成器提供一粒新的随机种子。函数srand()(来自stdlib.h)可以为随机数生成器播散种子。只要种子不同rand()函数就会产生不同的随机数序列。srand()称为随机数生成器的初始化器。
例程：
文件名： rand_srand.c
/* This program generates and prints ten random integers between 1 and RAND_MAX*/
#include <stdio.h>
#includ <stdlib.h>
int main()
{
        usigned int seed;        /*申明初始化器的种子，注意时usigned int 型的*/
        int k;
        pringt("Enter a positive integer seed value: /n");
        scanf("%u",&seed);
        srand(seed);
        printf("Random Numbers are:/n");
        for(k = 1; k <= 10; k++)
        printf("%i",rand());
        printf("/n");
        return 0;
}
你会发现，当你提供的种子相同时，随机数序列也时相同的。而且当种子为1时，与不使用srand()函数时一样的，也就是说rand()函数默认情况下初始化种子值为1；
在stdlib.h 中这两个函数的原型是：
int rand();
void srand (unsigned int);
扩充：
        x = rand()%11; /*产生1~10之间的随机整数*/
        y = rand()%51 - 25; /*产生-25 ~ 25之间的随机整数*/
        z = ((double)rand()/RAND_MAX)*(b-a) + a;/*产生区间[a,b]上的随机数*/
 
<三>
 
1-0：Microsoft VC++产生随机数的原理：
Srand ( )和Rand( )函数。它本质上是利用线性同余法，y=ax+b(mod m)。其中a，b，m都是常数。因此rand的产生决定于x，x被称为Seed。Seed需要程序中设定，一般情况下取系统时间作为种子。它产生的随机数之间的相关性很小，取值范围是0—32767（int），即双字节（16位数），若用unsigned int 双字节是65535，四字节是4294967295，一般可以满足要求。
1-1： 线性同余法：
?/P>
其中M是模数，A是乘数，C是增量，为初始值，当C=0时，称此算法为乘同余法；若C≠0，则称算法为混合同余法，当C取不为零的适当数值时，有一些优点，但优点并不突出，故常取C=0。模M大小是发生器周期长短的主要标志，常见有M为素数，取A为M的原根，则周期T=M-1。例如：
a=1220703125        
a=32719            （程序中用此组数）   
     a=16807          
代码：
void main( )
{
const int n=100;
double a=32719,m=1,f[n+1],g[n],seed;
m=pow(2,31);
cout<<"设置m值为  "<<m-1<<endl;
cout<<"输入种子"<<endl;  //输入种子
cin>>seed;
f[0]=seed;    
    for(int i=1;i<=n;i++)    //线性同余法生成随机数
      {
         f[i]=fmod((a*f[i-1]),(m-1));
             g[i-1]=f[i]/(m-1);
             cout.setf(ios::fixed);cout.precision(6); //设置输出精度
         cout<<i<<"   "<<'/t'<<g[i-1]<<endl;
      }
}
结果分析：统计数据的平均值为：0.485653
统计数据的方差为：0.320576
 
1-2：人字映射
递推公式
?/P>
就是有名的混沌映射中的“人字映射”或称“帐篷映射”，它的非周期轨道点的分布密度函数：人字映射与线性同余法结合，可产生统计性质优良的均匀随机数。
 for(int i=1;i<=n;i++)    //线性同余法生成随机数
      {
         f[i]=fmod((a*f[i-1]),m);
             if(f[i]<=m/2)     //与人字映射结合生成随机数
             {
                    f[i]=2*f[i];
             }
             else
             {
                    f[i]=2*(m-f[i])+1;
             }
1-3：平方取中法——冯·诺伊曼
1946年前后，由冯·诺伊曼提出，他的办法是去前面的随机数的平方，并抽取中部的数字。例如要生成10位数字，而且先前的值是5772156649，平方后得到33317792380594909201，所以下一个数是7923805949。
for(j=1;j<=n;j++)
      {
             i[j]=i[j-1]*i[j-1];  
        i[j]=i[j]/pow(10,5); 
        i[j]=fmod(i[j],pow(10,10));
        g[j]=i[j]/pow(10,10);
        cout.setf(ios::fixed);cout.precision(6); //设置输出精度
        cout<<j<<'/t'<<g[j]<<endl;
      }
二：任意分布随机数的生成
     利用(0，1)均匀分布的随机数可以产生任意分布的随机数。主要的方法有反函数法，舍选法，离散逼近法，极限近似法和随机变量函数法等。这里主要讨论了反函数法，当然对于具体分布函数可以采用不同的方法。
设随机变量X具有分布函数F(X)，则对一个给定的分布函数值，X的值为
                                              
其中inv表示反函数。现假设r是(0，1)均匀分布的随机变量R的一个值，已知R的分布函数为
                              
因此，如果r是R的一个值，则X具有概率
 
也就是说如果 (r1,r2,...,rn)是R的一组值，则相应可得到的一组值
                    
具有分布。从而，如果我们已知分布函数的反函数，我们就可以从(0，1)分布的均匀分布随机数得到所需分布的随机数了。
1-4：指数分布：
指数分布的分布函数为:
x<0时,F(x)=0    ； ,F(x)=1-exp
利用上面所述反函数法，可以求得:  x= ln(1-y)，这里不妨取常数 为1.
for(int j=0;j<n;j++)
       { 
              i=rand()%100;//产生从0-32767的任意一个值
        a[j]=double(i)/double(100); 
          a[j]=-log(a[j]);//  常数大于0，这里取1
          、、、、、、、
1-5：正态分布：
正态分布的概率密度是：
正态分布的分布函数是：
对于正态分布，利用反函数的方法来获取正态分布序列显然是很麻烦的，牵涉到很复杂的积分微分运算，同时为了方便，我们取，即标准正态分布。因此这里介绍了两种算法：
第一种：
Box和Muller在1958年给出了由均匀分布的随机变量生成正态分布的随机变量的算法。设U1, U2是区间 (0, 1)上均匀分布的随机变量，且相互独立。令  
  X1=sqrt(-2*log(U1)) * cos(2*PI*U2); 
  X2=sqrt(-2*log(U1)) * sin(2*PI*U2);  
那么X1, X2服从N(0,1)分布，且相互独立。
             p=rand()%100;//产生从0-32767的任意一个值
             b[j]=double(p)/double(100);
             a[j]=sqrt(-2*log(a[j]))*cos(2*3.1415926*b[j]);
第二种：
近似生成标准正态分布，独立同分布的多个随机变量和的分布趋近于正态分布,取k个均匀分布的(0,1)随机变量，，…… ，则它们的和近似服从正态分布。
  实践中,取k=12,(因为D( )=1/12),则新的随机变量y=x1+x2+...+x12-6，可以求出数学期望E(y)=0,方差D(y)=12*1/12=1，因此可以近似描述标准正态分布。
