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
	vector <Edge> adjacent;

	void addEdge(int v, long long w){
		adjacent.emplace_back(Edge(v, w));
	}
};

class Candidate{
public:
	int u, v;
	long long w;

	inline bool operator<(const Candidate &obj) const {
		return w < obj.w;
	}

	inline void set(int Val_u, int Val_v, long long Val_w){
		u = Val_u;
		v = Val_v;
		w = Val_w;
	}
};

class Disjoint_set{
public: 
	int rank[300005];
	int previous[300005];

	Disjoint_set(int N){
		for(int i = 1; i <= N; i++){
			rank[i] = 0;
			previous[i] = i;
		}
	}

	inline int find(int u){
		if(u != previous[u])
			previous[u] = find(previous[u]); 
        return previous[u]; 
	}

	void merge(int a, int b){
		a = find(a), b = find(b);

		if(rank[a] > rank[b])
			previous[b] = a;
		else
			previous[a] = b;

		if(rank[a] == rank[b])
			rank[b]++;
	}

};

int N, M, S, T;
Vertex Graph[300005];
Candidate candidates[300005];
long long dist[300005];
bool visited[300005];
list <Candidate*> temporary;
list <Candidate*> result;

struct Compare{
	inline bool operator()(const int &a, const int &b) const {
		return dist[a] > dist[b];
	}
};
priority_queue<int, vector <int>, Compare> Q;

void kruskal(){

	Disjoint_set component(N);
	sort(candidates, candidates + M);

	for(int i = 0; i < M;){
		do{
			int set_a = component.find(candidates[i].u), set_b = component.find(candidates[i].v);
			if(set_a != set_b)
	       		temporary.push_back(&candidates[i]);
			
			i++;
		}while(i != M && candidates[i].w == candidates[i - 1].w);

		while(!temporary.empty()){
			Candidate* current = temporary.back();
			result.push_back(current);
			temporary.pop_back();

			component.merge(component.find(current->u), component.find(current->v));
		}
	}

	M = result.size();

	for(auto e : result){
		Graph[e->u].addEdge(e->v, e->w);
		Graph[e->v].addEdge(e->u, e->w);
	}
}

void dijkstra(int S){
	dist[S] = 0ll;
	Q.push(S);

	for(int i = 1; i <= N; i++)
		if(i != S)
			dist[i] = LLONG_MAX / 10;

	while(!Q.empty()){
		int u = Q.top();
		visited[u] = 1;
		Q.pop();

		for(auto e : Graph[u].adjacent){
			if(dist[e.V] > dist[u] + e.W){
				dist[e.V] = dist[u] + e.W;
				if(!visited[e.V])
					Q.push(e.V);
			}
		}	
	}

	cout << dist[T] << endl;
}

int main(){

	cin.tie(NULL);
	ios_base::sync_with_stdio(false);

	// read constant
	cin >> N >> M >> S >> T;

	for(int i = 0; i < M; i++){
		int u, v;
		long long w;

		cin >> u >> v >> w;
		candidates[i].set(u, v, w);
	}

	// generate an union of all MST
	kruskal();

	// find the shortest path in the union of MST
	dijkstra(S);
}