//利用cout设置输出宽度，控制输出的小数点数，利用特殊字符占位;
//搞半天也没搞明白怎么插入图片，建议食用的时候把代码贴到IDE里泡泡，方便理解

#include<iostream>
#include<iomanip> //必备头文件

using namespace std;

int main(){

  cout << setw(5) << 123 << endl;   // 控制宽度为5，默认右对齐
  cout << setw(5) << setfill('@') << 123 << endl;    // 控制宽度为5，空位补足'@'
  cout << left << setw(5) << 123 << endl;   // 控制宽度为5，并且左对齐
  cout << right << setw(5) << 123 << endl;    // 控制宽度为5，并且右对齐
  cout << left << setfill('@') << setw(5) << 123 <<endl;    // 三个函数的顺序不影响最终结果
  
  cout << endl;
  
  cout << setprecision(3) << 1145.14000 << endl;    // 1.15e+003 == 1.15*10^3
      // 控制输出的有效位数
      // 如果控制的位数小于实际位数，输出会被压缩成科学计数法
  cout << setprecision(15) << 1145.14000 << endl;   // 1145.14
      // 如果控制的位数大于实际位数，则输出实际的有效位数
      // 无法实现精确小数位数和补0操作
  cout << fixed << setprecision(3) << 1145.14000 << endl;   // 1145.140
      // 保留小数点后3位，不足补0
  cout << scientific << setprecision(3) << 1145.14000 << endl;    // 1.145e+003
      // 科学计数法，并保留小数点后3位，不足补0
      
  return 0;
}
