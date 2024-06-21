#include <cmath>
#include <cstdio>
#include <iostream>

using namespace std;

int n = 0, nn = 0;     // 数组行列数，以及数字的个数
int open_cnt = 0;      // 记录open表中每一个扩展的节点
int open_node_cnt = 0; // open表节点个数
int num = 0;           // 记录移动步数

struct Node {
    int a[8][8];
    int x_0, y_0; // 空位置的坐标
    int f, g, h; // 代价函数，f(总代价) = g(查询深度) + h(当前数组与目标的差距值)
    int flag; // 上一次移动方向，防止原地踏步循环重复状态
    Node *father; // 该状态的上一个状态，追溯来源状态
} start_nums, end_nums;

struct Open_Close {
    int f;
    Node *np;
} open[500000], close[500000];

bool isable() {

    // 判断状态是否有解
    // 逆序对算法：前面的大于当前数的个数

    int s[nn], e[nn]; // 把两个矩阵存为一维数组，方便操作
    int start_num = 0, end_num = 0; // 两个矩阵中的逆序对个数

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            s[i * n + j] = start_nums.a[i][j];
            e[i * n + j] = end_nums.a[i][j];
        }
    }

    for (int i = 0; i < nn; ++i) {
        for (int j = 0; j < i; ++j) {
            if (s[j] != 0 && s[i] > s[j])
                ++start_num;
            if (e[j] != 0 && e[i] > e[j])
                ++end_num;
        }
    }

    if (n & 1) {
        return ((start_num & 1) == (end_num & 1));
        // 如果n为奇数，两行列的逆序对奇偶性相同则可以到达
        // 否则不可以到达
    }
    else {
        return ((start_num & 1) == (end_num & 1)) == ((start_nums.x_0 & 1) == (end_nums.x_0 & 1));
        // 当n为偶数时
        // 逆序对奇偶性相同的话，则空格所在行的奇偶性也要相同，才可到达
    }
    // 证明：当空格左右移动时不影响整体的奇偶性
    // 当空格上下移动时，若n为奇数，则整体奇偶性不变
    // 当空格上下移动时，若n为偶数，则整体奇偶性会改变

}

int get_h(Node *node) {

    // 求代价函数 h
    // 当前矩阵和目标矩阵数字不在相同位置的个数

    int h = 0;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (node->a[i][j] != end_nums.a[i][j]) {
                ++h;
            }
        }
    }

    return h;
}

void input() {
    cout << "请输入数组的阵列大小:" << endl;
    cin >> n;
    nn = n * n;
    cout << "请输入数组的初始状态(限制为" << n * n
         << "个数)(空格位置请输入0):" << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> start_nums.a[i][j];
            if (start_nums.a[i][j] == 0) {
                start_nums.x_0 = i;
                start_nums.y_0 = j;
                // 记录空格的位置
            }
        }
    }

    cout << "请输入目标状态(条件同上):" << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> end_nums.a[i][j];
            if (end_nums.a[i][j] == 0) {
                end_nums.x_0 = i;
                end_nums.y_0 = j;
            }
        }
    }

    start_nums.g = 0;                           // 函数g
    start_nums.h = get_h(&start_nums);          // 函数h
    start_nums.f = start_nums.g + start_nums.h; // 函数f
}

void show(Node *node) { // 显示
    Node *p = node;
    if (p != &start_nums)
        show(p->father);
    ++num;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << p->a[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "---------------\n";
    return;
}

bool isend(Node *node) {

    // 判断是否为目标状态

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (node->a[i][j] != end_nums.a[i][j]) {
                return false;
            }
        }
    }

    return true;
}

void sort(Open_Close *open) {

    // open表排序，只需要找出最小的

    int min_val = 0x3f3f3f3f, min_id = 0;
    Open_Close temp;

    for (int i = 0; i <= open_cnt; ++i) {
        if (open[i].f > 0 && min_val > open[i].f) {
            min_val = open[i].f;
            min_id = i;
        }
    }

    temp = open[min_id];
    open[min_id] = open[0];
    open[0] = temp;
}

