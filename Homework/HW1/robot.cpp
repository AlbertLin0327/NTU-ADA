#include <bits/stdc++.h>
using namespace std;

enum Method{MOVE, JUMP};
enum Direction{UP, LEFT, SELF};

long long n, m, k, c, maximum;
long long farm[500][500];
long long dp[13][500][500][2];
int maxPoints[12][2];

class Action{
public:
	int x, y;
	enum Method method;

	Action(int Ax, int Ay, enum Method Amethod){
		x = Ax, y = Ay, method = Amethod;
	}
};

vector <Action*> answers;

void construction(){
	for(int i = 0; i < n; i++){
		for(int j = 0; j < m; j++){
			
			if(i != 0 && j != 0 && dp[k][i - 1][j][0] > maximum && dp[k][i - 1][j][0] > dp[k][i][j - 1][0] ||\
			   i != 0 && j == 0 && dp[k][i - 1][j][0] > maximum){
				dp[k][i][j][0] = farm[i][j] + dp[k][i - 1][j][0];
				dp[k][i][j][1] = UP;
			}else if(i != 0 && j != 0 && dp[k][i][j - 1][0] > maximum && dp[k][i][j - 1][0] > dp[k][i - 1][j][0] ||\
					 i == 0 && j != 0 && dp[k][i][j - 1][0] > maximum){
				dp[k][i][j][0] = farm[i][j] + dp[k][i][j - 1][0];
				dp[k][i][j][1] = LEFT;
			}else{
				dp[k][i][j][0] = farm[i][j] + (maximum == LLONG_MIN ? 0 : maximum);
				dp[k][i][j][1] = SELF;
			}

			if(dp[k][i][j][0] > dp[k][maxPoints[k][0]][maxPoints[k][1]][0])
				maxPoints[k][0] = i, maxPoints[k][1] = j;
			
		}
	}	
}

void solve(){

	// construct current 
	construction();
	maximum = dp[k][maxPoints[k][0]][maxPoints[k][1]][0];

	if(--k >= 0){
		for(int i = 0; i < n; i++)
			for(int j = 0; j < m; j++)
				farm[i][j] -= c;

		solve();
	}
}

void printout(int start){
	long long total = LLONG_MIN, best_layer = -1;
	for(int i = start; i >= 0; i--){
		if(dp[i][n - 1][m - 1][0] > total){
			total = dp[i][n - 1][m - 1][0];
			best_layer = i;
		}
	}

	maxPoints[best_layer][0] = n - 1, maxPoints[best_layer][1] = m - 1;

	while(best_layer <= start){
		while(dp[best_layer][maxPoints[best_layer][0]][maxPoints[best_layer][1]][1] != SELF){
			answers.push_back(new Action(maxPoints[best_layer][0], maxPoints[best_layer][1], MOVE));

			if(dp[best_layer][maxPoints[best_layer][0]][maxPoints[best_layer][1]][1] == UP)
				maxPoints[best_layer][0]--;
			else
				maxPoints[best_layer][1]--;
		}

		answers.push_back(new Action(maxPoints[best_layer][0], maxPoints[best_layer][1], JUMP));
		best_layer++;
	}

	cout << total << endl;
	cout << answers.size() - 1 << endl;

	for(int i = answers.size() - 2; i >= 0; i--){
		if((*answers[i]).method == JUMP)
			cout << "Jump " << (*answers[i]).x << " " << (*answers[i]).y << endl;
		else
			cout << "Move " << (*answers[i]).x << " " << (*answers[i]).y << endl;

	}
}

int main(){

	// read in the variables
	cin >> n >> m >> k >> c;

	for(int i = 0; i < n; i++)
		for(int j = 0; j < m; j++)
			cin >> farm[i][j];

	int start = k;
	maximum = LLONG_MIN;
	solve();

	printout(start);
}