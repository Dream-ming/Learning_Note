# C++11特性笔记

* [1、原始字面量 R"xxx()xxx"](#1-------r-xxx--xxx-)
* [2、字符串和数值类型之间的转换](#2--------------)
    + [数值转换为字符串 to_string()](#---------to-string--)
    + [字符串转换为数字](#--------)
* [3、超长整形long long](#3-----long-long)
* [4、标准的类成员初始化](#4----------)
* [5、final 和 override](#5-final---override)
    + [final](#final)
    + [override](#override)
* [6、assert 、 throw 和 noexpect](#6-assert---throw---noexpect)
    + [断言assert](#--assert)
    + [throw 和 noexpect](#throw---noexpect)
* [7、自动类型推导 auto 和 decltype](#7--------auto---decltype)
    + [decltype](#decltype)
* [8、Lambda表达式](#8-lambda---)
* [9、NULL 和 nullptr 的区别](#9-null---nullptr----)
* [10、constexpr](#10-constexpr)
* [11、浅拷贝和深拷贝](#11--------)
* [12、左值、右值、左值引用、右值引用、move、forward](#12-----------------move-forward)
* [13、using](#13-using)
* [14、委托构造函数](#14-------)
* [15、继承构造函数](#15-------)
* [16、统一的初始化列表](#16---------)
* [17、智能指针](#17-----)
    + [共享的智能指针](#-------)
    + [独占的智能指针](#-------)
    + [弱引用智能指针](#-------)
    + [解决shared_ptr被重复析构](#--shared-ptr-----)
    + [解决shared_ptr循环引用导致无法析构问题](#--shared-ptr------------)
* [18、POD类型(plain old data)](#18-pod---plain-old-data-)
    + [平凡类型(trivial)](#-----trivial-)
    + [标准布局类型](#------)
    + [PDO类型的作用](#pdo-----)
* [19、默认函数控制（=default与=delete）](#19---------default--delete-)
    + [=default(等于默认)](#-default------)
    + [=delete(等于删除)](#-delete------)
* [20、友元的优化](#20------)
* [21、强类型枚举](#21------)
* [22、联合体(union)](#22-----union-)
* [23、处理日期和时间的chrono库](#23---------chrono-)
    + [duration（时间段）](#duration-----)
    + [time_point（时间点）](#time-point-----)
    + [clocks](#clocks)
    + [转换函数duration_cast和time_point_cast](#----duration-cast-time-point-cast)
* [24、多线程](#24----)
    + [this_thread命名空间](#this-thread----)
    + [线程库](#---)
    + [call_once()函数](#call-once----)
    + [线程同步之互斥锁](#--------)
    + [线程同步之原子变量](#---------)
    + [线程异步](#----)

## 1、原始字面量 R"xxx()xxx"

用途：处理转义字符反斜杠问题

用法：
* 括号左右两边的xxx起注释作用，左右两边必须内容相同
* 括号内写字符串，反斜杠将不再起转移作用，而是会直接输出
* 多行内容的时候直接打回车即可，无需\n

实例：

```c++
    string s1 = R"路径(D\\ME\\TEMP\\TEMP)路径";

    string s2 = R"(111
    222
333)";

    cout << s1 << endl << s2 << endl;

/*
输出内容：
D\\ME\\TEMP\\TEMP
111
  222
333
*/
```

## 2、字符串和数值类型之间的转换

### 数值转换为字符串 to_string()

用法：
* to_string函数的参数可以是int、float、unsigned long long等任何类型的单一数字
* 函数返回该数字的string串

实例：
```c++
    string pi = "pi is " + to_string(1.0 + 2.141592);

    cout << pi << endl;

/*
输出：
pi is 3.141592
*/
```

### 字符串转换为数字

用法：
* 以下的函数共有三个参数(string, size_t* pos, base)
* 参数分别是：传入字符串，传出参数，指定进制
* 传出参数记录从第几个字符开始无法转换，如123abc，传出3
* base直接写2、8、10、16即可，若不写编译器自动解析是否带0、0x等前缀，否则为十进制
* stoi：string转int
* stol：string转long
* stoll：string转long long
* stoull：string转unsigned long long
* stof：string转float
* stold：string转long double

实例：
```c++
	size_t p;
	string s1 = "2024.05.14";
	string s2 = "10";
	string s3 = "0x1a";
	
	cout << stof(s1, &p) << endl;
	cout << p << endl; // s1的转化p位数后，无法继续转换
	cout << stoi(s2, nullptr, 2) << endl; // 转换位二进制
	cout << stoi(s3, nullptr, 0) << endl; // 自动寻找进制
/*
2024.05
7
2
26
*/
```

## 3、超长整形long long

危：OI就开始用了，五年过去，现在才知道是c++11加入的

用法：
* long = long int
* long long = signed long long = long long int = signed long long int
* unsigned long long = unsigned long long int
* 最少8个字节，64位
* 系统定义的宏：LLONG_MAX、LLONG_MIN、ULLONG_MAX分别对应最大值最小值
* 相互赋值转换的时候：范围小的类型改成范围大的，有符号的改为无符号的

拓展：
* signed char：1个字节
* short int：2个字节
* int：4个字节
* long int：32位的电脑4个字节，64位的8个字节
* long long int：最少8个字节
* 一个字节 = 8位

## 4、标准的类成员初始化

用途：在类的内部快速初始化成员变量

用法：
* 在c++98，中类内部的静态常量成员可以用=来赋值，非静态成员必须在构造函数中赋值，其他的需要在类的外部赋值
* 在c++11中，所有常变量都可以在类内部直接用=或{}初始化，或者在初始化列表中进行初始化。在编译时，先跑成员初始化，在跑构造函数。所以最后呈现出初始化列表的值

实例：
```c++
class Init {
public:
    Init(int x) :a(x){} // 初始化列表后执行，呈现最终值

    int a = 1;
    int b{ 2 };  // {}赋值
    int c = 3;   // =赋值
};

int main() {

    Init tmp(10);
    cout << tmp.a << endl << tmp.b << endl << tmp.c << endl;

    return 0;
}
}
/*
输出：
10
2
3
*/
```

## 5、final 和 override

### final

用途：拒绝子类的函数复写，或拒绝其他类的继承

用法：
* 修饰函数时：父类写了一个虚函数等待子类继承使用，如果子类继承该函数，并在函数后写上 final 关键字，那么孙子类就无法再次重写该函数，其他子类在继承父类的函数时，可正常使用
* 修饰类时：在类名后面写 final ，其他类无法再继承该类

### override

用途：保证函数是由父类继承过来的，而不是拼写错误导致新定义了一个函数

用法：在子类继承父类的函数后面，加 override 关键字

```c++
class Base {
public:
    virtual void test() {
        cout << "father\n";
    }
};

class Child1 : public Base {
public:
    void test() final { // 继承Child的类，无法在重写test
        cout << "Child1\n";
    }
};

class Child2 final: public Base { // 无法继承 Child2
public:
	void test() override { // override 检查是否继承
		cout << "Child2\n";
	}
}
```

## 6、assert 、 throw 和 noexpect

### 断言assert

用途：在程序碰到不想要的情况时，中断程序

用法：
* assert(表达式)：当表达式成立时，程序继续执行，否则中断程序。表达式可带变量，在执行程序的时候判断即可。需要头文件<cassert>
* static_assert(表达式, "自定义错误信息弹窗内容")：静态断言，必须是一个常量表达式，在编译时就能够检查。

实例：
```c++
int main() {

    int a = 0;

    assert(a > 0); // 运行时报错

    static_assert(sizeof(long) == 4, "错误，不是32位平台"); // 编译时报错，若不等于4，会弹出自定义的字符串

    return 0;
}
```

### throw 和 noexpect

用途：碰到异常值时，可以抛出异常，并结束程序

用法：
```c++
struct EE {
    EE(string s) : s(s) {}
    string s;
};

void func() throw(int, double, EE) // 定义可被抛出的异常值类型，若()内为空，则函数无法抛出异常值
{
    try
    {
        throw EE("key"); // 若注释，则直接运行return
        return;
    }
    // 若抛出异常值，则不会return，而是跳到catch
    catch (EE temp) {
        cout << temp.s << endl;
        return; // 若注释，则会输出 !
    }
    cout << "!" << endl;
}

int main() {

    func();

    // 无论如何都会输出 end
    cout << "end" << endl;

    return 0;
}
```

在c++11中， noexpect 关键字，代替了throw()，表示函数无法抛出异常，且性能更佳

## 7、自动类型推导 auto 和 decltype

用途：自动推导变量的类型

auto 用法：
```c++
int main() {

    // auto == double
    auto x = 3.14;
    // auto == char
    auto ch = 'a';
    // 语法错误
    auto nb;

    int temp = 100;
    // auto == int
    auto* a = &temp;
    // auto == int*
    auto b = &temp;
    // auto == int
    auto& c = temp;

    // 当变量不是指针或引用时，不保留const、volatile
    // 当变量是指针或引用时，保留
    int tmp = 250;
    // auto == int
    const auto a1 = tmp;
    // auto == int
    auto a2 = a1;
    // auto == int
    const auto& a3 = tmp;
    // auto == const int
    auto& a4 = a3;

/*
    auto 的限制
    1、不能作为函数参数使用 int func(auto a)
    2、类初始化中仅能用于静态成员变量 static const auto x = 1
    3、不能定义数组
    4、不能传给 template<typename T>
*/

    // auto 的应用
    map<int, string> person;
    for (auto it = person.begin(); it != person.end(); ++it)
    {
        // auto == map<int, string>::iterator
        cout << it->first << " " << it->second << endl;
    }
    for (auto& it : person)
    {
        // auto == pair<int, string>
        cout << it.first << " " << it.second << endl;
    }

    return 0;
}
```

### decltype

用途：用于推导变量的类型，与 auto 用法不同，主要在泛型编程中使用

用法：
```c++
template <class T>
class Container
{
public:
    void func(T& c)
    {
        for (m_it = c.begin(); m_it != c.end(); ++m_it)
        {
            cout << *m_it << " ";
        }
        cout << endl;
    }
private:
    // ??? m_it;  这里不能确定迭代器类型
    decltype(T().begin()) m_it;
};

template <typename T, typename U>
// 返回类型后置语句，判断类型
auto add(T t, U u) -> decltype(t + u)
{
    return t + u;
}

int func()
{
    return 1;
}

int main()
{
    int x = 10;
    // 解析 x 的类型，定义 y
    decltype(x) y = x;
    // 解析 func 函数返回的类型，定义 z
    decltype(func()) z = x;

    const list<int> lst;
    Container<const list<int>> obj;
    obj.func(lst);

    return 0;
}
```

## 8、Lambda表达式

简介：定义一个匿名函数，可以捕获一定范围内的变量，简洁方便快捷。本质是一个仿函数，可以使用function和bind来存储。

用法：
* \[capture](params) opt -> ret { body; };
	* capture 是捕获列表
 		* []：不捕捉任何变量
      		* \[&]：捕获外部所有变量，并作为引用在函数体内使用
          	* \[=]：捕获外部所有变量，并拷贝副本在函数使用，且拷贝出来的变量是只读的
      	* \[=, &x]：拷贝捕获外部所有变量，但是 x 以引用的方式使用
      	* \[&, =x]：没有这种语法
      	* \[var]：拷贝捕获变量 var ，除此以外不用其他的变量
      	* \[&var]：引用捕获变量 var ，除此以外不用其他变量
      	* \[this]：捕获 this 指针，让表达式可以访问当前类成员， & 或 = 会默认捕获 this
 	* params 是参数列表
    		* 作为函数的参数使用，由调用函数语句提供，和其他函数一样的用法 
  	* opt 是函数选项
    		* mutable：拷贝捕获变量的时候，使拷贝对象可修改，但也仅是修改拷贝的变量
    	 	* exception：指定函数破除的异常 
  	* -> ret 是返回值类型
    		* ret 写一个类型名，表示Lambda表达式的返回值类型，不写的话程序也会自动推导
  	* body 是函数体
    		* 与正常函数一样 
  	* 花括号后面记得加分号 

实例：
```c++
class Test
{
public:
    void output(int x, int y)
    {
        auto x1 = [] {return m_number; };                      // error，未捕获不能使用外部变量
        auto x2 = [=] {return m_number + x + y; };             // ok，按值捕获this，无法修改类成员
        auto x3 = [&] {return m_number + x + y; };             // ok
        auto x4 = [this] {return m_number; };                  // ok，this除了[=]外，其余情况都是引用捕获，可以修改类成员
        auto x5 = [this] {return m_number + x + y; };          // error，仅捕获了this，无法使用x、y
        auto x6 = [this, x, y] {return m_number + x + y; };    // ok，引用捕获this，按值捕获x、y
        auto x7 = [this] {return m_number++; };                // ok
    }
    int m_number = 100;
};

int main()
{
    int x = 10, y = 20;
    auto x8 = [=, &x](int z) mutable -> int // 让按值捕获的拷贝对象 y 可修改，返回 int 值
        {
            cout << ++x << " " << ++y << " " << z << endl; // 11, 21, 30
            return z;
        }(30);
    // 如果表达式需要传参，直接在花括号后面传

    cout << x << " " << y << " " << endl;   // 11, 20
    return 0;
}
```

## 9、NULL 和 nullptr 的区别

NULL：在c语言中，NULL是void\*类型。到了c++11，不允许void\*隐式的给int*等其他类型赋值，会导致旧程序报错。于是c++11将NULL宏定义成了常数0。(define NULL 0)    

nullptr：c++11中引入的新指针，是一个指针变量的空指针

## 10、constexpr

const：在旧的版本中用于修饰只读变量和常量    
constexpr：c++11中新添关键字，用于修饰常量    
建议：c++11以后，const仅用于只读变量，constexpr用于常量

## 11、浅拷贝和深拷贝

浅拷贝：基本类型(int)开辟新的空间并复制值，数组等类型复制其指针，使新旧数组值向同一个地址    
深拷贝：为数组等类型也开辟新的空间，防止修改新数组时旧数组也被修改    
在类的拷贝时，如果没写拷贝构造函数，系统默认使用浅拷贝    
可用通过自定义拷贝构造函数或重载赋值运算符实现深拷贝   

## 12、左值、右值、左值引用、右值引用、move、forward

左值：存储在内存中，可用取地址    
* int a;
* Test T; (一个类)
右值：可用提供数据值的数据，不可取地址   
* 5(一个常量)
* Test getobj() { return Test(); } (一个临时的类)
左值引用：int& a = b; 传递参数时左引用，可以节约一次拷贝时间     
右值引用：int&& a = 5; 延续将亡值的寿命，节约一次拷贝时间
万能引用：在c++11中，当类型需要推导时（如模板类或auto定义），T&& 或 auto&& 即可传入左值又可传入右值
引用折叠：针对万能引用，在传递参数的过程中可能会有的四种情况如下
* 函数变量为左值引用，传入左值引用，得到左值引用
* 函数变量为左值引用，传入右值引用，得到左值引用
* 函数变量为右值引用，传入左值引用，得到左值引用
* 函数变量为右值引用，传入右值引用，得到右值引用
* 综上，只有定义的实参和传入的实参都是右值引用时，才会得到一个右值引用    
std::move(变量)：将传递的变量转换为右值引用，赋值给其他值后，消除原先变量对资源的拥有    
std::forward(变量)：完美转发，将右值引用的变量以右值的方式转发

在上例return Test中，如果main中有 Test f = getobj()，就可以使用右值引用实现移动拷贝

```c++
// 将临时值右值引用，赋给新值
// 移动拷贝，本质上是新资源的地址指向就资源的地址，然后把旧资源的地址置空
Test(Test&& a) : m_num(a.m_num)
{
    a.m_num = nullptr;
    cout << "move construct: my name is sunny" << endl;
}

-----

// 通过T&&万能引用，既可传入左值又可传入右值
template<typename T>
void func(T&& x)   // 若这里为左值引用，则无法传递一个常数
{
    cout << x << endl;
}

int main
{
    int num = 2024;
    func(num);
    func(2024);
    return 0;
}

-----

// 使用 move 将资源移动给其他变量
int main()
{
    
    string s = "111";

    vector<string> vec;

    vec.push_back(move(s));

    cout << vec[0] << endl << s << endl;

    return 0;
}
// 输出 111\n \n

-----

// forward完美转发
template <typename T>
void func(T& x)
{
    cout << "传入一个左值" << endl;
}

template <typename T>
void func(T&& x)
{
    cout << "传入一个右值" << endl;
}

template <typename T>
void send(T&& x) 
{
    // 当传入右值100时，根据引用折叠规则，x是一个T&&类型
    // 但是x定义了名字，有地址，本身是一个左值，传递给func时，还是会触发左值
    func(x);
    // 修改为 func(forward(x)); 即可
    // forward会将x的右值特性带入func函数
}

int main()
{
    
    int x = 100;
    send(x);
    send(100);

    return 0;
}
```

## 13、using

* 使用命名空间，如 using namespace std;
* 使用父类的函数，如果父类中有多个同名的重载函数，子类中也有一个同名函数，那么子类会将父类的函数屏蔽，要调用的话需要 using 父类::函数名;
* 给类型声明别名，如 using mytype = int; mytype a = 5;
* 定义函数指针，如 using func = int(*)(int, string); func是一个函数指针类型，用 func f = mytest; 定义一个指针f指向mytest函数，调用f需传递int，string两个参数，会返回一个int类型的变量
* 给模板定义别名，如：
``` c++
template <typename T>
using MMap = map<int, T>

MMap<string> m; // 等价map<int, string> m;
```

## 14、委托构造函数

在一个构造函数的初始化列表中使用另一个构造函数，达到代码复用的效果，如：
```c++
class Test {
public:
    Test() {};
    Test(int a) 
    {
        this->a = a;
    }
    Test(int a, int b) 
    {
        this->a = a;
        this->b = b;
    }
    Test(int a, int b, int c) 
    {
        this->a = a;
        this->b = b;
        this->c = c;
    }

    int a = 0;
    int b = 0;
    int c = 0;
};

// 改为

class Test {
public:
    Test() {};
    Test(int a) 
    {
        this->a = a;
    }
    Test(int a, int b) : Test(a)
    {
        this->b = b;
    }
    Test(int a, int b, int c) : Test(a, b)
    {
        this->c = c;
    }

    int a = 0;
    int b = 0;
    int c = 0;
};

```

## 15、继承构造函数
在子类中用using直接声明父类的构造函数，简化代码    
实际上是using的用法，不光继承构造函数，其他函数也可以继承，具体看using部分介绍
```c++
class Test {
public:
    Test() {};
    Test(int a) : a(a) {}
    Test(int a, int b) : a(a), b(b) {}
    Test(int a, int b, int c) :a(a), b(b), c(c) {}

    int a = 0;
    int b = 0;
    int c = 0;
};

class test : public Test
{
public:
    test() {};
    test(int a) : Test(a){}
    test(int a, int b) : Test(a, b) {}
    test(int a, int b, int c) : Test(a, b, c) {}
};

// 改为

class Test 
{
public:
    Test() {};
    Test(int a) : a(a) {}
    Test(int a, int b) : a(a), b(b) {}
    Test(int a, int b, int c) :a(a), b(b), c(c) {}

    int a = 0;
    int b = 0;
    int c = 0;
};

class test : public Test
{
public:
    using Test::Test;
};
```

## 16、统一的初始化列表

常见的初始化方式：
* 变量：int a = 5;
* 数组：int b[] = {1, 2, 3};
* 类：Test t(10, 20);

在c++11，统一了一种初始化方式，使用{}即可，变量名和花括号中间可加=也可不加
* 变量：int a{5}; 同 int a = {5};
* 数组：int b[]{1, 2, 3};
* 类：Test t{10, 20;
* 指针：int* p = new int{50};

## 17、智能指针

共享的智能指针(shared_ptr)、独占的智能指针(unique_ptr)、弱引用的智能指针(weak_ptr)

### 共享的智能指针

性质：多个指针指向同一块内存，可以记录个数。ptr.use_count() 得到指向ptr指向的内存的指针个数  

初始化方式：  
* 构造函数初始化：shared_ptr<int> ptr1(new int(3));  // 类的构造函数，ptr1指向的内存赋值为3
* 移动构造：shared_ptr<int> ptr2 = move(ptr1);   // 将ptr1指向的内存转移给ptr2，计数不变
* 拷贝构造：shared_ptr<int> ptr3 = ptr2;  // 计数+1
* make_shared函数初始化：shared_ptr<int> ptr4 = make_shared<int>(8); // 为ptr4开一块int内存，并赋值为8
* reset重置指针：ptr5.reset();  // ptr5不指向任何内存，计数为0
* reset初始化：ptr5.reset(new int(5));  // 为ptr5新开一块int内存，赋值为5，同时ptr5放弃原有内存

用法：    
* 指针调用函数，ptr->xxx调用ptr指向的类中的函数，ptr.xxx用于共享指针类的函数
	* ptr.reset(new Test("hello");
 	* 调用Test类的函数
		* 先获取原始指针，再使用
			* ptr.reset(new Test("hello");
			* Test* t = ptr.get();
    			* t->fac();
		* 通过智能指针对象直接操作
			* ptr->fac();
      	* 调用共享指针的函数
      		* ptr.get();  // 返回ptr指向的地址
* 取值操作本质与指针一样
	* shared_ptr<int> ptr(new int(8));
	* cout << *ptr;  // 8
  	* cout << &ptr;  // ptr的物理地址
	* cout << ptr;   // ptr指向的地址
 	* cout << ptr.get();  // 通过.get函数获取ptr指向的地址
* 指定删除器函数：shared_ptr在指向数组时无法自动析构，其他类型都会自动析构
	* shared_ptr<int[]> ptr(new int[5], lamda表达式); // 自定义表达式析构
 		*  lamda: [](int* t){ delete []t; }
      	* shared_ptr<int[]> ptr(new int[5], default_delete<int []>()); // 用系统的删除器删除，<写类型> 

 ### 独占的智能指针

性质：独占一块内存的指针 

初始化方式：    
* 构造函数初始化：unique_ptr<int> ptr1(new int(9));
* 移动构造：unique_ptr<int> ptr2 = move(ptr1);
* reset构造：ptr2.reset(new int(8));
* 不能拷贝构造！

用法与共享指针相同

### 弱引用智能指针

性质：用于辅助shared_ptr，帮sp解决重复析构，循环无法析构问题

构造：
* shared_ptr<int> sp(new int); // 放一个共享的智能指针
* weak_ptr<int> wp1;  // 直接构造一个空的指针
* weak_ptr<int> wp2(wp1);  // 将一个weak_ptr拷贝赋值给一个新的weak_ptr
* weak_ptr<int> wp3(sp);   // 用一个sp构造一个wp，wp也指向sp指向的内存

用法：
* wp.use_count() : 返回wp指向的内存，被几个sp指向
* wp.expired() : 检查内存是否被析构，当引用该内存的sp数量减为0，内存会被最后一个sp析构，本函数会返回true
* wp.lock() : 得到一个指向wp内存的sp对象
* wp.reset() : 使wp不指向任何内存

### 解决shared_ptr被重复析构

```c++
struct Test{
    shared_ptr<Test> getSharedPtr() {
        return shared_ptr<Test>(this); // 返回管理着当前类的智能指针对象，通过this本质返回的是原始地址
    }
    ~Test() {
        cout << "disstruct~" << endl;
    }
};

// 不能用原始地址给多个sp赋值
// 这样做不能使用共享指针的特性，每一个指针的use_count()都是1。且指针析构时会多次将同一块地址析构，导致报错
Test* t = new Test;
shared_ptr<Test> ptr1(t);
shared_ptr<Test> ptr2(t);
cout << ptr1.use_count() << endl; // 1
cout << ptr2.use_count() << endl; // 1

// 解决方案
// 第一个赋值的指针使用原始地址，后面要用的指针应该直接用前面定义过的指针赋值
Test* t = new Test;
shared_ptr<Test> ptr1(t);
shared_ptr<Test> ptr2(ptr1);
cout << ptr1.use_count() << endl; // 2
cout << ptr2.use_count() << endl; // 2

// 当struct或class中定义了返回this指针，其他共享指针也不能使用this初始化
// 本质与上述一致，return shared_ptr<Test>(this)的本质返回的是struct的原始地址
shared_ptr<Test> sp1(new Test);
shared_ptr<Test> sp2 = sp1->getSharedPtr();
cout << sp1.use_count() << endl; // 1
cout << sp2.use_count() << endl; // 1

// 解决方案
// 使用c++定义的std::enable_shared_from_this<T>类，内部的shared_from_this()函数，通过这个函数会返回一个sp
struct Test : public enable_shared_from_this<Test>
{
    shared_ptr<Test> getSharedPtr() {
        return shared_from_this();  // 返回管理着当前类的智能指针对象，通过函数返回sp对象
    }
    ~Test() {
        cout << "disstruct~" << endl;
    }
};
```

### 解决shared_ptr循环引用导致无法析构问题

[![9db86a7b4fda0331181c9caaee96faf.jpg](https://i.postimg.cc/xCzp2pqG/9db86a7b4fda0331181c9caaee96faf.jpg)](https://postimg.cc/vgGt7zrD)

```c++
struct TA;
struct TB;
struct TA {
    shared_ptr<TB> bptr;
    ~TA() {
        cout << "TA is disstruct" << endl;
    }
};
struct TB {
    shared_ptr<TA> aptr;
    ~TB() {
        cout << "TB is disstruct" << endl;
    }
};

shared_ptr<TA> ptr1(new TA); // ptr1指向一块TA内存
shared_ptr<TB> ptr2(new TB); // ptr2指向一块TB内存
cout << ptr1.use_count() << endl; // 1
cout << ptr2.use_count() << endl; // 1

ptr1->bptr = ptr2; // ptr1指向的TA内存，内部的bptr通过ptr2构造，指向了TB内存
ptr2->aptr = ptr1; // ptr2指向的TB内存，内部的aptr通过ptr1构造，指向了TA内存
cout << ptr1.use_count() << endl; // 2
cout << ptr2.use_count() << endl; // 2
// 两个sp环形引用，两个都析构不了(即内存泄漏)

// 将其中的一个或两个改为weak_ptr，就可以沿用sp的一部分属性，同时避免内存泄漏
struct TA {
    weak_ptr<TB> bptr;
    ~TA() {
        cout << "TA is disstruct" << endl;
    }
};

shared_ptr<TA> ptr1(new TA); // ptr1指向一块TA内存
shared_ptr<TB> ptr2(new TB); // ptr2指向一块TB内存
cout << ptr1.use_count() << endl; // 1
cout << ptr2.use_count() << endl; // 1

ptr1->bptr = ptr2; // ptr1指向的TA内存，内部的wp通过ptr2构造，但是TB的sp个数不变
ptr2->aptr = ptr1; // ptr2指向TB内存，ptra通过ptr1构造，TA的sp个数变为2
cout << ptr1.use_count() << endl; // 2，ptr1指向的TA被ptr1和aptr引用
cout << ptr2.use_count() << endl; // 1，ptr2指向的TB被ptr2引用，bptr是一个weak_ptr，不增加use_count
```

## 18、POD类型(plain old data)

普通的变量都是POD类型的。   
结构体和类需要满足一些条件才能称为POD类型，分为两种：平凡类型和标准布局类型　　　　

### 平凡类型(trivial)

要求
* 拥有平凡的(默认的)构造函数和析构函数（类里面没有自定义构造和析构函数)
* 拥有平凡的(默认的)拷贝构造函数和移动构造函数（同上)
* 拥有平凡的(默认的)拷贝赋值运算符和移动赋值运算符（没有重载等号）
* 不包含虚函数和虚基类

函数判断    
模板类is_trivial<T>中的成员value，若传入的是平凡的类型，value为true，否则为false   
cout << is_trivial<Test>::value << endl;   

### 标准布局类型

要求
* 所有的非静态成员拥有相同的访问权限（成员仅有public、private、protect的其中一种）
* 类或结构体继承时要满足以下两点之一
	* 派生类中有非静态成员，基类中没有非静态成员
 	* 基类有非静态成员，而派生类中没有非静态成员
  	* 注意！如果发生多重继承，在多个基类中仅能有一个有非静态成员
* 子类中的第一个非静态成员不能是基类的类型  // struct EE {};  struct FF{ EE a; };
* 没有虚函数和虚基类
* 所有非静态成员均符合标准布局类型，其基类也符合标准布局类型

函数判断   
模板类is_standard_layout<T>同上

### PDO类型的作用

* 可以安全地使用memset和memcpy对字节赋值
* 可以与C兼容
* 保证静态初始化的安全有效

## 19、默认函数控制（=default与=delete）

c++的六大函数：
* 无参构造函数：创建类对象
* 拷贝构造函数：拷贝类对象
* 移动构造函数：拷贝类对象
* 拷贝赋值函数：类对象赋值
* 移动赋值函数：类对象赋值
* 析构函数：销毁类对象

### =default(等于默认)

如果自己定义了构造函数，默认的无参构造函数就会消失，类就不属于平凡的POD类型    
如果想要使类变成POD类型，可以用＝default将默认的构造函数手动写出

```c++
// =default设置默认函数，只能是六大函数，且没有参数和函数体
class Test {
public:
    Test() = default; // 构造
    ~Test() = default; // 析构
    Test(const Test& t) = default; // 拷贝构造
    Test(Test&& t) = default; // 移动构造
    Test& operator=(const Test& t) = default; // 拷贝操作符重载
    Test& operator=(Test&& t) = default;  // 移动操作符重载
};
// 可以写在外面
Test::Test() = default;
```

### =delete(等于删除)

可以显示的将某一个函数删除   

```c++
class Test {
public:
    void print(int num) {
        cout << num << endl;
    }
    void print(char c) = delete;
};

Test t;
t.print(100);
t.print('a'); // 如果不写delete，编译器会将'a'转换为int类型，然后输出97
// 将print(char c)删除后，系统会报错
```

## 20、友元的优化

在c++11以前，要申明友元，需要写出关键字class，c++11中不需要    
申明友元使类之间可以相互使用protected、private的成员，在一定程度上破坏了c++封装的特性，但是为了书写简洁还是使用了    

```c++
class Tom; // 提前告诉编译器有这个类，防止报错

class Jack {
    friend class Tom;  // c++98
    friend Tom;        // c++11，申明可以放在类的任何位置
private:
    string name = "Jack";
    void print() {
        cout << name << endl;
    }
    friend Lucy;
};

class Lucy {
    friend Tom;
private:
    string name = "Lucy";
    void print() {
        cout << name << endl;
    }
};

class Tom {
public:
    void print() {
        cout << j.name << " " << l.name << endl;
        j.print();
        l.print();
    }
private:
    Jack j;
    Lucy l;
};

// 将class关键字省去后，可以为模板类申明友元
template<typename T>
class Test {
    friend T;
};
// 当T传入int、char等基础变量时，编译器会直接忽视，传入class类型时会将其声明为友元
```

## 21、强类型枚举

在c++98中，枚举的类型是全局可见的，在不同的枚举中定义相同的变量会报错，并且在输出的时候会进行隐式的类型转换，这十分不符合c++封装的特点   

```c++
enum {Red, Green, Blue};  // 匿名枚举
enum Color {Red, Green, Blue}; // 定义枚举名
// 编译报错：Red、Green、Blue重复定义

// 若上述定义只出现了一个
cout << Red << Green << Blue << endl;
// 会输出012，进行了隐式的类型转换
```

在c++11中，出现了强类型枚举，在enum后写class或struct关键字指明类型   
* 强作用域：不同枚举之间的枚举量不会相互干扰
* 转换限制：不允许隐式的转换类型
* 指定类型：在枚举名后面冒号接类型，可以指定枚举的类型
* 给枚举变量赋值：枚举变量在指定了其他类型后，可以直接赋其他类型的值
* 同时拓展了普通枚举的功能：可以通过作用域访问，可以指定枚举的类型

```c++
enum class Color {Red, Green, Blue};
enum struct CC : char {Red = 'a', Green, Blue};
enum color : char {Red = 'a', Green, Blue}; // 给普通枚举拓展功能

cout << color::Red << endl;  // 0，普通枚举可以通过作用域访问
cout << sizeof(CC::Red) << endl;  // 1，枚举变量指定为char类型
cout << (int)CC::Red << endl;  // 97，必须进行显式的类型转换
cout << (char)CC::Red << endl; // a，可以指定值
```

## 22、联合体(union)

性质：联合体内的成员共用一块内存，在一些场景下，可以节约内存

例子：某一个类需要存储人员信息，人员分为三类：学生，老师，校长，学生要记录学号，老师要记录电话，校长要记录姓名和身份证号。如果直接定义成员变量，那么学生就会为电话、姓名、身份证号开辟内存，但是学生并不需要记录这些，于是可以使用联合体来节约内存。

```c++
struct Principal {
    Principal(string n, string c) : name(n), cardId(c) {}
    string name;
    string cardId;
};

class PersonInfo {
public:
    enum class Category : char { Student = 'S', Teacher = 'T', Principal = 'P' }; // 强枚举类型

    PersonInfo(int num) : number(num), type(Category::Student) {}
    PersonInfo(string str) : phone(str), type(Category::Teacher) {}
    PersonInfo(string n, string c) : principal(n, c), type(Category::Principal) {} // principal的初始化会调用结构体的构造函数
    ~PersonInfo() {} // 析构函数被删除，需手动写出
    void print() {
        cout << "Person category: " << (char)type;
        switch (type)
        {
        case Category::Student:
            cout << "Student number" << number << endl;
            break;
        case Category::Teacher:
            cout << "Teacher phone" << phone << endl;
            break;
        case Category::Principal:
            cout << "Principal name" << principal.name << endl
                << "Principal cardId" << principal.cardId << endl;
            break;
        default:
            break;
        }
    }
private:
    Category type;
    union // union里面的变量共用同一块内存，每个人仅可能是学生、老师、校长的一种，其他变量就不必占用内存
    {
        int number;
        string phone;
        Principal principal; // 校长需要存储两个信息，用结构体放在一块内存中
    };
};

PersonInfo p1(10042);
PersonInfo p2("16622228888");
PersonInfo p3("Jack", "11001010");
    
p1.print();
p2.print();
p3.print();

/*
Person category: S
Student number: 10042
Person category: T
Teacher phone: 16622228888
Person category: P
Principal name: Jack
Principal cardId: 11001010
*/

```

## 23、处理日期和时间的chrono库

头文件：<chronon>   
命名空间：using namespace chronon;   
包含模板类：时间间隔duration、时钟clocks、时间点time_point

### duration（时间段）

定义一个时间段 duration<T, std::ratio<int, int>>   
T传入参数，可以是整数也可以是浮点数，表示周期个数。
ratio<分子，分母> 表示需要的1sec个数，如定义1min，需要<60,1>个1sec，定义1毫秒需要<1, 1000>个1sec

```c++
#include <chrono>
using namespace chrono;

// hour
duration<int, ratio<3600>> hour(1); // ratio里面的单位是秒，指定3600秒为1h
chrono::hours h(1); // 命名空间中可以直接使用hours
// min
duration<int, ratio<60>> min(1); // min(1)中的 1 作为周期参数给int，60个sec表示1个min周期
chrono::minutes m(1);
// sec
duration<int, ratio<1>> sec(1); // ration<1>可省略
duration<int> sec(1);
chrono::seconds s(1);
// millisec毫秒
duration<int, ratio<1, 1000>> millisec(1); // ration<分子，分母>, 1/1000秒为一毫秒
chrono::microseconds ms(1);
// microsec微秒
// nanosec纳秒
// picosec皮秒

// duration允许运算
chrono::minutes m1(9);
chrono::minutes m2(8);
m1++; // +-操作的是时钟周期
m2--;
chrono::minutes m3 = m1 - m2; // 有3个min

duration<int, ratio<60, 1>> t1(10); // 10个60s
duration<int, ratio<1, 1>> t2(60);  // 60个1s
auto t3 = t1 - t2; // 对于不同的ratio，会先统一单位，对分子取最大公约数，分母取最小公倍数
// t3 为duration<int, ratio<1, 1>> 
cout << t3.count() << endl;  // .count函数输出周期个数，t3单位为1s，计算出周期数为540
```

### time_point（时间点）

* time_since_epoch(); // 从 1970年1月1日8:00:00 到此时的时间段
* 想要得到一个时间点对象，通常需要配合clocks使用

### clocks

* system_clock：系统的时钟，在代码运行的过程中，如果修改了系统时钟，那么就无法精准的得到想要的时间段   
* steady_clock：固定的时钟，相当于秒表，可以进行耗时统计   
* high_resolution_clock：steady_clock的别名

```c++
// system_clock的用法
system_clock::time_point epoch; // 默认新纪元时间1970.1.1.8：00：00

chrono::hours h(10 * 24); // 10个24小时
system_clock::time_point epoch1 = epoch + h; // 新纪元的10天之后
system_clock::time_point epoch2(epoch + h); // 同上一行

system_clock::time_point nowTime = system_clock::now(); // now接口提供当前的时间
time_t allSec = system_clock::to_time_t(nowTime); // now提供的格式看不懂，需要转换
cout << "当前的时间：" << ctime(&allSec) << endl;  // ctime(time_t) 可以输出time_t格式的时间

// steady_clock的用法，该类仅有一个接口now，返回秒表的即时
void steadyClockTest() {
    auto p1 = steady_clock::now();
    for (int i = 0; i < 1000; ++i) {
        cout << "*";
    }
    auto p2 = steady_clock::now();
    auto d1 = p2 - p1; // auto将d1定义成了纳秒
    cout << d1.count() << endl; 
}
```

### 转换函数duration_cast和time_point_cast

duration<周期个数, ratio<分子，分母>> t1, t2;   
当t2想要赋值给t1，需要满足：
* 周期个数不能丢失精度，如double->int
* 周期长度，原周期能够整除目标周期，如ratio<1, 100>->ratio<1, 1000>   
若不满足条件，则需要使用转换函数，如 seconds s = duration_cast<seconds>(duration<double> (2.5));   

time_point_cast条件和用法类似


## 24、多线程

一个未启动的exe仅在硬盘上占一块空间   
一个启动的exe不占硬盘内存，占用内存和cpu资源，称之为进程   
进程在内存中对应一块虚拟地址空间，程序中的所有数据被加载到虚拟地址空间中，通过cpu的内存管理单元映射到物理物理内存的其他位置，数据映射是为了保护数据的安全。在程序中打印变量的地址，显示的都是虚拟地址   
进程对应的虚拟地址空间分为内核区和用户区。内核区的数据是给操作系统使用的，程序员无法操作。用户区有全局数据区、堆区、栈区、代码区   
如果一个进程不创建新的进程或线程，则称为单进程或单线程，独享虚拟地址空间   
如果进程创建了新的线程，该虚拟地址空间就会维护两个线程，进程退化为主线程，新的线程为子线程。主线程管理着所有子线程，当主线程结束，虚拟地址空间就释放了   
如果进程创建了新的进程，就会新开辟一块虚拟地址空间给新的进程   
并行：多核cpu同时运行多个进程   
并发：多个线程交替运行      

### this_thread命名空间

接口：
* get_id()：得到当前线程的id
* sleep_for(duration)：阻塞线程一段时间
	* 创建态：刚创建出来的状态，很快就会结束
 	* 就绪态：一直抢cpu，抢到cpu时间片就转为运行态
  	* 运行态：失去cpu，但没有运行完毕，转为就绪态
  	* 阻塞态(挂起态)：手动阻塞线程的状态，阻塞结束变成就绪态
  	* 退出态(终止态)：线程运行结束
* sleep_until(time_point)：阻塞线程到某一个时间点
* yield()：让线程主动放弃cpu资源，避免一个线程长期占用cpu资源。线程放弃cpu后，会继续抢cpu

### 线程库

从c++11开始，c++终于有了自己的线程库，此前要多线程一直在使用C语言的接口

用法：
* 头文件：<thread>
* 构造函数
	* thread t;  // 创建一个空的线程 
	* thread t(func, arg1, arg2...);  // 创建一个线程，并指定回调函数，可以没有参数，func可以写lamda表达式
 	* thread&& t1 = move(t); // 移动构造，将t线程的资源转移给t1
  	* thread t(&Test::func, arg...); // 给线程的回调函数指定类的函数成员，此处func为静态成员
  	* Test tt; thread t(&Test::func1, &tt, arg...); // 传递非静态成员函数时，第二个参数要指定类对象
* get_id()，取线程id函数
	* t.get_id();
 	* 子线程可以用thread的函数获取id，也可以用this_thread::get_id()。但主线程只能用后者
* join()，阻塞线程函数
	* t.join();
 	* 在主线程中，调用t.join()，实际上是阻塞主线程，让子线程有充足的时间运行
* detach()，分离线程函数
	* t.detach()
 	* 将t和主线程取消关联，但仍然在同一虚拟内存地址上。唯一的作用是，线程释放资源时，不再需要消耗主线程性能
* hardware_concurrency()，获取cpu核心数
	* int num = thread::hardware_concurrency();
 	* 获取机器的cpu核心数

### call_once()函数

某一个类中有一个静态成员，多个线程可能会对其进行初始化，但我们想让静态成员仅被初始化一次，就可以使用该函数  

once_flag g_flag;  // 函数的第一个参数，需要设置在所有线程都可以访问到的地方   
call_once(g_flag, func, arg...);  // func是call_once执行的函数，arg是给func传递的参数   
// call_once会保证func函数仅被调用一次

### 线程同步之互斥锁

#include <mutex> 头文件

* 独占的互斥锁
 	* mutex mx;
  	* mx.lock(); // 上锁
  	* mx.try_lock(); // 尝试上锁，如果上锁失败就先干别的事情
  	* mx.unlock(); // 解锁
* 递归的互斥锁，可以多次加锁
	* recursive_mutex mx;
 	* lock();
  	* try_lock();
  	* unlock();
* 超时独占互斥锁，当线程被阻塞到一定时间时，解除阻塞
	* timed_mutex mx;
 	* lock()、try_lock()、unlock()
  	* bool try_lock_for(chrono::seconds(1))  // 线程加锁成功则返回true，如果加锁失败，线程会阻塞一段时间，如果仍然没加锁，则返回false
  	* bool try_lock_until(时间点) // 同上，线程会阻塞到一个时间点
* 递归的超时互斥锁，将递归的互斥锁和超时独占互斥锁融合
	* recursive_timed_mutex mx;
 	* lock()、try_lock()、unlock()
  	* try_lock_for()、try_lock_until()

* lock_guard<mutex> guard(mx);
	* 给lock_gaurd模板类指定锁的对象，然后创建一个守卫锁
 	* 当锁的生命周期结束，会自动解锁
  	* 可以解决忘记unlock带来的死锁问题

### 线程同步之原子变量

原子指的是一系列不可被CPU上下文交换的机器指令，这些指令组合在一起就形成了原子操作。在多核CPU下，当某个CPU开始运行原子操作时，会暂停其他CPU内核对内存的操作，以保证不会发生数据混乱   

基本用法：   
* 构造与初始化
	* atomic<T> x;  // 模板类，支持int、long、longlong、指针等，不支持浮点数和复合类型(class, struct)
	* atomic<int> a(8);  // 定义一个int型原子变量，初始化为8
	* atomic_int a(8);  // 同上，特化函数，原型是上述的构造函数
* 赋值操作
	* a = 10;  // 直接赋值
 	* a.store(10); // 用shore函数赋值
  	* int tmp = a.exchange(10); // 用exchange函数赋值，会返回上一个值
* 取值操作
	* cout << a << endl; // 直接取值
  	* cout << a.load() << endl; // load函数取值
* 不能直接使用struct、class复合类型，但是可以存储其指针
	* Base base(123, "ace");
 	* atomic<Base*> atc_base(&base);
  	* Base* tmp = atc_base.load();
  	* cout << "name: " << tmp->name << endl;

线程同步：   
使用互斥锁实现线程同步的时候，将可能同时访问的共享数据进行加锁操作，以此保护共享数据   
使用原子变量，直接将共享数据设置为原子变量即可，但是复合类型的数据不能使用atomic

### 线程异步

一个线程在等待另一个线程的结果时，呈阻塞态，就是线程同步   
一个线程在等待另一个线程的结果时，去干其他的事情，就叫线程异步   

future模板类：在主线程中得到某个子线程任务函数返回的结果   
* #include <future>  // 头文件
* future<string> f;  // 构造，有无参构造和移动构造，不支持赋值
* f.get();  // 得到子线程返回的值，如果子线程还没有返回，则会阻塞当前线程

promise模板类：将数据和future对象绑定，方便操作。内部有一个future对象   
```c++
void func(promise<string>& p) {  // 传入一个promise对象
    this_thread::sleep_for(chrono::seconds(3));
    p.set_value("111");  // 立即给p赋值
    p.set_value_at_thread_exit("111");  // 等线程结束的时候，再给p赋值
    this_thread::sleep_for(chrono::seconds(1));
}

promise<string> pro; // 定义一个promise对象pro，内部含有一个future，数据类型为string
thread t1(func, ref(pro));  // 用ref函数，将pro传递给子线程。子线程给pro赋值就会传回来
future<string> f = pro.get_future();  // 取出pro中的future
string str = f.get();  // get得到子线程赋的值，如果子线程没有赋值，则阻塞
cout << str << endl; // 输出
t1.join();
```
