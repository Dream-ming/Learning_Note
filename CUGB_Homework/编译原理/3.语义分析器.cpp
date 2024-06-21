#include <cctype>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

char s[100][100] = {"\0"}; // 用来存储初始数据
string str;                // 用来存储整合后的数据
int location = 0;          // 用来定位算术表达式
bool flag = true;          // 用来判断该算术表达式是否合法
string tree_map[100];      // 用来存储语法树
const int width = 3;       // 设置间隔为3
char token[100] = {"\0"};  // 用来暂存单词
string error;              // 用来记录错误信息

string tetrad[100]; // 用来存储四元式
int tetradNum = 0;  // 记录四元式的个数

struct IDs {
    string name = ""; // 标识符的名字
    int type; // 因为文法定义中，数据类型只有int，所以type在这里只有两种取值，1表示为int类型，0表示无类型
    int value; // 标识符的值
    bool rel;  // 用于bool值的存储
};
IDs ID[100];   // 用来存储标识符的类型和值
int IDNum = 0; // 用于记录标识符的数量

struct LInfo {
    int row; // 用来标识语法树的行列
    int column;
    int interval = 0; // 两棵子树之间需要保持的间隔
    int addr; // 用来指向第addr个四元式，便于条件语句和循环语句中确定跳转的地址
    IDs id;
}; // 用来存储L属性文法需要传递的信息以及画语法树需要的信息

bool isKey(char *s);
bool isOP(char *s);
bool isDE(char &s);
void pre_process(char *buff, int &in_comment);
bool scanner(int k);

char keywords[34][20] = { // 关键字,包括main在内共有34个
    "auto",   "short",    "int",    "long",     "float", "double",   "char",
    "struct", "union",    "enum",   "typedef",  "const", "unsigned", "signed",
    "extern", "register", "static", "volatile", "void",  "if",       "else",
    "switch", "case",     "for",    "do",       "while", "goto",     "continue",
    "break",  "default",  "sizeof", "return",   "main",  "include"};
char operators[38][10] = { // 运算符,共38个
    "+",  "-",  "*",   "/",   "%",  "++", "--", "==", "!=", ">",
    ">=", "<",  "<=",  "&&",  "||", "!",  "=",  "+=", "-=", "*=",
    "/=", "%=", "<<=", ">>=", "&=", "^=", "|=", "&",  "|",  "^",
    "~",  "<<", ">>",  "?",   ":",  ",",  ".",  "->"};
char delimiters[7] = {'(', ')', '[', ']', '{', '}', ';'}; // 分隔符,共7个

bool isKey(char *s) { // 用来判断字符串是否为关键字,是则返回true,否则返回false
    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strcmp(s, keywords[i]) == 0)
            return true;
    }
    return false;
}

bool isOP(char *s) { // 用来判断字符串是否为运算符,是则返回true,否则返回false
    for (int i = 0; i < sizeof(operators) / sizeof(operators[0]); i++) {
        if (strcmp(s, operators[i]) == 0)
            return true;
    }
    return false;
}

bool isDE(char &s) { // 用来判断字符是否为分隔符,是则返回true,否则返回false
    if (strchr(delimiters, s) != NULL)
        return true;
    return false;
}

