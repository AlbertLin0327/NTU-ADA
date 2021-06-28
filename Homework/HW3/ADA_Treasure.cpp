#include <bits/stdc++.h>
using namespace std;

// store the list by adjacent list
class Edge{
public:
	// the ending vertex and the weight of the edge
	int V;
	long long W;

	Edge(int v, long long w){
		V = v;
		W = w;
	}
};

class Vertex{
public:
	long long W;
	vector <Edge> adjacent;

	Vertex(long long w = 0){
		W = w;
	}

	void addEdge(int v = 0, long long w = 0){
		adjacent.push_back(Edge(v, w));
	}
};

// define some constants
int N, M;
vector <Vertex> Graph(500005);
vector <Vertex> transpose_Graph(500005);
vector < set <int> > SCC;
vector <Vertex> DAG(500005);
bool visited[500005];
long long dp[500005];
stack <int> S;

void order(int v){
	visited[v] = 1;

	for(auto iter : Graph[v].adjacent)
		if(!visited[iter.V])
			order(iter.V);
	
	S.push(v);
	return;
}

void dfs_scc(int v, set <int> &scc){
	visited[v] = 1;
	scc.insert(v);

	for(auto iter : transpose_Graph[v].adjacent)
		if(!visited[iter.V])
			dfs_scc(iter.V, scc);

	return;
}

void Kosaraju_Sharir(){

	// order the vertex with finish time (decreasing order)
	for(int i = 1; i <= N; i++)
		if(!visited[i])
			order(i);

	// run DFS on transposed graph to find sink SCC
	for(int i = 1; i <= N; i++)
		visited[i] = 0;

	while(!S.empty()){
		int v = S.top();
		S.pop();

		if(!visited[v]){
			set <int> scc;
			dfs_scc(v, scc);
			SCC.push_back(scc);
		}
	}
}

void new_Graph(){
	int convert[500005];
	int index = 1;

	// make each SCC into new Node
	for(auto list : SCC){

		// merge components of SCC
		for(auto v : list){

			convert[v] = index;

			// add weight of vertex
			DAG[index].W += Graph[v].W;

			// add weight of edge within each SCC
			for(auto e : Graph[v].adjacent)
				if(list.find(e.V) != list.end())
					DAG[index].W += e.W;
		}

		index++;
	}

	// add edge between each SSC
	for(int i = 1; i <= N; i++)
		for(auto e : Graph[i].adjacent)
			if(convert[i] != convert[e.V])
				DAG[convert[i]].addEdge(convert[e.V], e.W);
			
	return;
}

void dfs(int v){
	visited[v] = 1;

	for(auto e : DAG[v].adjacent){
		if(!visited[e.V])
			dfs(e.V);

		dp[v] = max(dp[v], DAG[v].W + e.W + dp[e.V]);
	}
}

void longest_path(){
	int length = DAG.size();

	for(int i = 1; i <= length; i++)
		visited[i] = 0;

	for(int i = 1; i <= length; i++)
		dp[i] = DAG[i].W;

	for(int i = 1; i <= length; i++)
		if(!visited[i])
			dfs(i);
	
	long long ans = 0;
	for(int i = 1; i <= length; i++){
		ans = max(ans, dp[i]);
	}

	cout << ans << endl;
}

void solve(){

	// find each SCC in the directed graph
	Kosaraju_Sharir();

	// convert directed graph into DAG
	new_Graph();

	// find maximum path
	longest_path();
}

int main(){
	
	// read some constants
	cin >> N >> M;

	for(int i = 1; i <= N; i++){
		long long w; cin >> w;
		Graph[i].W = w;
	}

	for(int i = 0; i < M; i++){
		int start, end;
		long long weight;
		cin >> start >> end >> weight;
		Graph[start].addEdge(end, weight);
		transpose_Graph[end].addEdge(start, weight);
	}

	solve();

}