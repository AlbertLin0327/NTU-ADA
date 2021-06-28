#include <vector>
using namespace std;
#define FOR(i, way, to) for(int i = way[to] ; i != to ; i = way[i])

namespace DLX {
    int col_size, sz;
    vector<int> s, l, r, u, d, row, col, current;
    void Init(int n) {
        s.clear();
        l.clear();
        r.clear();
        u.clear();
        d.clear();
        row.clear();
        col.clear();
        current.clear();
        col_size = n;
        for (int i = 0; i <= n; ++ i) {
            u.emplace_back(i);
            d.emplace_back(i);
            l.emplace_back(i - 1);
            r.emplace_back(i + 1);
        }
        s.resize(n + 1, 0);
        row.resize(n + 1, 0);
        col.resize(n + 1, 0);
        r[n] = 0, l[0] = n, sz = n + 1;
    }
    void AddRow(int rr, vector<int> &sol) {
        int tmp = sz;
        for (auto to : sol) {
            l.emplace_back(sz - 1);
            r.emplace_back(sz + 1);
            d.emplace_back(to);
            u.emplace_back(u[to]);
            d[u[to]] = sz, u[to] = sz;
            row.emplace_back(rr);
            col.emplace_back(to);
            s[to] ++, sz ++;
        }
        r[sz - 1] = tmp, l[tmp] = sz - 1;
    }
    void Remove(int c) {
        l[r[c]] = l[c];
        r[l[c]] = r[c];
        FOR(i, d, c) FOR(j, r, i) {
            u[d[j]] = u[j];
            d[u[j]] = d[j];
            --s[col[j]];
        }
    }
    void Restore(int c) {
        FOR(i, u, c) FOR(j, l, i) {
            ++s[col[j]], u[d[j]] = d[u[j]] = j;
        }
        l[r[c]] = r[l[c]] = c;
    }
    void Dfs(int floor) {
        if(r[0] == 0) return;
        int c = r[0];
        FOR(i, r, 0) {
            if (s[i] <= s[c]) c = i;
            if (s[i] == 0) return;
        }
        Remove(c);
        FOR(i, d, c) {
            FOR(j, r, i) Remove(col[j]);
            current.emplace_back(row[i]);
            Dfs(floor + 1);
            if (r[0] == 0) return;
            current.pop_back();
            FOR(j, l, i) Restore(col[j]);
        }
        Restore(c);
    }
    vector<int> Solver() {
        return Dfs(0), current;
    }
}

#undef FOR