void pre_process(char *buff, int &in_comment) { // 预处理
    char data[100] = {'\0'};   // 用来存储处理过的数据
    char old_c = '\0';         // 用来存储上一个字符
    char cur_c;                // 用来存储当前字符
    int i = 0;                 // 计数器,记录buff
    int j = 0;                 // 计数器，记录data
    while (i < strlen(buff)) { // 去注释
        cur_c = buff[i++];     // 首先将获取的字符存入缓存中
        switch (in_comment) {
        case 0:
            if (cur_c == '\"') { // 进入双引号中
                data[j++] = cur_c;
                in_comment = 3;
            } else if (cur_c == '\'') { // 进入单引号中
                data[j++] = cur_c;
                in_comment = 4;
            } else if (old_c == '/' && cur_c == '*') { // 进入多行注释中
                j--;
                in_comment = 1;
            } else if (old_c == '/' && cur_c == '/') { // 进入单行注释中
                j--;
                in_comment = 2;
            } else { // 其他情况则直接将数据写入data中
                data[j++] = cur_c;
            }
            break;
        case 1:
            if (old_c == '*' && cur_c == '/')
                in_comment = 0; // 多行注释结束
            break;
        case 2:
            if (i == strlen(buff))
                in_comment = 0; // 单行注释到这行结束时标志位置为0
            break;
        case 3:
            data[j++] = cur_c;
            if (cur_c == '\"')
                in_comment = 0;
            break;
        case 4:
            data[j++] = cur_c;
            if (cur_c == '\'')
                in_comment = 0;
            break;
        }
        old_c = cur_c; // 保留上一个字符
    }

    i = 0;
    int k = 0;
    while (k < j) {                               // 分隔词
        if (isalpha(data[k]) || data[k] == '_') { // 若为字母或_
            while (!isDE(data[k]) &&
                   strchr("+-*/%=^~&|!><?:,.", data[k]) == NULL &&
                   !isspace(data[k])) {
                buff[i++] = data[k++];
            }
            buff[i++] = ' ';
        } else if (isdigit(data[k])) { // 若为数字
            while (isdigit(data[k])) {
                buff[i++] = data[k++];
            }
            buff[i++] = ' ';
        } else if (isspace(data[k])) {
            while (isspace(data[k])) { // 若为空白字符
                k++;
            }
        } else if (isDE(data[k])) { // 若为界符
            buff[i++] = data[k++];
            buff[i++] = ' ';
        } else if (data[k] == '\"') { // 若为双引号
            buff[i++] = data[k++];
            while (data[k] != '\"')
                buff[i++] = data[k++];
            buff[i++] = data[k++];
            buff[i++] = ' ';
        } else if (data[k] == '\'') { // 若为单引号
            buff[i++] = data[k++];
            while (data[k] != '\'')
                buff[i++] = data[k++];
            buff[i++] = data[k++];
            buff[i++] = ' ';
        } else if (
            strchr("+-*/%=^~&|!><?:,.", data[k]) !=
            NULL) { // 若为运算符,再查看下一个字符,要尽可能多包含一些运算符
            switch (data[k]) {
            case '+':
                buff[i++] = data[k++];
                if (data[k] == '+' || data[k] == '=')
                    buff[i++] = data[k++]; // 为++或+=运算符
                break;
            case '-':
                buff[i++] = data[k++];
                if (data[k] == '-' || data[k] == '=' || data[k] == '>')
                    buff[i++] = data[k++]; // 为--或-=或->运算符
                break;
            case '*':
                buff[i++] = data[k++];
                if (data[k] == '=')
                    buff[i++] = data[k++]; // 为*=运算符
                break;
            case '/':
                buff[i++] = data[k++];
                if (data[k] == '=')
                    buff[i++] = data[k++]; // 为/=运算符
                break;
            case '%':
                buff[i++] = data[k++];
                if (data[k] == '=')
                    buff[i++] = data[k++]; // 为%=运算符
                break;
            case '=':
                buff[i++] = data[k++];
                if (data[k] == '=')
                    buff[i++] = data[k++]; // 为==运算符
                break;
            case '^':
                buff[i++] = data[k++];
                if (data[k] == '=')
                    buff[i++] = data[k++]; // 为^=运算符
                break;
            case '&':
                buff[i++] = data[k++];
                if (data[k] == '&' || data[k] == '=')
                    buff[i++] = data[k++]; // 为&&或&=运算符
                break;
            case '|':
                buff[i++] = data[k++];
                if (data[k] == '|' || data[k] == '=')
                    buff[i++] = data[k++]; // 为||或|=运算符
                break;
            case '!':
                buff[i++] = data[k++];
                if (data[k] == '=')
                    buff[i++] = data[k++]; // 为!=运算符
                break;
            case '>':
                buff[i++] = data[k++];
                if (data[k] == '=')
                    buff[i++] = data[k++]; // 为>=运算符
                else if (data[k] == '>') {
                    buff[i++] = data[k++]; // 为>>运算符
                    if (data[k] == '=')
                        buff[i++] = data[k++]; // 为>>=运算符
                }
                break;
            case '<':
                buff[i++] = data[k++];
                if (data[k] == '=')
                    buff[i++] = data[k++]; // 为<=运算符
                else if (data[k] == '<') {
                    buff[i++] = data[k++]; // 为<<运算符
                    if (data[k] == '<')
                        buff[i++] = data[k++]; // 为<<=运算符
                }
                break;
            default:
                buff[i++] = data[k++];
            }
            buff[i++] = ' ';
        }
    }
    buff[i] = '\0'; // 处理完以后,会在最后留上一个空格
}

