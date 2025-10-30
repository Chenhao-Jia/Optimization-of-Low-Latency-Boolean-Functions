#ifndef CIRCUITOPTIMIZER_H
#define CIRCUITOPTIMIZER_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <set>
#include <algorithm>
#include <memory>
#include <iomanip>
#include <unordered_map>

#ifdef _WIN32
#include <windows.h>
#endif

// ============================================================================
//  Class: CircuitOptimizer
//  Purpose: Provides circuit-level Boolean function optimization, pattern
//            detection, term grouping, and Verilog generation for AES S-boxes
// ============================================================================
class CircuitOptimizer {
public:

    // ------------------------------------------------------------------------
    //  Term: Represents a single monomial term (e.g., x1x2x3) in a Boolean
    //  polynomial, including its variable set, degree, and usage status.
    // ------------------------------------------------------------------------
    struct Term {
        std::string expression;       // Original expression, e.g., "x1x2x3"
        std::set<int> variables;      // Set of variable indices (1,2,3)
        int degree;                   // Degree = number of variables
        bool used;                    // Flag: whether this term has been merged/used

        Term(const std::string& expr);
        bool hasCommonFactor(const Term& other, std::set<int>& commonVars) const;
        std::set<int> getDifference(const Term& other) const;
        std::set<int> getDifferenceFromSet(const std::set<int>& otherSet) const;
    };

    // ------------------------------------------------------------------------
    //  CircuitNode: Represents a logic gate node in the optimized circuit.
    //  Each node records its name, operation, Boolean expression, and depth.
    // ------------------------------------------------------------------------
    struct CircuitNode {
        std::string name;         // Node identifier, e.g., t12
        std::string operation;    // Gate type (AND, XOR, NOT, etc.)
        std::string expression;   // Algebraic form of the output
        int depth;                // Logic depth level
        CircuitNode(const std::string& n, const std::string& op,
                    const std::string& expr, int d)
                : name(n), operation(op), expression(expr), depth(d) {}
    };

    // ------------------------------------------------------------------------
    //  PairKind: Describes the pattern type of a factorization (used in reuse).
    // ------------------------------------------------------------------------
    enum class PairKind {
        TWO_UNIQUE_FACTORS,
        ONE_UNIQUE_FACTOR,
        SPECIAL_TWO_UNIQUE_FACTORS,
        UNKNOWN
    };

    // ------------------------------------------------------------------------
    //  OptimizedPair: Stores the relationship among high-, middle-, and
    //  low-degree terms during Boolean function decomposition and merging.
    //  Each pair also tracks gate names and circuit operations.
    // ------------------------------------------------------------------------
    struct OptimizedPair {
        enum PairType { ONE_UNIQUE_FACTOR, TWO_UNIQUE_FACTORS };
        PairType type;
        std::shared_ptr<Term> highDegreeTerm;
        std::vector<std::shared_ptr<Term>> middleTerms;
        std::shared_ptr<Term> lowDegreeTerm;
        std::set<int> commonVars;
        std::set<int> uniqueVars;
        std::vector<std::string> circuitOperations;

        OptimizedPair();
        OptimizedPair(const std::shared_ptr<Term>& high,
                      const std::shared_ptr<Term>& low,
                      const std::set<int>& common,
                      const std::set<int>& unique);
        OptimizedPair(const std::shared_ptr<Term>& high,
                      const std::vector<std::shared_ptr<Term>>& middle,
                      const std::shared_ptr<Term>& low,
                      const std::set<int>& common,
                      const std::set<int>& unique);

        PairKind kind = PairKind::UNKNOWN;   // Factorization pattern type
        std::string outputGateName;          // Output gate name (tXX)
        std::string patternKey() const;      // Generate unique key for reuse
    };

    CircuitOptimizer() = default;

    // Group all terms in a polynomial by degree
    std::map<int, std::vector<std::shared_ptr<Term>>> groupByDegree(const std::string& polynomial);

