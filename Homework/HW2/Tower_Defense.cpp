#include <bits/stdc++.h>
using namespace std;

#define MAX(a,b) (((a) > (b)) ? (a): (b))

// some constants
long long N, K, A, B, maximum;
long long dp[2][100001]; 
long long s[100001];

long long dq_max, prev_max;

void solve(){
	
	// traverse K tower (2 ~ K)
	for(int i = 2; i <= K; i++){

		deque <long long> dq;
		prev_max = LLONG_MIN;

		// traverse from i ~ N tower to find maximum
		for(int j = i; j <= N; j++){
			
			// remove those out of range
			while(!dq.empty() && dq.front() <= j - B){
				prev_max = MAX(prev_max, dp[(i - 1) % 2][dq.front()]);
				dq.pop_front();
			}

			// value of (i - 1, j - 1) and pop those smaller than the last
			dq_max = dp[(i - 1) % 2][j - 1] + A * (B - 1);
			while(!dq.empty() && dq_max >= dp[(i - 1) % 2][dq.back()] + A * MAX(0, B - (j - dq.back()))){
				prev_max = MAX(prev_max, dp[(i - 1) % 2][dq.back()]);
				dq.pop_back();
			}

			dq.push_back(j - 1);

			dq_max = dp[(i - 1) % 2][dq.front()] + A * MAX(0, B - (j - dq.front()));

			if(dq_max >= prev_max)
				dp[i % 2][j] = s[j] + dq_max;
			else
				dp[i % 2][j] = s[j] + prev_max;

			maximum = MAX(maximum, dp[i % 2][j]);
		}
	}
	cout << MAX(0, maximum) << endl;
}

int main(){

	// I/O speed up
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	// constants and dp map
	cin >> N >> K >> A >> B;
	maximum = LLONG_MIN;

	// read defense level and let the first tower to be 1 ~ N
	for(int i = 1; i <= N; i++){
		cin >> s[i], dp[1][i] = s[i];
		maximum = MAX(maximum, s[i]);
	}

	solve();
}