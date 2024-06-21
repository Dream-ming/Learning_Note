#include <iostream>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <string>
#include <cstring>
#include <map>
#include <vector>
#include <set>
#include <queue>
#include <iomanip>
//#define LL long long
using namespace std;
const int N = 2e3 + 10;
const int M = 4e6 + 10;
const int inf = 0x3f3f3f3f;

int T, n, m, num;
int nxt[M], to[M], head[N];
int dfn[N], low[N], idx;
int col, color[N];
int inst[N], top, st[M];

void add(int x, int y) {
	to[++num] = y;
	nxt[num] = head[x];
	head[x] = num;
}

void tarjan(int u) {

	st[++top] = u;
	inst[u] = 1;
	dfn[u] = low[u] = ++idx;

	for (int i = head[u]; i; i = nxt[i]) {
		int v = to[i];
		if (!dfn[v]) tarjan(v);
		if (inst[v]) low[u] = min(low[u], low[v]);
	}

	if (low[u] == dfn[u]) {
		++col;
		while (1) {
			int v = st[top];
			--top;
			inst[v] = 0;
			color[v] = col;
			if (u == v) break;
		}
	}

}

void init() {
	top = 0;
	num = 0;
	idx = 0;
	col = 0;
	memset(inst, 0, sizeof(inst));
	memset(st, 0, sizeof(st));
	memset(dfn, 0, sizeof(dfn));
	memset(low, 0, sizeof(low));
	memset(head, 0, sizeof(head));
	memset(color, 0, sizeof(color));
}

void solve() {

	int a1, a2, c1, c2;

	for (int i = 1; i <= m; ++i) {
		scanf("%d%d%d%d", &a1, &a2, &c1, &c2);
		add(a1 * 2 + c1, a2 * 2 + (c2 ^ 1));
		add(a2 * 2 + c2, a1 * 2 + (c1 ^ 1));
	}

	for (int i = 0; i < 2 * n; ++i) {
		if (!dfn[i]) tarjan(i);
	}

	for (int i = 0; i < 2 * n; i += 2) {
		if (color[i] == color[i + 1]) {
			printf("NO\n");
			return;
		}
	}
	printf("YES\n");

}

signed main() {

	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);

	T = 1;
	//T = read();
	while (~scanf("%d%d", &n, &m)) {
		init();
		solve();
	}
	//system("pause");
	return 0;
}
