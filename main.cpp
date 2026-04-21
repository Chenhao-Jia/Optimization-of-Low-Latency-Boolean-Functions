#include <bits/stdc++.h>
using namespace std;

// ---------- Helpers: subsets, partitions, etc. ----------

static inline int popcount_int(int x){ return __builtin_popcount((unsigned)x); }

// Return vector of ints (var indices) present in mask
static vector<int> mask_to_vars(int mask, int n){
    vector<int> v;
    for(int i=0;i<n;i++) if(mask & (1<<i)) v.push_back(i);
    return v;
}

// Enumerate all non-empty subsets of a given set of elements (by indices in "vars")
static vector<vector<int>> all_nonempty_subsets(const vector<int>& vars){
    vector<vector<int>> res;
    int m = (int)vars.size();
    for(int bm=1; bm < (1<<m); ++bm){
        vector<int> s;
        for(int i=0;i<m;i++) if(bm & (1<<i)) s.push_back(vars[i]);
        res.push_back(move(s));
    }
    return res;
}

// Generate all set partitions of a list "U" (unique, order of blocks does not matter).
static void partitions_rec(int idx, const vector<int>& U, vector<vector<int>>& cur, vector<vector<vector<int>>>& out){
    if(idx == (int)U.size()){
        out.push_back(cur);
        return;
    }
    int x = U[idx];
    // put x into existing block
    for(size_t i=0;i<cur.size();++i){
        cur[i].push_back(x);
        partitions_rec(idx+1, U, cur, out);
        cur[i].pop_back();
    }
    // or create a new block
    cur.push_back({x});
    partitions_rec(idx+1, U, cur, out);
    cur.pop_back();
}
static vector<vector<vector<int>>> all_partitions(vector<int> U){
    vector<vector<vector<int>>> out;
    vector<vector<int>> cur;
    partitions_rec(0, U, cur, out);
    // normalize each block (sort elements) and canonicalize by sorting blocks lexicographically
    for(auto& part: out){
        for(auto& b: part) sort(b.begin(), b.end());
        sort(part.begin(), part.end());
    }
    // unique
    sort(out.begin(), out.end());
    out.erase(unique(out.begin(), out.end()), out.end());
    return out;
}

// Utility: monomial mask from list of variable indices
static int vars_to_mask(const vector<int>& v){
    int m=0; for(int x: v) m |= (1<<x); return m;
}

// Join with commas
static string join(const vector<string>& a, const string& sep){
    string s;
    for(size_t i=0;i<a.size();++i){
        if(i) s += sep;
        s += a[i];
    }
    return s;
}

// Zero-extend 1-bit var to W bits in STP CVC: (0bin000...0 @ var)
static string zext(const string& bit, int W){
    string hi(W-1, '0');
    return "(0bin" + hi + " @ " + bit + ")";
}

// --- Simple ANF string parser ---
static void parse_anf_string(const string& anf, int n, vector<int>& b) {
    const int MONS = 1 << n;
    b.assign(MONS, 0);
    int i = 0, L = (int)anf.size();
    auto skip = [&](){
        while (i<L && (anf[i]==' ' || anf[i]=='\t' || anf[i]=='\n' || anf[i]=='\r' || anf[i]=='+'))
            ++i;
    };
    while (true) {
        skip();
        if (i>=L) break;
        int mask = 0;
        if (anf[i]=='1') { // constant
            mask = 0; ++i;
        } else {
            bool any = false;
            while (i<L && anf[i]=='x') {
                ++i;
                int idx = 0; bool has = false;
                while (i<L && isdigit((unsigned char)anf[i])) { has=true; idx = idx*10 + (anf[i]-'0'); ++i; }
                if (has && idx>=0 && idx<n) { mask |= (1<<idx); any=true; }
            }
            if (!any) { ++i; continue; }
        }
        b[mask] ^= 1;
        skip();
        if (i>=L) break;
        if (anf[i]=='+') ++i;
    }
}

// ---------- Model Generator ----------

