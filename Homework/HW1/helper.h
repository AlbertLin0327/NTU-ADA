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

    /* Start of internal functions */
    void _ins(int x, int k) {
        while (x <= N) arr[x] += k, x += x & -x;
    }

    long long _qr(int x) {
        long long ret = 0;
        while (x) ret += arr[x], x -= x & -x;
        return ret;
    }
    /* End of internal functions */

    // Explicitly fill the whole memory with zero
    // Complexity: O(N)
    void reset() {
        for (int i = N; i >= 0; --i)
            arr[i] = 0;
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