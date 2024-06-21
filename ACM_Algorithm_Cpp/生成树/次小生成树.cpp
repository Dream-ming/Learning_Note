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
#define int long long
using namespace std;
const int N = 2e5 + 10;
const int inf = 0x3f3f3f3f;
int T, n, m, l[N], ans, b[N], fans[N];
int dep[N], fa[N][25], root, maxf[N][25];
int head[N], nxt[N * 2], to[N * 2], num, val[N * 2];
struct EE {
	int x, y, w, id;
} a[N];

void add(int x, int y, int z) {
    to[++num] = y;
    nxt[num] = head[x];
    head[x] = num;
	val[num] = z;
}

int cmp(EE a, EE b) {
	return a.w < b.w;
}

void getdeep(int now, int u) {
    dep[now] = dep[u] + 1;
    fa[now][0] = u;

    for (int i = 1; (1 << i) <= dep[now]; ++i) {
        fa[now][i] = fa[fa[now][i - 1]][i - 1];
		maxf[now][i] = max(maxf[now][i - 1], maxf[fa[now][i - 1]][i - 1]);
	}

    for (int i = head[now]; i; i = nxt[i]) {
        if (to[i] == u) continue;
		maxf[to[i]][0] = val[i];
        getdeep(to[i], now);
    }
}

int LCA(int x, int y) {
	int res = 0;
    if (dep[x] < dep[y]) swap(x, y);

    for (int i = 20; i >= 0; --i) { 
        if (dep[fa[x][i]] >= dep[y]) {
			res = max(res, maxf[x][i]);
            x = fa[x][i];
		}
    }

    if (x == y) return res;

    for (int i = 20; i >= 0; --i) { 
        if (fa[x][i] != fa[y][i]) {
			res = max(res, max(maxf[x][i], maxf[y][i]));
            x = fa[x][i];
            y = fa[y][i];
        }
    }

	res = max(res, max(maxf[x][0], maxf[y][0]));

    return res;
}

int find(int x) {
	if (x == l[x]) return x;
	return l[x] = find(l[x]);
}

void bing(int x, int y) {
	x = find(x), y = find(y);
	if (x != y) l[x] = y;
}

void kruskal() {
	for (int i = 1; i <= m; ++i) {
		int tx = find(a[i].x), ty = find(a[i].y);
		if (tx == ty) continue;
		bing(tx, ty);
		b[i] = 1;
		ans += a[i].w;
		add(a[i].x, a[i].y, a[i].w);
		add(a[i].y, a[i].x, a[i].w);
	}
}


void solve() {

	cin >> n >> m;

	for (int i = 1; i <= n; ++i) l[i] = i;

	for (int i = 1; i <= m; ++i) {
		cin >> a[i].x >> a[i].y >> a[i].w;
		a[i].id = i;
	}

	sort(a + 1, a + m + 1, cmp);

	kruskal();

	getdeep(1, 0);

	for (int i = 1; i <= m; ++i) {
		if (b[i]) {
			fans[a[i].id] = ans; 
		}
		else {
			int tmp = LCA(a[i].x, a[i].y);
			//cout << tmp << endl;
			fans[a[i].id] = ans + a[i].w - tmp;
		}
	}

	for (int i = 1; i <= m; ++i) cout << fans[i] << endl;

}

signed main() {
	
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
	
    T = 1;
    //cin >> T;
    while (T--) {

		solve();

    }

    return 0;
}

// codeforces.com/problemset/problem/609/E
// 分别指定每一条边必须在树上，求最小生成树
// 思路：在最小生成树上的边为ans，否则求次小生成树
// 次小生成树，在最小生成树上加一条边成环，然后在环上删一条最大的边
// 利用LCA的性质，环一定过LCA，在get_deep和get_LCA的过程中维护两点到LCA的最大边maxf数组
