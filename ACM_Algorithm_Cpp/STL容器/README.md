# STL容器

## vector

```c++
// 定义
vector<int> v;  // 定义一个vector
vector<int> v[N];  // 定义一个vector数组，其中有N个vector
vector<int> v(len);  // 定义一个长度为len的vector
vector<int> v(len, x);  // 定义一个长度为len的vector，其中每个元素的初始值为x
vector<int> v = {1, 2, 3};  // 初始化vector，v:{1, 2, 3}
vector<int>::iterator it = v.begin();  // 定义一个迭代器
// 函数
v.push_back(4);  // 在v的尾部插入元素4
v.pop_back();  // 删除v的最后一个元素
lower_bound(v.begin(), v.end(), 2);  // 返回第一个大于等于2的元素的迭代器，若没有返回v.end();
upper_bound(v.begin(), v.end(), 2);  // 返回第一个大于2的元素的迭代器
v.size();  // 返回v中元素的个数
v.empty();  // 若v为空返回true
v.begin();  // 返回v中的第一个元素
v.front();  // 返回v中的第一个元素的迭代器
v.end();  // 返回v中最后一个元素后一个位置的迭代器
v.back();  // 返回v中的最后一个元素
v.clear();  // 清空v，包括开辟的空间
v.erase(v.begin()); // 删除迭代器指向的元素
v.erase(v.begin(), v.begin() + 2);  // 删除区间[v.begin(), v.begin() + 2)的所有元素
v.insert(v.begin(), 1);  // 在迭代器前面插入元素，并返回该元素的迭代器
// 遍历
// 下标遍历
for (int i = 0; i < v.size(); ++i)
    cout << i << " " << v[i] << " ";
// 迭代器遍历
for (auto it = v.begin(); it != v.end(); ++it)
    cout << it - v.begin() << " " << *it << " ";
// auto遍历，auto使用的是拷贝，需要修改值时写 for (auto& x:v)，并且auto遍历不提供索引，需要的话自己写一个
for (auto x : v)
    cout << x << " ";
```

## stack

```c++
// 定义
stack<int> stk;  // 定义一个stack
stack<int> stk[N];  // 定义一个stack数组，其中有N个stack
// 函数
stk.push(x);  // 在stack中插入元素x
stk.pop();  // 弹出栈顶元素
stk.top();  // 返回stack栈顶元素
stk.size();  // 返回stack中元素的个数
stk.empty();  // 若stk为空返回true
```

## string

```c++
// 定义
string str;  // 定义一个空的字符串
string str[N];  // 定义一个string数组
string str(5, 'a');  // 使用5个'a'初始化字符串
string str("abc");  // 使用字符串初始化
// 函数
str.push_back('d');  // 在str尾部插入一个字符
str.pop_back();  // 删除str尾部的字符
str.length();  // 返回str中的字符个数
str.size();  // 同length
str.empty();  // 如果str为空返回true
str.substr(1);  // 返回string中从下标为1开始至末尾的子串
str.substr(0, 2);  // 返回string中从下标为0开始长度为2的子串
str.insert(1, "x");  // 在下标为1的字符前插入字符串"x"
str.erase(1, 4);  // 删除从位置1开始的4个字符
str.erase(str.begin() + 1);  // 删除迭代器指向的元素
str.erase(str.begin(), str.end() - 2);  // 删除[str.begin(), str.end() - 2)的所有元素
str.find("b");  // 返回字符串"b"在str中第一次出现的位置
str.find("b", 2);  // 返回从位置2开始字符"b"在str中第一次出现的位置
str.rfind("b");  // 从右边开始查找，返回的仍是相对左边的位置
str.rfind("b", 2);  // 从下标为2的位置开始往左边找，找到第一个"b"，返回位置
```

## queue/priority_queue

```c++
// 定义
queue<int> que;  // 定义一个queue
queue<int> que[N];  // 定义一个queue数组
priority_queue<int> bigque;  // 定义一个大根堆，队头是最大的元素
priority_queue<int, vector<int>, greater<int> > smallque;  // 定义一个小根堆
// 函数
que.push(x);  // 在que队尾插入元素x
que.pop();  // 将que的队头弹出
que.front();  // 返回que的队头元素
que.back();  // 返回que的队尾元素
que.size();  // 返回que的元素个数
que.empty();  // 如果que为空返回true
bigque.top();  // 返回大根堆的队头元素
```