    // Optimize pairs that share one unique factor
    std::vector<OptimizedPair> optimizeOneUniqueFactor(
            std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
            int highDegree, int lowDegree, int requiredCommonVars);

    // Optimize pairs that share two unique factors
    std::vector<OptimizedPair> optimizeTwoUniqueFactors(
            std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
            int highDegree, int middleDegree, int lowDegree, int requiredCommonVars);

    // Optimize special case of "two unique factors" pattern (Q4 + 2*C3)
    std::vector<OptimizedPair> optimizeSpecialTwoUniqueFactors(
            std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
            int highDegree, int middleDegree, int lowDegree, int requiredCommonVars,
            std::vector<std::string>& generatedQuadraticTerms);

    // Create or retrieve a circuit node (prevents redundant gate creation)
    std::string findOrCreateNode(const std::string& operation,
                                 const std::string& expression,
                                 int depth);

    // Compute the XOR result of quadratic terms (cancel duplicates in GF(2))
    std::string getQuadraticXORResult(
            const std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
            const std::vector<std::string>& generatedQuadraticTerms);

    // Generate the AND-tree circuit for remaining terms not optimized
    void generateRemainingTermsCircuit(
            const std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
            const std::vector<std::string>& quadraticTerms);

    // Parse a quadratic term like "x3x7" into {"x3","x7"}
    std::vector<std::string> parseQuadraticTerm(const std::string& quadraticTerm);

    // ------------------------------------------------------------------------
    //  PairDepthInfo: Records final circuit depth and output for each term/pair.
    // ------------------------------------------------------------------------
    struct PairDepthInfo {
        int pairIndex;
        int finalDepth;
        std::string finalOutput;
        std::string pairType;
        std::string expression;
        bool isSingleTerm;
    };

    std::vector<PairDepthInfo> getPairDepthInfo() const { return pairDepthInfo; }
    void clearPairDepthInfo() { pairDepthInfo.clear(); }

    // Perform hierarchical XOR merging across all circuit outputs
    void hierarchicalMergeAllTerms();
    void printHierarchicalMergeResult() const;
    std::string getFinalMergeResult() const { return finalMergeResult; }

    // Debug and reporting utilities
    void printCircuitNodes() const;
    void clearCircuitNodes();
    void printRemainingTerms(const std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms);
    void printOptimizationStatistics(
            const std::vector<OptimizedPair>& pairs,
            const std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms);

    // ------------------------------------------------------------------------
    //  New interface: pair reuse registration and reuse across functions
    // ------------------------------------------------------------------------
    void rememberPairForReuse(const OptimizedPair& pair);  // Record mapping: expression -> tXX
    std::vector<OptimizedPair> tryReusePairsForPattern(
            std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
            const std::vector<OptimizedPair>& referencePairs);

    // Record a single term’s output (used for “Initial items by depth” report)
    void pushDepthInfoSingle(const std::string& expr,
                             const std::string& out,
                             int finalDepth,
                             const std::string& type);

    // ------------------------------------------------------------------------
    //  XOR merge reuse record structures and APIs
    // ------------------------------------------------------------------------
    struct ReusableXorMerge {
        int depth;                     // XOR execution depth
        std::string inKey;             // Canonical (A,B) key: depth + '|' + key(A,B)
        std::string inAExpr;           // Normalized left operand
        std::string inBExpr;           // Normalized right operand
        std::string outNode;           // Output gate name (tXX)
    };

    // Lookup table: key = depth + '|' + canonicalKey(a,b) → ReusableXorMerge
    std::unordered_map<std::string, ReusableXorMerge> xorCatalog_;

    // API for recording and reusing XOR merges between passes
    void rememberXorMerge(int depth, const std::string& leftNode, const std::string& rightNode, const std::string& outNode);
    std::string tryReuseXorAtDepth(int depth, const std::string& leftExpr, const std::string& rightExpr) const;

    // Optional: clear the XOR reuse catalog (normally kept between passes)
    void clearXorCatalog();

