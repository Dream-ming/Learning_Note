#include <iostream>
#include <fstream>
#include <json/json.h>

using namespace std;
using namespace Json;

/*
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
*/

void writeJson()
{
    Value root;
    root.append("luffy");
    root.append(19);
    root.append(170);
    root.append(false);

    Value subArray;
    subArray.append("ace");
    subArray.append("sabo");
    root.append(subArray);

    Value obj;
    obj["sex"] = "man";
    obj["girlfriend"] = "Hancock";
    root.append(obj);

#if 1
    string json = root.toStyledString();  // 格式化数据
#else 
    FastWriter w;
    string json = w.write(root); // 得到无换行符的数据
#endif
    // write -> ofstream
    // read -> ifstream
    ofstream ofs("test.json");
    ofs << json;
    ofs.close();
}

void readJson()
{
    ifstream ifs("test.json");
    Reader rd;
    Value root;
    rd.parse(ifs, root);

    if (root.isArray())
    {
        for (int i = 0; i < root.size(); ++i)
        {
            Value item = root[i];
            if (item.isInt())
            {
                cout << item.asInt() << ", ";
            }
            else if (item.isString())
            {
                cout << item.asString() << ", ";
            }
            else if (item.isBool())
            {
                cout << item.asBool() << ", ";
            }
            else if (item.isArray())
            {
                for (int j = 0; j < item.size(); ++j)
                {
                    cout << item[j].asString() << ", ";
                }
            }
            else if (item.isObject())
            {
                Value::Members keys = item.getMemberNames();
                for (int k = 0; k < keys.size(); ++k)
                {
                    cout << keys.at(k) << ":" << item[keys[k]] << ", ";
                }
            }
            cout << endl;
        }
    }
    else if (root.isObject())
    {

    }
}

int main()
{
    writeJson();
    readJson();
}
