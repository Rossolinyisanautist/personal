#include <iostream>

using namespace std;


int main() 


{

	int t; 
	cin >> t;


	while(t--)
	{
		int n;
		 cin >> n;

		 int res = n;

		for(int i = 2; i * i <=n; ++i)
		{
			if(n % i == 0)
			{
				while(n % i ==0)
					n /= i;

				res -= res / i;
			}
		}

		if(n > 1)
		{
			res -= res/n;
		}

		cout << res << '\n';

	}

}
