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

class CircuitOptimizer {
public:
    struct Term {
        std::string expression;
        std::set<int> variables;
        int degree;
        bool used;

        Term(const std::string& expr);
        bool hasCommonFactor(const Term& other, std::set<int>& commonVars) const;
        std::set<int> getDifference(const Term& other) const;
        std::set<int> getDifferenceFromSet(const std::set<int>& otherSet) const;
    };

    struct CircuitNode {
        std::string name;
        std::string operation;
        std::string expression;
        int depth;
        CircuitNode(const std::string& n, const std::string& op,
                    const std::string& expr, int d)
                : name(n), operation(op), expression(expr), depth(d) {}
    };

    // ===== 新增：模式类别 =====
    enum class PairKind {
        TWO_UNIQUE_FACTORS,
        ONE_UNIQUE_FACTOR,
        SPECIAL_TWO_UNIQUE_FACTORS,
        UNKNOWN
    };

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

        // ===== 新增 =====
        PairKind kind = PairKind::UNKNOWN;
        std::string outputGateName;
        std::string patternKey() const;
    };

    CircuitOptimizer() = default;

    std::map<int, std::vector<std::shared_ptr<Term>>> groupByDegree(const std::string& polynomial);

    std::vector<OptimizedPair> optimizeOneUniqueFactor(
            std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
            int highDegree, int lowDegree, int requiredCommonVars);

    std::vector<OptimizedPair> optimizeTwoUniqueFactors(
            std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
            int highDegree, int middleDegree, int lowDegree, int requiredCommonVars);

    std::vector<OptimizedPair> optimizeSpecialTwoUniqueFactors(
            std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
            int highDegree, int middleDegree, int lowDegree, int requiredCommonVars,
            std::vector<std::string>& generatedQuadraticTerms);

    std::string findOrCreateNode(const std::string& operation,
                                 const std::string& expression,
                                 int depth);

    std::string getQuadraticXORResult(
            const std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
            const std::vector<std::string>& generatedQuadraticTerms);

    void generateRemainingTermsCircuit(
            const std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
            const std::vector<std::string>& quadraticTerms);

    std::vector<std::string> parseQuadraticTerm(const std::string& quadraticTerm);

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

    void hierarchicalMergeAllTerms();
    void printHierarchicalMergeResult() const;
    std::string getFinalMergeResult() const { return finalMergeResult; }

    void printCircuitNodes() const;
    void clearCircuitNodes();
    void printRemainingTerms(const std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms);
    void printOptimizationStatistics(
            const std::vector<OptimizedPair>& pairs,
            const std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms);

    // ===== 新增接口 =====
    void rememberPairForReuse(const OptimizedPair& pair);  // 在生成 pair 时调用，记录表达式->t_xx
    // CircuitOptimizer.h 里加入（或改成）这一版声明
    std::vector<OptimizedPair> tryReusePairsForPattern(
            std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
            const std::vector<OptimizedPair>& referencePairs);


    // 把“单项式/单个电路输出”写进 pairDepthInfo（用于后续 Initial items by depth 统计）
    void pushDepthInfoSingle(const std::string& expr,
                             const std::string& out,
                             int finalDepth,
                             const std::string& type);

    // ======= XOR 归并复用记录 =======
    struct ReusableXorMerge {
        int depth;                     // 执行 XOR 的深度（current XOR level）
        std::string inKey;             // 归一后的 (A,B) 表达式键：depth + '|' + key(A,B)
        std::string inAExpr;           // 左输入的表达式（规范化）
        std::string inBExpr;           // 右输入的表达式（规范化）
        std::string outNode;           // 该 XOR 的输出门名（tXX）
    };

// 记录表：key = depth + '|' + canonicalKey(a,b)  -> ReusableXorMerge
    std::unordered_map<std::string, ReusableXorMerge> xorCatalog_;

// ======= 对外 API：在第一轮归并时记录，在第二轮归并时复用 =======
    void rememberXorMerge(int depth, const std::string& leftNode, const std::string& rightNode, const std::string& outNode);
    std::string tryReuseXorAtDepth(int depth, const std::string& leftExpr, const std::string& rightExpr) const;