bool scanner(int k) { // 词法分析处理
    int in_comment =
        0; // 0表示没问题,1表示在多行注释中,2表示在单行注释中,3表示在双引号中,4表示在单引号中
    for (int i = 0; i < k; i++) {
        pre_process(
            s[i],
            in_comment); // 首先预处理,去掉注释,词与词之间、词与运算符之间用一个空格隔开
    }
    if (in_comment != 0)
        return false; // 若标志位不等于0,则说明多行注释不到位,没有结束标志
    else
        return true;
}

int draw_line(int row, int num);
void string_out(string s, int row, int column, int loc);
int tree_out(string s, int row, int loc);
void printTree(ofstream &fout);
int readToken();
void bindString(int k);
int findID(const string &words);
void printSequence(ofstream &fout);
LInfo Block(LInfo info);
LInfo Decls(LInfo info);
LInfo NameList(LInfo info);
LInfo NameList1(LInfo info);
bool Type(char *words);
bool Name(char *words);
LInfo STMTS(LInfo info);
LInfo STMT(LInfo info);
LInfo STMT1(LInfo info);
LInfo BOOL(LInfo info);
bool RelOp(char *words);
LInfo Expr(LInfo info);
LInfo Expr1(LInfo info);
LInfo Term(LInfo info);
LInfo Term1(LInfo info);
LInfo Factor(LInfo info);
bool AddOp(char *words);
bool MulOp(char *words);
int getData();

int draw_line(int row,
              int num) { // 用来画横线,隔开兄弟节点,返回下次开始的起始位置
    tree_map[row].append(num, '-');
    return tree_map[row].size();
}

/**用来输出字符串
 * 其中column为该行的起始位置,loc为上一行竖线的位置,
 * loc默认为0,表示没有竖线,则此时通过column将该字符串放入到相应位置
 * 若不为0,则通过loc对该字符串进行位置的处理
 */
void string_out(string s, int row, int column, int loc = 0) {
    if (loc == 0) {
        if (tree_map[row].size() < column) { // 若不等,则说明中间需要填充空格
            int n = column - tree_map[row].size();
            tree_map[row].append(n, ' ');
        }
        tree_map[row].append(s);
    } else {
        int n1 = s.size() / 2;
        if (loc - n1 <=
            column) { // 若该节点的长度比父节点长,则还是通过column添加
            if (tree_map[row].size() <
                column) { // 若不等,则说明中间需要填充空格
                int n = column - tree_map[row].size();
                tree_map[row].append(n, ' ');
            }
            tree_map[row].append(s);
        } else { // 这种情况必须填充空格
            int n = loc - n1 - tree_map[row].size();
            tree_map[row].append(n, ' ');
            tree_map[row].append(s);
        }
    }
}

/**画父子节点之间的竖线,s表示父亲节点的字符,loc表示父亲节点的起始位置
 * 返回值用于处理运算符的位置
 */
int tree_out(string s, int row, int column) {
    int n1 = s.size() / 2;
    int n2 = column + n1 - tree_map[row].size();
    tree_map[row].append(n2, ' ');
    tree_map[row] += '|';
    return n1 + column;
}

void printTree(ofstream &fout) {
    for (int i = 0; i < 100; i++) {
        if (!tree_map[i].empty()) {
            fout << tree_map[i] << endl;
        } else
            break;
    }
}

int readToken() { // 用来根据空格从str中取词，并返回该词的长度，以便进行移位操作
    int i = 0;
    for (; str[location + i] != ' '; i++) {
        token[i] = str[location + i];
    }
    token[i] = '\0';
    return i;
}

void bindString(int k) { // 用来将s数组中的内容整合到str中
    for (int i = 0; i <= k; i++) {
        str.append(s[i]);
    }
}

int findID(const string
               &words) { // 从符号表中查找，若找到，则返回对应的位置，否则返回-1
    for (int i = 0; i < IDNum; i++) {
        if (words == ID[i].name)
            return i;
    }
    return -1;
}

void printSequence(ofstream &fout) {
    for (int i = 0; i < tetradNum; i++) {
        fout << i << " : " << tetrad[i] << endl;
    }
}

