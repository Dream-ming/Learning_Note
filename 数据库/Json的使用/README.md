# Json的使用

## 配置Json环境

1、在 [Jsoncpp开源库](https://github.com/open-source-parsers/jsoncpp) 中下载源码

2、用cmake把jsoncpp配置一遍，得到一个VS的项目 jsoncpp.sln

3、在VS中，把jsoncpp_lib编译，得到 jsoncpp.lib 和 jsoncpp.dll 文件，拿去用即可

4、在VS中用Json的时候，要在项目的属性里，添加 include 和 lib

## Json的用法

### 头文件

\# include <json/json.h>   
using namespace Json;

在VS里配置属性的时候，只添加到 include 这一层，所以头文件这里要额外添加一层文件名

### Value 类

Json可以看作一个包装器，包装支持的类型   
* null: 空值
* int: 有符号整数
* uint: 无符号整数
* real: 浮点数
* string: utf8格式的字符串
* bool: 布尔数
* array: 数组，即Json串中的 []
* object: 键值对，即Json串中的 {}，内部是一个map

Json文件举例，本身是一个大的数组，里面有各种类型的成员
```json
[
    "luffy", 
    19, 
    170, 
    false,
    ["ace", "sabo"],
    {
        "sex":"man", 
        "girlfriend":"Hancock"
    }
]
```

检测保存数据的类型函数
* bool isNull() const;
* bool isBool() const;
* bool isInt() const;
* bool isInt64() const;
* bool isUInt() const;
* bool isUInt64() const;
* bool isIntegral() const;  // 是否为整数
* bool isDouble() const;
* bool isNumeric() const;   // 是否为数值类型
* bool isString() const;
* bool isArray() const;     // 是否为数组
* bool isObject() const;    // 是否为键值对

将 Value 对象转换为实际类型
* Int asInt() const;
* UInt asUInt() const;
* Int64 asInt64() const;
* UInt64 asUInt64() const;
* LargestInt asLargestInt() const;     // 转换为 int64
* LargestUInt asLargestUInt() const;   // 转换为 uint64
* JSONCPP_STRING asString() const;     // 转换为 string
* const char* asCString() const;       // 转换为 const char*
* float asFloat() const;
* double asDouble() const;
* bool asBool() const;

对 Json 数组的操作
* 可以通过下标操作
* item.size(); // 取数组元素个数，for循环遍历下标即可使用
* item.append();
* 
