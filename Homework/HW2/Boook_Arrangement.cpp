#include <bits/stdc++.h>
using namespace std;

int N, end_of_nonzero, start_of_nonzero;
bool print;
long long previous1, previous2;
long long book1[100002], book2[100002];
long long additional1, additional2;

void greedy(){

	 // make sure there's at least one book in the range
	for(int i = start_of_nonzero; i <= end_of_nonzero; i++){

		// if there are books, subtract one from them
		if(book1[i])
			book1[i]--;	
		else
			additional1++;
		
		book2[i] = book1[i];
	}

	// renew the addition counter
	additional2 = additional1;

	// record the previous1 number
	previous1 = book1[start_of_nonzero];
	previous2 = book2[start_of_nonzero];

	// the case of leading number is start_of_nonzero
	if(!book2[start_of_nonzero + 1])
		additional2++;
	else
		book2[start_of_nonzero + 1]--;

	// the case of leading number is start_of_nonzero
	for(int i = start_of_nonzero; i < end_of_nonzero - 1; i++){
		if(previous1 > book1[i + 1]){
			additional1 += (previous1 - book1[i + 1]);
			book1[i + 1] = previous1;
		}

		if(previous2 > book2[i + 1]){
			additional2 += (previous2 - book2[i + 1]);
			book2[i + 1] = previous2;
		}

		// record the book left
		previous1 = book1[i + 1] - previous1;
		previous2 = book2[i + 1] - previous2;
	}

	// the last case
	if(previous1 < book1[end_of_nonzero]){
		additional1 += (book1[end_of_nonzero] - previous1);
		book1[end_of_nonzero - 1] += (book1[end_of_nonzero] - previous1);
	}else if(previous1 > book1[end_of_nonzero] + 1){
		additional1 += (previous1 - 1 - book1[end_of_nonzero]);
		book1[end_of_nonzero] = (previous1 - 1);
	}

	// the last case
	if(previous2 < book2[end_of_nonzero]){
		additional2 += (book2[end_of_nonzero] - previous2);
		book2[end_of_nonzero - 1] += (book2[end_of_nonzero] - previous2);
	}else if(previous2 > book2[end_of_nonzero] + 1){
		additional2 += (previous2 - 1 - book2[end_of_nonzero]);
		book2[end_of_nonzero] = (previous2 - 1);
	}

	cout << (additional1 < additional2 ? additional1 : additional2) << endl;

	if(print){
		if(additional1 <= additional2){
			for(int i = start_of_nonzero; i < end_of_nonzero; i++){
				cout << i << " ";
				for(int j = 0; j < book1[i]; j++)
					cout << i + 1 << " " << i << " ";

				book1[i + 1] -= book1[i];
				book1[i] = 0;
			}

			if(book1[end_of_nonzero] >= 0)
				cout << end_of_nonzero;
		}else{
			cout << start_of_nonzero + 1 << " ";
			for(int i = start_of_nonzero; i < end_of_nonzero; i++){
				cout << i << " ";
				for(int j = 0; j < book2[i]; j++)
					cout << i + 1 << " " << i << " ";

				book2[i + 1] -= book2[i];
				book2[i] = 0;
			}

			if(book2[end_of_nonzero] >= 0)
				cout << end_of_nonzero;
		}
	}
}

void base_case(){

	cout << book1[start_of_nonzero] - 1 << endl;
	
	if(print){
		if(start_of_nonzero == 1){
			cout << 1 << " ";
			for(int i = 0; i < book1[start_of_nonzero] - 1; i++)
				cout << 2 << " " << 1 << " ";
			cout << endl;
		}else{
			cout << start_of_nonzero << " ";
			for(int i = 0; i < book1[start_of_nonzero] - 1; i++)
				cout << start_of_nonzero - 1 << " " << start_of_nonzero << " ";
			cout << endl;
		}
	}
	
}

int main(){

	// read constant
	cin >> N; 
	for(int i = 1; i <= N; i++){
		cin >> book1[i];

		// find the first non-zero
		if(book1[i] && !start_of_nonzero)
			start_of_nonzero = i;

		// find the last non-zero
		if(book1[i])
			end_of_nonzero = i;
	}
	cin >> print;

	// perform greedy
	if(start_of_nonzero != end_of_nonzero)
		greedy();
	else
		base_case();
}