LInfo Block(LInfo info) {
    if (flag) {
        string_out("<Block>", info.row, info.column);
        int loc = tree_out("<Block>", ++info.row, info.column);
        int i = readToken();
        if (strcmp(token, "{") == 0) {
            location = location + i + 1;
            string_out(token, ++info.row, info.column, loc);
            info.column = draw_line(info.row, width);
            LInfo info1 = Decls(info);
            if (!flag)
                return info;
            info.column = draw_line(info.row, info1.interval + width);
            LInfo info2 = STMTS(info);
            if (!flag)
                return info;
            info.column = draw_line(info.row, info2.interval + width);
            i = readToken();
            if (strcmp(token, "}") == 0) {
                location = location + i + 1;
                string_out(token, info.row, info.column);
                info.interval =
                    info1.interval + info2.interval + width * 3 + 1 + 7;
                return info;
            } else {
                string s = token;
                error = s + "之前缺少}";
                flag = false;
                return info;
            }
        } else {
            string s = token;
            error = s + "之前缺少{";
            flag = false;
            return info;
        }
    }
}

LInfo Decls(LInfo info) {
    if (flag) {
        string_out("<Decls>", info.row, info.column);
        int loc = tree_out("<Decls>", ++info.row, info.column);
        int i = readToken();
        if (Type(token)) {
            info.id.type = 1; // 标记当前的类型为int类型
            location = location + i + 1;
            string_out("<Type>", ++info.row, info.column, loc);
            loc = tree_out("<Type>", info.row + 1, info.column);
            string_out(token, info.row + 2, info.column, loc);
            info.column = draw_line(info.row, width);
            LInfo info1 = NameList(info);
            if (!flag)
                return info;
            info.column = draw_line(info.row, info1.interval + width);
            i = readToken();
            if (strcmp(token, ";") == 0) {
                location = location + i + 1;
                string_out(token, info.row, info.column);
                info.column = draw_line(info.row, width);
                LInfo info2 = Decls(info);
                if (!flag)
                    return info;
                info.interval =
                    info1.interval + info2.interval + width * 3 + 1 + 7;
                return info;
            } else {
                string s = token;
                error = s + "之前缺少;";
                flag = false;
                return info;
            }
        } else { // 否则输出为empty
            string_out("empty", ++info.row, info.column, loc);
            info.interval = 7;
            return info;
        }
    }
}

LInfo NameList(LInfo info) {
    if (flag) {
        string_out("<NameList>", info.row, info.column);
        int loc = tree_out("<NameList>", ++info.row, info.column);
        int i = readToken();
        if (Name(token)) {
            ID[IDNum].name = token; // 将标识符存入符号表中
            ID[IDNum].type = info.id.type;
            IDNum++;
            location = location + i + 1;
            string_out("<Name>", ++info.row, info.column, loc);
            loc = tree_out("<Name>", info.row + 1, info.column + 2);
            string_out(token, info.row + 2, info.column, loc);
            info.column = draw_line(info.row, width);
            LInfo info1 = NameList1(info);
            if (!flag)
                return info;
            info.interval = info1.interval + width + 10;
            return info;
        } else {
            string s = token;
            error = s + "之前缺少id";
            flag = false;
            return info;
        }
    }
}

LInfo NameList1(LInfo info) {
    if (flag) {
        string_out("<NameList1>", info.row, info.column);
        int loc = tree_out("<NameList1>", ++info.row, info.column);
        int i = readToken();
        if (strcmp(token, ",") == 0) {
            location = location + i + 1;
            string_out(token, ++info.row, info.column, loc);
            info.column = draw_line(info.row, width);
            i = readToken();
            if (Name(token)) {
                ID[IDNum].name = token; // 将标识符存入符号表中
                ID[IDNum].type = info.id.type;
                IDNum++;
                location = location + i + 1;
                string_out("<Name>", ++info.row, info.column);
                tree_out("<Name>", info.row + 1, info.column);
                string_out(token, info.row + 2, info.column);
                info.column = draw_line(info.row, width);
                LInfo info1 = NameList1(info);
                if (!flag)
                    return info;
                info.interval = info1.interval + 6 + width * 2 + 11;
                return info;
            } else {
                string s = token;
                error = s + "之前缺少id";
                flag = false;
                return info;
            }
        } else { // 否则输出为empty
            string_out("empty", ++info.row, info.column, loc);
            info.interval = 11;
            return info;
        }
    }
}

