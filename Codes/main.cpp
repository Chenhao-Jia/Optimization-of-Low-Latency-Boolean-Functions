#include <bits/stdc++.h>
using namespace std;

// Generate the AES S-box f2 STRICT, exactly-once model and coverage matrix.
// Build: g++ main.cpp
// Run:   ./a.out

// Count variables in a monomial mask.
static inline int popcount_int(int x){ return __builtin_popcount((unsigned)x); }

// Decode a monomial mask into variable indices.
static vector<int> mask_to_vars(int mask, int n){
    vector<int> v;
    for(int i=0;i<n;i++) if(mask & (1<<i)) v.push_back(i);
    return v;
}

// Enumerate nonempty subsets in the original order.
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

// Build set partitions by backtracking.
static void partitions_rec(int idx, const vector<int>& U, vector<vector<int>>& cur, vector<vector<vector<int>>>& out){
    if(idx == (int)U.size()){
        out.push_back(cur);
        return;
    }
    int x = U[idx];

    for(size_t i=0;i<cur.size();++i){
        cur[i].push_back(x);
        partitions_rec(idx+1, U, cur, out);
        cur[i].pop_back();
    }

    cur.push_back({x});
    partitions_rec(idx+1, U, cur, out);
    cur.pop_back();
}

// Canonicalize partitions to preserve stable pattern IDs.
static vector<vector<vector<int>>> all_partitions(vector<int> U){
    vector<vector<vector<int>>> out;
    vector<vector<int>> cur;
    partitions_rec(0, U, cur, out);

    for(auto& part: out){
        for(auto& b: part) sort(b.begin(), b.end());
        sort(part.begin(), part.end());
    }

    sort(out.begin(), out.end());
    out.erase(unique(out.begin(), out.end()), out.end());
    return out;
}

// Encode variable indices as a monomial mask.
static int vars_to_mask(const vector<int>& v){
    int m=0; for(int x: v) m |= (1<<x); return m;
}

// Join CVC expression arguments.
static string join(const vector<string>& a, const string& sep){
    string s;
    for(size_t i=0;i<a.size();++i){
        if(i) s += sep;
        s += a[i];
    }
    return s;
}

// Zero-extend a one-bit CVC variable to W bits.
static string zext(const string& bit, int W){
    string hi(W-1, '0');
    return "(0bin" + hi + " @ " + bit + ")";
}

// Parse terms such as 1, x0 and x1x3; combine coefficients over GF(2).
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
        if (anf[i]=='1') {
            mask = 0; ++i;
        } else {
            bool any = false;
            while (i<L && anf[i]=='x') {
                ++i;
                int idx = 0; bool has = false;
                while (i<L && isdigit((unsigned char)anf[i])) { has=true; idx = idx*10 + (anf[i]-'0'); ++i; }
                if (has && idx>=0 && idx<n) { mask |= (1<<idx); any=true; }
            }
            if (!any) {
                ++i; continue;
            }
        }
        b[mask] ^= 1;
        skip();
        if (i>=L) break;
        if (anf[i]=='+') ++i;
    }
}

