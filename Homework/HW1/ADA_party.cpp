#include <bits/stdc++.h>
using namespace std;

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

long long N, K, total;
long long candies[500005];
long long right_table[250005][3];
long long left_table[250005][3];
long long module[500005];

inline long long ReadInt(){
    int c;
    do
        c = fgetc(stdin);
    while ( (c < '0' || c > '9') && c != EOF );

    long long a = 0;
    while ( c >= '0' && c <= '9'){
        a = a * 10 + (c - '0');
        c = fgetc(stdin);
    }
    return a;
}

void conquer(int start, int middle, int end){

	// construct the table of left half sub-arrays [middle + 1, end - 1]
	right_table[0][0] = 0, right_table[0][1] = INT_MIN, right_table[0][2] = INT_MAX;

	// if both MAX and min is at the left hand side
	left_table[0][0] = 0, left_table[0][1] = INT_MIN, left_table[0][2] = INT_MAX;

	for(int i = 1; i <= middle + 1 - start; i++){

		// value from middle to start
		// MAX from middle to start
		// min from middle to start
		left_table[i][0] = (left_table[i - 1][0] + candies[middle + 1 - i]) % K;
		left_table[i][1] = MAX(left_table[i - 1][1], candies[middle + 1 - i]);
		left_table[i][2] = MIN(left_table[i - 1][2], candies[middle + 1 - i]);

	}

	// if both MAX and min is at the right hand side
	int ptr = 0;
	for(int i = 1; i <= end - middle; i++){

		right_table[i][0] = (right_table[i - 1][0] + candies[middle + i]) % K;
		right_table[i][1] = MAX(right_table[i - 1][1], candies[middle + i]);
		right_table[i][2] = MIN(right_table[i - 1][2], candies[middle + i]);

		// find the last case which fulfilled the condition on the left hand side
		while(ptr + 1 <= middle - start + 1 && left_table[ptr + 1][1] <= right_table[i][1] && left_table[ptr + 1][2] > right_table[i][2])
			module[(left_table[++ptr][0] % K)]++;
		
		if(ptr != 0 && left_table[ptr][1] <= right_table[i][1] && left_table[ptr][2] > right_table[i][2])
			total += module[(K - (right_table[i][0] - right_table[i][1] - right_table[i][2]) % K) % K];
	}

	// hash map to store mod --> information
	for(int i = 0; i <= ptr; i++)
		module[(left_table[i][0] % K)] = 0;

	// if both MAX and min is at the left hand side
	ptr = 0;
	for(int i = 1; i <= middle + 1 - start; i++){

		// find the last case which fulfilled the condition on the right hand side
		while(ptr + 1 <= end - middle && left_table[i][1] > right_table[ptr + 1][1] && left_table[i][2] <= right_table[ptr + 1][2])
			module[(right_table[++ptr][0] % K)]++;

		if(ptr != 0 && left_table[i][1] > right_table[ptr][1] && left_table[i][2] <= right_table[ptr][2])
			total += module[(K - (left_table[i][0] - left_table[i][1] - left_table[i][2]) % K) % K];
	}

	// hash map to store mod --> information
	for(int i = 0; i <= ptr; i++)
		module[(right_table[i][0] % K)] = 0;

	// if MAX at left and min at right
	int lptr = 1, rptr1 = 0, rptr2 = 0;
	for(; lptr <= middle + 1 - start; lptr++){

		while(rptr1 + 1 <= end - middle && right_table[rptr1 + 1][1] < left_table[lptr][1] && right_table[rptr1 + 1][2] >= left_table[lptr][2]){
			module[(K + (right_table[rptr1 + 1][0] - right_table[rptr1 + 1][2]) % K) % K]--;
			rptr1++;
		}

		while((rptr2 < rptr1) || (rptr2 + 1 <= end - middle && right_table[rptr2 + 1][1] < left_table[lptr][1] && right_table[rptr2 + 1][2] <= left_table[lptr][2])){
			module[(K + (right_table[rptr2 + 1][0] - right_table[rptr2 + 1][2]) % K) % K]++;
			rptr2++;
		}

		if(rptr2 <= end - middle && right_table[rptr2][1] < left_table[lptr][1] && right_table[rptr2][2] <= left_table[lptr][2])
			total += module[(K - (left_table[lptr][0] - left_table[lptr][1]) % K) % K];
	}

	// hash map to store mod --> information
	for(int i = 0; i <= rptr2; i++)
		module[(K + (right_table[i][0] - right_table[i][2]) % K) % K] = 0;

	// if MAX at right and min at left
	int lptr1 = 0, lptr2 = 0, rptr = 1;
	for(; rptr <= end - middle; rptr++){

		while(lptr1 + 1 <= middle + 1 - start && left_table[lptr1 + 1][1] <= right_table[rptr][1] && left_table[lptr1 + 1][2] > right_table[rptr][2]){
			
			module[(K + (left_table[lptr1 + 1][0] - left_table[lptr1 + 1][2]) % K) % K]--;
			lptr1++;
		}

		while((lptr2 < lptr1) || (lptr2 + 1 <= middle + 1 - start && left_table[lptr2 + 1][1] <= right_table[rptr][1] && left_table[lptr2 + 1][2] <= right_table[rptr][2])){	
			module[(K + (left_table[lptr2 + 1][0] - left_table[lptr2 + 1][2]) % K) % K]++;
			lptr2++;
		}

		if(lptr2 <= middle + 1 - start && right_table[rptr][1] >= left_table[lptr2][1] && right_table[rptr][2] >= left_table[lptr2][2])
			total += module[(K - (right_table[rptr][0] - right_table[rptr][1]) % K) % K];
	}

	for(int i = 0; i <= lptr2; i++)
		module[(K + (left_table[i][0] - left_table[i][2]) % K) % K] = 0;

	return;
}

void divide(int start, int end){
	if(end - start == 0)
		return;
	else if(end - start == 1){
		total++;
		return;
	}

	int middle = (start + end) / 2;

	divide(start, middle);
	divide(middle + 1, end);

	conquer(start, middle, end);
	return;
}

int main(){

	N = ReadInt();
	K = ReadInt();

	for(int i = 1; i <= N; i++)
		candies[i] = ReadInt();
		
	divide(1, N);
	
	printf("%lld\n", total);

	return 0;
}