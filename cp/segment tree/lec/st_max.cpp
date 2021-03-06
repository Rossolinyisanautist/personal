#include <iostream>

using namespace std;
#define long long long

const long ll_min = (unsigned long) -1/2 + 1;


void build_st(long* dst, long* src, long root, long l, long r)
{
	if(l == r)
	{
		dst[root] = l; // save index
		return;
	}

	long mid = (l + r) / 2;

	build_st(dst, src, root * 2 + 1, l, mid); // build left child
	build_st(dst, src, root * 2 + 2, mid + 1, r); // build right child

	long l_max = src[ dst[root * 2 + 1] ];
	long r_max = src[ dst[root * 2 + 2] ];
	dst[root] = l_max >= r_max ? dst[root*2+1] : dst[root*2+2];
}

long max_st(long* stree, long* src, long curr, long L, long R, long l, long r)
{
	if(l <= L && r >= R) // node is fully nested
	{
		return stree[curr]; // index of max
	}

	if(l > R || r < L)
	{
		return -1;
	}

	long M = (L + R) / 2;

	long l_max_idx = max_st(stree, src, curr * 2 + 1, L, M, l, r);
	long r_max_idx = max_st(stree, src, curr * 2 + 2, M + 1, R, l, r);

	if(l_max_idx == -1) 
	{
		return r_max_idx;
	}
	if(r_max_idx == -1)
	{
		return l_max_idx;
	}

	return src[l_max_idx] >= src[r_max_idx] ? l_max_idx : r_max_idx;
}

int main(int argc, char** argv)
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);


	long max = ll_min;
//	cout << (unsigned long) -1/2 << endl;

	long N = 100000;
	cin >> N;
	long inp[N];
	long segtree[4 * N];

	for(size_t i = 0; i < N; ++i)
	{
		cin >> inp[i];
	}

	build_st(segtree, inp, 0, 0, N-1);

	long K, l, r;
	cin >> K;
	while(K--)
	{
		cin >> l >> r;
		long res = max_st(segtree, inp, 0, 0, N-1, l-1, r-1); 
		cout << res+1 << ' ';
	}
	cout << '\n';
	
	return 0;
}