bool Type(char *words) {
    if (strcmp(words, "int") == 0)
        return true;
    else
        return false;
}

bool Name(char *words) {
    if (!isOP(words) && !isKey(words) && !isDE(words[0]) &&
        !isdigit(words[0]) && words[0] != '\'' && words[0] != '\"') {
        if (words[0] == '_' || isalpha(words[0]))
            return true;
    }
    return false;
}

LInfo STMTS(LInfo info) {
    if (flag) {
        string_out("<STMTS>", info.row, info.column);
        int loc = tree_out("<STMTS>", ++info.row, info.column);
        int i = readToken();
        if (Name(token) || strcmp(token, "if") == 0 ||
            strcmp(token, "while") == 0) {
            ++info.row;
            LInfo info1 = STMT(info);
            if (!flag)
                return info;
            info.column = draw_line(info.row, info1.interval + width);
            LInfo info2 = STMTS(info);
            if (!flag)
                return info;
            info.interval = info1.interval + info2.interval + 7;
            return info;
        } else { // 否则输出为empty
            string_out("empty", ++info.row, info.column, loc);
            info.interval = 7;
            return info;
        }
    }
}

LInfo STMT(LInfo info) {
    if (flag) {
        string_out("<STMT>", info.row, info.column);
        int loc = tree_out("<STMT>", ++info.row, info.column);
        int i = readToken();
        location = location + i + 1;
        if (Name(token)) { // 若是标识符
            int num = findID(token);
            if (num ==
                -1) { // 因为是赋值语句，所以首先需要判断该标识符是否在前面已经声明过了，若未声明则报错
                string s = token;
                error = "标识符\"" + s + "\"未声明";
                flag = false;
                return info;
            }
            string_out("<Name>", ++info.row, info.column, loc);
            tree_out("<Name>", info.row + 1, info.column);
            string_out(token, info.row + 2, info.column, loc);
            info.column = draw_line(info.row, width);
            i = readToken();
            if (strcmp(token, "=") == 0) {
                location = location + i + 1;
                string_out(token, info.row, info.column);
                info.column = draw_line(info.row, width);
                LInfo info1 = Expr(info);
                if (!flag)
                    return info;
                ID[num].value = info1.id.value;
                tetrad[tetradNum++] = "(=, " + to_string(info1.id.value) +
                                      ", _, " + ID[num].name + ")";
                info.column = draw_line(info.row, info1.interval + width);
                i = readToken();
                if (strcmp(token, ";") == 0) {
                    location = location + i + 1;
                    string_out(token, info.row, info.column);
                    info.interval = info1.interval + width * 3 + 2 + 6;
                    return info;
                } else {
                    string s = token;
                    error = s + "之前缺少;";
                    flag = false;
                    return info;
                }
            } else {
                string s = token;
                error = s + "之前缺少=";
                flag = false;
                return info;
            }
        } else if (strcmp(token, "if") == 0) {
            string_out(token, ++info.row, info.column, loc);
            info.column = draw_line(info.row, width);
            i = readToken();
            if (strcmp(token, "(") == 0) {
                location = location + i + 1;
                string_out(token, info.row, info.column);
                info.column = draw_line(info.row, width);
                LInfo info1 = BOOL(info);
                if (!flag)
                    return info;
                info.column = draw_line(info.row, info1.interval + width);
                tetrad[tetradNum++] = "(jnz, BOOL, _, ";
                info.addr = tetradNum - 1;
                i = readToken();
                if (strcmp(token, ")") == 0) {
                    location = location + i + 1;
                    string_out(token, info.row, info.column);
                    info.column = draw_line(info.row, width);
                    LInfo info2 = STMT(info);
                    if (!flag)
                        return info;
                    info.column = draw_line(info.row, info2.interval + width);
                    tetrad[tetradNum++] = "(jmp, _, _, ";
                    tetrad[info.addr] =
                        tetrad[info.addr] + to_string(tetradNum) + ")";
                    info.addr = tetradNum - 1;

                    LInfo info3 = STMT1(info);
                    if (!flag)
                        return info;
                    info.interval = info1.interval + info2.interval +
                                    info3.interval + width * 5 + 2 + 6;
                    tetrad[info.addr] =
                        tetrad[info.addr] + to_string(tetradNum) + ")";
                    return info;
                } else {
                    string s = token;
                    error = s + "之前缺少)";
                    flag = false;
                    return info;
                }
            } else {
                string s = token;
                error = s + "之前缺少(";
                flag = false;
                return info;
            }
        } else if (strcmp(token, "while") == 0) {
            string_out(token, ++info.row, info.column, loc);
            info.column = draw_line(info.row, width);
            i = readToken();
            if (strcmp(token, "(") == 0) {
                location = location + i + 1;
                string_out(token, info.row, info.column);
                info.column = draw_line(info.row, width);
                LInfo info1 = BOOL(info);
                if (!flag)
                    return info;
                info.column = draw_line(info.row, info1.interval + width);
                i = readToken();
                if (strcmp(token, ")") == 0) {
                    location = location + i + 1;
                    string_out(token, info.row, info.column);
                    info.column = draw_line(info.row, width);
                    tetrad[tetradNum++] = "(jnz, BOOL, _, ";
                    info.addr = tetradNum - 1;

                    LInfo info2 = STMT(info);
                    if (!flag)
                        return info;
                    tetrad[tetradNum++] =
                        "(jmp, _, _, " + to_string(info.addr) + ")";
                    tetrad[info.addr] =
                        tetrad[info.addr] + to_string(tetradNum) + ")";
                    info.interval =
                        info1.interval + info2.interval + width * 4 + 2 + 6;
                    return info;
                } else {
                    string s = token;
                    error = s + "之前缺少)";
                    flag = false;
                    return info;
                }
            } else {
                string s = token;
                error = s + "之前缺少(";
                flag = false;
                return info;
            }
        } else {
            string s = token;
            error = s + "之前缺少id";
            flag = false;
            return info;
        }
    }
}

