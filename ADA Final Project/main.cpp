#include <bits/stdc++.h>
using namespace std;

class Operation{
public:

	// required computing Slices, the Duration required, and the number of Job per operations
	int slice, duration, number_of_job;
	int operation_index;
	int finish_time;
	vector <int> depends;
};

class Job{
public:
	
	int m; // number of operations in a job
	double w; // weight of the job
	int job_index; 
	int finish_time;
	
	vector <Operation> tasks;

	Job(int a, double b){
		m = a;
		b = w;
	}
};


int L, N;
vector <Job> jobs;

int main(){
	cin >> L >> N;

	for(int i = 0; i < N; i++){
		int m, w; 
		cin >> m >> w;

		jobs.push_back(Job(m, w));

		for(int j = 0; j < m; j++){
			Operation temp;
            cin >> temp.slice >> temp.duration >> temp.number_of_job;

            for(int k = 0; k < temp.number_of_job; k++){
                int previous;
                cin >> previous;
                temp.depends.push_back(previous);
            }

            jobs[i].tasks.push_back(temp);
		}
	}
}