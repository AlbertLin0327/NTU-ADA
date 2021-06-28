#include <bits/stdc++.h>
using namespace std;

#define MAX(x, y) ((x) > (y) ? (x) : (y))

class City{
public:
	long long int x,  y;

	City(long long int a, long long int b){
		x = a; y = b;
	}
};

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

// sort from smallest x to largest x
inline bool cmpX(City* a, City* b){
	return a->x < b->x;
}

// sort from smallest y to largest y
inline bool cmpY(City* a, City* b){
	return a->y < b->y;
}

// get the distance between 2 points
inline long long int cost(City* obj1, City* obj2){
	return (-1) * ((obj1->x - obj2->x) * (obj1->x - obj2->x) + (obj1->y - obj2->y) * (obj1->y - obj2->y));
}

void divide_and_conquer(City* data[], int start, int end, long long int* max_profit){

	// if there are two or three points left, use brute force to calculate them
	if(end - start == 2){
		*max_profit = MAX(cost(data[start], data[start + 1]), *max_profit);
		return;
	}else if(end - start == 3){
		long long int dist[3] = {(cost(data[start], data[start + 1])),\
					   (cost(data[start], data[start + 2])),\
					   (cost(data[start + 1], data[start + 2]))};
		*max_profit = MAX(MAX(MAX(dist[0], dist[1]), dist[2]), *max_profit);
		return;
	}

	// find current maximum between left and right partition
	int mid = (start + end) / 2;
	divide_and_conquer(data, start, mid, max_profit);
	divide_and_conquer(data, mid, end, max_profit);

	// start partition the middle part
	City* middle[end - start];
	int index = 0;

	// find those in distance with data[mid] in the left partition
	for(int i = mid; i >= start; i--){
		if((data[mid]->x - data[i]->x) * (data[mid]->x - data[i]->x) >= (-1) * (*max_profit))
			break;
		
		middle[index++] = data[i];
	}

	// find those in distance with data[mid] in the right partition
	for(int i = mid + 1; i < end; i++){
		if((data[mid]->x - data[i]->x) * (data[mid]->x - data[i]->x) >= (-1) * (*max_profit))
			break;

		middle[index++] = data[i];
	}

	long long int current = LLONG_MIN;
	sort(middle, middle + index, cmpY);

	// check all possibilities
	for(int i = 0; i < index; i++){
		for(int j = i + 1; j < index && j < i + 9; j++){
			if((middle[j]->y - middle[i]->y) * (middle[j]->y - middle[i]->y) >= (-1) * (*max_profit))
				break;

			current = cost(middle[i], middle[j]);
			*max_profit = MAX(current, *max_profit);
		}
	}

	return;
}

int main(){

	// n is the number of city pairs to be read
	int n; n = ReadInt();

	// some necessary variables for further processing
	City* data[n];
	long long int x, y;

	// read pairs of coordinates
	for(int i = 0; i < n; i++)
		data[i] = new City(ReadInt(), ReadInt());
	

	// sort the array by x coordinate
	sort(data, data + n, cmpX);

	// find maximum by divide and conquer
	long long int max_profit = LLONG_MIN;
	divide_and_conquer(data, 0, n, &max_profit);

	printf("%lld\n", max_profit);

	return 0;
}	