LInfo STMT1(LInfo info) {
    if (flag) {
        string_out("<STMT1>", info.row, info.column);
        int loc = tree_out("<STMT1>", ++info.row, info.column);
        int i = readToken();
        if (strcmp(token, "else") == 0) {
            location = location + i + 1;
            string_out(token, ++info.row, info.column, loc);
            info.column = draw_line(info.row, width);
            LInfo info1 = STMT(info);
            if (!flag)
                return info;
            info.interval = info1.interval + width + 7;
            return info;
        } else { // 否则输出为empty
            string_out("empty", ++info.row, info.column, loc);
            info.interval = 7;
            return info;
        }
    }
}

LInfo BOOL(LInfo info) {
    if (flag) {
        string_out("<BOOL>", info.row, info.column);
        tree_out("<BOOL>", ++info.row, info.column);
        ++info.row;
        LInfo info1 = Expr(info);
        if (!flag)
            return info;
        info.column = draw_line(info.row, info1.interval + width);
        int i = readToken();
        if (RelOp(token)) { // 若是关系运算符
            location = location + i + 1;
            string_out("<RelOp>", info.row, info.column);
            int loc = tree_out("<RelOp>", info.row + 1, info.column);
            string_out(token, info.row + 2, loc);
            info.column = draw_line(info.row, width);
            string temp = token;
            LInfo info2 = Expr(info);
            if (!flag)
                return info;
            if (temp ==
                "<") { // 由于C++的特性，这里输出的结果1表示true，0表示false
                bool result = info1.id.value < info2.id.value;
                tetrad[tetradNum++] = "(<, " + to_string(info1.id.value) +
                                      ", " + to_string(info2.id.value) + ", " +
                                      to_string(result) + ")";
                info.id.rel = result;
            } else if (temp == "<=") {
                bool result = info1.id.value <= info2.id.value;
                tetrad[tetradNum++] = "(<=, " + to_string(info1.id.value) +
                                      ", " + to_string(info2.id.value) + ", " +
                                      to_string(result) + ")";
                info.id.rel = result;
            } else if (temp == ">") {
                bool result = info1.id.value > info2.id.value;
                tetrad[tetradNum++] = "(>, " + to_string(info1.id.value) +
                                      ", " + to_string(info2.id.value) + ", " +
                                      to_string(result) + ")";
                info.id.rel = result;
            } else if (temp == ">=") {
                bool result = info1.id.value >= info2.id.value;
                tetrad[tetradNum++] = "(>=, " + to_string(info1.id.value) +
                                      ", " + to_string(info2.id.value) + ", " +
                                      to_string(result) + ")";
                info.id.rel = result;
            } else if (temp == "==") {
                bool result = info1.id.value == info2.id.value;
                tetrad[tetradNum++] = "(==, " + to_string(info1.id.value) +
                                      ", " + to_string(info2.id.value) + ", " +
                                      to_string(result) + ")";
                info.id.rel = result;
            } else {
                bool result = info1.id.value != info2.id.value;
                tetrad[tetradNum++] = "(!=, " + to_string(info1.id.value) +
                                      ", " + to_string(info2.id.value) + ", " +
                                      to_string(result) + ")";
                info.id.rel = result;
            }
            info.interval = info1.interval + info2.interval + width * 2 + 6;
            return info;
        } else {
            string s = token;
            error = s + "之前缺少关系运算符";
            flag = false;
            return info;
        }
    }
}