int main(){
    const int n = 8;               // number of variables
    const int MONS = 1<<n;         // 256 monomials
    const int W_cover = 16;        // width for cover sums (be generous)
    const int W_cost  = 16;        // width for cost sums

    // === 8-bit ANF provided by user ===

    /*
    ****************************AES Sbox***********************************

    const string anf_str =
            "x0 + x2 + x3 + x5 + x0x2 + x0x6 + x0x7 + x1x3 + x1x5 + x1x7 + x2x4 + x3x5 + x5x6 + x5x7 "
            "+ x0x1x3 + x0x1x5 + x0x1x7 + x0x2x3 + x0x2x4 + x0x3x6 + x0x4x7 + x0x5x7 "
            "+ x1x2x3 + x1x2x5 + x1x2x7 + x1x3x5 + x1x4x6 + x1x4x7 + x1x5x6 + x1x6x7 "
            "+ x2x3x4 + x2x3x7 + x2x4x5 + x2x4x6 + x2x4x7 + x2x5x7 + x2x6x7 "
            "+ x3x6x7 + x4x5x6 + x4x5x7 "
            "+ x0x1x2x3 + x0x1x2x5 + x0x1x2x6 + x0x1x3x5 + x0x1x3x6 + x0x1x3x7 + x0x1x4x6 + x0x1x4x7 "
            "+ x0x2x3x6 + x0x2x4x7 + x0x3x4x5 + x0x3x4x6 + x0x3x4x7 + x0x3x5x7 + x0x3x6x7 + x0x4x5x6 + x0x4x5x7 + x0x5x6x7 "
            "+ x1x2x3x4 + x1x2x4x7 + x1x3x4x7 + x1x3x5x6 + x1x3x5x7 + x1x4x5x7 + x1x4x6x7 "
            "+ x2x3x5x6 + x2x4x5x6 + x2x4x5x7 + x2x5x6x7 "
            "+ x3x4x5x6 + x3x4x6x7 + x3x5x6x7 + x4x5x6x7 "
            "+ x0x1x2x3x4 + x0x1x2x3x6 + x0x1x2x4x6 + x0x1x2x4x7 + x0x1x2x5x6 + x0x1x2x6x7 "
            "+ x0x1x3x4x7 + x0x1x3x5x6 + x0x1x5x6x7 "
            "+ x0x2x3x6x7 + x0x2x5x6x7 "
            "+ x0x3x4x5x7 + x0x3x4x6x7 "
            "+ x1x2x3x4x5 + x1x2x3x4x7 + x1x2x3x5x6 + x1x2x3x6x7 + x1x2x4x5x6 "
            "+ x1x3x4x5x7 + x1x3x5x6x7 + x1x4x5x6x7 "
            "+ x2x3x4x5x7 + x2x3x4x6x7 + x3x4x5x6x7 "
            "+ x0x1x2x3x4x6 + x0x1x2x3x5x7 + x0x1x2x4x5x7 + x0x1x2x4x6x7 "
            "+ x0x1x3x4x5x7 + x0x1x3x4x6x7 + x0x1x4x5x6x7 "
            "+ x0x2x3x4x5x7 + x0x2x3x4x6x7 + x0x2x3x5x6x7 "
            "+ x1x2x3x4x5x7 "
            "+ x0x1x2x3x4x5x7 + x0x1x2x3x4x6x7"; //f0

    const string anf_str =
            "1 + x1 + x2 + x4 + x0x2 + x0x4 + x0x6 + x0x7 + x1x3 + x2x4 + x2x7 + x3x7 + x4x5 + x4x6 "
            "+ x0x1x2 + x0x1x4 + x0x1x6 + x0x2x4 + x0x2x7 + x0x3x5 + x0x3x6 + x0x3x7 + x0x4x5 + x0x5x6 "
            "+ x1x2x3 + x1x2x6 + x1x2x7 + x1x3x4 + x1x3x5 + x1x3x6 + x1x3x7 + x1x4x6 + x1x4x7 + x1x5x6 + x1x5x7 "
            "+ x2x3x7 + x2x4x7 + x2x5x6 + x2x5x7 + x2x6x7 "
            "+ x3x4x5 + x3x4x6 + x3x5x7 + x3x6x7 + x4x6x7 "
            "+ x0x1x2x3 + x0x1x3x6 + x0x1x3x7 + x0x1x6x7 + x0x2x3x6 + x0x2x4x5 + x0x2x4x6 + x0x2x5x7 "
            "+ x0x3x4x6 + x0x3x4x7 + x0x3x5x6 + x0x4x5x7 + x0x4x6x7 "
            "+ x1x2x4x5 + x1x3x4x5 + x1x3x4x6 + x1x3x4x7 + x1x3x5x7 + x1x4x5x6 + x1x4x5x7 + x1x4x6x7 "
            "+ x2x3x4x5 + x2x3x5x6 + x2x3x5x7 + x2x4x5x6 + x2x4x5x7 + x2x5x6x7 "
            "+ x3x4x5x6 + x3x4x6x7 + x3x5x6x7 "
            "+ x0x1x2x3x4 + x0x1x2x3x6 + x0x1x2x3x7 + x0x1x2x4x5 + x0x1x2x5x6 + x0x1x2x5x7 "
            "+ x0x1x3x4x5 + x0x1x3x4x7 + x0x1x3x5x7 + x0x1x4x6x7 "
            "+ x0x2x3x4x6 + x0x2x3x5x7 + x0x2x3x6x7 + x0x2x4x5x6 "
            "+ x0x3x4x5x6 + x0x4x5x6x7 "
            "+ x1x2x3x4x6 + x1x2x3x5x6 + x1x2x4x5x7 + x1x4x5x6x7 "
            "+ x2x3x4x5x6 + x2x3x4x5x7 + x2x3x4x6x7 + x2x4x5x6x7 "
            "+ x0x1x2x3x4x6 + x0x1x2x4x6x7 + x0x1x2x5x6x7 + x0x1x3x4x6x7 + x0x1x3x5x6x7 "
            "+ x0x2x3x4x6x7 + x0x2x3x5x6x7 + x0x3x4x5x6x7 "
            "+ x1x2x3x4x6x7 + x1x2x3x5x6x7 + x1x2x4x5x6x7 "
            "+ x0x1x2x3x4x6x7 + x0x1x2x3x5x6x7"; //f1

    const string anf_str =
            "1 + x0 + x1 + x3 + x0x2 + x1x3 + x1x6 + x2x6 + x3x4 + x3x5 + x4x7 "
            "+ x0x1x2 + x0x1x5 + x0x1x6 + x0x2x3 + x0x2x4 + x0x2x5 + x0x2x6 + x0x3x5 + x0x3x6 + x0x3x7 + x0x4x5 + x0x4x6 + x0x6x7 "
            "+ x1x2x6 + x1x2x7 + x1x3x6 + x1x4x5 + x1x4x6 + x1x5x6 + x1x5x7 + x1x6x7 "
            "+ x2x3x4 + x2x3x5 + x2x4x6 + x2x4x7 + x2x5x6 + x2x6x7 "
            "+ x3x5x6 + x3x5x7 + x3x6x7 + x4x6x7 + x5x6x7 "
            "+ x0x1x2x7 + x0x1x3x4 + x0x1x3x7 + x0x2x3x4 + x0x2x3x5 + x0x2x3x6 + x0x2x4x6 + x0x2x6x7 "
            "+ x0x3x4x5 + x0x3x4x6 + x0x3x4x7 + x0x3x5x6 + x0x3x6x7 + x0x4x6x7 "
            "+ x1x2x3x4 + x1x2x4x5 + x1x2x4x6 + x1x2x6x7 + x1x3x4x5 + x1x3x4x6 + x1x3x6x7 + x1x4x5x6 "
            "+ x2x3x4x5 + x2x3x5x6 + x2x3x5x7 + x2x3x6x7 + x2x4x5x6 + x2x4x5x7 + x2x5x6x7 "
            "+ x3x4x5x7 + x3x4x6x7 + x4x5x6x7 "
            "+ x0x1x2x3x5 + x0x1x2x3x7 + x0x1x2x4x5 + x0x1x2x6x7 + x0x1x3x4x6 + x0x1x3x6x7 + x0x1x4x5x7 + x0x1x5x6x7 "
            "+ x0x2x3x4x7 + x0x2x3x5x7 + x0x2x3x6x7 + x0x2x4x5x7 + x0x2x4x6x7 "
            "+ x0x3x4x5x6 + x0x3x4x5x7 + x0x4x5x6x7 "
            "+ x1x2x3x4x5 + x1x2x3x4x6 + x1x2x3x4x7 + x1x2x3x5x6 + x1x2x3x6x7 + x1x3x4x5x6 + x1x3x5x6x7 + x1x4x5x6x7 "
            "+ x2x3x5x6x7 + x2x4x5x6x7 + x3x4x5x6x7 "
            "+ x0x1x2x3x5x6 + x0x1x2x3x5x7 + x0x1x2x4x5x6 + x0x1x3x4x5x6 + x0x1x3x4x5x7 "
            "+ x0x2x3x5x6x7 + x0x2x4x5x6x7 "
            "+ x1x2x3x5x6x7 + x1x2x4x5x6x7 + x2x3x4x5x6x7 "
            "+ x0x1x2x3x5x6x7 + x0x1x2x4x5x6x7"; //f2

    const string anf_str =
            "x2 + x4 + x5 + x6 + x7 + x0x1 + x0x2 + x0x4 + x0x7 + x1x3 + x1x6 + x1x7 + x2x3 + x2x4 + x2x5 + x2x6 + x2x7 + x3x4 + x3x6 + x3x7 + x4x5 + x6x7 "
            "+ x0x1x2 + x0x1x3 + x0x1x4 + x0x1x5 + x0x1x6 + x0x1x7 + x0x2x3 + x0x2x4 + x0x2x6 + x0x3x4 + x0x3x5 + x0x3x7 "
            "+ x1x2x4 + x1x2x5 + x1x3x4 + x1x3x7 + x1x4x5 + x1x5x7 "
            "+ x2x3x4 + x2x3x5 + x2x3x6 + x2x3x7 + x2x4x5 + x2x4x6 + x2x4x7 "
            "+ x3x4x5 + x3x4x7 + x3x5x6 + x4x5x7 "
            "+ x0x1x2x4 + x0x1x3x4 + x0x1x3x7 + x0x1x4x7 + x0x1x5x6 + x0x1x6x7 "
            "+ x0x2x3x4 + x0x2x4x5 + x0x2x4x7 + x0x2x5x6 + x0x2x5x7 + x0x2x6x7 "
            "+ x0x3x4x5 + x0x3x4x6 + x0x3x4x7 + x0x4x6x7 "
            "+ x1x2x3x7 + x1x2x4x5 + x1x2x4x7 + x1x2x6x7 + x1x3x4x7 + x1x3x5x6 + x1x3x6x7 + x1x4x6x7 + x1x5x6x7 "
            "+ x2x3x4x6 + x2x3x4x7 + x2x3x6x7 + x2x4x5x6 + x2x4x6x7 "
            "+ x3x4x5x6 + x3x4x5x7 "
            "+ x0x1x2x3x4 + x0x1x2x3x5 + x0x1x2x3x6 + x0x1x2x3x7 + x0x1x2x4x6 + x0x1x2x4x7 + x0x1x2x5x6 + x0x1x2x6x7 "
            "+ x0x1x3x4x5 + x0x1x3x4x7 + x0x1x3x5x7 + x0x1x4x6x7 + x0x1x5x6x7 "
            "+ x0x2x3x4x5 + x0x2x3x4x6 + x0x2x3x5x6 + x0x2x4x5x6 + x0x2x4x5x7 + x0x2x4x6x7 + x0x2x5x6x7 "
            "+ x0x4x5x6x7 "
            "+ x1x2x3x4x5 + x1x2x3x4x6 + x1x2x3x5x6 + x1x2x3x6x7 + x1x2x4x5x6 + x1x2x4x5x7 "
            "+ x1x3x4x5x6 + x1x3x4x5x7 + x1x3x4x6x7 + x1x4x5x6x7 "
            "+ x2x3x4x5x7 + x2x3x5x6x7 "
            "+ x3x4x5x6x7 "
            "+ x0x1x2x3x4x7 + x0x1x2x4x5x6 + x0x1x3x4x5x6 + x0x2x3x4x5x6 + x0x2x3x4x5x7 + x0x3x4x5x6x7 "
            "+ x1x2x3x4x5x6 + x1x2x3x4x6x7 + x1x2x3x5x6x7 + x1x2x4x5x6x7 "
            "+ x2x3x4x5x6x7 "
            "+ x0x1x2x3x4x5x7 + x0x1x3x4x5x6x7 + x0x2x3x4x5x6x7"; //f3

    const string anf_str =
            "x0 + x1 + x3 + x7 + x0x1 + x0x2 + x0x4 + x0x6 + x0x7 + x1x2 + x1x4 + x2x3 + x4x5 + x4x7 + x5x6 "
            "+ x0x1x2 + x0x2x4 + x0x2x5 + x0x3x4 + x0x4x5 + x0x4x7 + x0x5x6 + x0x5x7 + x0x6x7 + x1x2x4 + x1x2x6 "
            "+ x1x3x7 + x1x4x7 + x1x5x6 + x1x5x7 + x1x6x7 + x2x3x5 + x2x3x7 + x2x4x5 + x2x5x6 + x2x6x7 + x3x4x5 "
            "+ x3x4x6 + x3x4x7 + x3x5x7 + x3x6x7 + x4x5x6 + x4x5x7 + x4x6x7 + x0x1x2x3 + x0x1x2x4 + x0x1x2x6 "
            "+ x0x1x2x7 + x0x1x3x4 + x0x1x3x5 + x0x1x3x6 + x0x1x4x7 + x0x1x5x6 + x0x1x5x7 + x0x1x6x7 + x0x2x3x5 "
            "+ x0x2x3x6 + x0x2x5x6 + x0x3x4x6 + x0x3x5x6 + x0x3x5x7 + x0x4x5x6 + x0x4x5x7 + x1x2x3x4 + x1x2x3x5 "
            "+ x1x2x3x6 + x1x2x3x7 + x1x2x4x6 + x1x2x5x7 + x1x2x6x7 + x1x3x4x6 + x1x3x4x7 + x1x3x5x7 + x1x3x6x7 "
            "+ x1x4x5x6 + x1x4x6x7 + x2x3x4x5 + x2x3x4x6 + x2x3x5x6 + x2x4x5x6 + x2x5x6x7 + x3x5x6x7 + x4x5x6x7 "
            "+ x0x1x2x3x4 + x0x1x2x3x5 + x0x1x2x3x7 + x0x1x2x4x6 + x0x1x2x4x7 + x0x1x2x5x6 + x0x1x2x5x7 + x0x1x2x6x7 "
            "+ x0x1x3x4x6 + x0x1x3x4x7 + x0x1x4x5x7 + x0x2x3x4x5 + x0x2x3x4x7 + x0x2x3x5x7 + x0x2x3x6x7 + x0x2x4x5x7 "
            "+ x0x2x5x6x7 + x0x3x4x6x7 + x0x3x5x6x7 + x0x4x5x6x7 + x1x2x3x4x5 + x1x2x3x4x7 + x1x2x3x5x6 + x1x2x3x5x7 "
            "+ x1x2x4x5x6 + x1x2x4x5x7 + x1x2x4x6x7 + x1x2x5x6x7 + x1x3x4x6x7 + x2x3x4x5x7 + x2x3x4x6x7 + x2x4x5x6x7 "
            "+ x3x4x5x6x7 + x0x1x2x3x4x5 + x0x1x2x3x5x6 + x0x1x2x4x5x6 + x0x1x2x4x5x7 + x0x1x3x4x5x6 + x0x1x3x4x5x7 "
            "+ x0x1x3x5x6x7 + x0x1x4x5x6x7 + x0x2x3x4x5x6 + x0x2x3x4x6x7 + x0x2x3x5x6x7 + x0x2x4x5x6x7 + x0x3x4x5x6x7 "
            "+ x1x2x3x4x5x6 + x1x2x4x5x6x7 + x1x3x4x5x6x7 + x0x1x2x3x4x5x7 + x0x1x2x3x4x6x7 + x0x1x2x4x5x6x7 + x1x2x3x4x5x6x7"; //f4

            
    const string anf_str =
            "x0 + x2 + x6 + x7 + x0x1 + x0x3 + x0x7 + x1x2 + x1x7 + x2x7 + x3x4 + x3x6 + x3x7 + x4x5 + x4x7 + x5x7 "
            "+ x0x1x3 + x0x1x5 + x0x1x7 + x0x2x6 + x0x2x7 + x0x3x6 + x0x3x7 + x0x4x5 + x0x4x6 + x0x4x7 + x0x5x6 + x0x5x7 + x0x6x7 "
            "+ x1x2x4 + x1x2x6 + x1x3x4 + x1x3x7 + x1x4x5 + x1x4x7 + x1x5x6 + x1x6x7 "
            "+ x2x3x4 + x2x3x5 + x2x3x6 + x2x3x7 + x2x4x6 + x2x4x7 + x2x5x6 + x2x5x7 + x2x6x7 "
            "+ x3x4x5 + x3x4x6 + x3x4x7 + x3x5x6 + x3x5x7 + x3x6x7 + x4x5x7 + x4x6x7 "
            "+ x0x1x2x3 + x0x1x2x4 + x0x1x2x5 + x0x1x2x6 + x0x1x3x5 + x0x1x4x6 + x0x1x5x6 "
            "+ x0x2x3x5 + x0x2x3x6 + x0x2x4x6 + x0x2x4x7 + x0x2x5x6 + x0x3x4x5 + x0x3x4x7 + x0x3x5x6 + x0x3x5x7 + x0x4x5x7 + x0x5x6x7 "
            "+ x1x2x3x4 + x1x2x3x5 + x1x2x4x5 + x1x2x4x7 + x1x2x6x7 + x1x3x4x5 + x1x3x4x7 + x1x3x5x7 + x1x3x6x7 + x1x4x5x6 + x1x4x6x7 "
            "+ x2x4x5x6 + x2x4x5x7 + x2x4x6x7 + x2x5x6x7 + x3x4x5x6 + x3x4x5x7 + x3x4x6x7 "
            "+ x0x1x2x3x4 + x0x1x2x3x6 + x0x1x2x3x7 + x0x1x2x4x5 + x0x1x2x4x6 + x0x1x2x4x7 + x0x1x2x5x7 + x0x1x3x4x5 + x0x1x3x4x6 + x0x1x3x5x6 + x0x1x3x6x7 + x0x1x4x5x6 + x0x1x4x6x7 + x0x1x5x6x7 + x0x2x3x4x7 + x0x2x3x5x6 + x0x2x4x5x7 + x0x2x4x6x7 + x0x2x5x6x7 + x0x3x4x5x7 + x0x3x4x6x7 + x0x3x5x6x7 + x0x4x5x6x7 "
            "+ x1x2x3x4x6 + x1x2x3x5x6 + x1x2x3x5x7 + x1x3x4x5x6 + x1x3x4x6x7 + x1x4x5x6x7 + x2x3x4x5x6 + x2x3x5x6x7 + x2x4x5x6x7 + x3x4x5x6x7 "
            "+ x0x1x2x3x4x5 + x0x1x2x3x5x7 + x0x1x2x3x6x7 + x0x1x2x4x5x7 + x0x1x2x4x6x7 + x0x1x3x4x5x6 + x0x1x3x4x5x7 + x0x1x3x4x6x7 + x0x2x3x4x5x6 + x0x2x3x4x6x7 + x0x2x3x5x6x7 + x0x3x4x5x6x7 + x1x2x3x4x5x7 + x1x2x3x5x6x7 + x1x2x4x5x6x7 + x1x3x4x5x6x7 + x2x3x4x5x6x7 "
            "+ x0x1x2x3x4x5x6 + x0x1x2x3x4x5x7 + x0x1x2x3x4x6x7 + x0x1x2x4x5x6x7 + x0x1x3x4x5x6x7"; //f5
    

    const string anf_str =
            "1 + x0 + x1 + x4 + x7 + x0x4 + x0x5 + x0x6 + x0x7 + x1x3 + x1x5 + x2x3 + x3x6 + x3x7 + x4x5 + x4x6 + x4x7 + x5x7 + x6x7 "
            "+ x0x1x2 + x0x1x4 + x0x1x5 + x0x1x7 + x0x2x4 + x0x2x5 + x0x2x7 + x0x3x4 + x0x3x6 + x0x3x7 + x0x4x6 + x0x5x7 "
            "+ x1x2x4 + x1x2x6 + x1x3x4 + x1x3x7 + x1x4x5 + x1x4x6 + x1x6x7 + x2x3x7 + x2x4x5 + x2x4x6 "
            "+ x3x4x5 + x3x4x6 + x3x4x7 + x3x5x6 + x3x6x7 + x4x5x6 + x4x5x7 + x4x6x7 "
            "+ x0x1x3x4 + x0x1x3x5 + x0x1x3x6 + x0x1x3x7 + x0x1x5x6 "
            "+ x0x2x3x4 + x0x2x3x5 + x0x2x3x6 + x0x2x3x7 + x0x2x4x5 + x0x2x4x6 + x0x2x5x6 "
            "+ x0x3x4x5 + x0x3x4x6 + x0x3x5x6 + x0x3x6x7 "
            "+ x1x2x3x4 + x1x2x3x7 + x1x2x4x6 + x1x2x5x6 + x1x2x5x7 + x1x2x6x7 "
            "+ x1x3x4x7 + x1x4x5x7 + x1x4x6x7 + x1x5x6x7 "
            "+ x2x3x4x5 + x2x3x4x6 + x2x3x4x7 + x2x3x5x7 + x2x3x6x7 + x2x4x5x6 + x3x4x6x7 "
            "+ x0x1x2x3x5 + x0x1x2x3x7 + x0x1x2x4x7 + x0x1x2x5x7 + x0x1x2x6x7 "
            "+ x0x1x3x4x5 + x0x1x4x5x7 + x0x1x4x6x7 + x0x1x5x6x7 "
            "+ x0x2x3x4x5 + x0x2x3x4x6 + x0x2x3x4x7 + x0x2x4x5x7 + x0x2x4x6x7 + x0x3x4x5x7 + x0x3x4x6x7 + x0x3x5x6x7 + x0x4x5x6x7 "
            "+ x1x2x3x4x6 + x1x2x3x5x6 + x1x2x3x5x7 + x1x2x3x6x7 + x1x2x4x5x6 + x1x2x4x6x7 + x1x3x4x5x6 + x1x3x4x6x7 + x1x3x5x6x7 + x1x4x5x6x7 "
            "+ x2x3x4x5x7 + x2x3x5x6x7 + x3x4x5x6x7 "
            "+ x0x1x2x3x4x6 + x0x1x2x4x5x6 + x0x1x2x4x5x7 + x0x1x2x5x6x7 + x0x1x3x4x5x6 + x0x1x3x4x5x7 + x0x1x3x4x6x7 + x0x1x3x5x6x7 "
            "+ x0x2x3x4x5x7 + x0x2x3x5x6x7 + x0x3x4x5x6x7 "
            "+ x1x2x3x4x5x6 + x1x2x3x4x5x7 + x1x2x3x4x6x7 + x1x2x4x5x6x7 + x1x3x4x5x6x7 "
            "+ x0x1x2x3x4x6x7 + x0x1x2x3x5x6x7 + x0x1x2x4x5x6x7 + x0x1x3x4x5x6x7"; //f6


    const string anf_str =
            "1 + x3 + x4 + x5 + x7 + x0x1 + x0x2 + x0x5 + x1x2 + x1x3 + x1x5 + x1x6 + x1x7 + x2x7 + x3x5 + x3x6 + x3x7 + x4x5 + x4x6 + x5x6 + x6x7 "
            "+ x0x1x2 + x0x1x4 + x0x1x5 + x0x2x4 + x0x2x5 + x0x3x4 + x0x3x5 + x0x3x7 + x0x4x5 + x0x4x6 + x0x5x7 + x0x6x7 "
            "+ x1x2x3 + x1x2x5 + x1x2x6 + x1x3x6 + x1x3x7 + x1x4x7 + x1x5x6 + x1x5x7 + x1x6x7 + x2x4x5 + x2x4x7 + x2x5x7 + x3x4x6 + x3x4x7 + x3x5x6 + x3x5x7 + x3x6x7 + x4x5x6 "
            "+ x0x1x2x4 + x0x1x2x6 + x0x1x3x7 + x0x1x4x5 + x0x1x4x6 + x0x1x5x6 + x0x2x3x5 + x0x2x3x6 + x0x2x3x7 + x0x2x4x5 + x0x2x5x7 + x0x3x5x6 + x0x3x5x7 + x0x3x6x7 + x0x4x5x6 + x0x4x5x7 + x0x5x6x7 "
            "+ x1x2x3x5 + x1x2x3x6 + x1x2x3x7 + x1x2x4x7 + x1x2x5x7 + x1x3x4x6 + x1x3x4x7 + x1x3x5x6 + x1x4x5x6 + x1x5x6x7 "
            "+ x2x3x5x7 + x2x3x6x7 + x2x4x5x6 + x2x4x5x7 + x2x5x6x7 + x4x5x6x7 "
            "+ x0x1x2x3x5 + x0x1x2x4x7 + x0x1x2x5x6 + x0x1x2x6x7 + x0x1x3x4x7 + x0x1x3x5x6 + x0x1x3x5x7 + x0x1x4x6x7 + x0x1x5x6x7 "
            "+ x0x2x3x4x5 + x0x2x3x4x6 + x0x2x3x4x7 + x0x2x3x5x7 + x0x2x4x5x6 + x0x2x4x6x7 + x0x2x5x6x7 + x0x3x5x6x7 + x0x4x5x6x7 "
            "+ x1x2x3x4x5 + x1x2x3x4x7 + x1x2x3x5x6 + x1x2x3x6x7 + x1x2x4x5x6 + x1x2x4x6x7 + x1x3x4x5x7 + x1x3x4x6x7 + x1x4x5x6x7 "
            "+ x2x3x4x5x7 + x2x3x5x6x7 + x3x4x5x6x7 "
            "+ x0x1x2x3x4x5 + x0x1x2x3x4x6 + x0x1x2x3x5x6 + x0x1x2x3x6x7 + x0x1x2x4x5x7 + x0x1x2x5x6x7 + x0x1x3x4x5x6 + x0x1x3x4x6x7 + x0x1x4x5x6x7 "
            "+ x0x2x3x4x5x7 + x0x2x3x5x6x7 + x0x2x4x5x6x7 + x0x3x4x5x6x7 "
            "+ x1x2x3x4x5x7 + x1x3x4x5x6x7 "
            "+ x0x1x2x3x4x5x7 + x0x1x2x3x5x6x7 + x0x1x3x4x5x6x7"; //f7
***************************************************************************




*********************************AES_inv_Sbox******************************

    const string anf_str = "x4 + x0x3 + x0x7 + x1x2 + x1x5 + x1x6 + x1x7 + x2x5 + x2x6 + x3x4 + x3x5 + x3x7 + x4x7 + x5x6 + x6x7 + x0x1x3"
                           "    + x0x1x7 + x0x2x4 + x0x2x5 + x0x2x6 + x0x3x5 + x0x3x6 + x0x5x7 + x1x2x4 + x1x2x7 + x1x3x5 + x1x3x6 + x1x6x7 + x2x3x7"
                           "    + x2x4x6 + x2x5x6 + x3x4x7 + x3x5x6 + x3x6x7 + x4x5x6 + x4x5x7 + x0x1x2x3 + x0x1x2x4 + x0x1x2x5 + x0x1x3x6"
                           "    + x0x1x4x7 + x0x1x5x7 + x0x2x3x4 + x0x2x4x5 + x0x2x4x6 + x0x2x4x7 + x0x2x5x6 + x0x3x4x5 + x0x3x5x7 + x0x4x6x7"
                           "    + x0x5x6x7 + x1x2x3x4 + x1x2x3x7 + x1x2x4x7 + x1x2x6x7 + x1x3x6x7 + x1x4x5x7 + x1x4x6x7 + x2x3x4x6 + x2x3x5x7"
                           "    + x2x3x6x7 + x2x4x5x7 + x2x4x6x7 + x2x5x6x7 + x3x4x5x6 + x3x4x6x7 + x3x5x6x7 + x4x5x6x7 + x0x1x2x3x7 + x0x1x2x4x5"
                           "    + x0x1x2x4x6 + x0x1x2x5x7 + x0x1x2x6x7 + x0x1x3x6x7 + x0x1x4x6x7 + x0x1x5x6x7 + x0x2x3x4x5 + x0x2x4x5x7 + x0x2x5x6x7"
                           "    + x0x3x4x5x7 + x0x3x4x6x7 + x0x3x5x6x7 + x0x4x5x6x7 + x1x2x3x4x5 + x1x2x3x4x6 + x1x2x3x5x6 + x1x2x3x5x7 + x1x2x4x5x6"
                           "    + x1x2x4x5x7 + x1x2x4x6x7 + x1x2x5x6x7 + x1x3x5x6x7 + x2x3x4x5x6 + x2x3x5x6x7 + x3x4x5x6x7 + x0x1x2x3x4x5"
                           "    + x0x1x2x3x5x6 + x0x1x2x3x5x7 + x0x1x2x4x5x6 + x0x1x2x4x5x7 + x0x1x3x4x5x7 + x0x1x3x5x6x7 + x0x1x4x5x6x7"
                           "    + x0x2x3x4x5x6 + x0x2x3x4x6x7 + x0x2x3x5x6x7 + x1x2x3x4x5x6 + x1x2x3x4x6x7 + x1x2x4x5x6x7 + x0x1x2x3x5x6x7"; // AES_invSbox_f0


    const string anf_str = "1 + x0 + x4 + x5 + x7 + x0x2 + x0x4 + x0x7 + x1x2 + x1x4 + x1x7 + x2x3 + x2x4 + x2x6 + x2x7 + x3x6 + x3x7 + x4x5"
                           "    + x5x7 + x6x7 + x0x1x3 + x0x1x4 + x0x1x5 + x0x2x3 + x0x2x4 + x0x2x7 + x0x3x4 + x0x3x7 + x0x4x5 + x0x4x6 + x0x4x7"
                           "    + x0x5x7 + x1x2x5 + x1x2x6 + x1x3x5 + x1x3x6 + x1x3x7 + x1x4x7 + x1x6x7 + x2x3x4 + x2x3x5 + x2x3x7 + x2x5x6 + x2x6x7"
                           "    + x3x4x5 + x3x5x7 + x3x6x7 + x4x5x6 + x4x5x7 + x5x6x7 + x0x1x2x3 + x0x1x2x4 + x0x1x2x5 + x0x1x4x6 + x0x1x4x7"
                           "    + x0x1x5x6 + x0x1x5x7 + x0x1x6x7 + x0x2x3x4 + x0x2x3x6 + x0x2x3x7 + x0x2x5x6 + x0x2x6x7 + x0x3x4x5 + x0x3x4x6"
                           "    + x0x3x4x7 + x0x3x6x7 + x0x4x5x7 + x1x2x3x6 + x1x2x3x7 + x1x3x4x5 + x1x3x5x6 + x1x3x6x7 + x1x4x5x7 + x1x4x6x7"
                           "    + x2x3x4x6 + x2x3x5x6 + x2x3x5x7 + x2x4x5x6 + x3x4x5x7 + x4x5x6x7 + x0x1x2x3x4 + x0x1x2x3x7 + x0x1x2x4x5"
                           "    + x0x1x2x4x6 + x0x1x2x6x7 + x0x1x3x4x7 + x0x1x3x5x6 + x0x1x3x6x7 + x0x1x4x5x6 + x0x1x4x6x7 + x0x1x5x6x7 + x0x2x3x4x5"
                           "    + x0x2x3x4x6 + x0x2x3x4x7 + x0x2x4x5x7 + x0x2x4x6x7 + x0x2x5x6x7 + x0x3x4x5x6 + x0x3x4x5x7 + x0x3x5x6x7 + x1x2x3x4x6"
                           "    + x1x2x3x4x7 + x1x2x3x5x6 + x1x2x3x5x7 + x1x2x4x5x7 + x1x2x4x6x7 + x1x2x5x6x7 + x1x4x5x6x7 + x2x3x4x5x6 + x2x3x5x6x7"
                           "    + x0x1x2x3x4x5 + x0x1x2x3x4x7 + x0x1x2x3x5x7 + x0x1x2x3x6x7 + x0x1x2x4x5x6 + x0x1x2x4x5x7 + x0x1x2x5x6x7"
                           "    + x0x1x3x4x5x6 + x0x1x3x4x5x7 + x0x1x3x4x6x7 + x0x1x3x5x6x7 + x0x1x4x5x6x7 + x0x2x3x4x6x7 + x0x2x4x5x6x7"
                           "    + x0x3x4x5x6x7 + x1x2x3x4x5x7 + x1x2x3x5x6x7 + x0x1x2x4x5x6x7"; // AES_invSbox_f1


    const string anf_str = "x0 + x1 + x3 + x4 + x5 + x6 + x0x2 + x0x7 + x1x2 + x1x3 + x1x6 + x2x3 + x2x6 + x2x7 + x3x4 + x3x6 + x4x6 + x4x7"
                           "    + x5x6 + x6x7 + x0x1x2 + x0x1x3 + x0x1x5 + x0x1x6 + x0x1x7 + x0x2x3 + x0x2x5 + x0x2x7 + x0x3x4 + x0x3x6 + x0x4x7"
                           "    + x1x2x3 + x1x2x5 + x1x2x7 + x1x3x4 + x1x3x5 + x1x4x5 + x1x4x6 + x1x5x6 + x1x5x7 + x1x6x7 + x2x3x4 + x2x4x7 + x2x5x6"
                           "    + x2x6x7 + x3x5x7 + x4x5x6 + x4x6x7 + x5x6x7 + x0x1x2x3 + x0x1x2x7 + x0x1x3x6 + x0x1x3x7 + x0x1x4x5 + x0x1x4x7"
                           "    + x0x1x5x6 + x0x1x5x7 + x0x2x3x4 + x0x2x3x5 + x0x2x4x6 + x0x3x4x7 + x0x4x5x7 + x0x4x6x7 + x1x2x3x5 + x1x2x3x7"
                           "    + x1x2x4x5 + x1x3x4x6 + x1x3x4x7 + x1x3x5x6 + x1x4x5x7 + x2x3x4x5 + x2x3x5x6 + x2x3x5x7 + x2x3x6x7 + x3x4x6x7"
                           "    + x4x5x6x7 + x0x1x2x3x4 + x0x1x2x3x6 + x0x1x2x3x7 + x0x1x2x4x5 + x0x1x2x4x7 + x0x1x3x4x6 + x0x1x3x5x7 + x0x1x3x6x7"
                           "    + x0x1x4x5x6 + x0x1x4x6x7 + x0x2x3x4x5 + x0x2x3x4x7 + x0x2x3x5x7 + x0x2x3x6x7 + x0x2x4x5x6 + x0x2x4x6x7 + x0x3x4x5x7"
                           "    + x0x3x5x6x7 + x0x4x5x6x7 + x1x2x3x4x5 + x1x2x3x4x7 + x1x2x3x5x6 + x1x2x3x5x7 + x1x2x3x6x7 + x1x2x5x6x7 + x1x3x4x5x6"
                           "    + x1x4x5x6x7 + x2x3x4x5x6 + x2x3x4x6x7 + x2x3x5x6x7 + x0x1x2x3x4x5 + x0x1x2x3x6x7 + x0x1x2x4x5x7 + x0x1x2x5x6x7"
                           "    + x0x1x3x4x5x6 + x0x1x3x4x5x7 + x0x1x4x5x6x7 + x0x2x3x4x5x6 + x0x2x3x5x6x7 + x1x2x3x5x6x7 + x2x3x4x5x6x7"
                           "    + x0x1x2x3x4x5x6 + x0x1x2x3x4x5x7 + x0x1x2x3x4x6x7 + x0x1x2x4x5x6x7"; // AES_invSbox_f2


    const string anf_str = "1 + x6 + x7 + x0x2 + x0x5 + x0x6 + x0x7 + x1x3 + x1x5 + x1x6 + x1x7 + x2x3 + x2x4 + x2x5 + x2x6 + x2x7 + x3x6 + x4x5"
                           "    + x5x7 + x0x1x5 + x0x1x7 + x0x2x4 + x0x2x5 + x0x3x4 + x0x3x6 + x0x3x7 + x0x4x6 + x0x5x6 + x0x5x7 + x0x6x7 + x1x2x4"
                           "    + x1x2x5 + x1x2x7 + x1x3x4 + x1x3x6 + x1x3x7 + x1x4x7 + x1x5x6 + x1x5x7 + x1x6x7 + x2x3x5 + x2x3x6 + x2x4x6 + x2x5x7"
                           "    + x3x4x6 + x3x5x7 + x4x5x7 + x5x6x7 + x0x1x2x3 + x0x1x2x4 + x0x1x2x6 + x0x1x3x6 + x0x1x3x7 + x0x1x4x5 + x0x1x4x7"
                           "    + x0x1x5x6 + x0x1x5x7 + x0x1x6x7 + x0x2x3x4 + x0x2x3x5 + x0x2x4x6 + x0x2x5x6 + x0x2x5x7 + x0x3x4x5 + x0x3x4x6"
                           "    + x0x3x6x7 + x0x4x5x6 + x0x4x5x7 + x0x4x6x7 + x0x5x6x7 + x1x2x3x5 + x1x2x5x6 + x1x2x6x7 + x1x3x4x5 + x1x3x5x6"
                           "    + x1x3x5x7 + x1x3x6x7 + x1x4x5x6 + x1x4x5x7 + x1x4x6x7 + x2x3x4x6 + x2x4x5x6 + x2x4x6x7 + x3x4x5x6 + x3x4x5x7"
                           "    + x3x4x6x7 + x3x5x6x7 + x0x1x2x3x5 + x0x1x2x3x6 + x0x1x2x4x6 + x0x1x2x5x6 + x0x1x2x6x7 + x0x1x3x4x6 + x0x1x3x4x7"
                           "    + x0x1x3x5x7 + x0x1x3x6x7 + x0x1x4x5x6 + x0x1x4x5x7 + x0x1x4x6x7 + x0x2x3x4x5 + x0x2x3x4x6 + x0x2x3x5x6 + x0x2x3x6x7"
                           "    + x0x2x4x5x6 + x0x3x4x5x7 + x0x4x5x6x7 + x1x2x3x4x5 + x1x2x3x5x6 + x1x2x3x5x7 + x1x2x3x6x7 + x1x2x4x5x6 + x1x2x5x6x7"
                           "    + x1x3x4x5x7 + x1x3x4x6x7 + x1x3x5x6x7 + x2x3x4x5x7 + x2x4x5x6x7 + x0x1x2x3x4x5 + x0x1x2x3x4x6 + x0x1x2x3x5x7"
                           "    + x0x1x2x4x5x7 + x0x1x2x4x6x7 + x0x1x3x4x5x6 + x0x1x3x4x5x7 + x0x1x3x4x6x7 + x0x2x3x4x5x7 + x0x2x3x4x6x7"
                           "    + x0x3x4x5x6x7 + x1x2x3x4x5x6 + x1x2x3x4x6x7 + x1x2x3x5x6x7 + x1x2x4x5x6x7 + x1x3x4x5x6x7 + x0x1x2x3x4x5x7"
                           "    + x0x1x2x3x4x6x7 + x0x1x2x3x5x6x7 + x0x1x3x4x5x6x7"; // AES_invSbox_f3


    const string anf_str = "x0 + x3 + x4 + x6 + x7 + x0x2 + x1x4 + x1x6 + x2x6 + x3x6 + x4x5 + x5x6 + x5x7 + x0x1x4 + x0x1x7 + x0x2x5 + x0x2x6"
                           "    + x0x2x7 + x0x3x4 + x0x4x5 + x0x4x7 + x0x5x7 + x0x6x7 + x1x2x3 + x1x2x5 + x1x3x6 + x1x4x6 + x1x4x7 + x1x5x7 + x1x6x7"
                           "    + x2x3x7 + x2x4x6 + x2x4x7 + x2x5x6 + x2x6x7 + x3x4x5 + x3x5x6 + x5x6x7 + x0x1x2x5 + x0x1x2x6 + x0x1x2x7 + x0x1x3x4"
                           "    + x0x1x3x5 + x0x1x3x6 + x0x1x4x6 + x0x1x6x7 + x0x2x4x5 + x0x2x4x6 + x0x2x4x7 + x0x2x6x7 + x0x3x4x7 + x0x4x6x7"
                           "    + x0x5x6x7 + x1x2x3x7 + x1x2x4x5 + x1x2x4x7 + x1x2x5x7 + x1x3x4x5 + x1x3x4x7 + x1x3x5x7 + x1x3x6x7 + x1x4x5x6"
                           "    + x1x4x5x7 + x1x4x6x7 + x2x3x4x5 + x2x3x5x6 + x2x3x5x7 + x2x3x6x7 + x2x4x5x6 + x2x5x6x7 + x3x4x5x6 + x3x4x5x7"
                           "    + x3x4x6x7 + x0x1x2x3x4 + x0x1x2x3x5 + x0x1x2x3x7 + x0x1x2x4x6 + x0x1x2x4x7 + x0x1x2x6x7 + x0x1x3x4x6 + x0x1x3x4x7"
                           "    + x0x1x3x5x6 + x0x1x3x5x7 + x0x1x4x5x6 + x0x2x3x4x6 + x0x2x3x4x7 + x0x2x3x5x6 + x0x2x3x6x7 + x0x2x4x5x7 + x0x2x5x6x7"
                           "    + x0x3x4x5x6 + x0x3x4x5x7 + x0x3x4x6x7 + x1x2x3x5x6 + x1x2x3x6x7 + x1x2x4x5x7 + x1x2x5x6x7 + x1x3x4x5x7 + x1x3x4x6x7"
                           "    + x1x3x5x6x7 + x2x3x4x5x6 + x2x3x4x6x7 + x2x4x5x6x7 + x3x4x5x6x7 + x0x1x2x3x4x6 + x0x1x2x3x5x7 + x0x1x2x4x5x7"
                           "    + x0x1x2x5x6x7 + x0x1x3x4x5x7 + x0x1x4x5x6x7 + x0x2x4x5x6x7 + x1x2x3x4x5x6 + x1x2x3x4x6x7 + x0x1x2x3x4x5x6"
                           "    + x0x1x2x3x4x5x7 + x0x1x3x4x5x6x7 + x0x2x3x4x5x6x7"; // AES_invSbox_f4

    const string anf_str = "x2 + x3 + x4 + x0x1 + x0x5 + x1x2 + x1x5 + x1x6 + x2x4 + x2x7 + x3x4 + x3x5 + x3x6 + x3x7 + x4x5 + x4x6 + x4x7"
                           "    + x5x6 + x5x7 + x6x7 + x0x1x2 + x0x1x5 + x0x2x3 + x0x2x5 + x0x2x6 + x0x3x4 + x0x3x6 + x0x3x7 + x0x4x5 + x0x4x6"
                           "    + x0x5x6 + x0x5x7 + x0x6x7 + x1x2x3 + x1x2x4 + x1x2x6 + x1x2x7 + x1x3x6 + x1x4x7 + x1x5x6 + x1x6x7 + x2x3x5 + x2x3x6"
                           "    + x2x3x7 + x2x4x6 + x2x5x7 + x2x6x7 + x3x4x6 + x3x5x6 + x3x5x7 + x4x5x6 + x4x6x7 + x5x6x7 + x0x1x2x4 + x0x1x2x5"
                           "    + x0x1x2x6 + x0x1x3x6 + x0x1x4x6 + x0x1x5x6 + x0x1x5x7 + x0x1x6x7 + x0x2x3x4 + x0x2x4x5 + x0x2x4x6 + x0x2x5x6"
                           "    + x0x2x5x7 + x0x3x4x5 + x0x3x4x7 + x0x3x5x6 + x0x3x5x7 + x0x3x6x7 + x1x2x3x5 + x1x2x3x7 + x1x2x4x5 + x1x2x4x6"
                           "    + x1x2x5x6 + x1x2x6x7 + x1x3x4x5 + x1x3x4x6 + x1x3x4x7 + x1x3x5x6 + x1x3x5x7 + x1x3x6x7 + x1x4x5x6 + x1x4x5x7"
                           "    + x1x4x6x7 + x1x5x6x7 + x2x3x4x5 + x2x3x4x7 + x2x3x5x6 + x2x5x6x7 + x3x4x5x6 + x3x4x6x7 + x3x5x6x7 + x0x1x2x3x5"
                           "    + x0x1x2x3x6 + x0x1x2x3x7 + x0x1x2x4x5 + x0x1x2x6x7 + x0x1x3x4x5 + x0x1x3x4x6 + x0x1x3x5x7 + x0x1x3x6x7 + x0x1x4x5x7"
                           "    + x0x2x3x4x5 + x0x2x3x4x6 + x0x2x3x4x7 + x0x2x3x5x6 + x0x2x3x5x7 + x0x2x3x6x7 + x0x2x4x6x7 + x0x2x5x6x7 + x0x3x4x5x6"
                           "    + x0x3x5x6x7 + x1x2x3x4x6 + x1x2x4x5x6 + x1x2x4x5x7 + x1x2x4x6x7 + x1x2x5x6x7 + x1x3x4x5x6 + x1x4x5x6x7 + x2x3x4x5x6"
                           "    + x2x3x4x5x7 + x2x3x4x6x7 + x3x4x5x6x7 + x0x1x2x3x4x7 + x0x1x2x4x5x6 + x0x1x3x4x5x7 + x0x1x3x4x6x7 + x0x1x3x5x6x7"
                           "    + x0x2x3x4x5x6 + x0x2x3x4x5x7 + x0x2x3x5x6x7 + x0x2x4x5x6x7 + x0x3x4x5x6x7 + x1x2x3x4x5x6 + x1x3x4x5x6x7"
                           "    + x2x3x4x5x6x7 + x0x1x2x3x4x5x7 + x0x1x2x3x4x6x7 + x0x1x2x3x5x6x7 + x0x2x3x4x5x6x7 + x1x2x3x4x5x6x7"; // AES_invSbox_f5


    const string anf_str = "1 + x2 + x3 + x5 + x7 + x0x2 + x0x3 + x0x5 + x0x6 + x1x4 + x1x5 + x2x3 + x2x4 + x0x1x2 + x0x1x3 + x0x2x4 + x0x2x5"
                           "    + x0x2x6 + x0x4x5 + x0x4x6 + x0x4x7 + x0x5x7 + x0x6x7 + x1x2x4 + x1x2x6 + x1x2x7 + x1x3x7 + x1x4x7 + x1x5x6 + x2x3x5"
                           "    + x2x4x5 + x2x4x6 + x2x5x7 + x3x4x5 + x3x4x6 + x3x5x7 + x4x5x6 + x4x6x7 + x5x6x7 + x0x1x2x3 + x0x1x2x5 + x0x1x3x4"
                           "    + x0x1x3x5 + x0x1x4x5 + x0x1x5x6 + x0x1x5x7 + x0x1x6x7 + x0x2x3x4 + x0x2x3x5 + x0x2x3x6 + x0x2x4x5 + x0x2x5x7"
                           "    + x0x2x6x7 + x0x3x4x5 + x0x3x4x6 + x0x3x4x7 + x0x3x6x7 + x0x4x5x7 + x0x5x6x7 + x1x2x3x4 + x1x2x3x7 + x1x2x4x6"
                           "    + x1x2x5x6 + x1x2x5x7 + x1x2x6x7 + x1x3x4x5 + x1x3x4x6 + x1x3x5x6 + x1x3x5x7 + x1x4x5x6 + x1x4x6x7 + x1x5x6x7"
                           "    + x2x3x4x5 + x2x3x4x7 + x2x3x5x7 + x2x4x6x7 + x3x4x5x7 + x3x4x6x7 + x3x5x6x7 + x4x5x6x7 + x0x1x2x3x6 + x0x1x2x4x6"
                           "    + x0x1x2x4x7 + x0x1x2x5x6 + x0x1x2x5x7 + x0x1x2x6x7 + x0x1x3x4x7 + x0x1x3x5x6 + x0x1x4x5x6 + x0x1x4x5x7 + x0x1x4x6x7"
                           "    + x0x1x5x6x7 + x0x2x4x5x6 + x0x2x4x5x7 + x0x3x4x5x7 + x0x4x5x6x7 + x1x2x3x4x5 + x1x2x3x4x6 + x1x2x3x4x7 + x1x2x3x5x7"
                           "    + x1x2x3x6x7 + x1x2x4x5x6 + x1x2x4x6x7 + x1x2x5x6x7 + x1x3x4x5x6 + x1x3x4x5x7 + x1x4x5x6x7 + x2x3x4x5x6 + x2x3x4x5x7"
                           "    + x2x3x5x6x7 + x3x4x5x6x7 + x0x1x2x3x4x5 + x0x1x2x3x4x6 + x0x1x2x3x4x7 + x0x1x2x3x5x6 + x0x1x2x4x5x6 + x0x1x3x4x6x7"
                           "    + x0x1x3x5x6x7 + x0x2x3x4x5x6 + x0x2x3x4x6x7 + x0x2x3x5x6x7 + x0x2x4x5x6x7 + x1x2x3x4x5x6 + x1x2x3x4x5x7"
                           "    + x1x2x3x4x6x7 + x1x2x3x5x6x7 + x2x3x4x5x6x7 + x0x1x2x3x4x5x6 + x0x1x2x3x4x6x7 + x0x1x2x3x5x6x7 + x0x1x2x4x5x6x7"
                           "    + x1x2x3x4x5x6x7"; // AES_invSbox_f6


    const string anf_str = "x4 + x7 + x0x1 + x0x2 + x0x5 + x0x6 + x1x4 + x1x7 + x2x4 + x3x4 + x3x5 + x3x6 + x5x6 + x5x7 + x0x1x3 + x0x2x3"
                           "    + x0x2x5 + x0x2x7 + x0x3x5 + x0x3x7 + x0x4x6 + x0x5x6 + x0x6x7 + x1x2x6 + x1x3x4 + x1x3x6 + x1x4x5 + x1x5x6 + x1x5x7"
                           "    + x2x3x4 + x2x3x5 + x2x3x7 + x2x4x6 + x2x4x7 + x2x6x7 + x3x4x5 + x3x4x7 + x4x5x6 + x5x6x7 + x0x1x2x3 + x0x1x2x4"
                           "    + x0x1x2x6 + x0x1x2x7 + x0x1x3x4 + x0x1x4x6 + x0x1x4x7 + x0x1x5x6 + x0x1x5x7 + x0x2x3x5 + x0x2x4x5 + x0x2x4x7"
                           "    + x0x2x5x6 + x0x2x5x7 + x0x2x6x7 + x0x3x5x7 + x0x3x6x7 + x0x4x5x6 + x0x4x6x7 + x1x2x3x4 + x1x2x3x6 + x1x2x3x7"
                           "    + x1x2x4x6 + x1x5x6x7 + x2x3x4x7 + x2x4x5x7 + x2x4x6x7 + x3x4x5x7 + x3x4x6x7 + x3x5x6x7 + x4x5x6x7 + x0x1x2x3x4"
                           "    + x0x1x2x3x5 + x0x1x2x3x6 + x0x1x2x3x7 + x0x1x2x4x5 + x0x1x2x5x6 + x0x1x2x6x7 + x0x1x3x4x5 + x0x1x3x4x7 + x0x1x3x5x7"
                           "    + x0x1x3x6x7 + x0x1x4x5x7 + x0x2x3x4x7 + x0x2x3x5x7 + x0x2x3x6x7 + x0x2x4x5x7 + x0x2x4x6x7 + x0x3x4x5x7 + x0x4x5x6x7"
                           "    + x1x2x3x4x7 + x1x2x3x5x6 + x1x2x3x5x7 + x1x2x4x5x6 + x1x2x4x5x7 + x1x3x4x5x6 + x1x3x4x6x7 + x1x4x5x6x7 + x2x3x4x6x7"
                           "    + x2x4x5x6x7 + x0x1x2x3x5x7 + x0x1x2x3x6x7 + x0x1x2x4x5x7 + x0x1x2x4x6x7 + x0x1x2x5x6x7 + x0x1x3x4x5x6"
                           "    + x0x1x3x4x6x7 + x0x1x3x5x6x7 + x0x1x4x5x6x7 + x0x2x3x4x5x6 + x0x2x3x4x6x7 + x0x2x4x5x6x7 + x1x2x3x4x5x6"
                           "    + x1x2x3x4x6x7 + x1x2x4x5x6x7 + x0x1x2x3x4x6x7"; // AES_invSbox_f7
***************************************************************************


****************************ARIA S2 Sbox***********************************

    const string anf_str = "1 + x1 + x2 + x3 + x4 + x6 + x7 + x0x3 + x0x6 + x1x3 + x1x5 + x1x7 + x2x3 + x2x6 + x2x7 + x3x5 + x3x6 + x4x6 + x4x7"
                           "    + x5x7 + x0x1x4 + x0x1x5 + x0x1x7 + x0x2x4 + x0x2x5 + x0x2x7 + x0x3x5 + x0x3x6 + x0x3x7 + x0x6x7 + x1x2x3 + x1x2x6"
                           "    + x1x2x7 + x1x3x4 + x1x3x6 + x1x4x5 + x1x5x6 + x1x5x7 + x1x6x7 + x2x3x4 + x2x3x6 + x2x4x5 + x2x4x7 + x2x5x6 + x3x4x5"
                           "    + x3x4x7 + x4x5x6 + x4x5x7 + x5x6x7 + x0x1x2x3 + x0x1x2x6 + x0x1x2x7 + x0x1x3x5 + x0x1x3x6 + x0x1x3x7 + x0x1x4x5"
                           "    + x0x1x4x6 + x0x1x4x7 + x0x1x5x6 + x0x1x6x7 + x0x2x4x5 + x0x2x6x7 + x0x3x5x6 + x0x3x5x7 + x0x5x6x7 + x1x2x3x6"
                           "    + x1x2x4x5 + x1x2x4x7 + x1x2x5x6 + x1x2x6x7 + x1x3x4x5 + x1x3x4x6 + x1x3x4x7 + x1x3x5x7 + x1x4x5x7 + x2x3x4x5"
                           "    + x2x3x4x6 + x2x3x4x7 + x2x3x5x6 + x2x3x6x7 + x2x4x5x7 + x2x4x6x7 + x2x5x6x7 + x3x4x5x6 + x3x4x6x7 + x4x5x6x7"
                           "    + x0x1x2x3x4 + x0x1x2x3x5 + x0x1x2x3x6 + x0x1x2x4x5 + x0x1x2x5x6 + x0x1x2x5x7 + x0x1x3x4x5 + x0x1x3x4x7 + x0x1x3x5x6"
                           "    + x0x1x4x5x6 + x0x1x5x6x7 + x0x2x3x4x6 + x0x2x3x5x6 + x0x2x3x5x7 + x0x2x3x6x7 + x0x2x4x5x7 + x0x2x5x6x7 + x0x3x4x6x7"
                           "    + x0x3x5x6x7 + x0x4x5x6x7 + x1x2x3x4x5 + x1x2x3x4x7 + x1x2x4x5x6 + x1x2x4x5x7 + x1x3x4x5x6 + x1x3x4x6x7 + x1x3x5x6x7"
                           "    + x2x4x5x6x7 + x3x4x5x6x7 + x0x1x2x3x5x7 + x0x1x2x4x6x7 + x0x1x2x5x6x7 + x0x1x3x4x6x7 + x0x2x3x4x5x6 + x0x2x3x4x5x7"
                           "    + x0x2x3x4x6x7 + x1x2x3x4x5x6 + x1x2x3x5x6x7 + x1x3x4x5x6x7 + x0x1x2x3x4x5x6 + x0x1x2x3x4x5x7 + x0x1x2x4x5x6x7"
                           "    + x0x1x3x4x5x6x7"; //ARIA_S2_f0


    const string anf_str = "1 + x1 + x2 + x5 + x0x6 + x0x7 + x1x5 + x1x6 + x1x7 + x2x4 + x2x6 + x3x4 + x4x7 + x5x6 + x5x7 + x0x1x2 + x0x1x3"
                           "    + x0x1x6 + x0x1x7 + x0x2x5 + x0x2x6 + x0x3x5 + x0x3x7 + x0x4x5 + x0x4x6 + x0x4x7 + x0x5x7 + x0x6x7 + x1x2x3 + x1x2x5"
                           "    + x1x2x6 + x1x3x5 + x1x3x6 + x1x4x5 + x1x4x7 + x1x5x7 + x2x3x5 + x2x3x7 + x2x4x5 + x2x5x6 + x2x5x7 + x3x5x6 + x3x5x7"
                           "    + x4x5x6 + x4x5x7 + x4x6x7 + x5x6x7 + x0x1x2x3 + x0x1x2x5 + x0x1x2x6 + x0x1x2x7 + x0x1x3x4 + x0x1x3x5 + x0x1x3x6"
                           "    + x0x1x4x6 + x0x1x4x7 + x0x2x3x4 + x0x2x3x5 + x0x2x4x6 + x0x2x4x7 + x0x2x6x7 + x0x3x5x6 + x0x3x5x7 + x0x4x5x6"
                           "    + x0x4x5x7 + x0x4x6x7 + x0x5x6x7 + x1x2x4x5 + x1x2x4x6 + x1x2x4x7 + x1x2x6x7 + x1x3x4x5 + x1x3x4x6 + x1x3x4x7"
                           "    + x1x3x5x6 + x1x3x5x7 + x1x3x6x7 + x1x4x5x6 + x1x4x5x7 + x1x4x6x7 + x2x3x4x5 + x2x3x5x7 + x2x3x6x7 + x3x4x5x6"
                           "    + x3x4x5x7 + x3x5x6x7 + x0x1x2x3x4 + x0x1x2x3x5 + x0x1x2x3x6 + x0x1x2x3x7 + x0x1x2x4x5 + x0x1x2x4x6 + x0x1x2x4x7"
                           "    + x0x1x2x5x6 + x0x1x3x4x6 + x0x1x3x4x7 + x0x1x3x5x6 + x0x1x3x6x7 + x0x1x4x5x7 + x0x2x3x4x7 + x0x2x3x5x7 + x0x2x4x5x7"
                           "    + x0x2x4x6x7 + x0x2x5x6x7 + x0x3x4x5x6 + x0x3x4x6x7 + x0x4x5x6x7 + x1x2x3x4x6 + x1x2x4x5x6 + x1x3x4x5x6 + x1x3x4x5x7"
                           "    + x2x3x4x5x7 + x2x3x4x6x7 + x2x3x5x6x7 + x2x4x5x6x7 + x0x1x2x3x4x6 + x0x1x2x3x5x6 + x0x1x2x4x5x6 + x0x1x2x4x5x7"
                           "    + x0x1x2x4x6x7 + x0x1x3x4x5x6 + x0x1x3x4x6x7 + x0x1x4x5x6x7 + x0x2x3x4x5x7 + x0x2x3x4x6x7 + x0x2x4x5x6x7"
                           "    + x1x2x3x4x5x7 + x1x2x3x5x6x7 + x1x2x4x5x6x7 + x2x3x4x5x6x7 + x0x1x2x3x4x5x7 + x0x1x2x3x4x6x7 + x0x1x2x3x5x6x7"
                           "    + x0x1x2x4x5x6x7"; // ARIA_S2_f1


    const string anf_str = "1 + x2 + x3 + x5 + x6 + x7 + x0x4 + x0x5 + x0x7 + x1x2 + x1x4 + x1x5 + x1x6 + x1x7 + x2x3 + x2x4 + x2x5 + x2x6"
                           "    + x2x7 + x3x4 + x4x5 + x4x6 + x5x6 + x5x7 + x0x1x2 + x0x1x3 + x0x1x6 + x0x2x3 + x0x2x4 + x0x2x6 + x0x2x7 + x0x3x4"
                           "    + x0x3x5 + x0x3x6 + x0x4x5 + x0x4x6 + x0x4x7 + x0x5x6 + x0x6x7 + x1x2x4 + x1x2x5 + x1x3x7 + x1x4x6 + x1x4x7 + x1x5x6"
                           "    + x2x3x4 + x2x3x6 + x2x3x7 + x2x4x5 + x2x4x7 + x2x5x6 + x2x6x7 + x3x4x5 + x3x4x7 + x3x5x7 + x4x5x7 + x0x1x2x3"
                           "    + x0x1x2x6 + x0x1x2x7 + x0x1x3x4 + x0x1x5x6 + x0x1x5x7 + x0x2x3x7 + x0x2x4x6 + x0x2x4x7 + x0x2x5x6 + x0x2x5x7"
                           "    + x0x2x6x7 + x0x3x4x6 + x0x3x4x7 + x0x3x5x7 + x0x3x6x7 + x0x4x5x6 + x0x4x5x7 + x0x4x6x7 + x1x2x3x5 + x1x2x3x6"
                           "    + x1x2x3x7 + x1x2x4x5 + x1x2x4x7 + x1x2x5x6 + x1x2x6x7 + x1x3x4x6 + x1x3x4x7 + x1x3x5x6 + x1x3x5x7 + x1x4x5x6"
                           "    + x1x4x5x7 + x1x4x6x7 + x2x3x4x6 + x2x3x5x6 + x2x3x5x7 + x2x4x5x6 + x2x4x6x7 + x2x5x6x7 + x3x4x5x6 + x3x4x5x7"
                           "    + x3x4x6x7 + x3x5x6x7 + x4x5x6x7 + x0x1x2x3x5 + x0x1x2x3x6 + x0x1x2x3x7 + x0x1x2x4x7 + x0x1x2x6x7 + x0x1x3x4x6"
                           "    + x0x1x3x5x7 + x0x2x3x4x5 + x0x2x3x5x6 + x0x2x3x5x7 + x0x2x3x6x7 + x0x2x4x5x6 + x0x2x4x5x7 + x0x3x4x5x7 + x0x4x5x6x7"
                           "    + x1x2x3x4x7 + x1x2x3x5x6 + x1x2x4x5x6 + x1x2x5x6x7 + x1x3x4x5x7 + x1x3x4x6x7 + x1x3x5x6x7 + x2x3x4x5x7 + x2x3x4x6x7"
                           "    + x2x4x5x6x7 + x3x4x5x6x7 + x0x1x2x3x4x5 + x0x1x2x3x4x6 + x0x1x2x3x4x7 + x0x1x2x3x5x6 + x0x1x2x4x5x6 + x0x1x2x5x6x7"
                           "    + x0x1x3x4x5x6 + x0x1x3x4x6x7 + x0x1x4x5x6x7 + x0x2x3x4x6x7 + x0x2x4x5x6x7 + x0x3x4x5x6x7 + x1x2x3x4x5x6"
                           "    + x1x2x3x4x5x7 + x1x2x3x5x6x7 + x1x2x4x5x6x7 + x2x3x4x5x6x7 + x0x1x2x3x5x6x7 + x0x2x3x4x5x6x7 + x1x2x3x4x5x6x7"; // ARIA_S2_f2


    const string anf_str = "x2 + x3 + x5 + x6 + x0x1 + x0x5 + x0x6 + x1x3 + x1x5 + x1x7 + x2x4 + x2x5 + x2x7 + x3x5 + x4x6 + x5x7 + x0x1x2"
                           "    + x0x1x3 + x0x1x5 + x0x2x4 + x0x2x7 + x0x3x7 + x0x4x5 + x0x5x7 + x0x6x7 + x1x2x5 + x1x2x7 + x1x3x6 + x1x4x5 + x1x5x6"
                           "    + x2x3x6 + x2x4x6 + x2x5x6 + x2x6x7 + x3x4x6 + x3x5x6 + x3x5x7 + x4x5x6 + x5x6x7 + x0x1x2x4 + x0x1x2x7 + x0x1x3x4"
                           "    + x0x1x3x5 + x0x1x3x6 + x0x1x3x7 + x0x1x4x7 + x0x1x6x7 + x0x2x3x4 + x0x2x3x7 + x0x2x4x7 + x0x2x5x7 + x0x3x4x5"
                           "    + x0x3x4x6 + x1x2x4x7 + x1x2x5x6 + x1x2x5x7 + x1x2x6x7 + x1x3x4x5 + x1x3x4x6 + x1x3x5x6 + x1x4x5x6 + x1x4x5x7"
                           "    + x2x3x5x6 + x2x3x6x7 + x2x4x5x6 + x2x4x5x7 + x2x4x6x7 + x2x5x6x7 + x3x4x5x6 + x4x5x6x7 + x0x1x2x3x4 + x0x1x2x3x5"
                           "    + x0x1x2x3x6 + x0x1x2x3x7 + x0x1x2x4x5 + x0x1x2x4x6 + x0x1x2x5x6 + x0x1x2x5x7 + x0x1x2x6x7 + x0x1x3x4x6 + x0x1x3x4x7"
                           "    + x0x1x3x5x7 + x0x1x3x6x7 + x0x1x5x6x7 + x0x2x3x5x6 + x0x2x3x5x7 + x0x2x3x6x7 + x0x2x4x5x6 + x0x2x4x5x7 + x0x2x4x6x7"
                           "    + x0x2x5x6x7 + x0x3x4x5x6 + x0x3x4x6x7 + x0x3x5x6x7 + x0x4x5x6x7 + x1x2x3x4x6 + x1x2x3x4x7 + x1x2x3x5x6 + x1x2x3x5x7"
                           "    + x1x2x3x6x7 + x1x2x4x5x7 + x1x2x4x6x7 + x1x3x4x5x6 + x1x3x4x5x7 + x1x4x5x6x7 + x2x3x5x6x7 + x2x4x5x6x7 + x3x4x5x6x7"
                           "    + x0x1x2x3x4x6 + x0x1x2x3x4x7 + x0x1x2x3x5x6 + x0x1x2x3x5x7 + x0x1x2x4x5x6 + x0x1x2x4x5x7 + x0x1x2x5x6x7"
                           "    + x0x1x3x4x5x6 + x0x1x3x4x6x7 + x0x1x3x5x6x7 + x0x2x3x4x5x6 + x0x2x4x5x6x7 + x1x2x3x4x5x7 + x1x2x4x5x6x7"
                           "    + x1x3x4x5x6x7 + x0x1x2x3x4x5x7 + x0x1x2x3x4x6x7 + x0x2x3x4x5x6x7"; // ARIA_S2_f3


    const string anf_str = "x0 + x1 + x2 + x3 + x7 + x0x3 + x0x5 + x1x2 + x1x3 + x1x6 + x1x7 + x2x5 + x2x6 + x3x5 + x3x6 + x3x7 + x4x5 + x4x6"
                           "    + x5x6 + x5x7 + x0x1x3 + x0x1x6 + x0x2x4 + x0x2x5 + x0x3x5 + x0x3x6 + x0x3x7 + x0x4x5 + x0x5x6 + x0x5x7 + x0x6x7"
                           "    + x1x2x3 + x1x2x4 + x1x2x7 + x1x3x4 + x1x3x5 + x1x3x7 + x1x4x5 + x1x5x7 + x1x6x7 + x2x3x4 + x2x4x5 + x2x4x7 + x2x5x6"
                           "    + x3x4x5 + x3x4x7 + x3x5x6 + x3x5x7 + x3x6x7 + x0x1x3x5 + x0x1x3x7 + x0x1x5x6 + x0x1x6x7 + x0x2x3x6 + x0x2x3x7"
                           "    + x0x2x4x7 + x0x2x5x6 + x0x2x5x7 + x0x2x6x7 + x0x3x4x6 + x0x3x4x7 + x0x4x5x6 + x0x4x6x7 + x1x2x3x4 + x1x2x3x5"
                           "    + x1x2x4x6 + x1x2x4x7 + x1x2x5x6 + x1x2x5x7 + x1x2x6x7 + x1x3x4x5 + x1x4x5x6 + x2x3x4x5 + x2x3x5x6 + x2x3x5x7"
                           "    + x3x4x5x6 + x3x4x6x7 + x3x5x6x7 + x4x5x6x7 + x0x1x2x3x4 + x0x1x2x3x6 + x0x1x2x3x7 + x0x1x2x4x5 + x0x1x2x4x6"
                           "    + x0x1x2x6x7 + x0x1x3x4x5 + x0x1x3x4x6 + x0x1x3x5x7 + x0x1x3x6x7 + x0x1x4x5x6 + x0x1x4x5x7 + x0x1x4x6x7 + x0x2x3x6x7"
                           "    + x0x2x4x5x6 + x0x2x4x5x7 + x0x2x4x6x7 + x0x2x5x6x7 + x0x3x4x5x7 + x0x3x4x6x7 + x0x4x5x6x7 + x1x2x3x4x6 + x1x2x3x4x7"
                           "    + x1x2x3x6x7 + x1x2x4x5x6 + x1x2x4x5x7 + x1x2x4x6x7 + x1x3x4x5x6 + x1x3x4x6x7 + x2x3x4x5x6 + x2x3x4x5x7 + x2x3x4x6x7"
                           "    + x2x3x5x6x7 + x2x4x5x6x7 + x0x1x2x3x4x5 + x0x1x2x3x4x7 + x0x1x2x3x6x7 + x0x1x2x4x5x7 + x0x1x2x5x6x7 + x0x1x3x4x5x6"
                           "    + x0x1x3x4x5x7 + x0x1x3x4x6x7 + x0x1x3x5x6x7 + x0x1x4x5x6x7 + x0x2x3x4x5x7 + x0x2x3x5x6x7 + x0x3x4x5x6x7"
                           "    + x1x2x3x4x5x7 + x1x2x4x5x6x7 + x0x1x2x3x4x5x6 + x0x1x2x3x4x5x7 + x0x1x2x3x4x6x7 + x0x1x2x3x5x6x7 + x0x1x3x4x5x6x7"
                           "    + x0x2x3x4x5x6x7"; // ARIA_S2_f4


    const string anf_str = "x0 + x1 + x2 + x3 + x5 + x6 + x7 + x0x2 + x0x4 + x0x6 + x1x2 + x1x5 + x1x6 + x1x7 + x2x4 + x2x5 + x2x7 + x3x5 + x3x6"
                           "    + x4x6 + x6x7 + x0x1x2 + x0x1x5 + x0x1x6 + x0x1x7 + x0x2x3 + x0x2x5 + x0x3x4 + x0x3x7 + x0x4x5 + x0x4x6 + x0x4x7"
                           "    + x0x5x6 + x0x5x7 + x1x2x3 + x1x2x5 + x1x2x7 + x1x3x4 + x1x3x6 + x1x4x5 + x1x4x7 + x2x3x4 + x2x3x6 + x2x4x7 + x2x5x7"
                           "    + x2x6x7 + x3x4x5 + x3x4x6 + x3x4x7 + x3x5x7 + x3x6x7 + x4x5x6 + x4x5x7 + x4x6x7 + x5x6x7 + x0x1x2x5 + x0x1x2x7"
                           "    + x0x1x3x4 + x0x1x3x6 + x0x1x4x5 + x0x1x5x6 + x0x2x3x4 + x0x2x3x5 + x0x2x3x6 + x0x2x4x6 + x0x2x5x7 + x0x3x4x6"
                           "    + x0x4x5x7 + x0x4x6x7 + x1x2x3x4 + x1x2x3x5 + x1x2x3x6 + x1x2x3x7 + x1x2x4x5 + x1x2x4x7 + x1x2x5x6 + x1x2x6x7"
                           "    + x1x3x4x6 + x1x3x4x7 + x1x3x5x6 + x1x4x5x6 + x1x4x5x7 + x1x5x6x7 + x2x3x4x5 + x2x3x5x6 + x2x4x6x7 + x3x4x5x6"
                           "    + x4x5x6x7 + x0x1x2x3x4 + x0x1x2x3x6 + x0x1x2x6x7 + x0x1x3x4x5 + x0x1x3x4x6 + x0x1x3x5x7 + x0x1x3x6x7 + x0x1x5x6x7"
                           "    + x0x2x3x4x5 + x0x2x3x4x7 + x0x2x3x5x6 + x0x2x3x5x7 + x0x2x3x6x7 + x0x2x4x5x6 + x0x2x4x5x7 + x0x2x5x6x7 + x0x3x4x5x7"
                           "    + x0x4x5x6x7 + x1x2x3x4x7 + x1x2x3x5x7 + x1x2x3x6x7 + x1x3x4x5x6 + x1x3x4x5x7 + x1x3x4x6x7 + x1x3x5x6x7 + x2x3x4x5x6"
                           "    + x2x3x4x5x7 + x3x4x5x6x7 + x0x1x2x3x4x5 + x0x1x2x3x4x7 + x0x1x2x3x6x7 + x0x1x2x4x5x6 + x0x1x2x4x5x7 + x0x1x2x5x6x7"
                           "    + x0x1x3x4x5x7 + x0x2x3x4x5x6 + x0x2x3x5x6x7 + x1x2x3x4x5x7 + x1x2x3x4x6x7 + x1x2x3x5x6x7 + x0x1x2x3x4x5x7"
                           "    + x0x1x2x3x4x6x7 + x0x1x3x4x5x6x7 + x0x2x3x4x5x6x7"; // ARIA_S2_f5


    const string anf_str = "1 + x0 + x1 + x2 + x5 + x6 + x0x1 + x0x2 + x0x5 + x0x6 + x1x5 + x1x6 + x1x7 + x2x4 + x2x5 + x2x6 + x2x7 + x3x4"
                           "    + x4x6 + x4x7 + x5x7 + x0x1x3 + x0x1x6 + x0x2x3 + x0x2x5 + x0x2x6 + x0x2x7 + x0x3x5 + x0x3x6 + x0x4x6 + x0x5x7"
                           "    + x1x2x5 + x1x2x6 + x1x4x6 + x1x5x7 + x1x6x7 + x2x3x4 + x2x3x5 + x2x3x6 + x2x4x7 + x3x4x6 + x3x6x7 + x4x5x6 + x4x6x7"
                           "    + x0x1x2x4 + x0x1x3x5 + x0x1x3x6 + x0x1x4x7 + x0x1x6x7 + x0x2x3x5 + x0x2x3x6 + x0x2x3x7 + x0x2x4x6 + x0x2x4x7"
                           "    + x0x2x5x7 + x0x2x6x7 + x0x3x4x7 + x0x3x5x6 + x0x3x6x7 + x0x4x6x7 + x1x2x3x4 + x1x2x4x5 + x1x2x4x6 + x1x2x4x7"
                           "    + x1x2x5x6 + x1x2x5x7 + x1x3x5x6 + x1x3x6x7 + x1x4x5x7 + x2x3x4x5 + x2x3x5x7 + x2x4x6x7 + x3x4x5x6 + x3x4x5x7"
                           "    + x3x4x6x7 + x0x1x2x3x4 + x0x1x2x3x6 + x0x1x2x4x6 + x0x1x2x5x6 + x0x1x2x5x7 + x0x1x3x4x7 + x0x1x3x5x7 + x0x1x4x5x7"
                           "    + x0x2x3x4x7 + x0x2x3x5x6 + x0x2x4x5x6 + x0x2x5x6x7 + x0x3x4x5x7 + x0x3x4x6x7 + x0x3x5x6x7 + x1x2x3x4x5 + x1x2x3x5x7"
                           "    + x1x2x4x5x7 + x1x2x4x6x7 + x1x3x4x5x7 + x1x3x5x6x7 + x0x1x2x3x4x6 + x0x1x2x3x4x7 + x0x1x2x4x5x7 + x0x1x2x5x6x7"
                           "    + x0x1x3x4x5x7 + x0x1x3x4x6x7 + x0x1x3x5x6x7 + x0x2x3x4x5x6 + x0x2x3x5x6x7 + x1x2x3x4x5x7 + x1x2x3x5x6x7"
                           "    + x1x3x4x5x6x7 + x2x3x4x5x6x7 + x0x1x2x3x4x5x7 + x0x1x2x3x4x6x7 + x0x1x2x3x5x6x7 + x0x1x2x4x5x6x7 + x0x2x3x4x5x6x7"; // ARIA_S2_f6


    const string anf_str = "x1 + x2 + x0x1 + x0x4 + x1x2 + x1x3 + x1x5 + x1x6 + x2x3 + x2x4 + x2x7 + x4x7 + x0x1x3 + x0x1x5 + x0x2x3 + x0x2x4"
                           "    + x0x2x7 + x0x3x5 + x0x4x5 + x0x4x7 + x0x5x7 + x0x6x7 + x1x2x4 + x1x2x7 + x1x3x4 + x1x3x5 + x1x3x6 + x1x4x5 + x1x5x7"
                           "    + x1x6x7 + x2x3x4 + x2x4x5 + x2x6x7 + x3x4x5 + x3x5x7 + x3x6x7 + x4x5x6 + x4x6x7 + x0x1x2x3 + x0x1x2x4 + x0x1x2x5"
                           "    + x0x1x3x4 + x0x1x3x7 + x0x1x4x5 + x0x1x4x7 + x0x2x3x4 + x0x2x3x6 + x0x2x4x6 + x0x2x4x7 + x0x2x5x6 + x0x2x5x7"
                           "    + x0x3x4x7 + x0x3x6x7 + x1x2x3x5 + x1x2x3x6 + x1x2x4x6 + x1x2x5x6 + x1x2x6x7 + x1x3x4x6 + x1x3x4x7 + x1x3x5x7"
                           "    + x1x4x5x6 + x2x3x4x5 + x2x3x4x6 + x2x3x4x7 + x2x3x5x6 + x2x4x5x6 + x3x4x5x6 + x3x5x6x7 + x0x1x2x3x4 + x0x1x2x3x6"
                           "    + x0x1x2x3x7 + x0x1x2x4x6 + x0x1x2x4x7 + x0x1x2x5x7 + x0x1x2x6x7 + x0x1x3x4x5 + x0x1x3x5x7 + x0x1x4x5x7 + x0x1x5x6x7"
                           "    + x0x2x3x5x7 + x0x2x3x6x7 + x0x2x4x5x6 + x0x2x4x5x7 + x1x2x3x4x6 + x1x2x3x5x6 + x1x2x3x5x7 + x1x2x3x6x7 + x1x2x4x5x6"
                           "    + x1x3x4x5x6 + x1x4x5x6x7 + x2x3x4x5x7 + x2x3x4x6x7 + x3x4x5x6x7 + x0x1x2x3x4x5 + x0x1x2x3x4x6 + x0x1x2x3x5x6"
                           "    + x0x1x2x3x5x7 + x0x1x2x3x6x7 + x0x1x2x4x5x6 + x0x1x2x4x5x7 + x0x1x2x4x6x7 + x0x1x3x4x6x7 + x0x1x3x5x6x7"
                           "    + x0x2x3x4x5x7 + x0x2x3x4x6x7 + x0x2x3x5x6x7 + x0x2x4x5x6x7 + x1x2x3x4x5x6 + x1x2x3x4x5x7 + x1x2x3x4x6x7"
                           "    + x1x2x4x5x6x7 + x0x1x2x4x5x6x7"; // ARIA_S2_f7

***************************************************************************




**********************************Camellia S1 Sbox*****************************************

    const string anf_str = "x0 + x1 + x2 + x4 + x5 + x7 + x0x1 + x0x2 + x0x3 + x0x5 + x0x7 + x1x5 + x2x7 + x3x5 + x4x5 + x4x6 + x4x7 + x0x1x2"
                           "    x0x1x5 + x0x1x7 + x0x3x7 + x0x4x6 + x0x4x7 + x0x5x6 + x0x6x7 + x1x2x3 + x1x2x4 + x1x2x5 + x1x4x6 + x1x5x7 + x2x3x4"
                           "    x2x3x6 + x2x4x7 + x2x5x7 + x3x4x7 + x3x5x6 + x3x5x7 + x4x5x6 + x4x5x7 + x5x6x7 + x0x1x2x6 + x0x1x3x7 + x0x1x4x5"
                           "    x0x1x5x6 + x0x1x5x7 + x0x2x3x4 + x0x2x3x7 + x0x2x4x6 + x0x2x5x6 + x0x2x5x7 + x0x2x6x7 + x0x3x4x5 + x0x3x4x7"
                           "    x0x3x5x6 + x0x4x5x6 + x0x4x5x7 + x0x5x6x7 + x1x2x3x6 + x1x2x3x7 + x1x2x4x6 + x1x2x5x6 + x1x2x5x7 + x1x3x4x5"
                           "    x1x3x4x6 + x1x3x5x6 + x1x3x5x7 + x1x3x6x7 + x1x4x5x6 + x2x3x4x6 + x2x3x5x6 + x2x3x5x7 + x2x3x6x7 + x2x4x5x7"
                           "    x2x4x6x7 + x2x5x6x7 + x3x4x5x7 + x3x4x6x7 + x4x5x6x7 + x0x1x2x3x5 + x0x1x2x4x5 + x0x1x2x4x7 + x0x1x2x5x6"
                           "    x0x1x2x5x7 + x0x1x2x6x7 + x0x1x3x4x5 + x0x1x3x6x7 + x0x1x4x5x6 + x0x2x3x6x7 + x0x2x4x5x6 + x0x2x4x6x7 + x0x2x5x6x7"
                           "    x0x3x4x5x7 + x0x3x4x6x7 + x0x4x5x6x7 + x1x2x3x5x6 + x1x2x3x5x7 + x1x2x3x6x7 + x1x2x4x5x6 + x1x2x4x5x7 + x1x2x5x6x7"
                           "    x1x3x4x5x7 + x2x3x4x5x7 + x2x3x4x6x7 + x2x4x5x6x7 + x3x4x5x6x7 + x0x1x2x3x4x5 + x0x1x2x3x4x6 + x0x1x2x3x4x7"
                           "    x0x1x2x3x6x7 + x0x1x2x4x6x7 + x0x1x2x5x6x7 + x0x1x3x4x5x6 + x0x1x3x4x6x7 + x0x1x3x5x6x7 + x0x1x4x5x6x7"
                           "    x0x2x3x4x5x6 + x0x2x3x4x5x7 + x0x2x3x4x6x7 + x1x2x3x4x5x6 + x1x2x3x5x6x7 + x1x2x4x5x6x7 + x1x3x4x5x6x7"
                           "    x2x3x4x5x6x7 + x0x1x2x3x4x5x6 + x0x1x2x3x4x5x7 + x0x1x2x4x5x6x7 + x1x2x3x4x5x6x7"; //camellia f0

    const string anf_str = "1 + x0 + x1 + x2 + x3 + x5 + x6 + x7 + x0x2 + x0x3 + x0x4 + x0x5 + x0x6 + x1x2 + x1x4 + x1x6 + x1x7 + x2x3 + x2x6"
                           "    x2x7 + x3x4 + x4x5 + x4x6 + x5x7 + x0x1x2 + x0x1x4 + x0x1x5 + x0x2x5 + x0x2x6 + x0x2x7 + x0x4x5 + x0x4x7 + x0x5x6"
                           "    x1x2x3 + x1x2x4 + x1x2x6 + x1x4x7 + x1x5x6 + x1x5x7 + x1x6x7 + x2x3x7 + x2x4x5 + x2x5x6 + x2x5x7 + x3x4x5 + x3x5x7"
                           "    x4x5x6 + x4x5x7 + x0x1x2x4 + x0x1x2x6 + x0x1x2x7 + x0x1x3x5 + x0x1x3x7 + x0x1x4x5 + x0x1x4x6 + x0x1x4x7 + x0x1x5x6"
                           "    x0x1x5x7 + x0x1x6x7 + x0x2x3x4 + x0x2x3x5 + x0x2x3x7 + x0x2x4x5 + x0x2x4x6 + x0x2x5x6 + x0x2x5x7 + x0x2x6x7"
                           "    x0x3x4x5 + x0x4x5x6 + x0x5x6x7 + x1x2x3x5 + x1x2x4x6 + x1x2x5x6 + x1x2x5x7 + x1x2x6x7 + x1x3x4x5 + x1x3x6x7"
                           "    x1x4x5x7 + x1x5x6x7 + x2x3x4x6 + x2x3x5x6 + x2x4x5x7 + x2x5x6x7 + x3x4x5x6 + x3x4x5x7 + x3x4x6x7 + x3x5x6x7"
                           "    x0x1x2x3x6 + x0x1x2x3x7 + x0x1x2x4x5 + x0x1x2x4x6 + x0x1x2x4x7 + x0x1x2x5x7 + x0x1x3x4x5 + x0x1x3x4x7 + x0x1x3x5x7"
                           "    x0x1x3x6x7 + x0x1x4x5x7 + x0x1x5x6x7 + x0x2x3x6x7 + x0x2x4x5x6 + x0x2x4x6x7 + x0x2x5x6x7 + x0x3x4x6x7 + x0x3x5x6x7"
                           "    x0x4x5x6x7 + x1x2x3x4x5 + x1x2x3x4x7 + x1x2x5x6x7 + x1x3x4x5x6 + x1x4x5x6x7 + x2x3x4x5x7 + x2x3x5x6x7 + x2x4x5x6x7"
                           "    x3x4x5x6x7 + x0x1x2x3x4x5 + x0x1x2x3x5x7 + x0x1x2x4x6x7 + x0x1x4x5x6x7 + x0x2x3x4x6x7 + x0x2x4x5x6x7 + x1x2x3x4x6x7"
                           "    x1x2x4x5x6x7 + x1x3x4x5x6x7 + x0x1x2x3x4x5x6 + x0x1x2x3x4x5x7 + x0x1x2x3x4x6x7 + x0x1x2x3x5x6x7 + x0x1x2x4x5x6x7"
                           "    x0x1x3x4x5x6x7 + x0x2x3x4x5x6x7 + x1x2x3x4x5x6x7"; //camellia f1

    const string anf_str = "1 + x1 + x2 + x7 + x0x1 + x0x3 + x0x4 + x1x2 + x1x4 + x1x5 + x1x7 + x2x3 + x2x4 + x2x5 + x2x6 + x3x5 + x3x7 + x4x6"
                           "    x5x6 + x5x7 + x6x7 + x0x1x3 + x0x2x4 + x0x2x5 + x0x2x6 + x0x2x7 + x0x3x4 + x0x4x5 + x0x4x6 + x0x4x7 + x0x5x6"
                           "    x0x5x7 + x1x2x4 + x1x3x5 + x1x3x7 + x1x4x5 + x1x4x6 + x1x4x7 + x1x5x6 + x2x3x5 + x2x3x6 + x2x4x6 + x2x4x7 + x2x5x6"
                           "    x2x6x7 + x3x4x7 + x3x5x7 + x4x5x7 + x4x6x7 + x0x1x2x4 + x0x1x2x5 + x0x1x3x5 + x0x1x3x6 + x0x1x3x7 + x0x1x4x5"
                           "    x0x1x4x6 + x0x1x5x7 + x0x2x3x6 + x0x2x3x7 + x0x2x4x6 + x0x2x5x6 + x0x3x4x6 + x0x3x5x7 + x0x3x6x7 + x0x4x5x7"
                           "    x0x5x6x7 + x1x2x3x4 + x1x2x3x6 + x1x2x3x7 + x1x2x4x6 + x1x2x4x7 + x1x2x5x7 + x1x3x4x5 + x1x3x6x7 + x1x4x5x6"
                           "    x1x4x5x7 + x1x4x6x7 + x2x3x4x5 + x2x3x6x7 + x2x4x5x6 + x2x4x6x7 + x3x4x5x7 + x3x4x6x7 + x3x5x6x7 + x0x1x2x3x5"
                           "    x0x1x2x3x7 + x0x1x2x4x5 + x0x1x2x4x6 + x0x1x2x5x6 + x0x1x2x5x7 + x0x1x3x4x6 + x0x1x3x4x7 + x0x1x3x5x7 + x0x1x3x6x7"
                           "    x0x1x4x6x7 + x0x2x3x4x7 + x0x2x3x5x7 + x0x2x3x6x7 + x0x2x4x5x7 + x0x3x4x5x6 + x0x3x4x6x7 + x0x3x5x6x7 + x1x2x3x4x6"
                           "    x1x2x3x5x6 + x1x2x3x5x7 + x1x2x4x5x6 + x1x2x4x5x7 + x1x3x4x5x6 + x1x3x4x5x7 + x1x3x4x6x7 + x1x4x5x6x7 + x2x3x5x6x7"
                           "    x0x1x2x3x4x7 + x0x1x2x3x6x7 + x0x1x2x4x5x6 + x0x1x2x4x5x7 + x0x1x2x4x6x7 + x0x1x3x4x5x6 + x0x1x3x4x5x7"
                           "    x0x1x4x5x6x7 + x0x2x3x5x6x7 + x0x3x4x5x6x7 + x2x3x4x5x6x7 + x0x1x2x3x4x5x7 + x0x1x2x3x4x6x7 + x0x2x3x4x5x6x7"; //camellia f2
   
    const string anf_str = "1 + x0 + x1 + x2 + x3 + x4 + x6 + x7 + x0x1 + x0x2 + x0x6 + x1x5 + x1x7 + x2x3 + x2x5 + x2x6 + x3x4 + x3x6 + x3x7"
                           "    x4x7 + x6x7 + x0x1x2 + x0x1x4 + x0x1x7 + x0x2x3 + x0x2x5 + x0x2x6 + x0x3x4 + x0x3x5 + x0x3x7 + x0x4x5 + x0x5x7"
                           "    x0x6x7 + x1x2x4 + x1x3x4 + x1x3x5 + x1x3x6 + x1x4x5 + x1x4x7 + x1x5x6 + x1x5x7 + x2x3x4 + x2x4x5 + x3x4x5 + x3x4x7"
                           "    x3x5x7 + x4x5x6 + x4x6x7 + x0x1x2x4 + x0x1x2x5 + x0x1x3x4 + x0x1x3x6 + x0x1x3x7 + x0x1x4x5 + x0x1x4x6 + x0x1x5x6"
                           "    x0x1x6x7 + x0x2x3x4 + x0x2x3x6 + x0x2x3x7 + x0x2x4x7 + x0x2x5x6 + x0x2x6x7 + x0x3x4x5 + x0x3x4x6 + x0x3x4x7"
                           "    x0x3x6x7 + x0x4x5x6 + x0x4x6x7 + x0x5x6x7 + x1x2x3x6 + x1x2x4x7 + x1x2x5x6 + x1x2x5x7 + x1x3x4x5 + x1x3x4x6"
                           "    x1x3x4x7 + x1x3x5x7 + x1x4x5x7 + x1x5x6x7 + x2x3x4x5 + x2x3x4x6 + x2x3x4x7 + x2x3x5x6 + x2x4x5x7 + x2x5x6x7"
                           "    x3x4x5x6 + x0x1x2x3x4 + x0x1x2x3x5 + x0x1x2x3x7 + x0x1x2x5x6 + x0x1x2x5x7 + x0x1x2x6x7 + x0x1x3x4x6 + x0x1x4x5x7"
                           "    x0x1x5x6x7 + x0x2x3x4x5 + x0x2x3x5x7 + x0x2x3x6x7 + x0x2x4x5x6 + x0x2x4x5x7 + x0x2x5x6x7 + x0x3x4x5x7 + x0x3x5x6x7"
                           "    x0x4x5x6x7 + x1x2x3x4x6 + x1x2x3x4x7 + x1x2x3x5x6 + x1x2x3x6x7 + x1x2x4x5x6 + x1x2x4x5x7 + x1x2x4x6x7 + x1x2x5x6x7"
                           "    x1x3x4x5x7 + x1x3x5x6x7 + x2x3x4x5x7 + x2x3x4x6x7 + x2x3x5x6x7 + x2x4x5x6x7 + x3x4x5x6x7 + x0x1x2x3x5x6"
                           "    x0x1x2x4x6x7 + x0x1x3x4x5x7 + x0x1x3x4x6x7 + x0x1x3x5x6x7 + x0x2x3x4x5x6 + x0x2x3x4x6x7 + x0x2x4x5x6x7"
                           "    x1x2x3x4x5x6 + x1x2x3x4x6x7 + x2x3x4x5x6x7 + x0x1x2x3x4x5x6 + x0x1x2x3x5x6x7 + x0x1x2x4x5x6x7 + x0x2x3x4x5x6x7"; //camellia f3
/*
    const string anf_str = "x0 + x1 + x6 + x0x1 + x0x2 + x0x3 + x0x4 + x0x5 + x0x7 + x1x2 + x1x4 + x1x6 + x2x4 + x2x5 + x2x7 + x3x4 + x3x7"
                           "    x4x5 + x4x6 + x5x6 + x0x1x2 + x0x1x3 + x0x1x5 + x0x1x6 + x0x1x7 + x0x2x3 + x0x2x5 + x0x2x6 + x0x2x7 + x0x3x4"
                           "    x0x3x5 + x0x3x6 + x0x4x6 + x1x2x3 + x1x2x6 + x1x3x5 + x1x3x7 + x1x4x5 + x1x6x7 + x2x3x4 + x2x3x5 + x2x5x7 + x2x6x7"
                           "    x3x4x5 + x3x4x7 + x4x5x6 + x0x1x2x4 + x0x1x2x5 + x0x1x3x4 + x0x1x3x5 + x0x1x3x6 + x0x1x4x6 + x0x1x4x7 + x0x2x3x4"
                           "    x0x2x3x7 + x0x2x4x5 + x0x2x4x7 + x0x3x4x6 + x0x3x5x6 + x0x3x6x7 + x0x4x5x6 + x0x5x6x7 + x1x2x3x4 + x1x2x3x6"
                           "    x1x2x4x5 + x1x2x4x6 + x1x2x5x6 + x1x3x4x5 + x1x3x4x6 + x1x3x5x6 + x1x3x5x7 + x1x4x5x6 + x2x3x4x5 + x2x3x5x6"
                           "    x2x3x5x7 + x2x3x6x7 + x2x4x5x7 + x2x5x6x7 + x3x4x5x6 + x3x4x5x7 + x3x5x6x7 + x4x5x6x7 + x0x1x2x4x7 + x0x1x2x5x6"
                           "    x0x1x2x5x7 + x0x1x2x6x7 + x0x1x3x4x6 + x0x1x3x5x7 + x0x1x3x6x7 + x0x1x4x5x6 + x0x1x4x5x7 + x0x1x4x6x7 + x0x2x3x4x5"
                           "    x0x2x3x4x6 + x0x2x3x4x7 + x0x2x3x5x6 + x0x2x3x6x7 + x0x2x4x5x6 + x0x2x4x6x7 + x0x2x5x6x7 + x0x3x4x6x7 + x0x4x5x6x7"
                           "    x1x2x3x4x5 + x1x2x3x4x7 + x1x2x3x5x6 + x1x2x3x6x7 + x1x2x4x5x6 + x1x3x4x5x6 + x1x3x4x5x7 + x1x3x4x6x7 + x1x3x5x6x7"
                           "    x1x4x5x6x7 + x2x3x4x5x6 + x2x3x4x5x7 + x0x1x2x3x4x6 + x0x1x2x3x4x7 + x0x1x2x3x5x6 + x0x1x2x4x5x6 + x0x1x2x4x5x7"
                           "    x0x1x3x4x6x7 + x0x1x3x5x6x7 + x0x1x4x5x6x7 + x0x3x4x5x6x7 + x1x2x3x4x5x6 + x1x2x3x4x5x7 + x2x3x4x5x6x7"
                           "    x0x1x2x3x4x5x6 + x0x1x3x4x5x6x7 + x1x2x3x4x5x6x7"; //camellia f4

    const string anf_str = "x2 + x4 + x6 + x0x6 + x1x3 + x1x6 + x1x7 + x2x4 + x2x6 + x2x7 + x3x5 + x3x6 + x3x7 + x4x5 + x4x6 + x5x6 + x5x7"
                           "    x0x1x2 + x0x1x3 + x0x1x4 + x0x1x5 + x0x1x7 + x0x2x3 + x0x2x4 + x0x2x7 + x0x3x4 + x0x3x5 + x0x3x6 + x0x4x6 + x0x4x7"
                           "    x0x5x6 + x0x5x7 + x0x6x7 + x1x2x3 + x1x2x4 + x1x2x7 + x1x3x4 + x1x3x5 + x1x5x6 + x1x5x7 + x2x3x4 + x2x3x5 + x2x3x7"
                           "    x2x4x5 + x2x4x6 + x2x5x6 + x2x5x7 + x2x6x7 + x3x4x6 + x3x4x7 + x3x5x6 + x3x5x7 + x3x6x7 + x0x1x2x5 + x0x1x3x4"
                           "    x0x1x3x5 + x0x1x3x7 + x0x1x4x5 + x0x1x4x6 + x0x1x5x7 + x0x1x6x7 + x0x2x3x4 + x0x2x3x5 + x0x2x3x6 + x0x2x4x5"
                           "    x0x2x4x6 + x0x2x5x6 + x0x3x4x5 + x0x3x4x7 + x0x3x5x7 + x0x3x6x7 + x0x4x5x6 + x1x2x3x4 + x1x2x3x7 + x1x2x5x6"
                           "    x1x2x5x7 + x1x3x4x7 + x1x3x5x7 + x1x3x6x7 + x1x4x5x7 + x1x4x6x7 + x2x3x6x7 + x2x4x5x6 + x2x4x5x7 + x3x4x5x6"
                           "    x3x4x6x7 + x3x5x6x7 + x0x1x2x3x6 + x0x1x2x3x7 + x0x1x2x4x7 + x0x1x2x5x6 + x0x1x2x6x7 + x0x1x3x4x5 + x0x1x3x5x7"
                           "    x0x1x4x6x7 + x0x2x4x5x6 + x0x2x4x5x7 + x0x2x5x6x7 + x0x3x4x5x6 + x0x3x4x5x7 + x0x3x4x6x7 + x0x4x5x6x7 + x1x2x3x4x7"
                           "    x1x2x3x5x6 + x1x2x3x5x7 + x1x2x3x6x7 + x1x2x4x5x7 + x1x2x4x6x7 + x1x2x5x6x7 + x1x3x5x6x7 + x2x3x4x5x7 + x2x3x4x6x7"
                           "    x2x4x5x6x7 + x0x1x2x3x4x7 + x0x1x2x3x5x6 + x0x1x2x3x5x7 + x0x1x2x3x6x7 + x0x1x3x4x5x6 + x0x1x3x4x6x7 + x0x1x3x5x6x7"
                           "    x0x2x3x4x5x6 + x0x2x3x4x5x7 + x0x2x3x5x6x7 + x1x2x3x4x5x6 + x1x2x3x4x5x7 + x1x2x3x5x6x7 + x1x2x4x5x6x7"
                           "    x1x3x4x5x6x7 + x0x1x2x3x4x5x6 + x0x1x2x3x4x5x7 + x0x1x2x3x5x6x7 + x0x2x3x4x5x6x7 + x1x2x3x4x5x6x7"; //camellia f5
  
    const string anf_str = "x0 + x1 + x2 + x3 + x5 + x7 + x0x2 + x0x4 + x0x6 + x1x3 + x1x4 + x1x5 + x2x3 + x3x4 + x3x7 + x4x6 + x4x7 + x5x6"
                           "    x5x7 + x6x7 + x0x1x2 + x0x1x5 + x0x1x7 + x0x2x4 + x0x2x6 + x0x3x4 + x0x3x6 + x0x4x5 + x0x4x6 + x0x4x7 + x0x5x6"
                           "    x1x2x5 + x1x2x6 + x1x2x7 + x1x3x4 + x1x3x5 + x1x3x7 + x1x4x7 + x1x5x7 + x2x3x6 + x2x3x7 + x2x4x5 + x2x4x7 + x2x5x7"
                           "    x3x4x5 + x3x5x6 + x3x5x7 + x3x6x7 + x5x6x7 + x0x1x2x4 + x0x1x2x5 + x0x1x2x7 + x0x1x3x4 + x0x1x3x5 + x0x1x3x6"
                           "    x0x1x4x5 + x0x1x4x6 + x0x1x4x7 + x0x2x3x4 + x0x2x3x6 + x0x2x3x7 + x0x4x5x6 + x0x4x5x7 + x0x4x6x7 + x1x2x4x6"
                           "    x1x2x4x7 + x1x2x5x6 + x1x2x5x7 + x1x3x4x6 + x1x3x5x6 + x1x3x5x7 + x1x3x6x7 + x1x4x5x7 + x1x4x6x7 + x1x5x6x7"
                           "    x2x3x4x5 + x2x3x4x6 + x2x3x4x7 + x2x3x5x6 + x2x3x5x7 + x2x4x6x7 + x2x5x6x7 + x3x4x5x7 + x3x4x6x7 + x3x5x6x7"
                           "    x0x1x2x3x4 + x0x1x2x4x5 + x0x1x2x4x6 + x0x1x2x4x7 + x0x1x2x5x7 + x0x1x2x6x7 + x0x1x3x6x7 + x0x1x4x5x7 + x0x2x3x4x6"
                           "    x0x2x3x4x7 + x0x2x3x5x7 + x0x2x3x6x7 + x0x2x4x6x7 + x0x2x5x6x7 + x0x3x4x5x7 + x0x3x5x6x7 + x0x4x5x6x7 + x1x2x3x4x5"
                           "    x1x2x3x5x6 + x1x2x3x5x7 + x1x2x4x5x6 + x1x3x4x5x6 + x2x4x5x6x7 + x0x1x2x3x4x5 + x0x1x2x3x4x6 + x0x1x2x3x5x7"
                           "    x0x1x2x4x5x6 + x0x1x2x4x5x7 + x0x1x3x4x5x6 + x0x1x3x4x6x7 + x0x1x3x5x6x7 + x0x1x4x5x6x7 + x0x2x3x4x5x7"
                           "    x0x2x3x5x6x7 + x0x3x4x5x6x7 + x1x2x3x5x6x7 + x2x3x4x5x6x7 + x0x1x2x3x4x5x6 + x0x1x2x3x4x5x7 + x0x1x2x3x4x6x7"
                           "    x0x1x2x3x5x6x7 + x0x1x2x4x5x6x7 + x0x2x3x4x5x6x7 + x1x2x3x4x5x6x7"; //camellia f6
 
    const string anf_str = "x1 + x3 + x5 + x0x2 + x0x3 + x1x2 + x1x3 + x1x4 + x1x5 + x1x6 + x2x3 + x2x5 + x2x6 + x3x5 + x4x5 + x4x6 + x4x7"
                           "    x5x6 + x0x1x2 + x0x1x4 + x0x1x6 + x0x2x6 + x0x2x7 + x0x3x5 + x0x4x6 + x0x6x7 + x1x2x3 + x1x2x4 + x1x2x5 + x1x3x4"
                           "    x1x3x6 + x1x3x7 + x1x4x5 + x1x4x6 + x1x4x7 + x1x5x6 + x1x5x7 + x2x3x4 + x2x3x7 + x2x4x5 + x2x4x7 + x2x5x6 + x2x5x7"
                           "    x3x4x5 + x3x4x6 + x3x5x6 + x4x5x7 + x4x6x7 + x5x6x7 + x0x1x2x3 + x0x1x2x4 + x0x1x2x6 + x0x1x3x4 + x0x1x3x5"
                           "    x0x1x3x6 + x0x1x3x7 + x0x1x4x6 + x0x1x5x6 + x0x2x3x7 + x0x2x4x6 + x0x2x4x7 + x0x2x5x6 + x0x2x5x7 + x0x2x6x7"
                           "    x0x3x4x6 + x0x3x6x7 + x0x4x5x6 + x0x4x5x7 + x0x5x6x7 + x1x2x5x7 + x1x2x6x7 + x1x3x4x6 + x1x3x4x7 + x1x3x5x6"
                           "    x1x4x5x6 + x1x4x5x7 + x1x5x6x7 + x2x3x4x7 + x2x4x5x6 + x2x4x5x7 + x2x5x6x7 + x3x4x5x6 + x3x4x6x7 + x3x5x6x7"
                           "    x4x5x6x7 + x0x1x2x3x7 + x0x1x2x4x5 + x0x1x2x5x7 + x0x1x3x5x6 + x0x1x4x6x7 + x0x1x5x6x7 + x0x2x3x4x5 + x0x2x3x4x6"
                           "    x0x2x4x5x7 + x0x2x5x6x7 + x0x3x4x5x7 + x0x3x4x6x7 + x0x3x5x6x7 + x1x2x3x4x5 + x1x2x3x4x6 + x1x2x3x4x7 + x1x2x3x5x6"
                           "    x1x2x3x5x7 + x1x2x4x6x7 + x1x3x4x5x6 + x1x3x4x5x7 + x2x3x4x5x6 + x2x3x5x6x7 + x0x1x2x3x4x6 + x0x1x2x3x4x7"
                           "    x0x1x2x4x5x6 + x0x1x2x4x5x7 + x0x1x2x4x6x7 + x0x1x2x5x6x7 + x0x1x3x4x5x7 + x0x1x4x5x6x7 + x0x2x3x4x5x7"
                           "    x0x2x3x4x6x7 + x0x2x4x5x6x7 + x0x3x4x5x6x7 + x1x2x3x5x6x7 + x1x3x4x5x6x7 + x2x3x4x5x6x7 + x0x1x2x3x4x5x6"
                           "    x0x1x2x3x5x6x7 + x0x1x3x4x5x6x7"; //camellia f7
***************************************************************************




**********************************CLEFIA S1 Sbox***************************

    const string anf_str = "x1 + x3 + x4 + x6 + x7 + x0x4 + x0x7 + x1x2 + x1x4 + x1x6 + x1x7 + x2x5 + x3x4 + x3x6 + x4x5 + x4x6 + x5x6 + x6x7"
                           "    + x0x1x2 + x0x1x3 + x0x1x4 + x0x1x6 + x0x2x3 + x0x3x5 + x0x3x6 + x0x3x7 + x0x4x5 + x0x4x6 + x0x5x6 + x0x6x7 + x1x2x4"
                           "    + x1x3x5 + x1x4x5 + x1x4x7 + x1x5x6 + x2x3x4 + x2x4x6 + x2x5x6 + x2x5x7 + x3x4x5 + x3x5x6 + x3x5x7 + x4x5x6 + x4x5x7"
                           "    + x4x6x7 + x0x1x2x3 + x0x1x3x5 + x0x1x3x6 + x0x1x3x7 + x0x1x4x5 + x0x1x4x6 + x0x1x4x7 + x0x1x5x6 + x0x1x6x7"
                           "    + x0x2x3x5 + x0x2x3x7 + x0x2x4x7 + x0x2x5x6 + x0x2x6x7 + x0x3x4x7 + x0x3x5x6 + x0x3x6x7 + x0x4x5x6 + x0x4x6x7"
                           "    + x1x2x3x5 + x1x2x3x6 + x1x2x6x7 + x1x3x4x6 + x1x3x4x7 + x1x3x5x6 + x1x4x5x6 + x1x5x6x7 + x2x3x4x5 + x2x3x4x6"
                           "    + x2x3x6x7 + x2x4x6x7 + x2x5x6x7 + x3x4x6x7 + x3x5x6x7 + x4x5x6x7 "
                           "    + x0x1x2x4x6 + x0x1x2x5x6 + x0x1x2x5x7 + x0x1x2x6x7 + x0x1x3x4x7 + x0x1x3x6x7 + x0x1x4x5x6 + x0x1x5x6x7 + x0x2x3x4x6 + x0x2x3x5x7 + x0x2x4x5x7 + x0x2x4x6x7"
                           "    + x0x3x4x6x7 + x0x3x5x6x7 + x0x4x5x6x7 + x1x2x3x4x5 + x1x2x3x4x6 + x1x2x3x4x7 + x1x2x3x5x6 + x1x2x5x6x7 + x1x3x4x5x7"
                           "    + x1x3x5x6x7 + x2x3x4x5x7 + x2x3x4x6x7 + x2x3x5x6x7 + x2x4x5x6x7"
                           "    + x0x1x2x3x4x5 + x0x1x2x3x4x6 + x0x1x2x3x4x7 + x0x1x2x3x5x6 + x0x1x2x4x5x7 + x0x1x2x4x6x7 + x0x1x2x5x6x7 + x0x1x4x5x6x7 + x0x2x3x4x5x6 + x0x2x3x4x5x7"
                           "    + x0x2x3x4x6x7 + x0x2x4x5x6x7 + x1x2x3x4x5x7 + x1x2x3x4x6x7 + x1x2x3x5x6x7 + x1x2x4x5x6x7 "
                           "    + x0x1x2x3x4x5x6 + x0x1x2x3x4x5x7 + x0x1x2x4x5x6x7 + x0x2x3x4x5x6x7 + x1x2x3x4x5x6x7 + x0x1x2x3x4x5x6x7"; //CLEFIA S1 f0


    const string anf_str = "1 + x2 + x3 + x4 + x0x1 + x0x2 + x0x4 + x0x5 + x0x7 + x1x2 + x1x4 + x2x3 + x2x6 + x2x7 + x3x4 + x3x7 + x5x6 + x5x7\n"
                           "    + x0x1x6 + x0x1x7 + x0x4x5 + x0x4x6 + x0x5x6 + x0x5x7 + x1x2x4 + x1x2x7 + x1x3x4 + x1x3x5 + x1x4x5 + x1x6x7 + x2x5x6\n"
                           "\n"
                           "    + x2x5x7 + x3x4x6 + x3x4x7 + x3x6x7 + x4x5x6 + x4x5x7 + x5x6x7 + x0x1x2x4 + x0x1x2x5 + x0x1x3x5 + x0x1x3x7\n"
                           "    + x0x1x4x5 + x0x1x4x7 + x0x1x5x6 + x0x1x5x7 + x0x2x3x4 + x0x2x3x5 + x0x2x3x6 + x0x2x3x7 + x0x2x4x6 + x0x2x4x7\n"
                           "    + x0x2x6x7 + x0x3x4x5 + x0x3x4x6 + x0x3x5x6 + x0x3x6x7 + x0x4x5x6 + x0x4x6x7 + x0x5x6x7 + x1x2x3x4 + x1x2x3x6\n"
                           "    + x1x2x3x7 + x1x2x4x5 + x1x2x5x7 + x1x3x4x7 + x1x3x5x7 + x1x3x6x7 + x1x4x5x6 + x1x4x5x7 + x1x4x6x7 + x2x3x4x5\n"
                           "    + x2x3x4x6 + x2x3x5x6 + x2x4x5x7 + x2x4x6x7 + x3x4x5x6 + x3x4x6x7 + x0x1x2x3x4 + x0x1x2x4x5 + x0x1x2x4x6\n"
                           "    + x0x1x2x6x7 + x0x1x3x4x5 + x0x1x3x4x6 + x0x1x3x4x7 + x0x1x3x5x6 + x0x1x3x5x7 + x0x2x3x4x5 + x0x2x3x4x7 + x0x2x3x5x6\n"
                           "\n"
                           "    + x0x2x3x5x7 + x0x2x4x5x6 + x0x2x5x6x7 + x0x3x5x6x7 + x0x4x5x6x7 + x1x2x3x4x5 + x1x2x3x5x6 + x1x2x3x5x7 + x1x2x3x6x7\n"
                           "\n"
                           "    + x1x2x4x5x6 + x1x2x4x5x7 + x1x3x4x5x7 + x1x3x4x6x7 + x2x3x4x5x6 + x2x3x4x5x7 + x2x3x4x6x7 + x3x4x5x6x7\n"
                           "    + x0x1x2x3x4x5 + x0x1x2x3x4x6 + x0x1x2x3x4x7 + x0x1x2x4x5x6 + x0x1x2x4x6x7 + x0x1x3x4x6x7 + x0x1x3x5x6x7\n"
                           "    + x0x1x4x5x6x7 + x0x2x3x4x5x7 + x0x2x3x4x6x7 + x0x2x3x5x6x7 + x0x2x4x5x6x7 + x0x3x4x5x6x7 + x1x2x3x4x5x6\n"
                           "    + x1x2x3x4x6x7 + x1x2x3x5x6x7 + x0x1x2x3x4x5x6 + x0x1x2x3x4x6x7 + x0x1x3x4x5x6x7 + x0x2x3x4x5x6x7 + x0x1x2x3x4x5x6x7"; //CLEFIA S1 f1


    const string anf_str = "1 + x0 + x2 + x5 + x6 + x7 + x0x1 + x0x6 + x0x7 + x1x2 + x1x4 + x1x7 + x2x4 + x2x5 + x2x6 + x3x4 + x3x5 + x3x7\n"
                           "    + x4x6 + x4x7 + x5x6 + x5x7 + x0x1x2 + x0x1x6 + x0x1x7 + x0x2x6 + x0x3x4 + x0x3x5 + x0x4x5 + x0x4x6 + x0x4x7\n"
                           "    + x0x5x7 + x1x2x3 + x1x2x6 + x1x2x7 + x1x3x4 + x1x6x7 + x2x3x4 + x2x4x6 + x2x5x6 + x2x5x7 + x3x4x7 + x4x5x6 + x5x6x7\n"
                           "\n"
                           "    + x0x1x2x5 + x0x1x2x6 + x0x1x3x4 + x0x1x3x6 + x0x1x4x5 + x0x2x3x5 + x0x2x3x6 + x0x2x5x6 + x0x2x5x7 + x0x3x5x6\n"
                           "    + x0x3x5x7 + x1x2x3x7 + x1x2x4x7 + x1x2x5x6 + x1x2x5x7 + x1x2x6x7 + x1x3x4x5 + x1x3x4x6 + x1x3x5x6 + x1x3x6x7\n"
                           "    + x1x4x5x6 + x1x5x6x7 + x2x3x4x6 + x2x3x4x7 + x2x3x5x7 + x2x3x6x7 + x2x4x5x7 + x2x5x6x7 + x3x4x5x6 + x3x5x6x7\n"
                           "    + x0x1x2x3x4 + x0x1x2x3x6 + x0x1x2x3x7 + x0x1x2x4x5 + x0x1x2x4x6 + x0x1x2x4x7 + x0x1x2x5x7 + x0x1x2x6x7 + x0x1x3x4x7\n"
                           "\n"
                           "    + x0x1x3x5x6 + x0x1x3x6x7 + x0x1x4x6x7 + x0x2x3x4x5 + x0x2x3x4x6 + x0x2x3x5x6 + x0x2x3x5x7 + x0x2x3x6x7 + x0x2x4x5x7\n"
                           "\n"
                           "    + x0x2x4x6x7 + x0x3x4x6x7 + x0x3x5x6x7 + x0x4x5x6x7 + x1x2x3x4x5 + x1x2x3x6x7 + x1x2x4x5x7 + x1x3x4x5x7 + x1x3x5x6x7\n"
                           "\n"
                           "    + x1x4x5x6x7 + x2x3x4x5x6 + x2x3x5x6x7 + x2x4x5x6x7 + x0x1x2x3x4x6 + x0x1x2x3x5x7 + x0x1x2x3x6x7 + x0x1x2x4x5x6\n"
                           "    + x0x1x2x4x5x7 + x0x1x2x5x6x7 + x0x1x3x4x5x6 + x0x1x3x4x5x7 + x0x1x3x4x6x7 + x0x2x3x4x6x7 + x0x3x4x5x6x7\n"
                           "    + x1x2x3x5x6x7 + x1x3x4x5x6x7 + x2x3x4x5x6x7 + x0x1x2x3x4x5x6 + x0x1x2x3x4x6x7 + x0x1x2x4x5x6x7 + x0x1x3x4x5x6x7\n"
                           "    + x0x2x3x4x5x6x7 + x1x2x3x4x5x6x7 + x0x1x2x3x4x5x6x7"; //CLEFIA S1 f2


    const string anf_str = "x0 + x1 + x2 + x3 + x4 + x7 + x0x1 + x0x2 + x0x3 + x0x6 + x1x4 + x1x5 + x1x7 + x2x3 + x2x6 + x3x4 + x3x7 + x4x7\n"
                           "    + x6x7 + x0x1x4 + x0x1x5 + x0x1x7 + x0x2x3 + x0x2x5 + x0x3x4 + x0x3x5 + x0x4x5 + x0x4x7 + x0x5x7 + x1x2x7 + x1x3x4\n"
                           "    + x1x3x5 + x1x3x6 + x2x3x4 + x2x3x5 + x2x4x6 + x2x6x7 + x3x4x6 + x3x5x6 + x4x5x6 + x4x6x7 + x5x6x7 + x0x1x2x6\n"
                           "    + x0x1x3x4 + x0x1x3x6 + x0x1x4x7 + x0x1x6x7 + x0x2x3x4 + x0x2x3x5 + x0x2x3x6 + x0x2x3x7 + x0x2x4x5 + x0x2x4x6\n"
                           "    + x0x2x4x7 + x0x2x5x7 + x0x2x6x7 + x0x3x4x5 + x0x3x6x7 + x0x4x5x6 + x0x4x6x7 + x1x2x3x4 + x1x2x4x6 + x1x2x4x7\n"
                           "    + x1x2x5x6 + x1x2x5x7 + x1x3x4x5 + x1x3x4x7 + x1x4x5x6 + x1x4x5x7 + x2x3x4x6 + x2x3x4x7 + x2x3x6x7 + x2x4x5x7\n"
                           "    + x3x4x5x6 + x3x4x6x7 + x3x5x6x7 + x0x1x2x3x4 + x0x1x2x3x7 + x0x1x2x4x6 + x0x1x2x5x6 + x0x1x2x6x7 + x0x1x3x4x6\n"
                           "    + x0x1x3x4x7 + x0x1x3x5x6 + x0x1x3x6x7 + x0x1x4x5x7 + x0x1x4x6x7 + x0x2x3x4x5 + x0x2x3x4x6 + x0x2x3x5x6 + x0x2x3x5x7\n"
                           "\n"
                           "    + x0x2x4x5x6 + x0x3x4x5x6 + x0x3x4x6x7 + x0x4x5x6x7 + x1x2x3x4x5 + x1x2x3x5x6 + x1x2x3x5x7 + x1x2x4x5x6 + x1x2x4x6x7\n"
                           "\n"
                           "    + x1x2x5x6x7 + x1x3x4x5x7 + x1x3x5x6x7 + x2x3x4x5x7 + x2x3x4x6x7 + x0x1x2x3x4x7 + x0x1x2x3x5x7 + x0x1x2x3x6x7\n"
                           "    + x0x1x2x4x5x6 + x0x1x2x4x6x7 + x0x1x2x5x6x7 + x0x1x3x4x6x7 + x0x1x3x5x6x7 + x0x1x4x5x6x7 + x0x2x3x4x5x6\n"
                           "    + x0x2x3x4x5x7 + x1x2x3x4x6x7 + x1x2x4x5x6x7 + x1x3x4x5x6x7 + x0x1x2x3x4x5x6 + x0x1x2x3x4x6x7 + x0x1x2x3x5x6x7\n"
                           "    + x0x1x2x4x5x6x7 + x0x1x3x4x5x6x7 + x1x2x3x4x5x6x7"; //CLEFIA S1 f3


    const string anf_str = "1 + x0 + x2 + x6 + x0x3 + x0x5 + x0x6 + x1x2 + x1x3 + x1x5 + x1x6 + x1x7 + x2x7 + x3x5 + x3x7 + x4x5 + x4x7 + x5x6\n"
                           "    + x6x7 + x0x1x3 + x0x1x4 + x0x1x6 + x0x1x7 + x0x2x3 + x0x2x4 + x0x2x6 + x0x2x7 + x0x3x5 + x0x3x6 + x0x4x5 + x0x5x7\n"
                           "    + x0x6x7 + x1x2x3 + x1x2x4 + x1x2x6 + x1x2x7 + x1x3x5 + x1x3x6 + x1x3x7 + x1x4x5 + x1x4x6 + x1x4x7 + x1x5x6 + x1x5x7\n"
                           "\n"
                           "    + x1x6x7 + x2x3x4 + x2x3x6 + x2x4x5 + x2x4x6 + x2x5x7 + x2x6x7 + x3x4x6 + x3x4x7 + x3x6x7 + x4x5x6 + x4x5x7 + x4x6x7\n"
                           "\n"
                           "    + x5x6x7 + x0x1x2x3 + x0x1x2x4 + x0x1x2x5 + x0x1x2x7 + x0x1x3x5 + x0x1x4x5 + x0x1x4x6 + x0x1x4x7 + x0x1x5x6\n"
                           "    + x0x2x3x4 + x0x2x3x5 + x0x2x3x7 + x0x2x6x7 + x0x3x4x5 + x0x3x4x6 + x0x3x4x7 + x0x3x5x7 + x0x4x5x6 + x0x4x5x7\n"
                           "    + x0x5x6x7 + x1x2x3x4 + x1x2x3x6 + x1x2x3x7 + x1x2x4x6 + x1x2x4x7 + x1x2x5x6 + x1x2x5x7 + x1x3x4x7 + x1x3x5x6\n"
                           "    + x1x3x6x7 + x1x4x6x7 + x2x3x4x5 + x2x3x4x6 + x2x3x5x7 + x2x4x5x7 + x2x5x6x7 + x3x4x5x6 + x3x5x6x7 + x4x5x6x7\n"
                           "    + x0x1x2x3x6 + x0x1x2x4x5 + x0x1x2x4x6 + x0x1x2x4x7 + x0x1x2x5x6 + x0x1x3x4x6 + x0x1x3x4x7 + x0x1x3x5x6 + x0x1x3x6x7\n"
                           "\n"
                           "    + x0x1x4x5x6 + x0x1x4x6x7 + x0x2x3x4x7 + x0x2x4x5x7 + x0x2x4x6x7 + x0x2x5x6x7 + x0x3x4x5x6 + x0x3x4x5x7 + x0x3x5x6x7\n"
                           "\n"
                           "    + x0x4x5x6x7 + x1x2x3x4x6 + x1x2x3x4x7 + x1x2x3x5x6 + x1x2x3x5x7 + x1x2x3x6x7 + x1x2x4x5x6 + x1x2x5x6x7 + x1x3x4x5x6\n"
                           "\n"
                           "    + x1x3x4x5x7 + x1x3x4x6x7 + x1x3x5x6x7 + x1x4x5x6x7 + x2x3x4x5x6 + x2x3x4x5x7 + x2x3x4x6x7 + x2x3x5x6x7\n"
                           "    + x0x1x2x4x5x7 + x0x1x2x4x6x7 + x0x1x3x4x6x7 + x0x2x3x4x5x6 + x0x2x3x4x6x7 + x0x2x3x5x6x7 + x0x2x4x5x6x7\n"
                           "    + x0x3x4x5x6x7 + x1x2x3x4x5x7 + x1x2x3x5x6x7 + x1x2x4x5x6x7 + x1x3x4x5x6x7 + x0x1x2x3x4x5x6 + x0x1x3x4x5x6x7\n"
                           "    + x0x2x3x4x5x6x7 + x1x2x3x4x5x6x7 + x0x1x2x3x4x5x6x7"; //CLEFIA S1 f4


    const string anf_str = "1 + x1 + x2 + x4 + x6 + x7 + x0x1 + x0x2 + x0x3 + x0x5 + x0x6 + x0x7 + x1x3 + x1x4 + x1x5 + x2x4 + x2x7 + x3x4\n"
                           "    + x3x6 + x3x7 + x5x7 + x6x7 + x0x1x3 + x0x1x7 + x0x4x5 + x0x6x7 + x1x2x3 + x1x2x5 + x1x2x6 + x1x3x6 + x1x4x6\n"
                           "    + x1x5x6 + x1x6x7 + x2x3x4 + x2x3x6 + x2x3x7 + x2x4x5 + x2x5x6 + x3x4x5 + x3x4x6 + x3x5x7 + x3x6x7 + x4x5x7 + x4x6x7\n"
                           "\n"
                           "    + x0x1x2x6 + x0x1x3x4 + x0x1x3x7 + x0x1x4x5 + x0x1x5x6 + x0x1x5x7 + x0x1x6x7 + x0x2x3x4 + x0x2x3x5 + x0x2x3x7\n"
                           "    + x0x2x4x6 + x0x2x4x7 + x0x3x4x5 + x0x3x4x6 + x0x3x4x7 + x0x3x5x6 + x0x3x5x7 + x0x4x5x7 + x1x2x3x5 + x1x2x3x6\n"
                           "    + x1x2x3x7 + x1x2x4x5 + x1x2x4x6 + x1x2x4x7 + x1x2x5x6 + x1x2x5x7 + x1x2x6x7 + x1x3x4x6 + x1x3x4x7 + x1x3x5x6\n"
                           "    + x1x3x5x7 + x1x3x6x7 + x1x4x5x7 + x2x3x4x6 + x2x3x5x7 + x2x4x5x6 + x2x4x5x7 + x2x4x6x7 + x3x4x5x6 + x0x1x2x3x6\n"
                           "    + x0x1x2x3x7 + x0x1x2x4x5 + x0x1x2x4x6 + x0x1x2x4x7 + x0x1x3x4x6 + x0x1x4x5x6 + x0x1x4x5x7 + x0x2x3x4x5 + x0x2x3x5x6\n"
                           "\n"
                           "    + x0x2x4x5x6 + x0x2x4x5x7 + x0x2x4x6x7 + x0x2x5x6x7 + x0x3x4x5x6 + x0x3x4x6x7 + x0x3x5x6x7 + x0x4x5x6x7 + x1x2x3x4x5\n"
                           "\n"
                           "    + x1x2x3x4x6 + x1x2x3x4x7 + x1x2x3x6x7 + x1x2x5x6x7 + x1x3x4x5x6 + x1x3x5x6x7 + x1x4x5x6x7 + x2x3x4x5x6 + x2x3x4x5x7\n"
                           "\n"
                           "    + x2x4x5x6x7 + x0x1x2x3x4x5 + x0x1x2x3x4x6 + x0x1x2x3x5x6 + x0x1x2x3x6x7 + x0x1x2x4x5x7 + x0x1x2x4x6x7\n"
                           "    + x0x1x3x4x6x7 + x0x1x3x5x6x7 + x0x2x3x4x5x7 + x0x2x3x4x6x7 + x0x2x3x5x6x7 + x0x3x4x5x6x7 + x1x2x3x4x5x6\n"
                           "    + x1x2x3x4x5x7 + x1x2x3x4x6x7 + x1x2x4x5x6x7 + x0x1x2x3x4x5x7 + x0x1x2x3x5x6x7 + x0x1x3x4x5x6x7"; //CLEFIA S1 f5


    const string anf_str = "x1 + x2 + x3 + x5 + x6 + x7 + x0x1 + x0x5 + x0x6 + x0x7 + x1x2 + x1x4 + x1x5 + x1x6 + x2x5 + x2x7 + x3x5 + x4x6\n"
                           "    + x5x6 + x0x1x3 + x0x1x4 + x0x1x5 + x0x1x7 + x0x2x3 + x0x2x4 + x0x2x7 + x0x3x4 + x0x4x6 + x0x5x7 + x0x6x7 + x1x2x3\n"
                           "    + x1x2x4 + x1x2x5 + x1x2x6 + x1x2x7 + x1x3x4 + x1x3x5 + x1x3x6 + x1x3x7 + x1x4x6 + x1x4x7 + x1x5x7 + x2x3x4 + x2x4x7\n"
                           "\n"
                           "    + x2x5x6 + x2x6x7 + x3x4x5 + x3x4x6 + x3x4x7 + x3x5x6 + x4x6x7 + x0x1x2x3 + x0x1x2x6 + x0x1x3x4 + x0x1x3x5\n"
                           "    + x0x1x3x6 + x0x1x3x7 + x0x1x5x7 + x0x2x3x4 + x0x2x4x5 + x0x2x4x6 + x0x2x4x7 + x0x2x5x7 + x0x2x6x7 + x0x3x4x5\n"
                           "    + x0x3x4x6 + x0x3x4x7 + x0x3x5x7 + x0x4x5x7 + x0x4x6x7 + x0x5x6x7 + x1x2x3x6 + x1x2x4x6 + x1x2x4x7 + x1x2x5x7\n"
                           "    + x1x2x6x7 + x1x3x4x7 + x1x3x5x6 + x1x3x6x7 + x1x4x6x7 + x1x5x6x7 + x2x3x4x7 + x2x3x5x6 + x2x3x6x7 + x2x4x6x7\n"
                           "    + x2x5x6x7 + x3x4x5x6 + x3x5x6x7 + x0x1x2x3x4 + x0x1x2x3x5 + x0x1x2x3x6 + x0x1x2x3x7 + x0x1x2x4x5 + x0x1x2x4x6\n"
                           "    + x0x1x2x4x7 + x0x1x2x5x6 + x0x1x3x4x5 + x0x1x3x5x6 + x0x1x3x5x7 + x0x1x3x6x7 + x0x1x4x5x7 + x0x2x3x4x5 + x0x2x3x4x7\n"
                           "\n"
                           "    + x0x2x3x5x6 + x0x2x3x5x7 + x0x2x4x5x6 + x0x2x4x6x7 + x0x2x5x6x7 + x0x3x4x5x6 + x0x3x4x5x7 + x0x3x4x6x7 + x1x2x3x4x5\n"
                           "\n"
                           "    + x1x2x3x4x7 + x1x2x3x5x6 + x1x2x3x5x7 + x1x3x4x6x7 + x1x3x5x6x7 + x2x3x4x5x7 + x2x4x5x6x7 + x3x4x5x6x7\n"
                           "    + x0x1x2x3x5x6 + x0x1x2x3x5x7 + x0x1x2x3x6x7 + x0x1x2x4x5x6 + x0x1x2x4x6x7 + x0x1x3x4x5x6 + x0x1x4x5x6x7\n"
                           "    + x0x2x3x4x5x6 + x0x2x3x4x5x7 + x0x2x3x4x6x7 + x0x2x3x5x6x7 + x0x3x4x5x6x7 + x1x2x3x4x5x6 + x1x2x3x4x5x7\n"
                           "    + x1x3x4x5x6x7 + x0x1x2x3x5x6x7 + x0x1x2x4x5x6x7 + x0x1x3x4x5x6x7 + x0x2x3x4x5x6x7 + x1x2x3x4x5x6x7"; //CLEFIA S1 f6


    const string anf_str = "x0 + x1 + x3 + x6 + x0x1 + x0x2 + x0x4 + x0x7 + x1x2 + x1x3 + x1x4 + x1x5 + x2x4 + x2x7 + x3x4 + x3x7 + x4x5 + x4x6\n"
                           "    + x5x6 + x5x7 + x0x1x2 + x0x1x3 + x0x1x4 + x0x1x5 + x0x1x6 + x0x2x3 + x0x2x4 + x0x2x6 + x0x3x6 + x0x3x7 + x0x4x7\n"
                           "    + x0x5x6 + x0x6x7 + x1x2x3 + x1x2x4 + x1x2x5 + x1x2x7 + x1x3x4 + x1x3x6 + x1x3x7 + x1x4x6 + x1x4x7 + x2x3x5 + x2x3x6\n"
                           "\n"
                           "    + x2x4x7 + x2x5x6 + x2x5x7 + x2x6x7 + x3x4x5 + x3x4x6 + x3x5x7 + x0x1x2x4 + x0x1x2x5 + x0x1x2x6 + x0x1x3x4\n"
                           "    + x0x1x3x5 + x0x1x3x6 + x0x1x3x7 + x0x1x4x6 + x0x1x4x7 + x0x1x5x6 + x0x1x5x7 + x0x1x6x7 + x0x2x3x4 + x0x2x3x7\n"
                           "    + x0x2x4x6 + x0x2x4x7 + x0x2x6x7 + x0x3x4x5 + x0x3x4x6 + x0x3x5x6 + x0x3x5x7 + x0x4x5x7 + x0x5x6x7 + x1x2x3x4\n"
                           "    + x1x2x3x5 + x1x2x3x6 + x1x2x3x7 + x1x2x4x6 + x1x2x4x7 + x1x2x5x6 + x1x2x5x7 + x1x3x4x5 + x1x4x5x6 + x1x4x5x7\n"
                           "    + x1x4x6x7 + x1x5x6x7 + x2x3x4x5 + x2x3x5x6 + x2x3x5x7 + x2x3x6x7 + x2x4x5x7 + x2x4x6x7 + x2x5x6x7 + x3x4x5x7\n"
                           "    + x3x4x6x7 + x0x1x2x3x6 + x0x1x2x5x6 + x0x1x3x4x5 + x0x1x3x4x6 + x0x1x3x5x6 + x0x1x3x6x7 + x0x1x4x5x7 + x0x1x5x6x7\n"
                           "    + x0x2x3x4x5 + x0x2x3x5x6 + x0x2x3x5x7 + x0x2x4x5x7 + x0x2x5x6x7 + x0x3x4x5x6 + x0x3x4x6x7 + x0x3x5x6x7 + x1x2x3x4x5\n"
                           "\n"
                           "    + x1x2x3x4x7 + x1x2x3x5x7 + x1x2x4x6x7 + x1x3x5x6x7 + x1x4x5x6x7 + x2x3x4x5x6 + x2x3x4x5x7 + x2x3x4x6x7\n"
                           "    + x0x1x2x3x4x5 + x0x1x2x3x5x6 + x0x1x2x4x5x6 + x0x1x3x4x5x6 + x0x2x3x4x5x6 + x0x2x3x4x5x7 + x0x2x3x4x6x7\n"
                           "    + x0x2x4x5x6x7 + x0x3x4x5x6x7 + x1x2x3x4x5x6 + x1x2x3x4x6x7 + x1x2x4x5x6x7 + x1x3x4x5x6x7 + x0x1x2x3x4x6x7\n"
                           "    + x0x1x2x4x5x6x7 + x0x1x3x4x5x6x7 + x0x2x3x4x5x6x7 + x1x2x3x4x5x6x7"; //CLEFIA S1 f7
***************************************************************************



*******************************Skinny-128 Sbox******************************


    const string anf_str = "1 + x0 + x1 + x6 + x7 + x0x5 + x0x6 + x0x7 + x1x2 + x1x4 + x1x5 + x1x6 + x1x7 + x4x7 + x5x6 + x5x7 + x6x7 + x0x4x7"
                           "    + x0x5x6 + x0x6x7 + x1x2x5 + x1x2x6 + x1x2x7 + x1x3x4 + x1x4x6 + x2x4x5 + x2x5x6 + x2x5x7 + x2x6x7 + x3x4x5 + x3x5x6"
                           "    + x3x6x7 + x4x5x7 + x4x6x7 + x5x6x7 + x0x1x4x5 + x0x1x5x6 + x0x1x6x7 + x0x4x5x7 + x0x4x6x7 + x1x2x4x5 + x1x2x4x7"
                           "    + x1x3x4x6 + x1x3x4x7 + x1x3x5x6 + x1x3x6x7 + x1x4x5x6 + x1x4x5x7 + x1x4x6x7 + x2x4x5x7 + x2x4x6x7 + x2x5x6x7"
                           "    + x3x4x5x7 + x3x4x6x7 + x4x5x6x7 + x0x1x4x5x7 + x0x1x4x6x7 + x0x4x5x6x7 + x1x3x4x5x6 + x1x4x5x6x7 + x2x4x5x6x7"
                           "    + x3x4x5x6x7 + x0x1x4x5x6x7";//skinny128 f7
    const string anf_str ="x0 + x1 + x5 + x6 + x0x5 + x1x2 + x1x4 + x1x7 + x2x5 + x2x6 + x3x5 + x5x6 + x0x1x5 + x0x4x6 + x0x5x7 + x0x6x7\n"
                          "    + x1x3x4 + x1x3x5 + x1x3x7 + x1x4x5 + x1x4x6 + x1x5x7 + x1x6x7 + x2x5x6 + x3x4x6 + x3x5x7 + x3x6x7 + x0x1x4x6\n"
                          "    + x0x1x5x7 + x0x1x6x7 + x0x4x5x6 + x0x5x6x7 + x1x3x4x5 + x1x4x5x6 + x1x5x6x7 + x3x4x5x6 + x3x5x6x7 + x0x1x4x5x6\n"
                          "    + x0x1x5x6x7";   // skinny128 f6
    const string anf_str = "1 + x1 + x5 + x6 + x5x6"; //skinny128 f5
    const string anf_str = "x5 + x6 + x7 + x4x5 + x4x7"; //skinny128 f4
    const string anf_str = "x0 + x1 + x3 + x4 + x0x1 + x0x2 + x0x4 + x0x5 + x0x7 + x1x2 + x1x4 + x1x5 + x1x7 + x2x3 + x3x4 + x3x5 + x3x7\n"
                           "    + x0x1x2 + x0x1x4 + x0x1x5 + x0x1x7 + x0x4x5 + x1x4x5 + x3x4x5 + x0x1x4x5"; //skinny128 f3
    const string anf_str = "1 + x4 + x5 + x7 + x4x5"; //skinny128 f2
    const string anf_str = "1 + x0 + x1 + x3 + x0x1"; //skinny128 f1
    const string anf_str = "x2 + x0x4 + x0x5 + x0x7 + x1x4 + x1x5 + x1x7 + x3x4 + x3x5 + x3x7 + x0x1x4 + x0x1x5 + x0x1x7 + x0x4x5 + x1x4x5 + x3x4x5 + x0x1x4x5"; //skinny128 f0
***************************************************************************


********************************Skinny-128_inv Sbox*****************************

    const string anf_str = "1 + x0 + x5 + x6 + x0x5"; // skinny128_inv f0
    const string anf_str = "x5 + x6 + x7 + x0x4 + x0x6 + x0x7 + x1x4 + x1x6 + x1x7 + x3x4 + x3x6 + x3x7 + x4x6 + x4x7 + x0x1x4 + x0x1x6 + x0x1x7\n"
                           "    + x0x2x4 + x0x2x6 + x0x2x7 + x0x4x6 + x1x2x4 + x1x2x6 + x1x2x7 + x1x4x6 + x2x3x4 + x2x3x6 + x2x3x7 + x3x4x6\n"
                           "    + x0x1x2x4 + x0x1x2x6 + x0x1x2x7 + x0x1x4x6 + x0x2x4x6 + x1x2x4x6 + x2x3x4x6 + x0x1x2x4x6"; // skinny128_inv f1
    const string anf_str = "1 + x0 + x1 + x2 + x1x2"; // skinny128_inv f2
    const string anf_str = "x0 + x1 + x0x4 + x0x5 + x0x6 + x1x6 + x3x6 + x4x6 + x5x7 + x6x7 + x0x1x6 + x0x2x4 + x0x2x7 + x0x3x4 + x0x3x6\n"
                           "    + x0x3x7 + x0x4x7 + x0x5x6 + x0x5x7 + x0x6x7 + x1x2x6 + x1x4x5 + x1x5x6 + x1x5x7 + x1x6x7 + x2x3x6 + x3x4x5 + x3x5x6\n"
                           "    + x3x5x7 + x3x6x7 + x4x5x6 + x4x5x7 + x4x6x7 + x0x1x2x6 + x0x1x4x5 + x0x1x5x6 + x0x1x5x7 + x0x1x6x7 + x0x2x3x4\n"
                           "    + x0x2x3x6 + x0x2x3x7 + x0x2x4x6 + x0x2x6x7 + x0x3x4x5 + x0x3x4x6 + x0x3x5x6 + x0x3x5x7 + x0x4x5x6 + x0x4x5x7\n"
                           "    + x1x2x4x5 + x1x2x5x6 + x1x2x5x7 + x1x2x6x7 + x1x4x5x6 + x2x3x4x5 + x2x3x5x6 + x2x3x5x7 + x2x3x6x7 + x3x4x5x6\n"
                           "    + x0x1x2x4x5 + x0x1x2x5x6 + x0x1x2x5x7 + x0x1x2x6x7 + x0x1x4x5x6 + x0x2x3x4x5 + x0x2x3x4x6 + x0x2x3x5x6 + x0x2x3x5x7\n"
                           "    + x0x3x4x5x6 + x1x2x4x5x6 + x2x3x4x5x6 + x0x1x2x4x5x6 + x0x2x3x4x5x6"; // skinny128_inv f3
    const string anf_str = "1 + x0 + x1 + x3 + x0x1"; // skinny128_inv f4
    const string anf_str = "1 + x4 + x6 + x7 + x4x6"; // skinny128_inv f5
    const string anf_str = "x0 + x1 + x3 + x4 + x0x1 + x0x2 + x1x2 + x2x3 + x0x1x2"; // skinny128_inv f6
    const string anf_str = "x2 + x0x4 + x0x6 + x0x7 + x1x4 + x1x6 + x1x7 + x3x4 + x3x6 + x3x7 + x0x1x4 + x0x1x6 + x0x1x7 + x0x4x6 + x1x4x6\n"
                           "    + x3x4x6 + x0x1x4x6"; // skinny128_inv f7
***************************************************************************



***********************************SPEEDY_inv Sbox****************************************

    const string anf_str = "x4 + x0x2 + x0x5 + x1x4 + x1x5 + x4x5 + x0x1x2 + x0x1x5 + x0x2x3 + x0x2x4 + x0x4x5 + x2x4x5 + x0x2x3x4 + x1x2x3x4\n"
                           "    + x1x2x3x5 + x1x3x4x5 + x1x2x3x4x5";  //SPEEDY_inv f0
    const string anf_str = "x0x2 + x0x3 + x0x4 + x1x2 + x1x3 + x1x4 + x3x5 + x0x1x3 + x0x1x4 + x0x2x3 + x0x2x4 + x0x2x5 + x0x3x4 + x0x4x5\n"
                           "    + x1x2x3 + x1x2x4 + x1x3x4 + x1x4x5 + x2x3x5 + x3x4x5 + x0x1x3x4 + x0x1x4x5 + x0x2x3x4 + x0x2x3x5 + x0x3x4x5\n"
                           "    + x1x2x3x4 + x2x3x4x5";  //SPEEDY_inv f1
    const string anf_str = "x0 + x3 + x5 + x0x2 + x0x3 + x0x4 + x2x4 + x2x5 + x3x4 + x3x5 + x4x5 + x0x1x5 + x0x3x4 + x0x4x5 + x1x2x4 + x1x2x5\n"
                           "    + x1x3x5 + x2x3x5 + x0x1x2x4 + x0x1x2x5 + x0x2x3x5 + x0x2x4x5 + x0x3x4x5 + x1x2x4x5 + x1x2x3x4x5";  //SPEEDY_inv f2
    const string anf_str = "x0 + x1 + x5 + x0x1 + x0x4 + x0x5 + x1x2 + x1x3 + x1x4 + x1x5 + x2x5 + x4x5 + x0x1x2 + x0x1x3 + x0x1x4 + x0x1x5\n"
                           "    + x0x2x5 + x1x2x3 + x1x2x4 + x1x3x5 + x2x4x5 + x0x1x2x3 + x0x1x3x4 + x0x1x3x5 + x1x2x3x4 + x1x2x4x5";  //SPEEDY_inv f3
    const string anf_str = "x0 + x3 + x0x1 + x0x3 + x0x4 + x1x2 + x2x3 + x2x5 + x3x4 + x3x5 + x4x5 + x0x1x2 + x0x2x3 + x0x2x4 + x0x2x5 + x0x3x4\n"
                           "    + x0x3x5 + x1x2x3 + x1x2x4 + x1x3x5 + x2x4x5 + x3x4x5 + x0x1x2x4 + x0x2x4x5 + x0x3x4x5 + x1x2x3x5 + x1x2x4x5\n"
                           "    + x2x3x4x5 + x0x1x2x3x4";  //SPEEDY_inv f4
    const string anf_str = "1 + x0 + x2 + x3 + x4 + x0x2 + x0x3 + x0x4 + x1x4 + x2x3 + x2x4 + x3x5 + x4x5 + x0x2x3 + x0x2x4 + x0x3x4 + x0x3x5\n"
                           "    + x1x2x4 + x1x2x5 + x1x3x4 + x1x4x5 + x2x4x5 + x0x1x2x3 + x0x1x2x5 + x0x1x3x5 + x0x1x4x5 + x0x3x4x5 + x1x2x3x5\n"
                           "    + x1x2x4x5 + x1x3x4x5 + x0x1x2x3x4";  //SPEEDY_inv f5

***************************************************************************



**********************************Midori_inv Sbox***************************

    const string anf_str = "1 + x0x1 + x0x2 + x2x3 + x0x1x2 + x0x2x3"; // MidoriS0_inv f0
    const string anf_str = "1 + x0 + x3 + x0x3 + x0x1x2 + x0x2x3 + x1x2x3"; // MidoriS0_inv f1
    const string anf_str = "x1 + x3 + x0x1 + x0x3 + x1x3"; // MidoriS0_inv f2
    const string anf_str = "x2 + x0x3 + x1x3 + x0x1x2 + x0x2x3 + x1x2x3"; // MidoriS0_inv f3

***************************************************************************
*/





    // initialize b[] by parsing the ANF string
    vector<int> b(MONS, 0);
    parse_anf_string(anf_str, n, b);

    // === Enumerate all negation patterns for every main monomial that appears (deg>=1) ===
    struct Pattern{
        int id;
        int main_mask;
        vector<int> expansion;   // list of monomial masks covered by this pattern
    };
    vector<Pattern> patterns;
    vector<vector<int>> cover_by_m(MONS);  // for each monomial m, list of pattern ids that include m

    int pid = 0;
    for(int main=0; main<MONS; ++main){
        if(!b[main]) continue;
        if(popcount_int(main) < 1) continue; // exclude constant
        vector<int> S = mask_to_vars(main, n);

        // U: any non-empty subset of S
        for(const auto& U : all_nonempty_subsets(S)){
            // partitions of U
            auto parts = all_partitions(U);
            for(const auto& blocks : parts){
                // expansion = (S \\ U) union (union of any subset of blocks)
                vector<int> base;
                {
                    // compute base = S minus U
                    vector<int> Uset = U;
                    sort(Uset.begin(), Uset.end());
                    for(int v: S){
                        if(!binary_search(Uset.begin(), Uset.end(), v)) base.push_back(v);
                    }
                }
                // precompute each block mask
                vector<int> block_masks;
                for(const auto& blk : blocks) block_masks.push_back(vars_to_mask(blk));

                // enumerate all subsets of blocks
                int B = (int)blocks.size();
                set<int> exp_set;
                for(int sbm=0; sbm < (1<<B); ++sbm){
                    int mask = vars_to_mask(base);
                    for(int k=0;k<B;k++) if(sbm & (1<<k)) mask |= block_masks[k];
                    exp_set.insert(mask);
                }
                // build pattern
                Pattern p; p.id = pid++; p.main_mask = main;
                for(int m : exp_set){ p.expansion.push_back(m); cover_by_m[m].push_back(p.id); }
                sort(p.expansion.begin(), p.expansion.end());
                patterns.push_back(move(p));
            }
        }
    }

    // === Emit coverage matrix CSV for inspection ===
    {
        FILE* csv = fopen("coverage_matrix_patterns_Skinny_inv_8bit_f0.csv", "w");
        fprintf(csv, "pattern_id,main_mask,degree,exp_list");
        for(int m=0;m<MONS;++m) fprintf(csv, ",m%d", m);
        fprintf(csv, "\n");
        for(const auto& p: patterns){
            fprintf(csv, "%d,%d,%d,\"[", p.id, p.main_mask, popcount_int(p.main_mask));
            for(size_t i=0;i<p.expansion.size();++i){
                if(i) fprintf(csv, ",");
                fprintf(csv, "m%d", p.expansion[i]);
            }
            fprintf(csv, "]\"");
            vector<int> occ(MONS,0);
            for(int m: p.expansion) occ[m]=1;
            for(int m=0;m<MONS;++m) fprintf(csv, ",%d", occ[m]);
            fprintf(csv, "\n");
        }
        fclose(csv);
    }

    // === Pre-compute allowed patterns (STRICT filter) ===
    // mark patterns that touch any absent monomial => forbid
    vector<char> must_zero(patterns.size(), 0);
    for(int m=0;m<MONS;++m){
        if(b[m]) continue;                 // only absent monomials
        for(int pid : cover_by_m[m]) must_zero[pid] = 1;
    }
    // collect kept pattern ids (we will ONLY declare these s_p variables in CVC)
    vector<int> kept_pids;
    kept_pids.reserve(patterns.size());
    for(size_t i=0;i<patterns.size();++i) if(!must_zero[i]) kept_pids.push_back((int)i);

    // === Start writing the CVC model ===
    FILE* fp = fopen("anf_negpattern_Skinny_inv_8bit_f0.cvc", "w");

    // Header
    fprintf(fp, "%% Generated by gen_negpattern_model_Skinny_inv_8bit_f0 (STRICT, FILTERED s_p)\n");
    fprintf(fp, "%% Only patterns that never produce absent monomials are declared.\n");
    fprintf(fp, "%% Pattern IDs keep original numbering (see CSV).\n\n");

    // --- Grouped declarations (comma-separated) ---
    auto decl_group = [&](const vector<string>& names, const string& type){
        string line;
        for(size_t i=0;i<names.size();++i){
            if(i) line += " , ";
            line += names[i];
        }
        if(!names.empty())
            fprintf(fp, "%s : %s;\n", line.c_str(), type.c_str());
    };

    // b_m0..b_m(M-1)
    {
        vector<string> names;
        names.reserve(MONS);
        for(int m=0;m<MONS;++m){ names.push_back("b_m"+to_string(m)); }
        decl_group(names, "BITVECTOR(1)");
    }
    // s_p<ID> only for kept patterns
    {
        vector<string> names;
        names.reserve(kept_pids.size());
        for(int pid : kept_pids) names.push_back("s_p"+to_string(pid));
        decl_group(names, "BITVECTOR(1)");
    }
    // r_m0..r_m(M-1)
    {
        vector<string> names;
        names.reserve(MONS);
        for(int m=0;m<MONS;++m){ names.push_back("r_m"+to_string(m)); }
        decl_group(names, "BITVECTOR(1)");
    }
    // cost vars
    fprintf(fp, "cost_patterns , cost_singles , total_cost , BCOST : BITVECTOR(%d);\n\n", W_cost);

    // --- Fix b_m constants for the chosen ANF ---
    for(int m=0;m<MONS;++m){
        fprintf(fp, "ASSERT( b_m%d = 0bin%d );\n", m, b[m]);
    }
    fprintf(fp, "\n");

    // --- r_m = 0 when b_m = 0 ---
    for(int m=0;m<MONS;++m) if(!b[m]) fprintf(fp, "ASSERT( r_m%d = 0bin0 );\n", m);
    fprintf(fp, "\n");

    // --- Pattern usability: s_p -> (main present initially) ---
    // only for kept patterns
    for(int pid : kept_pids){
        const auto& p = patterns[pid];
        fprintf(fp, "ASSERT( (s_p%d = 0bin0) OR (b_m%d = 0bin1) );\n", pid, p.main_mask);
    }
    fprintf(fp, "\n");

    // --- Exactly-once for monomials in ANF: sum of covering kept s_p plus r_m equals 1 ---
    auto zext_local = [&](const string& bit, int W)->string{
        return string("(0bin") + string(W-1, '0') + " @ " + bit + ")";
    };
    for(int m=0;m<MONS;++m){
        if(!b[m]) continue;
        vector<string> terms;
        for(int pid : cover_by_m[m]){
            if(!must_zero[pid]) terms.push_back( zext_local("s_p"+to_string(pid), W_cover) );
        }
        terms.push_back( zext_local("r_m"+to_string(m), W_cover) );
        string rhs = string("0bin") + string(W_cover-1, '0') + "1";
        if(terms.size()==1){
            fprintf(fp, "ASSERT( %s = %s );\n", terms[0].c_str(), rhs.c_str());
        }else{
            string sum = string("BVPLUS(") + to_string(W_cover) + ", " + join(terms, " , ") + ")";
            fprintf(fp, "ASSERT( %s = %s );\n", sum.c_str(), rhs.c_str());
        }
    }
    fprintf(fp, "\n");

    // --- Cost = #kept patterns + #singles ---
    {
        vector<string> zpat, zsing;
        for(int pid : kept_pids) zpat.push_back( zext("s_p"+to_string(pid), W_cost) );
        for(int m=0;m<MONS;++m) if(b[m]) zsing.push_back( zext("r_m"+to_string(m), W_cost) );

        string sumP = (zpat.empty()) ? string("0bin") + string(W_cost, '0')
                                     : (zpat.size()==1) ? zpat[0]
                                                        : string("BVPLUS(") + to_string(W_cost) + ", " + join(zpat," , ") + ")";
        string sumR = (zsing.empty())? string("0bin") + string(W_cost, '0')
                                     : (zsing.size()==1)? zsing[0]
                                                        : string("BVPLUS(") + to_string(W_cost) + ", " + join(zsing," , ") + ")";

        fprintf(fp, "ASSERT( cost_patterns = %s );\n", sumP.c_str());
        fprintf(fp, "ASSERT( cost_singles  = %s );\n", sumR.c_str());
        fprintf(fp, "ASSERT( total_cost    = BVPLUS(%d, cost_patterns , cost_singles) );\n\n", W_cost);
    }

    // --- Budget ---
    fprintf(fp, "ASSERT( BCOST = 0bin0000000000100000 );  %% e.g., 32\n");
    fprintf(fp, "ASSERT( total_cost =  BCOST );\n\n");

    // --- Query ---
    fprintf(fp, "QUERY(FALSE);\nCOUNTEREXAMPLE;\n");

    fclose(fp);

    // Notes
    fprintf(stdout, "[OK] CVC written \n");
    fprintf(stdout, "[OK] Coverage CSV written\n");
    fprintf(stdout, "patterns(all) = %zu, kept = %zu, pruned = %zu\n",
            patterns.size(), kept_pids.size(), patterns.size()-kept_pids.size());
    return 0;
}
