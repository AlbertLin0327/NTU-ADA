#include <bits/stdc++.h>
using namespace std;

class Cluster{
public:
	int time;
	long long type, size;

	Cluster(int ti, long long ty, long long s){
		time = ti;
		type = ty;
		size = s;
	}
};

int main(){

	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	// read some constants
	int N, Q;
	cin >> N >> Q;

	long long C, t, k;
	string instruction;

	// deque to record the line and also remember the number of each type
	deque <Cluster> line;
	unordered_map <int, long long> counter;
	
	for(int i = 0; i < Q; i++){
		cin >> instruction >> C >> t >> k;
		Cluster newCluster(i, t, C + k);

		// deal with front and back
		if(instruction == "front"){
			while(k > 0){

				// if k is more that the front element
				if(k >= line.front().size){
					k -= line.front().size;
					counter[line.front().type] += (i - line.front().time) * line.front().size;
					line.pop_front();

				// if k is less
				}else{
					line.front().size -= k;
					counter[line.front().type] += (i - line.front().time) * k;
					k = 0;
				}
			}
			line.push_front(newCluster);
		}else{

			// quite the same as front
			while(k > 0){
				if(k >= line.back().size){
					k -= line.back().size;
					counter[line.back().type] += (i - line.back().time) * line.back().size;
					line.pop_back();
				}else{
					line.back().size -= k;
					counter[line.back().type] += (i - line.back().time) * k;
					k = 0;
				}
			}
			line.push_back(newCluster);
		}
	} 

	// deal with the element left
	while(!line.empty()){
		counter[line.front().type] += (Q - line.front().time) * line.front().size;
		line.pop_front();
	}

	for(int i = 1; i <= N; i++)
		cout << counter[i] << " ";
	cout << endl;
}