bool RelOp(char *words) {
    if (strcmp(words, "<") == 0 || strcmp(words, "<=") == 0 ||
        strcmp(words, ">") == 0 || strcmp(words, ">=") == 0 ||
        strcmp(words, "==") == 0 || strcmp(words, "!=") == 0) {
        return true;
    }
    return false;
}

LInfo Expr(LInfo info) {
    if (flag) {
        string_out("<Expr>", info.row, info.column);
        tree_out("<Expr>", ++info.row, info.column);
        ++info.row;
        LInfo info1 = Term(info);
        if (!flag)
            return info;
        info.column = draw_line(info.row, info1.interval + width);
        info.id = info1.id;
        LInfo info2 = Expr1(info);
        if (!flag)
            return info;
        info.interval = info1.interval + info2.interval + width + 6;
        info.id = info2.id;
        return info;
    }
}

LInfo Expr1(LInfo info) {
    if (flag) {
        string_out("<Expr1>", info.row, info.column);
        int loc = tree_out("<Expr1>", ++info.row, info.column);
        int i = readToken();
        if (AddOp(token)) { // 若字符为+或-
            location = location + i + 1;
            string_out("<AddOp>", ++info.row, info.column, loc);
            loc = tree_out("<AddOp>", info.row + 1, info.column);
            string_out(token, info.row + 2, info.column, loc);
            info.column = draw_line(info.row, width);
            string temp = token;
            LInfo info1 = Term(info);
            if (!flag)
                return info;
            if (temp == "+") {
                int result = info.id.value + info1.id.value;
                tetrad[tetradNum++] = "(+, " + to_string(info.id.value) + ", " +
                                      to_string(info1.id.value) + ", " +
                                      to_string(result) + ")";
                info.id.value = result;
            } else {
                int result = info.id.value - info1.id.value;
                tetrad[tetradNum++] = "(-, " + to_string(info.id.value) + ", " +
                                      to_string(info1.id.value) + ", " +
                                      to_string(result) + ")";
                info.id.value = result;
            }
            info.column = draw_line(info.row, info1.interval + width);
            LInfo info2 = Expr1(info);
            if (!flag)
                return info;
            info.interval = info1.interval + info2.interval + width * 2 + 7;
            info.id = info2.id;
            return info;
        } else { // 否则输出为empty
            string_out("empty", ++info.row, info.column, loc);
            info.interval = 7;
            return info;
        }
    }
}

LInfo Term(LInfo info) {
    if (flag) {
        string_out("<Term>", info.row, info.column);
        tree_out("<Term>", ++info.row, info.column);
        ++info.row;
        LInfo info1 = Factor(info);
        if (!flag)
            return info;
        info.column = draw_line(info.row, info1.interval + width);
        info.id = info1.id;
        LInfo info2 = Term1(info);
        if (!flag)
            return info;
        info.interval = info1.interval + info2.interval + width + 6;
        info.id = info2.id;
        return info;
    }
}