## deque

```c++
// 定义
deque<int> deq;  // 定义一个deque
deque<int> deq[N];  // 定义一个deque数组
deque<int> deq(len);  // 定义一个长度为len的deque
deque<int> deq(len, x);  // 定义一个长度为len的deque，初始化每个元素为x
deque<int> deq2(deq1);  // 用deq1给deq2赋值
deque<int> deq2(deq1.begin(), deq1.begin() + 3);  // 将deq1中的0~2元素赋值给deq2
deque<int> deq = {1, 2, 3};  // 初始化deque，并赋值
deque<int>::iterator it = deq.begin();  // 定义一个迭代器
// 函数
deq.push_back(4);  // 在deque尾部插入元素4
deq.pop_back();  // 删除deque的尾部元素
deq.push_front(4);  // 在deque队头插入元素
deq.pop_front();  // 删除队头元素
deq.size();
deq.empty();
deq.front();  // 得到队头元素
deq.begin();  // 得到队友迭代器
deq.back();
deq.end();
deq.clear();
deq.erase(deq.begin());  // 删除迭代器指向的元素
deq.erase(deq.begin(), deq.begin() + 2);  // 删除[deq.begin(), v.begin() + 2)区间的所有元素
deq.insert(deq.begin(), 1);  // 在迭代器指向的位置前面插入一个元素，并返回该元素的迭代器
// 遍历
// 用下标遍历
for (int i = 0; i < deq.size(); ++i)
    cout << i << " " << deq[i] << " ";
// 用迭代器遍历
for (auto it = deq.begin(); it != deq.end(); ++it)
    cout << it - deq.begin() << " " << *it << " ";
// 用auto遍历，无法得到下标
for (auto x : deq)
    cout << x << " ";
```

## map/multimap

底层用红黑树实现，会按照键值排序，插入查询为O(logN)    
multimap允许一个键有多个值，其他内容与map完全相同

```c++
// 定义
map<string, int> mp;  // 定义一个map。将string映射成int
map<string, int> mp[N];  // 定义一个map数组
multimap<string, int> mulmp;  // 定义一个multimap
// 函数
mp["abc"] = 3;  // 将"abc"映射到3
mp["abc"]++;  // 将"abc"映射的整数加一
mp.insert(make_pair("cd", 2));  // 插入一个键值对
mp.insert({"cd", 2});  // 插入一个键值对
mp.size();
mp.empty();
mp.clear();
mp.erase("cd");  // 清除键为 "cd" 的键值对
mp["ab"];  // 返回"ab"的映射值
mp.begin();
mp.end();
mp.find("ad");  // 返回第一个键值为"ad"的迭代器，不存在返回end
mp.count("abc");  // 返回键值为"abc"的数量
mp.lower_bound("abc");  // 返回第一个键值大于等于"abc"的元素的迭代器
mp.upper_bound("abc");  // 返回第一个键值大于"abc"的元素的迭代器
// 遍历
// 使用迭代器遍历
for (auto it = mp.begin(); it != mp.end(); ++it)
    cout << (*it).first << " " << (*it).second << endl;
// 使用auto遍历
for (auto x : mp)
    cout << x.first << " " << x.second << endl;
// 使用扩展推断范围的遍历(c++17)
for (auto &[k, v] : mp)
    cout << k << " " << v << endl;
```

## set/multiset

底层用的是红黑树。个人觉得set的功能完全被map包含，懒得写了

## unordered_map/unordered_set

unordered内部采用哈希表结构，插入的元素是无序的。增删查改均为O(1)的复杂度。因此也不支持lower_bound等函数   
同时set仍然被map包含
```c++
// 定义
unordered_map<int, int> mp;
unordered_map<int, int> mp[N];
// 函数
mp.insert(make_pair(1, 2));
mp.insert({3, 4});
mp.size();
mp.empty();
mp.erase(3);
mp.find(1);
mp.count(1);
mp.begin();
mp.end();
mp.clear();
// 遍历
for (auto it = mp.begin(); it != mp.end(); ++it)
    cout << (*it).first << " " << (*it).second << endl;
for (auto x : mp)
    cout << x.first << " " << x.second << endl;
for (auto &[k, v] : mp)
    cout << k << " " << v << endl;
```
