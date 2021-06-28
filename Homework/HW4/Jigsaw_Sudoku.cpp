#include <bits/stdc++.h>
#include "helper.h"
using namespace std;

class Combination{
public:
    vector < vector <int> > result;
    vector <int> temp;

    void helper(int n, int k, int step){
        if(k == 0){
			if(n == 0)
				result.push_back(temp);
            return;
        }

        if(n < 0 || k < 0)
            return;

        for(int i = step ; i <= 9 ; i++){
            temp.push_back(i);
            helper(n - i, k - 1, i + 1);
            temp.pop_back();
        }
    }

    vector < vector <int> > find(int k, int n){

    	// find combination
        helper(n, k, 1);
        return result;
    }
};

map <char, int> converter;
map <char, int> counter;
char constraint[10][10];
int board[10][10];

void solve(){

	// initialize Set X to size of 324 * 9
	DLX::Init(324 + converter.size() * 9);

	// process every number
	for(int i = 0; i < 729; i++){
		vector <int> temp;

		// i = (number, row, column)
		int column = i % 9, row = (i / 9) % 9, number = (i / 81) % 9;

		temp.push_back(1 + row * 9 + column);
		temp.push_back(1 + 81 + row * 9 + number);
		temp.push_back(1 + 162 + column * 9 + number);
		temp.push_back(1 + 243 + (row / 3) * 27 + ((column / 3) % 3) * 9 + number);
		temp.push_back(1 + 324 + distance(converter.begin(), converter.find(constraint[row][column])) * 9 + number);

		DLX::AddRow(i, temp);
	}

	// process mask for constraint
	int constraint_masks = 1;
	for(auto it : converter){

		Combination C;
		vector < vector <int> > masks = C.find(counter[it.first], converter[it.first]);
		int step = distance(converter.begin(), converter.find(it.first));

		for(auto v : masks){
			vector <int> temp;

			bool mask[10] = {0};

			for(auto num : v)
				mask[num] = 1;

			for(int num = 1; num <= 9; num++)
				if(!mask[num])
					temp.push_back(1 + 324 + step * 9 + num - 1);

			if(!temp.empty())
				DLX::AddRow(1000 + constraint_masks, temp);

			constraint_masks++;
		}
	}

	vector <int> ans = DLX::Solver();

	for(auto c : ans)
		if(c <= 729)
			board[1 + (c / 9) % 9][1 + c % 9] = 1 + (c / 81) % 9;
		
	for(int i = 1; i <= 9; i++){
		for(int j = 1; j <= 9; j++)
			cout << board[i][j];
		cout << endl;
	}
}

int main(){

	// read in the symbol for each grid
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			cin >> constraint[i][j];
			counter[constraint[i][j]]++;
		}
	}

	// read in the convert number for each symbol
	char symbol;
	int number;
	
	while(cin >> symbol){
		cin >> number;
		converter[symbol] = number;
	}

	// run dancing link to solve Sudoku
	solve();

}