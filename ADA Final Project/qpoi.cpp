#include<iostream>
#include<cstdio>
#include<algorithm>
#include<vector>
#include<queue>
#include<stack>
#define OPERSIZE 104
using namespace std;

struct Oper {
  int s, d;
  double w;
  int idx;
  int jobidx;
  long long start_time;
  vector<int> used_slice;
  int depend;
  bool operator<(const Oper& b) const{
    return (double)w/(d*s) < (double)b.w/(b.d*b.s);
  }
};

long long t = 0;

struct Job {
  double w;
  int idx;
  int n_oper;
  int done;
  vector<Oper> oper;
  int ADM[OPERSIZE][OPERSIZE]={}; //ADM[i][j]==1, j depends on i.
};

struct Slice {
  int slice[100]; // record end time.
  int user_job[100];
  int user_oper[100];
  int used_slice, max_slice;
  int totaloper;
  int doneoper;
  priority_queue<Oper> pq;
  
  void init(int n) {
    for (int i = 0; i <= n; ++i) {
      slice[i] = user_job[i] = user_oper[i] = -1;
    }
    used_slice = 0;
    max_slice = n;
    totaloper = doneoper = 0;
  }

  void doneJob(vector<Job>& jobs) {
    for (int i = 1; i <= max_slice; ++i) {
      if (slice[i] == t) {
	int cj = user_job[i], cop = user_oper[i];
	used_slice -= jobs[cj].oper[cop].used_slice.size();
	for (auto& k : jobs[cj].oper[cop].used_slice) {
	  slice[k] = -1;    // clear other slice shared by same oper.
	  user_job[k] = -1;
	  user_oper[k] = -1;
	}
	for (int j = 1; j <= jobs[cj].n_oper; ++j) {
	  if (jobs[cj].ADM[cop][j] != 0) {
	    jobs[cj].ADM[cop][j] = 0;
	    if (--jobs[cj].oper[j].depend == 0) {
	      addOper(jobs[cj], jobs[cj].oper[j]);//push to queue.
	    }	    
	  }
	}
      }
    }
  }

  void addOper(Job& job, Oper& op) {
    pq.push(op);
  }
  void doJob(vector<Job>& jobs) {
    stack<Oper> store;
    while (used_slice != max_slice && !pq.empty()) {
      Oper cur = pq.top();
      pq.pop();
      int cj = cur.jobidx, cop = cur.idx;
      if (cur.s <= max_slice - used_slice) {
	int count = 0;
	doneoper+=1;
	jobs[cj].oper[cop].start_time = t;
	for (int i = 1; count < cur.s; ++i) {
	  if (slice[i] == -1) {
	    slice[i] = t + cur.d;
	    user_job[i] = cj;
	    user_oper[i] = cop;
	    ++count;
	    jobs[cj].oper[cop].used_slice.push_back(i);
	  }
	}
	used_slice += cur.s;
      }
      else
	store.push(cur);
    }
    while (!store.empty()){
      pq.push(store.top());
      store.pop();
    }
  }
};
Slice station;

void output(vector<Job>& jobs) {
  for (int i = 1; i <= jobs.size(); ++i) {
    for (int j = 1; j <= jobs[i].n_oper; ++j) {
      cout << jobs[i].oper[j].start_time << " ";
      for (auto& sl : jobs[i].oper[j].used_slice) {
	cout << sl << " ";
      }
      cout << "\n";
    }
  }
}

void readInput(vector<Job>& jobs, int n_job) {
  for (int i = 1; i <= n_job; ++i) {
    cin >> jobs[i].n_oper >> jobs[i].w;
    jobs[i].idx = i;
    jobs[i].oper.resize(jobs[i].n_oper+1);
    station.totaloper += jobs[i].n_oper;
    for (int j = 1; j <= jobs[i].n_oper; ++j) {
      cin >> jobs[i].oper[j].s >> jobs[i].oper[j].d;
      jobs[i].oper[j].w = jobs[i].w;
      jobs[i].oper[j].idx = j;
      jobs[i].oper[j].jobidx = i;
      int depend;
      cin >> depend;
      jobs[i].oper[j].depend = depend;
      for (int k = 0; k < depend; ++k) {
	int t;
	cin >> t;
	jobs[i].ADM[t][j] = 1;
      }
    }		  
  }
}

void Firstjob(Slice& slice, vector<Job>& jobs) {
  for (auto& J : jobs) {
    for (int i = 1; i <= J.n_oper; ++i) {
      if (J.oper[i].depend == 0) {
	slice.addOper(J, J.oper[i]);
      }
    }
  }
}

int main() {
  //  cin.tie(NULL);
  //ios::sync_with_stdio(false);
  int n_slice, n_job;
  vector<Job> jobs(n_job+1);
  int total_done = 0;
  cin >> n_slice >> n_job;
  station.init(n_slice);
  
  readInput(jobs, n_job);

  Firstjob(station, jobs);
  /*
  for (int i = 1; i <= n_job; ++i) {
    cout << "i=" << i << endl;
    cout << jobs[i].idx << endl;
    for (int j = 1; j <= jobs[i].n_oper; ++j) {
      cout << jobs[i].oper[j].idx << " " << jobs[i].oper[j].jobidx << endl;
    }
  }
  */
  while (station.doneoper != station.totaloper) {
    station.doneJob(jobs);
    station.doJob(jobs);
    t += 1;
  }
  output(jobs);
  
}
