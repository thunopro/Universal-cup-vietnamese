#include<bits/stdc++.h>
using namespace std; 
#define maxn 1009 
#define ll long long 
#define fi first 
#define se second 
#define re exit(0);
#define left id<<1 
#define right id<<1|1 
#define pb push_back 

const int mod = 1e9+7 ; 
const int INF = 1e9 ; 

typedef vector<int> vi ; 
typedef pair<int,int> pii ; 
typedef vector<pii> vii ; 

template < typename T > void chkmin ( T &a , T b ) { if ( a > b ) a = b ; } ; 
template < typename T > void chkmax ( T &a , T b ) { if ( a < b ) a = b ; } ; 

void add ( int &a , int b ) 
{
	a += b ; 
	if ( a >= mod ) a -= mod ; 
	if ( a < 0 ) a += mod ; 
}

void rf () 
{
	freopen ("bai1.inp","r",stdin) ; 
}

int _pow ( int a , int n ) 
{
	if ( n == 0 ) return 1 ; 
	int res = _pow (a,n/2) ; 
	if ( n % 2 ) return 1ll*res*res%mod*a%mod ; 
	else return 1ll*res*res%mod ; 
}

int main () 
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);cout.tie(0); 
	
}
