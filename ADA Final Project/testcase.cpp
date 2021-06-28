#include <bits/stdc++.h>
using namespace std;

int main(){

	srand (time(NULL));

	// slice
	int slice = 8 ,job = 30;

	cout << slice << "\n" << job << endl;

	int m_left = 100;

	for(int i = 1; i <= job; i++){
		int m = rand() % (m_left - job + i) + 1;
		m_left -= m;
		cout << m << endl;

		int w_temp = rand() % 64000000;
		double w = (double)w_temp / 1000000;
		cout << setprecision(6) << w << endl;

		for(int j = 1; j <= m; j++){
			int s = rand() % slice + 1, d = rand() % 96 + 1, p = rand() % j;
			cout << s << " " << d << " " << p;

			bool used[j]; 
			memset(used, 0, sizeof(used));

			for(int k = 0; k < p; k++){
				int a = rand() % (j - 1) + 1;

				while(used[a] == 1)
					a = rand() % (j - 1) + 1;
				
				used[a] = 1;
				cout << " " << a;
			}
			cout << endl;
		}
	}

}