LInfo Term1(LInfo info) {
    if (flag) {
        string_out("<Term1>", info.row, info.column);
        int loc = tree_out("<Term1>", ++info.row, info.column);
        int i = readToken();
        if (MulOp(token)) { // 若字符为*或/
            location = location + i + 1;
            string_out("<MulOp>", ++info.row, info.column, loc);
            loc = tree_out("<MulOp>", info.row + 1, info.column);
            string_out(token, info.row + 2, info.column, loc);
            info.column = draw_line(info.row, width);
            string temp = token;
            LInfo info1 = Factor(info);
            if (!flag)
                return info;
            if (temp == "*") {
                int result = info.id.value * info1.id.value;
                tetrad[tetradNum++] = "(*, " + to_string(info.id.value) + ", " +
                                      to_string(info1.id.value) + ", " +
                                      to_string(result) + ")";
                info.id.value = result;
            } else {
                int result = info.id.value / info1.id.value;
                tetrad[tetradNum++] = "(/, " + to_string(info.id.value) + ", " +
                                      to_string(info1.id.value) + ", " +
                                      to_string(result) + ")";
                info.id.value = result;
            }
            info.column = draw_line(info.row, info1.interval + width);
            LInfo info2 = Term1(info);
            if (!flag)
                return info;
            info.interval = info1.interval + info2.interval + width * 2 + 7;
            info.id = info2.id;
            return info;
        } else { // 否则输出为empty
            string_out("empty", ++info.row, info.column, loc);
            info.interval = 7;
            return info;
        }
    }
}

LInfo Factor(LInfo info) {
    if (flag) {
        string_out("<Factor>", info.row, info.column);
        int loc = tree_out("<Factor>", ++info.row, info.column);
        int i = readToken();
        location = location + i + 1;
        if (Name(token)) {
            int num = findID(token);
            if (num == -1) {
                string s = token;
                error = "标识符\"" + s + "\"未声明";
                flag = false;
                return info;
            }
            info.id = ID[num];
            string_out(token, ++info.row, info.column, loc);
            info.interval = 8;
            return info;
        } else if (isdigit(token[0])) {
            LInfo info1;
            info1.id.value = atoi(token);
            info1.id.type = 1;
            info.id = info1.id;
            string_out(token, ++info.row, info.column, loc);
            info.interval = 8;
            return info;
        } else if (strcmp(token, "(") == 0) {
            string_out(token, ++info.row, info.column, loc);
            info.column = draw_line(info.row, width);
            LInfo info1 = Expr(info);
            if (!flag)
                return info;
            i = readToken();
            if (strcmp(token, ")") == 0) {
                location = location + i + 1;
                info.column = draw_line(info.row, info1.interval + width);
                string_out(token, info.row, info.column);
                info.interval = info1.interval + width * 2 + 8;
                info.id = info1.id;
                return info;
            } else { // 若一直没有),则说明该算术表达式错误
                string s = token;
                error = s + "之前缺少)";
                flag = false;
                return info;
            }
        } else {
            string s = token;
            error = s + "之前缺少id";
            flag = false;
            return info;
        }
    }
}

bool AddOp(char *words) {
    if (strcmp(words, "+") == 0 || strcmp(words, "-") == 0)
        return true;
    return false;
}

bool MulOp(char *words) {
    if (strcmp(words, "*") == 0 || strcmp(words, "/") == 0)
        return true;
    return false;
}

int getData() {
    int k = 0;
    cout << "请输入一个代码块(#表示结束)：" << endl;
    cin.getline(s[0], 100);
    while (k < 100 && strcmp(s[k], "#") != 0) {
        cin.getline(s[++k], 100);
    }
    return k;
}

int main() {
    int k = getData(); // 获取输入
    if (scanner(k)) {  // 先进行词法分析
        bindString(k); // 将多行输入数据整合到string类型的str中
        cout << str << endl;
        LInfo info;
        info.row = info.column = 0;
        Block(info); // 进入语法分析
        if (str[location] == '#') {
            cout << "Correct!" << endl;
            cout << "接下来输出语法树!" << endl;
            ofstream fout("TreeOut.txt");
            printTree(fout);
            cout << "语法树输出成功!请查收TreeOut.txt文件!" << endl;
            fout.close();
            fout.open("output.txt");
            printSequence(fout);
            fout.close();
            cout << "四元式输出成功!请查收output.txt文件!" << endl;
        } else {
            cout << "Error!语法分析未通过!" << endl;
            cout << error << endl; // 将错误信息输出
        }
    } else {
        cout << "Error!预处理未通过!" << endl;
    }
    system("pause");
    return 0;
}