// 可选：如需一键清空 XOR 复用目录（一般第二轮要复用，不要清）
    void clearXorCatalog();

// ======= 辅助：表达式归一与节点查询 =======
    static std::string makeXorKey(const std::string& exprA, const std::string& exprB); // 生成无序键
    bool getNodeExpr(const std::string& nodeName, std::string& outExpr, int* outDepth = nullptr) const;

    // 生成 Verilog 网表：把当前 circuitNodes 里所有 tXX 逻辑门导出为 Verilog
    void exportVerilog(const std::string& filepath,
                       const std::string& moduleName = "AES_SBOX_TGATES") const;



private:
    std::vector<CircuitNode> circuitNodes;
    int nodeCounter = 0;
    std::vector<PairDepthInfo> pairDepthInfo;
    std::map<int, std::string> depthMergeResults;
    std::vector<std::string> hierarchicalOperations;
    std::string finalMergeResult;

    OptimizedPair findOneUniqueFactorPair(
            std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
            int highDegree, int lowDegree, int requiredCommonVars);

    OptimizedPair findTwoUniqueFactorsPair(
            std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
            int highDegree, int middleDegree, int lowDegree, int requiredCommonVars);

    OptimizedPair findSpecialTwoUniqueFactorsPair(
            std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
            int highDegree, int middleDegree, int lowDegree, int requiredCommonVars);

    std::string generateCircuitForSpecialTwoUniqueFactors(OptimizedPair& pair);
    void generateCircuitForOneUniqueFactor(OptimizedPair& pair);
    void generateCircuitForTwoUniqueFactors(OptimizedPair& pair);
    int combineOperationsWithReuse(OptimizedPair& pair, std::vector<std::string>& inputs, int depth);

    std::string mergeItemsAtDepth(int depth, const std::vector<std::string>& items, int startDepth);
    std::string mergeItemsAtDepth(int depth, std::vector<std::string> items, int startDepth,
                                  const std::map<int, std::pair<std::string, int>>& availableResults);
    std::map<int, std::vector<std::string>> getAllItemsByDepth() const;
    std::string cleanTerm(const std::string& term);

    // ===== 新增复用字典 =====
    std::map<std::string, std::string> reuseDict_;


public:
// ==== 新增：可复用优化对的记录 ====
    struct ReusablePairRecord {
        PairKind kind;                      // TWO_UNIQUE_FACTORS / ONE_UNIQUE_FACTOR / SPECIAL_TWO_UNIQUE_FACTORS
        std::string signature;              // 规范化签名（见 makeSignature_*）
        std::string outputGateName;         // 实现该优化对的 t_xx

        // 组成部分（便于复用时核对 & 打印）
        std::string highExpr;               // 最高次项表达式（如 "x0x1x2x3x4x5"）
        std::vector<std::string> middleExprs; // 中间项（2 个或 1 个，取决于模式）
        std::string lowExpr;                // 低次项表达式（某些模式会为空）
    };

// ==== 新增：对外 API ====


// ==== 私有：存储与签名/工具 ====
private:
    std::unordered_map<std::string, ReusablePairRecord> reuseCatalog_;  // signature -> record

    static std::string makeSignatureTwoUnique(const std::string& high,
                                              const std::string& m1,
                                              const std::string& m2,
                                              const std::string& low);
    static std::string makeSignatureOneUnique(const std::string& high,
                                              const std::string& low);
    static std::string makeSignatureSpecialTwo(const std::string& q4,
                                               const std::string& c31,
                                               const std::string& c32);
    static std::string canonicalExpr(const std::string& expr); // 把 "x3x1x2" 规整成 "x1x2x3"

    static int degreeOfExpr(const std::string& expr); // 统计表达式的次数
    // 注意：需要修改 groupedTerms（标记 used），所以这里不能是 const map&
    bool findTermByExpr(
            std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
            const std::string& expr,
            std::shared_ptr<Term>& outTerm) const;


};




#endif // CIRCUITOPTIMIZER_H
