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
const int mo=1e9+7;
const int inf=0x3f3f3f3f3f3f3f3f;

int T, n, m, num, ans;
vector< int > l(N);
struct EE { int l, r, val; } e[N];

void add( int x , int y , int z ){ e[++num].l=x; e[num].r=y; e[num].val=z; }

int cmp( EE a , EE b ){ return a.val<b.val; }

int find( int x ){
	if( x==l[x] ) return x;
	return l[x]=find(l[x]);
}

void Kruskal(){

	for( int i=1; i<=n; ++i ) l[i]=i;

	for( int i=1; i<=num; ++i ){
		int x=find(e[i].l), y=find(e[i].r);
		if( x!=y ){
			l[x]=y;
			ans+=e[i].val;
		}
	}
}

void solve(){

	ans=0;

	cin>>n>>m;

	for( int x,y,z, i=1; i<=m; ++i ){
		cin>>x>>y>>z;
		add( x , y , z );
	}

	sort( e+1 , e+num+1 , cmp );

	Kruskal();

	cout<<ans<<'\n'; 

}

signed main(){
 
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	
	T=1;
	//cin>>T;
	while( T-- ){

		solve();

	} 

	system("pause");
	return 0;
}