void space_move(int flag, Node *node) { // 移动空格，向四个方向扩展

    int x_0 = node->x_0, y_0 = node->y_0;

    if (flag == 1 && node->y_0 > 0) { // 空格左移，不可以在最左边
        Node *nxt = new Node();

        // 复制状态
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                nxt->a[i][j] = node->a[i][j];
            }
        }

        // 移动空格
        nxt->a[x_0][y_0] = node->a[x_0][y_0 - 1]; // 新状态空位置变成他左边位置的值
        nxt->a[x_0][y_0 - 1] = 0; // 左边的位置变成空位置
        nxt->x_0 = x_0;       // 更新空位置的坐标
        nxt->y_0 = y_0 - 1;
        nxt->flag = 3;      // 记录空位置的来源方向
        nxt->father = node; // 记录新状态的来源父亲状态
        nxt->g = node->g + 1;
        nxt->h = get_h(nxt);
        nxt->f = nxt->g + nxt->h; // 算出f、g、h值

        ++open_cnt; // 新状态添加进open表
        ++open_node_cnt;
        open[open_cnt].np = nxt;
        open[open_cnt].f = nxt->f;

    } else if (flag == 2 && node->x_0 < n - 1) { // 空格下移，不可以在最下面一行
        Node *nxt = new Node();

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                nxt->a[i][j] = node->a[i][j];
            }
        }

        nxt->a[x_0][y_0] = node->a[x_0 + 1][y_0]; // 移动
        nxt->a[x_0 + 1][y_0] = 0;
        nxt->x_0 = x_0 + 1;
        nxt->y_0 = y_0;
        nxt->flag = 4;
        nxt->father = node;
        nxt->g = node->g + 1;
        nxt->h = get_h(nxt);
        nxt->f = nxt->g + nxt->h;

        ++open_cnt; // 添加
        ++open_node_cnt;
        open[open_cnt].np = nxt;
        open[open_cnt].f = nxt->f;

    } else if (flag == 3 && node->y_0 < n - 1) { // 空格右移，不可以在最右边一行
        Node *nxt = new Node();

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                nxt->a[i][j] = node->a[i][j];
            }
        }

        nxt->a[x_0][y_0] = node->a[x_0][y_0 + 1]; //  右移
        nxt->a[x_0][y_0 + 1] = 0;
        nxt->x_0 = x_0;
        nxt->y_0 = y_0 + 1;
        nxt->flag = 1;
        nxt->father = node;
        nxt->g = node->g + 1;
        nxt->h = get_h(nxt);
        nxt->f = nxt->g + nxt->h;

        ++open_cnt;
        ++open_node_cnt;
        open[open_cnt].np = nxt;   // 添加到open表
        open[open_cnt].f = nxt->f; //  求 f（）

    } else if (flag == 4 && node->x_0 > 0) { // 空格上移
        Node *nxt = new Node();

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                nxt->a[i][j] = node->a[i][j];
            }
        }

        nxt->a[x_0][y_0] = node->a[x_0 - 1][y_0]; // 上移
        nxt->a[x_0 - 1][y_0] = 0;
        nxt->x_0 = x_0 - 1;
        nxt->y_0 = y_0;
        nxt->flag = 2;
        nxt->father = node;
        nxt->g = node->g + 1;
        nxt->h = get_h(nxt);
        nxt->f = nxt->g + nxt->h;

        ++open_cnt;
        ++open_node_cnt;
        open[open_cnt].np = nxt;   // 添加到open表
        open[open_cnt].f = nxt->f; //  求 f（）
    }
}

void expand(Node *node) { // 节点扩展

    // node->flag记录当前节点空格来源的方向
    // 如果将空格往来的方向移动，相当于浪费两步，状态不改变，需要规避

    for (int i = 1; i <= 4; ++i) {
        if (i == node->flag)
            continue;
        space_move(i, node);
    }

    return;
}

int main() {

    input();

    open[0].np = &start_nums; // 将初始状态放入open表
    open_node_cnt = 1;

    if (isable()) {
        while (open_node_cnt) { // open表不为空
            if (isend(open[0].np)) {
                cout << "\n路径：\n---------------\n";
                show(open[0].np);
                cout << "Step = " << num - 1 << endl;
                break;
            }

            expand(open[0].np); // 扩展最优节点的子节点
            open[0].np = NULL;
            open[0].f = -1;
            --open_node_cnt; // open表数量-1
            sort(open);      // open表排序
        }
    } 
    else {
        cout << "目标状态不可达\n";
    }
    
    system("pause");
    return 0;
}
