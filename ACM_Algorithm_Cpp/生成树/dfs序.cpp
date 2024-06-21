#include<iostream>
#include<cstdio>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstring>
#include<map>
#include<vector>
#include<set>
#include<queue>
#include<iomanip>
#include<stdlib.h>
#define int long long
#define P pair<int,int>
#define MP make_pair

using namespace std;
const int N=1e6+6;
const int M=2e6+6;
const int inf=0x3f3f3f3f3f3f3f3f;

int T, n, m, k, r, x, y, opt, ans, num, cnt;
vector< int > a(N);

vector< int > to(M), nxt(M), head(M), id(N), in(N), out(N);

void add( int x , int y ){ to[++num]=y; nxt[num]=head[x]; head[x]=num; }

void dfs( int u , int fa ){

	id[++cnt]=u;
	in[u]=cnt;

	for( int i=head[u]; i; i=nxt[i] ){
		if( to[i]==fa ) continue;
		dfs( to[i] , u );
	}

	out[u]=cnt;

}

vector< int > tree(M);

int lowbit( int x ){ return x&(-x); }

void updata( int x , int k ){
	while( x<=n ){
		tree[x]+=k;
		x+=lowbit(x);
	}
}

int ask_sum( int x ){
	int res=0;
	while( x>0 ){
		res+=tree[x];
		x-=lowbit(x);
	}
	return res;
}

void solve(){

	cin>>n>>m>>r;

	for( int i=1; i<=n; ++i ) cin>>a[i];

	for( int i=1; i<n; ++i ){
		cin>>x>>y;
		add( x , y ); add( y , x );
	}

	dfs( r , -1 );

	for( int i=1; i<=n; ++i ) updata( in[i] , a[i] );

	while( m-- ){
		cin>>opt;
		if( opt==1 ) cin>>x>>y, updata( in[x] , y );
		else cin>>x, cout<<ask_sum(out[x])-ask_sum(in[x]-1)<<endl;
	}

	return;
}


signed main(){
 
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	
	T=1;
	//cin>>T;
	while( T-- ){

		solve();

	}

	return 0;
}


/*
模板题
给一颗有根树， 树的编号为1...N。根节点的编号为r。
每个节点都有一个权值，a[i]
接下来有M个操作：
1、( 1 , a , x ) 将节点a的权值加x
2、( 2 , a ) 求a的子树上所有节点的权值之和
*/

/*
求dfs序，in和out数组，维护一个树状数组
ask(out[a])-ask(in[a]-1)即可
*/
