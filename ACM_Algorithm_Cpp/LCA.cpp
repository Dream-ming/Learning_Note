#include<iostream>
#include<cstdio>
#include<algorithm>
#include<cmath>
#include<queue>
#include<map>
#include<string>
#include<cstring>
#include<vector>
#include<set>
#define int long long
#define P pair<int,int>
#define M make_pair

using namespace std;
const int Ma=0x3f3f3f3f3f3f3f3f;
const int N=2e6+5;
const int mo=998244353;
int T,n,m,a[N],dep[N],fa[N][25],root;
int head[N],nxt[N],to[N],num;
char c;
string s;

int read(){
	int x=0,f=1; char c=getchar();
	while( c<'0' || c>'9' ){ if( c=='-' ) f=-1; c=getchar(); }
	while( c>='0'&&c<='9' ){ x=(x<<1)+(x<<3)+c-48; c=getchar(); }
	return f*x;
}

int MAX( int a , int b ){ return a>b?a:b; }

int MIN( int a , int b ){ return a<b?a:b; }

void add( int x , int y ){ to[++num]=y; nxt[num]=head[x]; head[x]=num; }

void getdeep( int now , int u ){ // 预处理深度

	dep[now]=dep[u]+1;
	fa[now][0]=u;
	
	for( int i=1; (1<<i)<=dep[now]; ++i )
		fa[now][i]=fa[fa[now][i-1]][i-1]; 
		
	for( int i=head[now]; i; i=nxt[i] ){
		if( to[i]==u ) continue;
		getdeep( to[i] , now );
	}
	
}

int LCA( int x , int y ){ // 倍增法
	
	if( dep[x]<dep[y] ) swap( x , y );
	
	for( int i=20; i>=0; --i ){ // 让两点跳到同一高度
		if( dep[fa[x][i]]>=dep[y] ) x=fa[x][i];
		if( x==y ) return x;
	}
	
	for( int i=20; i>=0; --i ){ // x和y一起往上跳，找到LCA的子节点
		if( fa[x][i]!=fa[y][i] ){ // 保证不同，就不会大于等于LCA
			x=fa[x][i];
			y=fa[y][i];
		}
	}
	return fa[x][0];
}

signed main(){

	T=1;
	while( T-- ){
		
		n=read(); m=read(); root=read();
		
		for( int i=1,x,y; i<n; ++i ){
			x=read(); y=read();
			add( x , y ); add( y , x );
		}
		
		getdeep( root , 0 );
		
		for( int i=1,x,y; i<=m; ++i ){
			x=read(); y=read();
			cout<<LCA( x , y )<<endl;
		}		
	}
	
	return 0;
}
