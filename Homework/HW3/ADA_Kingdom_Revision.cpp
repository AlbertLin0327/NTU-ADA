#include <bits/stdc++.h>
using namespace std;

// store the list by adjacent list
class Vertex{
public:
	map <int, long long> adjacent;

	void addEdge(int v, long long w){
		adjacent[v] = w;
	}
};

// some variables
int N;
bool visited[500005];
long long dist[500005];
Vertex Graph[500005];
priority_queue <long long> Q;
vector <long long> path;

void search_length(int v){
	visited[v] = 1;

	for(auto e : Graph[v].adjacent){
		if(!visited[e.first]){
			dist[e.first] = max(dist[e.first], dist[v] + Graph[v].adjacent[e.first]);
			search_length(e.first);
		}
	}
}

long long dfs(int v, int parent){
	long long max_path = 0;
	for(auto e : Graph[v].adjacent){
		long long current_path = 0;

		if(e.first == parent)
			continue;
		
		current_path = dfs(e.first, v);

		if(current_path > max_path)
			swap(max_path, current_path);
		
		path.push_back(current_path);
	}

	return max_path + (parent == 0 ? 0 : Graph[v].adjacent[parent]);
}

void solve(){

	// the starting vertex to obtain the largest path
	search_length(1);

	int max_index = 1;
	for(int i = 2; i <= N; i++)
		if(dist[i] > dist[max_index])
			max_index = i;

	// find the length of each path
	long long length = dfs(max_index, 0);
	sort(path.begin(), path.end(), greater <long long>());

	cout << "0 " << length;
	for(int i = 0; i < N - 2; i++){
		length += path[i];
		cout << " " << length;
	}
	cout << endl;
}

int main(){

	cin.tie(NULL);
	ios_base::sync_with_stdio(false);

	cin >> N;
	for(int i = 0; i < N - 1; i++){
		int u, v;
		long long w;

		cin >> u >> v >> w;
		Graph[u].addEdge(v, w);
		Graph[v].addEdge(u, w);
	}

	solve();
}