    // Expression canonicalization and node expression lookup helpers
    static std::string makeXorKey(const std::string& exprA, const std::string& exprB); // Generate unordered canonical key
    bool getNodeExpr(const std::string& nodeName, std::string& outExpr, int* outDepth = nullptr) const;

    // Export current logic network (circuitNodes) to a Verilog netlist
    void exportVerilog(const std::string& filepath,
                       const std::string& moduleName = "AES_SBOX_TGATES") const;

private:
    // ------------------------------------------------------------------------
    //  Internal data structures and circuit management
    // ------------------------------------------------------------------------
    std::vector<CircuitNode> circuitNodes;
    int nodeCounter = 0;
    std::vector<PairDepthInfo> pairDepthInfo;
    std::map<int, std::string> depthMergeResults;
    std::vector<std::string> hierarchicalOperations;
    std::string finalMergeResult;

    // Core pattern-detection functions
    OptimizedPair findOneUniqueFactorPair(
            std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
            int highDegree, int lowDegree, int requiredCommonVars);

    OptimizedPair findTwoUniqueFactorsPair(
            std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
            int highDegree, int middleDegree, int lowDegree, int requiredCommonVars);

    OptimizedPair findSpecialTwoUniqueFactorsPair(
            std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
            int highDegree, int middleDegree, int lowDegree, int requiredCommonVars);

    // Circuit generation for different optimization patterns
    std::string generateCircuitForSpecialTwoUniqueFactors(OptimizedPair& pair);
    void generateCircuitForOneUniqueFactor(OptimizedPair& pair);
    void generateCircuitForTwoUniqueFactors(OptimizedPair& pair);
    int combineOperationsWithReuse(OptimizedPair& pair, std::vector<std::string>& inputs, int depth);

    // Hierarchical XOR merge utilities
    std::string mergeItemsAtDepth(int depth, const std::vector<std::string>& items, int startDepth);
    std::string mergeItemsAtDepth(int depth, std::vector<std::string> items, int startDepth,
                                  const std::map<int, std::pair<std::string, int>>& availableResults);
    std::map<int, std::vector<std::string>> getAllItemsByDepth() const;
    std::string cleanTerm(const std::string& term);

    // Reuse dictionary (expression → gate name)
    std::map<std::string, std::string> reuseDict_;

public:
    // ------------------------------------------------------------------------
    //  ReusablePairRecord: Stores the reusable pair information for reuse
    // ------------------------------------------------------------------------
    struct ReusablePairRecord {
        PairKind kind;                      // Pattern type
        std::string signature;              // Canonical signature
        std::string outputGateName;         // Gate implementing the pair

        // Components for reuse verification and reporting
        std::string highExpr;               // High-degree term (e.g., "x0x1x2x3x4x5")
        std::vector<std::string> middleExprs; // Middle terms (2 or 1 depending on pattern)
        std::string lowExpr;                // Low-degree term (may be empty)
    };

private:
    // Reuse catalog: key = signature → reusable record
    std::unordered_map<std::string, ReusablePairRecord> reuseCatalog_;

    // Signature generation helpers for pattern identification
    static std::string makeSignatureTwoUnique(const std::string& high,
                                              const std::string& m1,
                                              const std::string& m2,
                                              const std::string& low);
    static std::string makeSignatureOneUnique(const std::string& high,
                                              const std::string& low);
    static std::string makeSignatureSpecialTwo(const std::string& q4,
                                               const std::string& c31,
                                               const std::string& c32);

    // Canonicalize expressions like "x3x1x2" → "x1x2x3"
    static std::string canonicalExpr(const std::string& expr);

    // Compute algebraic degree of an expression
    static int degreeOfExpr(const std::string& expr);

    // Find a term by its expression and mark it as used
    bool findTermByExpr(
            std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
            const std::string& expr,
            std::shared_ptr<Term>& outTerm) const;
};

#endif // CIRCUITOPTIMIZER_H
