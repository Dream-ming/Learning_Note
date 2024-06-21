#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cmath>
#include <queue>
#include <map>
#include <string>
#include <cstring>
#include <vector>
#include <set>
//#define int long long
using namespace std;
const int N = 2e5 + 10;
const int inf = 0x3f3f3f3f;
int T, n, m, ans, siz[N], weg[N], z1, z2;
int head[N], nxt[N * 2], to[N * 2], num;
struct EE {
	int x, y, w, id;
} a[N];

void add(int x, int y) {
    to[++num] = y;
    nxt[num] = head[x];
    head[x] = num;
}

void tree_dp(int u, int fa) {

	siz[u] = 1;
	weg[u] = 0;

	for (int i = head[u]; i; i = nxt[i]) {
		int v = to[i];
		if (v == fa) continue;
		tree_dp(v, u);
		siz[u] += siz[v];
		weg[u] = max(weg[u], siz[v]);
	}

	weg[u] = max(weg[u], n - siz[u]);

	if (ans > weg[u]) {
		ans = weg[u];
		z1 = u;
		z2 = 0;
	}
	else if (ans == weg[u]) {
		z2 = z1;
		z1 = u;
	}

}


void solve() {

	ans = inf;
	num = z1 = z2 = 0;

	cin >> n;
	for (int i = 0; i <= n * 2; ++i) head[i] = 0; 

	for (int i = 1, x, y; i < n; ++i) {
		cin >> x >> y;
		add(x, y);
		add(y, x);
	}

	tree_dp(1, 0);

	cout << z1 << endl;
	if (z2) cout << z2 << endl;

}

signed main() {
	
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
	
    T = 1;
    cin >> T;
    while (T--) {

		solve();

    }

    return 0;
}

// 求树的重心
// 性质1：去掉重心后，最重的子树重量最小;
// 性质2：一棵树的重心最多有两个，并且两个重心一定相连
// 树形dp，记录子树最大重量更新重量weg
// 记录子树重量和n - siz[u]，依次更新上面的子树的重量和。
