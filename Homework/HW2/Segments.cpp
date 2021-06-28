#include <bits/stdc++.h>
#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define MAX(x,y) ((x) > (y) ? (x) : (y))

// Fenwick tree implementation
// Credit: ADA TA 
namespace Memory {
    constexpr long long N = 200005; // Number of cells, change this if you need more cells

    long long arr[N + 1]; // Note: This is NOT the memory itself, please don't try to access
                          //       this array directly unless you know what you're doing

    /* Start of internal functions */
    void _ins(long long x, long long k) {
        while (x <= N) arr[x] += k, x += x & -x;
    }

    long long _qr(long long x) {
        long long ret = 0;
        while (x) ret += arr[x], x -= x & -x;
        return ret;
    }
    /* End of internal functions */

    // Explicitly fill the whole memory with zero
    // Complexity: O(N)
    void reset() {
        for (long long i = N; i >= 0; --i)
            arr[i] = 0;
    }
    
    // Add k to every cell in range [l, r]
    // Complexity: O(log(N))
    void add(long long l, long long r, long long k) {
    	l++; r++; // shift index to match our usage
        _ins(l, k);
        _ins(r + 1, -k);
    }

    // Get the value in cell x
    // Complexity: O(log(N))
    long long get(long long x) {
    	x++; // shift index to match our usage
        return _qr(x);
    }
}

using namespace std;

enum STATUS{GREATER, LESS};

class Segment{
private:
	long long W, L, R, index;

public:
	long long getIndex() const{
		return index;
	}

	void setIndex(long long x){
		index = x;
	}

	long long getL() const{
		return L;
	}

	long long getR() const{
		return R;
	}

	long long getW() const{
		return W;
	}

	void setL(long long x){
		L = x;
	}

	void setR(long long x){
		R = x;
	}

	void setW(long long x){
		W = x;
	}
};

// some constants
long long N, M;
long long K;
Segment segment_set[200005];
long long segment_set_index[200005];

// fast I/O
inline long long ReadInt(){
    char c;
    do
        c = getc_unlocked(stdin);
    while ( (c < '0' || c > '9') && c != EOF );

    long long a = 0;
    while ( c >= '0' && c <= '9'){
        a = a * 10 + (c - '0');
        c = getc_unlocked(stdin);
    }
    return a;
}

inline void WriteInt(long long n) { 
	if(n < 0){ 
		n = -n; 
		putchar_unlocked('-'); 
	} 

	int i = 21; 
	char output_buffer[21]; 
	do{ 
		output_buffer[--i] = (n % 10) + '0'; 
		n /= 10; 
	}while(n); 

	do{ 
		putchar_unlocked(output_buffer[i]); 
	}while(++i < 21); 
} 

long long weight[200005];

struct priority{
	bool operator()(const long long &a, const long long &b){
		return segment_set[segment_set_index[a]].getR() < segment_set[segment_set_index[b]].getR();
	}
};

inline STATUS greedy(long long V){	
	
	long long chosen = K;
	long long pq_index = 0;

	// set as a priority queue
	priority_queue <long long, vector <long long>, priority> pq;

	// check that every index i is greater than V
	for(long long i = 0; i < M; i++){

		//continue to add to priority queue
		long long current = Memory::get(i);

		if(current >= V)
			continue;

		while(current < V){

			// compare function type = less
			while(pq_index < N && segment_set[segment_set_index[pq_index]].getL() <= i)
				if(segment_set[segment_set_index[pq_index]].getR() > i)
					pq.push(pq_index++);
				else
					pq_index++;

			// if this segment isn't usable then pop it
			while(!pq.empty() && (segment_set[segment_set_index[pq.top()]].getR() <= i || segment_set[segment_set_index[pq.top()]].getW() == weight[pq.top()]))
				pq.pop();

			// if there is no segments left or we've already chosen K segments
			if(pq.empty() || chosen < 0)
				return LESS;

			long long add_weight = MIN(V - current, segment_set[segment_set_index[pq.top()]].getW() - weight[pq.top()]);
			if(add_weight > 0){

				chosen -= add_weight;
				current += add_weight;
				weight[pq.top()] += add_weight;

				Memory::add(segment_set[segment_set_index[pq.top()]].getL(), segment_set[segment_set_index[pq.top()]].getR() - 1, add_weight);
				
			}
			
		}
	}

	return chosen < 0 ? LESS : GREATER;
}

inline void clear(){
	for(long long i = MAX(N, M); i >= 0; i--){
		weight[i] = 0ll;
		Memory::arr[i] = 0ll;
	}
}

inline long long bsearch(long long start, long long end){

	if(start < end){
		clear();
		long long V = start + (end - start) / 2;
		
		if(greedy(V) == GREATER)
			return bsearch(V + 1, end);
		else
			return bsearch(start, V - 1);
		
	}
	return end;
}

inline bool segment_cmp(const long long &a, const long long &b){
	return segment_set[a].getL() <= segment_set[b].getL();
}

int main(){

	// read some constants
	N = ReadInt();
	M = ReadInt();
	K = ReadInt();

	for(long long i = 0; i < N; i++){
		segment_set[i].setL(ReadInt());
		segment_set[i].setR(ReadInt());
		segment_set[i].setW(ReadInt());
		segment_set[i].setIndex(i);
	}

	// sort segment set
	long long idx = 0;
	iota(segment_set_index, segment_set_index + N, idx++);
	stable_sort(segment_set_index, segment_set_index + N, segment_cmp);

	// find the best
	long long best = bsearch(0, K);
	
	clear();
	if(greedy(best) == LESS)
		greedy(--best);

	WriteInt(best);
	putchar_unlocked('\n');

	// find the cost of each segment set
	long long sequence[N];
	for(int i = 0; i < N; i++)
		sequence[segment_set[segment_set_index[i]].getIndex()] = weight[i];

	for(int i = 0; i < N; i++){
		WriteInt(sequence[i]);
		putchar_unlocked(' ');
	}

	return 0;
}