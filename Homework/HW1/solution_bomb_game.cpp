/*
  Helper header file for HW1 Problem B, NTU CSIE ADA, Fall 2020
 
  Author: ADA TA
 
  Usage:
      This header file provides an one dimensional memory that 
      supports O(log(N)) time range addition operation. You can 
      simply include this header file, or copy and paste this 
      snippet on top of your source code. Feel free to modify 
      this to meet your needs.
*/
 
#ifndef _HELPER_H
#define _HELPER_H
 
#include <cassert>
 
namespace Memory {
    constexpr int N = 200000; // Number of cells, change this if you need more cells
 
    long long arr[N + 1]; // Note: This is NOT the memory itself, please don't try to access
                          //       this array directly unless you know what you're doing
 
    // Explicitly fill the whole memory with zero
    // Complexity: O(N)
    void reset() {
        for (int i = N; i >= 0; --i)
            arr[i] = 0;
    }
 
    void _ins(int x, int k) {
        while (x <= N) arr[x] += k, x += x & -x;
    }
    long long _qr(int x) {
        long long ret = 0;
        while (x) ret += arr[x], x -= x & -x;
        return ret;
    }
    
    // Add k to every cell in range [l, r]
    // Complexity: O(log(N))
    void add(int l, int r, int k) {
        assert(1 <= l && l <= r && r <= N && "add: the argument should satisfy 1 <= l <= r <= N");
        _ins(l, k);
        _ins(r + 1, -k);
    }
 
    // Get the value in cell x
    // Complexity: O(log(N))
    long long get(int x) {
        assert(1 <= x && x <= N && "get: x should be a positive integer in range [1, N]");
        return _qr(x);
    }
}
#endif // _HELPER_H
 
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
#define pb push_back
#define ALL(_a) (_a).begin(), (_a).end()
 
struct player {
    int t, x, exp;
};
 
struct attack {
    int t, l, r, exp, k;
};
 
constexpr int MAXn = 1e5 + 5;
long long ans[MAXn];
 
// [l, r)
void DnQ(int l, int r, vector<player> P, vector<attack> A) {
    if (l == r - 1) return;
    vector<player> Pl, Pr;
    vector<attack> Al, Ar;
    int h = (l + r) / 2;
    for (auto p : P)
        if (p.t < h) Pl.pb(p);
        else Pr.pb(p);
    for (auto a : A)
        if (a.t < h) Al.pb(a);
        else Ar.pb(a);
    DnQ(l, h, Pl, Al);
    DnQ(h, r, Pr, Ar);
    sort(ALL(Pl), [](auto &a, auto &b) { return a.exp > b.exp; });
    sort(ALL(Ar), [](auto &a, auto &b) { return a.exp > b.exp; });
    int it = 0;
    for (auto p : Pl) {
        while (it < (int)Ar.size() && Ar[it].exp >= p.exp)
            Memory::add(Ar[it].l, Ar[it].r, Ar[it].k), it++;
        ans[p.t] += Memory::get(p.x);
    }
    while (it > 0)
        --it, Memory::add(Ar[it].l, Ar[it].r, -Ar[it].k);
}
 
int main() {
    int n, m;
    cin >> n >> m;
    vector<player> P;
    vector<attack> A;
    for (int i = 1; i <= m; i++) {
        char c;
        cin >> c;
        if (c == 'P') {
            int x, exp;
            cin >> x >> exp;
            P.pb({i, x, exp});
        } else {
            int l, r, exp, k;
            cin >> l >> r >> exp >> k;
            A.pb({i, l, r, exp, k});
        }
    }
    DnQ(1, m + 1, P, A);
    for (auto p : P)
        cout << ans[p.t] << "\n";
}
 