#include <bits/stdc++.h>
using namespace std;

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

long long N, K, D;
long long arr[202];
long long cost[202];
long long score[202];

class Node {
public:
	long long remain_cost = INT_MIN; // subtracting from K to 0
	long long operations; // how many times did I change a character
	bool changed; // did I change this node
	bool printed_point; // whether I need to print the pointer
	Node* prev;
};

Node dp[603][203][203];

// whether arr[i] and arr[j] is legal
bool legal(int i, int j){
	if(arr[i] < arr[j])
		return (arr[j] - arr[i]) <= D;
	return (arr[i] - arr[j]) <= D;
}

void print(){
	int maximum = 0;
	for(int i = 1; i <= 3 * N; i++)
		if(dp[i][1][N].remain_cost >= 0)
			maximum = i;

	cout << maximum << endl;
	cout << dp[maximum][1][N].operations << endl;

	set <long long> M;

	for(auto iter = &dp[maximum][1][N]; iter != NULL; iter = iter->prev){
		long long left = ((iter - &dp[0][0][0]) % 41209) / 203;
		long long right = ((iter - &dp[0][0][0]) % 203);

		if(!iter->printed_point)
			continue;

		M.insert(left);
		M.insert(right);

		if(iter->changed){
			if(cost[left] < cost[right])
				cout << left << " " << arr[right] << endl;
			else
				cout << right << " " << arr[left] << endl;
		}
	}

	cout << M.size() << endl;
	for(auto c : M)
		cout << c << " ";
	cout << endl;
}

void solve(){

	for(int len = 1; len <= N; len++){
		for(int left = 1; left <= N - len + 1; left++){
			int right = left + len - 1;

			// if length == 1, then it is legal
			if(len == 1){
				dp[score[left]][left][left].remain_cost = K;
				dp[score[left]][left][right].changed = false;
				dp[score[left]][left][left].printed_point = true;
				
			}else if(len == 2){
	
				if(legal(left, right)){
					dp[score[left] + score[right]][left][right].remain_cost = K;
					dp[score[left] + score[right]][left][right].changed = false;
					dp[score[left] + score[right]][left][right].printed_point = true;

				}else{

					// consider three case, full range, left part and right part
					dp[score[left] + score[right]][left][right].remain_cost = K - MIN(cost[left], cost[right]);
					dp[score[left] + score[right]][left][right].operations = 1;
					dp[score[left] + score[right]][left][right].changed = true;
					dp[score[left] + score[right]][left][right].printed_point = true;

					if(score[right] != score[left]){
						dp[score[right]][left][right].remain_cost = dp[score[right]][right][right].remain_cost;
						dp[score[right]][left][right].prev = &dp[score[right]][right][right];
						dp[score[right]][left][right].changed = false;
						dp[score[right]][left][right].printed_point = false;

						dp[score[left]][left][right].remain_cost = dp[score[left]][left][left].remain_cost;
						dp[score[left]][left][right].prev = &dp[score[left]][left][left];
						dp[score[left]][left][right].changed = false;
						dp[score[left]][left][right].printed_point = false;

					}else{
						if(dp[score[right]][right][right].remain_cost > dp[score[left]][left][left].remain_cost){
							dp[score[right]][left][right].remain_cost = dp[score[right]][right][right].remain_cost;
							dp[score[right]][left][right].prev = &dp[score[right]][right][right];
							dp[score[right]][left][right].changed = false;
							dp[score[right]][left][right].printed_point = false;
						}else{
							dp[score[left]][left][right].remain_cost = dp[score[left]][left][left].remain_cost;
							dp[score[left]][left][right].prev = &dp[score[left]][left][left];
							dp[score[left]][left][right].changed = false;
							dp[score[left]][left][right].printed_point = false;
						}
					}
				}

			}else{
				for(int s = 1; s <= 3 * N; s++){

					long long prev_score = s - score[left] - score[right];
			
					if(legal(left, right) && prev_score > 0){
						
						long long left_part = dp[s][left][right - 1].remain_cost;
						long long right_part = dp[s][left + 1][right].remain_cost;
						long long full_part = dp[prev_score][left + 1][right - 1].remain_cost;

						// if the full partition is the largest
						if(full_part > right_part && full_part > left_part){
							dp[s][left][right].remain_cost = full_part;
							dp[s][left][right].operations = dp[prev_score][left + 1][right - 1].operations;
							dp[s][left][right].prev = &dp[prev_score][left + 1][right - 1];
							dp[s][left][right].changed = false;
							dp[s][left][right].printed_point = true;

						// if the right partition is the largest
						}else if(right_part >= full_part && right_part >= left_part){
							dp[s][left][right].remain_cost = right_part;
							dp[s][left][right].operations = dp[s][left + 1][right].operations;
							dp[s][left][right].prev = &dp[s][left + 1][right];
							dp[s][left][right].changed = false;
							dp[s][left][right].printed_point = false;
						
						// if the left partition is the largest
						}else{
							dp[s][left][right].remain_cost = left_part;
							dp[s][left][right].operations = dp[s][left][right - 1].operations;
							dp[s][left][right].prev = &dp[s][left][right - 1];
							dp[s][left][right].changed = false;
							dp[s][left][right].printed_point = false;
						}

					}else{
						long long left_part = dp[s][left][right - 1].remain_cost;
						long long right_part = dp[s][left + 1][right].remain_cost;
						long long full_part = (prev_score > 0  && dp[prev_score][left + 1][right - 1].remain_cost > 0 ? dp[prev_score][left + 1][right - 1].remain_cost - MIN(cost[left], cost[right]) : INT_MIN);

						// if the full partition is the largest
						if(full_part > right_part && full_part > left_part){
							dp[s][left][right].remain_cost = full_part;
							dp[s][left][right].operations = dp[prev_score][left + 1][right - 1].operations + 1;
							dp[s][left][right].prev = &dp[prev_score][left + 1][right - 1];
							dp[s][left][right].changed = true;
							dp[s][left][right].printed_point = true;

						// if the right partition is the largest
						}else if(right_part >= full_part && right_part >= left_part){
							dp[s][left][right].remain_cost = right_part;
							dp[s][left][right].operations = dp[s][left + 1][right].operations;
							dp[s][left][right].prev = &dp[s][left + 1][right];
							dp[s][left][right].changed = false;
							dp[s][left][right].printed_point = false;
						
						// if the left partition is the largest
						}else{
							dp[s][left][right].remain_cost = left_part;
							dp[s][left][right].operations = dp[s][left][right - 1].operations;
							dp[s][left][right].prev = &dp[s][left][right - 1];
							dp[s][left][right].changed = false;
							dp[s][left][right].printed_point = false;
						}
					}

				}
			}
		}
	}

	print();
	
}

int main(){

 cin >> N >> K >> D;
 for(int i = 1; i <= N; i++) cin >> arr[i];
 for(int i = 1; i <= N; i++) cin >> cost[i];
 for(int i = 1; i <= N; i++) cin >> score[i];

 solve();

}	