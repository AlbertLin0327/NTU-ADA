#include<iostream>
#include<algorithm>
#include<vector>
#include<queue>
#include<bitset>
#include<stack>
#include<random>
#include<chrono>
#include<cmath>
#define OPERSIZE 104
#define SLICESIZE 40
#define TIMEMAX 300000
using namespace std;

// some param
double job_d_weight = 1.21;
double job_s_weight = 0.591;
double job_w_weight = 1.911;
double random_rate = 0.0; // should be 0 ~ 1

struct Oper;
struct Job {
  double w;
  int idx;
  int n_oper;
  int done;
  double job_cp;
  int d;
  vector<Oper> oper;
  int ADM[OPERSIZE][OPERSIZE]={}; //ADM[i][j]==1, j depends on i.
};

vector<Job> jobs;
vector<Job> ans;
vector<Job> origin_jobs;

struct Oper {
  int s, d;
  double w;
  int idx;
  int jobidx;
  bool done;
  double job_cp;
  long long start_time;
  int ready_time;
  vector<int> used_slice;
  int depend;
  vector<int> dependent;
  bool operator<(const Oper& b) const{
    return d > b.d;
  }
};

long long t = 0;
int n_slice;
struct Slice {
  bitset<TIMEMAX> slice[SLICESIZE];
  int used_slice, max_slice;
  int totaloper;
  int doneoper;
  int start_time;
  priority_queue<Oper> pq;
  
  void init(int n = n_slice) {
    max_slice = n;
    for (int i = 0; i <= max_slice; ++i)
      slice[i].reset();
    used_slice = 0;
    start_time = 0;
  }

  vector<int> getIdle(int t) {
    vector<int> ret;
    for (int i = 1; i <= max_slice; ++i)
      if (!(slice[i][t] & 1))
	ret.push_back(i);
    return ret;
  }

  void fillSlice(vector<int>& use, int t, int d) { // must check if silce is enough to insert before call this.
    for (auto& i : use)
      for (int j = t; j < t + d; ++j)
	slice[i][j] = 1;
  }

  void update() {
    while (getIdle(start_time).size() == 0) ++start_time;
  }
  
  vector<int> idleAnd(vector<int>& a, vector<int>& b) {
    vector<int> ret;
    auto i = a.begin(), j = b.begin();
    while (i != a.end() && j != b.end()) {
      if (*i == *j) {
	ret.push_back(*i);
	++i, ++j;
      }
      else if (*i < *j)
	++i;
      else if (*j < *i)
	++j;
    }
    return ret;
  }
  
  int addJob(Job job) {
    int ret = start_time;

    priority_queue<Oper> ready;
    for (int i = 1; i <= job.n_oper; ++i)
      if (job.oper[i].depend == 0)
	ready.push(job.oper[i]);
    job.done = 0;
    while (job.done != job.n_oper) {
      Oper cur = ready.top();
      ready.pop();
      int t = start_time;
      for (auto& de : cur.dependent) {
	if (job.oper[de].start_time + job.oper[de].d > t)
	  t = job.oper[de].start_time + job.oper[de].d;
      }
      bool find = false;
      vector<int> idle;

      while (!find) {
	idle = getIdle(t);
	find = true;
	int tt = t;
	for (; tt < t + cur.d; ++tt) {
	  vector<int> temp = getIdle(tt);
	  idle = idleAnd(idle, temp);
	  if (idle.size() < cur.s) {
	    find = false;
	    break;
	  }
	}
	if (!find)
	  t = tt+1;
      }
      
      while (idle.size() > cur.s) idle.pop_back();
      fillSlice(idle, t, cur.d);
      job.oper[cur.idx].start_time = t;// t - 1
      ans[job.idx].oper[cur.idx].start_time = t; // t -1
      ans[job.idx].oper[cur.idx].used_slice = idle;
      ++job.done;
      if (t + cur.d > ret) ret = t + cur.d; // t - 1

      for (int i = 1; i <= job.n_oper; ++i) {
	if (job.ADM[cur.idx][i] == 1) {
	  job.ADM[cur.idx][i] = 0;
	  if (--job.oper[i].depend == 0)
	    ready.push(job.oper[i]);
	}
      }
    }
    return ret;
  }
  
};
Slice station;

void output(vector<Job>& ans) {
  for (int i = 1; i <= ans.size(); ++i) {
    for (int j = 1; j <= ans[i].n_oper; ++j) {
      cout << ans[i].oper[j].start_time << " ";
      for (auto& sl : ans[i].oper[j].used_slice) {
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
    ans[i].oper.resize(jobs[i].n_oper+1);
    station.totaloper += jobs[i].n_oper;
    double curjob_s = 0.0;
    for (int j = 1; j <= jobs[i].n_oper; ++j) {
      cin >> jobs[i].oper[j].s >> jobs[i].oper[j].d;
      jobs[i].oper[j].w = jobs[i].w;
      jobs[i].oper[j].idx = j;
      jobs[i].oper[j].jobidx = i;
      curjob_s += jobs[i].oper[j].s;
      int depend;
      cin >> depend;
      jobs[i].oper[j].depend = depend;
      for (int k = 1; k <= depend; ++k) {
	int t;
	cin >> t;
	jobs[i].oper[j].dependent.push_back(t);
	jobs[i].ADM[t][j] = 1;
      }
    }

    int curjob_d = station.addJob(jobs[i]);
    station.init(n_slice);
    //    jobs[i].job_cp = jobs[i].w / curjob_d * job_d_weight
    //      + jobs[i].w / curjob_s * job_s_weight
    //      + jobs[i].w * job_w_weight;                         // job_cp here
    jobs[i].job_cp = pow(jobs[i].w, job_w_weight) / (pow(curjob_d, job_d_weight) * pow(curjob_s, job_s_weight));
    for (int j = 1; j <= jobs[i].n_oper; ++j) {
      jobs[i].oper[j].job_cp = jobs[i].job_cp;
    }
  }
  ans = origin_jobs = jobs;    
}

bool job_cmp (Job& a, Job& b) {
  return a.job_cp > b.job_cp;
}
template<typename T>
bool idx_cmp (T& a, T& b) {
  return a.idx < b.idx;
}

void Shuffle(vector<Job>& jobs, double rate) {
  int seed = std::chrono::system_clock::now().time_since_epoch().count();
  mt19937 gen(seed);
  for (int i = 1; i < jobs.size()-1; ++i) {
    int ran = gen() % 100 + 1;
    if (ran < rate * 100) {
      swap(jobs[i], jobs[i+1]);
    }
  }
}

int main(int argc, char *argv[]) {

  int n_job;
  cin >> n_slice >> n_job;
  
  if (argc > 1)
    job_s_weight = atof(argv[1]);
  if (argc > 2)
    job_d_weight = atof(argv[2]);
  
  jobs.resize(n_job+1);
  ans.resize(n_job+1);
  station.init(n_slice);
  readInput(jobs, n_job);

  sort(jobs.begin()+1, jobs.end(), job_cmp);
  for (int i = 0; i < n_job; ++i)
    Shuffle(jobs, random_rate);
  for (int i = 1; i <= n_job; ++i) {
    station.addJob(jobs[i]);
    //    station.update();
  }
  output(ans);
}