int main(){
    const int n = 8;
    const int MONS = 1<<n;       // All 256 monomial masks.
    const int W_cover = 16;     // Coverage-sum width.
    const int W_cost  = 16;     // Cost-sum width.

    // Paths are relative to the current working directory.
    const char* cvc_path = "./Automatic Search Model/anf_negpattern_global_AES_8bit_f2.cvc";
    const char* csv_path = "./Negation_pattern_AES/coverage_matrix_patterns_AES_8bit_f2.csv";

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
            "+ x0x1x2x3x5x6x7 + x0x1x2x4x5x6x7"; // AES Sbox f_2

    vector<int> b(MONS, 0);
    parse_anf_string(anf_str, n, b);

    // Enumerate all patterns; retain main1's unfiltered CVC encoding.
    struct Pattern{
        int id;
        int main_mask;
        vector<int> expansion;  // Covered monomial masks.
    };
    vector<Pattern> patterns;
    vector<vector<int>> cover_by_m(MONS);  // Monomial-to-pattern index.

    int pid = 0;
    for(int main=0; main<MONS; ++main){
        if(!b[main]) continue;
        if(popcount_int(main) < 1) continue;
        vector<int> S = mask_to_vars(main, n);

        for(const auto& U : all_nonempty_subsets(S)){
            auto parts = all_partitions(U);
            for(const auto& blocks : parts){
                // Unnegated variables: base = S minus U.
                vector<int> base;
                {
                    vector<int> Uset = U;
                    sort(Uset.begin(), Uset.end());
                    for(int v: S){
                        if(!binary_search(Uset.begin(), Uset.end(), v)) base.push_back(v);
                    }
                }

                vector<int> block_masks;
                for(const auto& blk : blocks) block_masks.push_back(vars_to_mask(blk));

                // Expand base * product(1 + block) over all block subsets.
                int B = (int)blocks.size();
                set<int> exp_set;
                for(int sbm=0; sbm < (1<<B); ++sbm){
                    int mask = vars_to_mask(base);
                    for(int k=0;k<B;k++) if(sbm & (1<<k)) mask |= block_masks[k];
                    exp_set.insert(mask);
                }

                Pattern p; p.id = pid++; p.main_mask = main;
                for(int m : exp_set){ p.expansion.push_back(m); cover_by_m[m].push_back(p.id); }
                sort(p.expansion.begin(), p.expansion.end());
                patterns.push_back(move(p));
            }
        }
    }

    // Use binary mode and explicit CRLF to reproduce both reference files.
    {
        FILE* csv = fopen(csv_path, "wb");
        if (!csv) {
            perror(csv_path);
            return 1;
        }
        fprintf(csv, "pattern_id,main_mask,degree,exp_list");
        for(int m=0;m<MONS;++m) fprintf(csv, ",m%d", m);
        fprintf(csv, "\r\n");
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
            fprintf(csv, "\r\n");
        }
        const bool csv_write_failed = ferror(csv) != 0;
        const int csv_close_status = fclose(csv);
        if (csv_write_failed || csv_close_status != 0) {
            fprintf(stderr, "[ERROR] Failed to write %s\n", csv_path);
            return 1;
        }
    }

    FILE* fp = fopen(cvc_path, "wb");
    if (!fp) {
        perror(cvc_path);
        return 1;
    }

    // Preserve the reference CVC header for byte-for-byte compatibility.
    fprintf(fp, "%% Generated by gen_negpattern_model_8bit_f2.cpp\r\n");
    fprintf(fp, "%% 8-bit ANF + negation pattern search (exactly-once, global cost)\r\n");
    fprintf(fp, "%% STRICT: monomials ABSENT from the ANF may NOT appear in any chosen pattern expansion.\r\n\r\n");

    // Group declarations without changing variable order.
    auto decl_group = [&](const vector<string>& names, const string& type){
        string line;
        for(size_t i=0;i<names.size();++i){
            if(i) line += " , ";
            line += names[i];
        }
        fprintf(fp, "%s : %s;\r\n", line.c_str(), type.c_str());
    };

    {
        vector<string> names;
        names.reserve(MONS);
        // Fixed ANF coefficients.
        for(int m=0;m<MONS;++m){ names.push_back("b_m"+to_string(m)); }
        decl_group(names, "BITVECTOR(1)");
    }

    {
        vector<string> names;
        names.reserve(patterns.size());
        // Selection bits for every enumerated pattern.
        for(const auto& p: patterns){ names.push_back("s_p"+to_string(p.id)); }
        decl_group(names, "BITVECTOR(1)");
    }

    {
        vector<string> names;
        names.reserve(MONS);
        // Residual-monomial selection bits.
        for(int m=0;m<MONS;++m){ names.push_back("r_m"+to_string(m)); }
        decl_group(names, "BITVECTOR(1)");
    }

    fprintf(fp, "cost_patterns , cost_singles , total_cost , BCOST : BITVECTOR(%d);\r\n\r\n", W_cost);

    // Fix the ANF coefficient vector.
    for(int m=0;m<MONS;++m){
        fprintf(fp, "ASSERT( b_m%d = 0bin%d );\r\n", m, b[m]);
    }
    fprintf(fp, "\r\n");

    // Absent monomials cannot be selected as residuals.
    for(int m=0;m<MONS;++m) if(!b[m]) fprintf(fp, "ASSERT( r_m%d = 0bin0 );\r\n", m);
    fprintf(fp, "\r\n");

    // A selected pattern must have its main monomial in the ANF.
    for(const auto& p: patterns){
        fprintf(fp, "ASSERT( (s_p%d = 0bin0) OR (b_m%d = 0bin1) );\r\n", p.id, p.main_mask);
    }
    fprintf(fp, "\r\n");

    // STRICT: disable patterns containing any monomial absent from the ANF.
    vector<char> must_zero(patterns.size(), 0);
    for(int m=0;m<MONS;++m){
        if(b[m]) continue;
        for(int pid : cover_by_m[m]) must_zero[pid] = 1;
    }
    for(size_t i=0;i<patterns.size(); ++i){
        if(must_zero[i]) fprintf(fp, "ASSERT( s_p%zu = 0bin0 );\r\n", i);
    }
    fprintf(fp, "\r\n");

    // Exactly-once coverage: sum(covering pattern bits) + residual bit = 1.
    auto zext_local = [&](const string& bit, int W)->string{
        return string("(0bin") + string(W-1, '0') + " @ " + bit + ")";
    };
    for(int m=0;m<MONS;++m){
        if(!b[m]) continue;
        vector<string> terms;
        for(int pid : cover_by_m[m]) terms.push_back( zext_local("s_p"+to_string(pid), W_cover) );
        terms.push_back( zext_local("r_m"+to_string(m), W_cover) );
        string rhs = string("0bin") + string(W_cover-1, '0') + "1";
        if(terms.size()==1){
            fprintf(fp, "ASSERT( %s = %s );\r\n", terms[0].c_str(), rhs.c_str());
        }else{
            string sum = string("BVPLUS(") + to_string(W_cover) + ", " + join(terms, " , ") + ")";
            fprintf(fp, "ASSERT( %s = %s );\r\n", sum.c_str(), rhs.c_str());
        }
    }
    fprintf(fp, "\r\n");

    // Cost = selected patterns + selected residual monomials.
    {
        vector<string> zpat, zsing;
        for(const auto& p: patterns) zpat.push_back( zext("s_p"+to_string(p.id), W_cost) );
        for(int m=0;m<MONS;++m) if(b[m]) zsing.push_back( zext("r_m"+to_string(m), W_cost) );

        string sumP = (zpat.empty()) ? string("0bin") + string(W_cost, '0')
                                     : (zpat.size()==1) ? zpat[0]
                                                        : string("BVPLUS(") + to_string(W_cost) + ", " + join(zpat," , ") + ")";
        string sumR = (zsing.empty())? string("0bin") + string(W_cost, '0')
                                     : (zsing.size()==1)? zsing[0]
                                                        : string("BVPLUS(") + to_string(W_cost) + ", " + join(zsing," , ") + ")";

        fprintf(fp, "ASSERT( cost_patterns = %s );\r\n", sumP.c_str());
        fprintf(fp, "ASSERT( cost_singles  = %s );\r\n", sumR.c_str());
        fprintf(fp, "ASSERT( total_cost    = BVPLUS(%d, cost_patterns , cost_singles) );\r\n\r\n", W_cost);
    }


    // Its outdated "e.g., 40" comment is retained only for identical output.
    fprintf(fp, "ASSERT( BCOST = 0bin0000000000100010 );  %% e.g., 34\r\n");
    fprintf(fp, "ASSERT( BVLE(total_cost, BCOST) );\r\n\r\n");

    // Request a satisfying assignment through QUERY(FALSE).
    fprintf(fp, "QUERY(FALSE);\r\nCOUNTEREXAMPLE;\r\n");

    const bool cvc_write_failed = ferror(fp) != 0;
    const int cvc_close_status = fclose(fp);
    if (cvc_write_failed || cvc_close_status != 0) {
        fprintf(stderr, "[ERROR] Failed to write %s\n", cvc_path);
        return 1;
    }

    fprintf(stdout, "[OK] CVC written to %s\n", cvc_path);
    fprintf(stdout, "[OK] Coverage CSV written to %s\n", csv_path);
    fprintf(stdout, "patterns = %zu\n", patterns.size());
    return 0;
}
