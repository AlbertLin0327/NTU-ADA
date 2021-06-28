#include <bits/stdc++.h>
#include "helper.h"
using namespace std;

enum MODE{PLAYER, ATTACK};
long long instructions[100005][5];
int inst_seq[100005];
long long damage[100005];

void combine(int start, int middle, int end){

	// tmp array storing the newest sequence
	int ptr1 = start, ptr2 = middle;
	int tmp[end - start + 2];
	int num = 0;

	vector <int> add_list;

	while(ptr1 < middle && ptr2 < end){

		// when play at left and bomb at right
		if(instructions[inst_seq[ptr1]][1] > instructions[inst_seq[ptr2]][1]){
			if(instructions[inst_seq[ptr1]][0] == PLAYER)
				damage[instructions[inst_seq[ptr1]][3]] += Memory::get(instructions[inst_seq[ptr1]][2]);
			
			tmp[num++] = inst_seq[ptr1++];
		}else{
			if(instructions[inst_seq[ptr2]][0] == ATTACK){
				add_list.push_back(inst_seq[ptr2]);
				Memory::add(instructions[inst_seq[ptr2]][3], instructions[inst_seq[ptr2]][4], instructions[inst_seq[ptr2]][2]);
			}
			
			tmp[num++] = inst_seq[ptr2++];
		}
	}

	// fill in the rest of the left half
	while(ptr1 < middle){
		if(instructions[inst_seq[ptr1]][0] == PLAYER)
			damage[instructions[inst_seq[ptr1]][3]] += Memory::get(instructions[inst_seq[ptr1]][2]);
		
		tmp[num++] = inst_seq[ptr1++];
	}

	// fill in the rest of the right half
	while(ptr2 < end)
		tmp[num++] = inst_seq[ptr2++];

	// update to the instruction array
	for(int i = 0; i < num; i++)
		inst_seq[start + i] = tmp[i];

	for(auto i : add_list)
		Memory::add(instructions[i][3], instructions[i][4], -1 * instructions[i][2]);
	
	return;
}

void divide_and_conquer(int start, int end){
	if(end - start <= 1)
		return;

	int middle = (end + start) / 2;
	divide_and_conquer(start, middle);
	divide_and_conquer(middle, end);

	combine(start, middle, end);
}

int main(){

	Memory::reset();

	// read constants
	int n, m; cin >> n >> m;
	int player_id = 0;

	for(int i = 0; i < m; i++){

		char mode; cin >> mode;
		inst_seq[i] = i;

		if(mode == 'P'){
			instructions[i][0] = PLAYER;

			long long c, d; cin >> c >> d;

			instructions[i][1] = d, instructions[i][2] = c;
			instructions[i][3] = player_id++;
		}else{
			instructions[i][0] = ATTACK;

			long long l, r, p, k;
			cin >> l >> r >> p >> k;

			instructions[i][1] = p, instructions[i][2] = k;
			instructions[i][3] = l, instructions[i][4] = r;
		}
	}

	divide_and_conquer(0, m);

	for(int i = 0; i < player_id; i++)
		cout << damage[i] << "\n";

	cout << flush;
}