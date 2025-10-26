#include "CircuitOptimizer.h"
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#endif

// ========== 输出重定向 ==========
static void redirectOutputToFile(const std::string& filename) {
    static std::ofstream file(filename);
    std::cout.rdbuf(file.rdbuf());
}

// ========== 工具函数 ==========
static std::vector<CircuitOptimizer::OptimizedPair>
filterByKind(const std::vector<CircuitOptimizer::OptimizedPair>& all, CircuitOptimizer::PairKind k) {
    std::vector<CircuitOptimizer::OptimizedPair> out;
    for (auto& p : all) if (p.kind == k) out.push_back(p);
    return out;
}

static void printPairsWithOps(const std::vector<CircuitOptimizer::OptimizedPair>& pairs) {
    for (size_t i=0;i<pairs.size();++i) {
        const auto& p = pairs[i];
        const char* kindStr = (p.kind == CircuitOptimizer::PairKind::TWO_UNIQUE_FACTORS) ? "TWO_UNIQUE_FACTORS" :
                              (p.kind == CircuitOptimizer::PairKind::ONE_UNIQUE_FACTOR)  ? "ONE_UNIQUE_FACTOR" :
                              (p.kind == CircuitOptimizer::PairKind::SPECIAL_TWO_UNIQUE_FACTORS) ? "SPECIAL_TWO_UNIQUE_FACTORS" : "UNKNOWN";
        std::cout << "\nOptimization Pair " << (i+1) << " [" << kindStr << "] Output: "
                  << (p.outputGateName.empty()? "(unknown)" : p.outputGateName) << "\n";
        if (p.highDegreeTerm) std::cout << "  High: " << p.highDegreeTerm->expression << "\n";
        if (!p.middleTerms.empty()) {
            std::cout << "  Middle:";
            for (auto& m : p.middleTerms) if (m) std::cout << " " << m->expression;
            std::cout << "\n";
        }
        if (p.lowDegreeTerm) std::cout << "  Low: " << p.lowDegreeTerm->expression << "\n";
        if (!p.circuitOperations.empty()) {
            std::cout << "  Circuit operations:\n";
            for (auto& op : p.circuitOperations) std::cout << "    " << op << "\n";
        }
    }
}

// ========== 单个布尔函数：跑 7 步 + 打印 ==========
static void runAndReportOneFunction(const std::string& title,
                                    CircuitOptimizer& optimizer,
                                    const std::string& poly,
                                    std::vector<CircuitOptimizer::OptimizedPair>& outPairs,
                                    std::vector<std::string>& outGeneratedQuadratics,
                                    bool tryReuseFirst,
                                    const std::vector<CircuitOptimizer::OptimizedPair>& referencePairsForReuse)
{
    std::cout << "\n\n==================== " << title << " ====================\n";

    auto grouped = optimizer.groupByDegree(poly);

    // step1: 6 + 2*5 + 4
    if (tryReuseFirst) {
        auto reused = optimizer.tryReusePairsForPattern(grouped, filterByKind(referencePairsForReuse, CircuitOptimizer::PairKind::TWO_UNIQUE_FACTORS));
        outPairs.insert(outPairs.end(), reused.begin(), reused.end());
    }
    auto s1 = optimizer.optimizeTwoUniqueFactors(grouped, 6, 5, 4, 4);
    for (auto& p:s1){ p.kind=CircuitOptimizer::PairKind::TWO_UNIQUE_FACTORS; if(!tryReuseFirst) optimizer.rememberPairForReuse(p); }
    outPairs.insert(outPairs.end(), s1.begin(), s1.end());

    // step2: 6 + 5
    if (tryReuseFirst) {
        auto reused = optimizer.tryReusePairsForPattern(grouped, filterByKind(referencePairsForReuse, CircuitOptimizer::PairKind::ONE_UNIQUE_FACTOR));
        outPairs.insert(outPairs.end(), reused.begin(), reused.end());
    }
    auto s2 = optimizer.optimizeOneUniqueFactor(grouped, 6, 5, 5);
    for (auto& p:s2){ p.kind=CircuitOptimizer::PairKind::ONE_UNIQUE_FACTOR; if(!tryReuseFirst) optimizer.rememberPairForReuse(p); }
    outPairs.insert(outPairs.end(), s2.begin(), s2.end());

    // step3: 7 + 5
    if (tryReuseFirst) {
        auto reused = optimizer.tryReusePairsForPattern(grouped, filterByKind(referencePairsForReuse, CircuitOptimizer::PairKind::ONE_UNIQUE_FACTOR));
        outPairs.insert(outPairs.end(), reused.begin(), reused.end());
    }
    auto s3 = optimizer.optimizeOneUniqueFactor(grouped, 7, 5, 5);
    for (auto& p:s3){ p.kind=CircuitOptimizer::PairKind::ONE_UNIQUE_FACTOR; if(!tryReuseFirst) optimizer.rememberPairForReuse(p); }
    outPairs.insert(outPairs.end(), s3.begin(), s3.end());

    // step4: 7 + 6
    if (tryReuseFirst) {
        auto reused = optimizer.tryReusePairsForPattern(grouped, filterByKind(referencePairsForReuse, CircuitOptimizer::PairKind::ONE_UNIQUE_FACTOR));
        outPairs.insert(outPairs.end(), reused.begin(), reused.end());
    }
    auto s4 = optimizer.optimizeOneUniqueFactor(grouped, 7, 6, 6);
    for (auto& p:s4){ p.kind=CircuitOptimizer::PairKind::ONE_UNIQUE_FACTOR; if(!tryReuseFirst) optimizer.rememberPairForReuse(p); }
    outPairs.insert(outPairs.end(), s4.begin(), s4.end());

    // step5: 4 + (3,3) + virtual 2
    if (tryReuseFirst) {
        auto reused = optimizer.tryReusePairsForPattern(grouped, filterByKind(referencePairsForReuse, CircuitOptimizer::PairKind::SPECIAL_TWO_UNIQUE_FACTORS));
        outPairs.insert(outPairs.end(), reused.begin(), reused.end());
    }
    auto s5 = optimizer.optimizeSpecialTwoUniqueFactors(grouped, 4, 3, 2, 2, outGeneratedQuadratics);
    for (auto& p:s5){ p.kind=CircuitOptimizer::PairKind::SPECIAL_TWO_UNIQUE_FACTORS; if(!tryReuseFirst) optimizer.rememberPairForReuse(p); }
    outPairs.insert(outPairs.end(), s5.begin(), s5.end());



    // step7: 4 + 3
    if (tryReuseFirst) {
        auto reused = optimizer.tryReusePairsForPattern(grouped, filterByKind(referencePairsForReuse, CircuitOptimizer::PairKind::ONE_UNIQUE_FACTOR));
        outPairs.insert(outPairs.end(), reused.begin(), reused.end());
    }
    auto s7 = optimizer.optimizeOneUniqueFactor(grouped, 4, 3, 3);
    for (auto& p:s7){ p.kind=CircuitOptimizer::PairKind::ONE_UNIQUE_FACTOR; if(!tryReuseFirst) optimizer.rememberPairForReuse(p); }
    outPairs.insert(outPairs.end(), s7.begin(), s7.end());


    // step6: 5 + 4
    if (tryReuseFirst) {
        auto reused = optimizer.tryReusePairsForPattern(grouped, filterByKind(referencePairsForReuse, CircuitOptimizer::PairKind::ONE_UNIQUE_FACTOR));
        outPairs.insert(outPairs.end(), reused.begin(), reused.end());
    }
    auto s6 = optimizer.optimizeOneUniqueFactor(grouped, 5, 4, 4);
    for (auto& p:s6){ p.kind=CircuitOptimizer::PairKind::ONE_UNIQUE_FACTOR; if(!tryReuseFirst) optimizer.rememberPairForReuse(p); }
    outPairs.insert(outPairs.end(), s6.begin(), s6.end());

    // 打印 pair 细节、电路节点、统计与剩余项
    printPairsWithOps(outPairs);
    optimizer.printCircuitNodes();
    optimizer.printOptimizationStatistics(outPairs, grouped);
    optimizer.printRemainingTerms(grouped);

    // 二次项 XOR 结果 + 把所有剩余项实现（也登记深度）
    std::string quadraticXOR = optimizer.getQuadraticXORResult(grouped, outGeneratedQuadratics);
    std::cout << "\nQuadratic Terms XOR Result:\n============================\n"
              << (quadraticXOR.empty()? "0" : quadraticXOR) << "\n";

    std::vector<std::string> quadraticTermList;
    {
        std::stringstream ss(quadraticXOR);
        std::string trm;
        while (std::getline(ss, trm, '+')) {
            trm.erase(std::remove_if(trm.begin(), trm.end(), ::isspace), trm.end());
            if (!trm.empty() && trm != "0") quadraticTermList.push_back(trm);
        }
    }
    optimizer.generateRemainingTermsCircuit(grouped, quadraticTermList);

    // All Terms by Final Depth
    std::cout << "\nAll Terms by Final Depth:\n==========================\n";
    auto depthInfo = optimizer.getPairDepthInfo();
    std::map<int, std::vector<CircuitOptimizer::PairDepthInfo>> byDepth;
    for (auto& it : depthInfo) byDepth[it.finalDepth].push_back(it);
    for (auto& d : byDepth) {
        std::cout << "\nDepth " << d.first << " (" << d.second.size() << " items):\n";
        for (auto& x : d.second) {
            if (x.isSingleTerm)
                std::cout << "  Term " << x.pairIndex << " (" << x.pairType << ") -> "
                          << x.expression << " -> Output: " << x.finalOutput << "\n";
            else
                std::cout << "  Term " << x.pairIndex << " (" << x.pairType << ") -> Output: "
                          << x.finalOutput << "\n";
        }
    }

    // 分层合并
    optimizer.hierarchicalMergeAllTerms();
    optimizer.printHierarchicalMergeResult();
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    CircuitOptimizer optimizer;
    redirectOutputToFile("output_10_11test.txt");

    // ====== 8 个布尔函数 ======
    std::string poly1 =
            "x0 + x2 + x6 + x7 + x0x1 + x0x3 + x0x7 + x1x2 + x1x7 + x2x7 + x3x4 + x3x6 + x3x7 + x4x5 + x4x7 + x5x7 + "
            "x0x1x3 + x0x1x5 + x0x1x7 + x0x2x6 + x0x2x7 + x0x3x6 + x0x3x7 + x0x4x5 + x0x4x6 + x0x4x7 + x0x5x6 + x0x5x7 + "
            "x0x6x7 + x1x2x4 + x1x2x6 + x1x3x4 + x1x3x7 + x1x4x5 + x1x4x7 + x1x5x6 + x1x6x7 + x2x3x4 + x2x3x5 + x2x3x6 + "
            "x2x3x7 + x2x4x6 + x2x4x7 + x2x5x6 + x2x5x7 + x2x6x7 + x3x4x5 + x3x4x6 + x3x4x7 + x3x5x6 + x3x5x7 + x3x6x7 + "
            "x4x5x7 + x4x6x7 + x0x1x2x3 + x0x1x2x4 + x0x1x2x5 + x0x1x2x6 + x0x1x3x5 + x0x1x4x6 + x0x1x5x6 + x0x2x3x5 + "
            "x0x2x3x6 + x0x2x4x6 + x0x2x4x7 + x0x2x5x6 + x0x3x4x5 + x0x3x4x7 + x0x3x5x6 + x0x3x5x7 + x0x4x5x7 + x0x5x6x7 + "
            "x1x2x3x4 + x1x2x3x5 + x1x2x4x5 + x1x2x4x7 + x1x2x6x7 + x1x3x4x5 + x1x3x4x7 + x1x3x5x7 + x1x3x6x7 + x1x4x5x6 + "
            "x1x4x6x7 + x2x4x5x6 + x2x4x5x7 + x2x4x6x7 + x2x5x6x7 + x3x4x5x6 + x3x4x5x7 + x3x4x6x7 + x0x1x2x3x4 + "
            "x0x1x2x3x6 + x0x1x2x3x7 + x0x1x2x4x5 + x0x1x2x4x6 + x0x1x2x4x7 + x0x1x2x5x7 + x0x1x3x4x5 + x0x1x3x4x6 + "
            "x0x1x3x5x6 + x0x1x3x6x7 + x0x1x4x5x6 + x0x1x4x6x7 + x0x1x5x6x7 + x0x2x3x4x7 + x0x2x3x5x6 + x0x2x4x5x7 + "
            "x0x2x4x6x7 + x0x2x5x6x7 + x0x3x4x5x7 + x0x3x4x6x7 + x0x3x5x6x7 + x0x4x5x6x7 + x1x2x3x4x6 + x1x2x3x5x6 + "
            "x1x2x3x5x7 + x1x3x4x5x6 + x1x3x4x6x7 + x1x4x5x6x7 + x2x3x4x5x6 + x2x3x5x6x7 + x2x4x5x6x7 + x3x4x5x6x7 + "
            "x0x1x2x3x4x5 + x0x1x2x3x5x7 + x0x1x2x3x6x7 + x0x1x2x4x5x7 + x0x1x2x4x6x7 + x0x1x3x4x5x6 + x0x1x3x4x5x7 + "
            "x0x1x3x4x6x7 + x0x2x3x4x5x6 + x0x2x3x4x6x7 + x0x2x3x5x6x7 + x0x3x4x5x6x7 + x1x2x3x4x5x7 + x1x2x3x5x6x7 + "
            "x1x2x4x5x6x7 + x1x3x4x5x6x7 + x2x3x4x5x6x7 + x0x1x2x3x4x5x6 + x0x1x2x3x4x5x7 + x0x1x2x3x4x6x7 + "
            "x0x1x2x4x5x6x7 + x0x1x3x4x5x6x7";

    std::string poly2 =
            "x0 + x1 + x3 + x7 + x0x1 + x0x2 + x0x4 + x0x6 + x0x7 + x1x2 + x1x4 + "
            "x2x3 + x4x5 + x4x7 + x5x6 + "
            "x0x1x2 + x0x2x4 + x0x2x5 + x0x3x4 + x0x4x5 + "
            "x0x4x7 + x0x5x6 + x0x5x7 + x0x6x7 + "
            "x1x2x4 + x1x2x6 + x1x3x7 + x1x4x7 + x1x5x6 + x1x5x7 + x1x6x7 + "
            "x2x3x5 + x2x3x7 + x2x4x5 + x2x5x6 + x2x6x7 + "
            "x3x4x5 + x3x4x6 + x3x4x7 + x3x5x7 + x3x6x7 + "
            "x4x5x6 + x4x5x7 + x4x6x7 + "
            "x0x1x2x3 + x0x1x2x4 + x0x1x2x6 + x0x1x2x7 + "
            "x0x1x3x4 + x0x1x3x5 + x0x1x3x6 + x0x1x4x7 + "
            "x0x1x5x6 + x0x1x5x7 + x0x1x6x7 + "
            "x0x2x3x5 + x0x2x3x6 + x0x2x5x6 + x0x3x4x6 + "
            "x0x3x5x6 + x0x3x5x7 + x0x4x5x6 + x0x4x5x7 + "
            "x1x2x3x4 + x1x2x3x5 + x1x2x3x6 + x1x2x3x7 + "
            "x1x2x4x6 + x1x2x5x7 + x1x2x6x7 + "
            "x1x3x4x6 + x1x3x4x7 + x1x3x5x7 + x1x3x6x7 + "
            "x1x4x5x6 + x1x4x6x7 + x2x3x4x5 + x2x3x4x6 + "
            "x2x3x5x6 + x2x4x5x6 + x2x5x6x7 + "
            "x3x5x6x7 + x4x5x6x7 + "
            "x0x1x2x3x4 + x0x1x2x3x5 + x0x1x2x3x7 + x0x1x2x4x6 + x0x1x2x4x7 + "
            "x0x1x2x5x6 + x0x1x2x5x7 + x0x1x2x6x7 + "
            "x0x1x3x4x6 + x0x1x3x4x7 + x0x1x4x5x7 + "
            "x0x2x3x4x5 + x0x2x3x4x7 + x0x2x3x5x7 + x0x2x3x6x7 + "
            "x0x2x4x5x7 + x0x2x5x6x7 + x0x3x4x6x7 + x0x3x5x6x7 + x0x4x5x6x7 + "
            "x1x2x3x4x5 + x1x2x3x4x7 + x1x2x3x5x6 + x1x2x3x5x7 + "
            "x1x2x4x5x6 + x1x2x4x5x7 + x1x2x4x6x7 + x1x2x5x6x7 + "
            "x1x3x4x6x7 + x2x3x4x5x7 + x2x3x4x6x7 + x2x4x5x6x7 + x3x4x5x6x7 + "
            "x0x1x2x3x4x5 + x0x1x2x3x5x6 + x0x1x2x4x5x6 + x0x1x2x4x5x7 + "
            "x0x1x3x4x5x6 + x0x1x3x4x5x7 + x0x1x3x5x6x7 + x0x1x4x5x6x7 + "
            "x0x2x3x4x5x6 + x0x2x3x4x6x7 + x0x2x3x5x6x7 + x0x2x4x5x6x7 + x0x3x4x5x6x7 + "
            "x1x2x3x4x5x6 + x1x2x4x5x6x7 + x1x3x4x5x6x7 + "
            "x0x1x2x3x4x5x7 + x0x1x2x3x4x6x7 + x0x1x2x4x5x6x7 + "
            "x1x2x3x4x5x6x7";

    std::string poly3 =
            "1 + x0 + x1 + x4 + x7 + x0x4 + x0x5 + x0x6 + x0x7 + x1x3 + x1x5 + x2x3 + x3x6 + x3x7 + x4x5 + x4x6 + x4x7 + x5x7 + x6x7 + "
            "x0x1x2 + x0x1x4 + x0x1x5 + x0x1x7 + x0x2x4 + x0x2x5 + x0x2x7 + x0x3x4 + x0x3x6 + x0x3x7 + x0x4x6 + x0x5x7 + "
            "x1x2x4 + x1x2x6 + x1x3x4 + x1x3x7 + x1x4x5 + x1x4x6 + x1x6x7 + x2x3x7 + x2x4x5 + x2x4x6 + "
            "x3x4x5 + x3x4x6 + x3x4x7 + x3x5x6 + x3x6x7 + x4x5x6 + x4x5x7 + x4x6x7 + "
            "x0x1x3x4 + x0x1x3x5 + x0x1x3x6 + x0x1x3x7 + x0x1x5x6 + x0x2x3x4 + x0x2x3x5 + x0x2x3x6 + x0x2x3x7 + x0x2x4x5 + x0x2x4x6 + x0x2x5x6 + "
            "x0x3x4x5 + x0x3x4x6 + x0x3x5x6 + x0x3x6x7 + "
            "x1x2x3x4 + x1x2x3x7 + x1x2x4x6 + x1x2x5x6 + x1x2x5x7 + x1x2x6x7 + x1x3x4x7 + x1x4x5x7 + x1x4x6x7 + x1x5x6x7 + "
            "x2x3x4x5 + x2x3x4x6 + x2x3x4x7 + x2x3x5x7 + x2x3x6x7 + x2x4x5x6 + x3x4x6x7 + "
            "x0x1x2x3x5 + x0x1x2x3x7 + x0x1x2x4x7 + x0x1x2x5x7 + x0x1x2x6x7 + x0x1x3x4x5 + x0x1x4x5x7 + x0x1x4x6x7 + x0x1x5x6x7 + "
            "x0x2x3x4x5 + x0x2x3x4x6 + x0x2x3x4x7 + x0x2x4x5x7 + x0x2x4x6x7 + x0x3x4x5x7 + x0x3x4x6x7 + x0x3x5x6x7 + x0x4x5x6x7 + "
            "x1x2x3x4x6 + x1x2x3x5x6 + x1x2x3x5x7 + x1x2x3x6x7 + x1x2x4x5x6 + x1x2x4x6x7 + x1x3x4x5x6 + x1x3x4x6x7 + x1x3x5x6x7 + x1x4x5x6x7 + "
            "x2x3x4x5x7 + x2x3x5x6x7 + x3x4x5x6x7 + "
            "x0x1x2x3x4x6 + x0x1x2x4x5x6 + x0x1x2x4x5x7 + x0x1x2x5x6x7 + x0x1x3x4x5x6 + x0x1x3x4x5x7 + x0x1x3x4x6x7 + x0x1x3x5x6x7 + "
            "x0x2x3x4x5x7 + x0x2x3x5x6x7 + x0x3x4x5x6x7 + "
            "x1x2x3x4x5x6 + x1x2x3x4x5x7 + x1x2x3x4x6x7 + x1x2x4x5x6x7 + x1x3x4x5x6x7 + "
            "x0x1x2x3x4x6x7 + x0x1x2x3x5x6x7 + x0x1x2x4x5x6x7 + x0x1x3x4x5x6x7";

    std::string poly4 =
            "1 + x3 + x4 + x5 + x7 + x0x1 + x0x2 + x0x5 + x1x2 + x1x3 + x1x5 + x1x6 + x1x7 + x2x7 + x3x5 + x3x6 + x3x7 + x4x5 + x4x6 + x5x6 + x6x7 + "
            "x0x1x2 + x0x1x4 + x0x1x5 + x0x2x4 + x0x2x5 + x0x3x4 + x0x3x5 + x0x3x7 + x0x4x5 + x0x4x6 + x0x5x7 + x0x6x7 + "
            "x1x2x3 + x1x2x5 + x1x2x6 + x1x3x6 + x1x3x7 + x1x4x7 + x1x5x6 + x1x5x7 + x1x6x7 + x2x4x5 + x2x4x7 + x2x5x7 + "
            "x3x4x6 + x3x4x7 + x3x5x6 + x3x5x7 + x3x6x7 + x4x5x6 + "
            "x0x1x2x4 + x0x1x2x6 + x0x1x3x7 + x0x1x4x5 + x0x1x4x6 + x0x1x5x6 + x0x2x3x5 + x0x2x3x6 + x0x2x3x7 + x0x2x4x5 + x0x2x5x7 + "
            "x0x3x5x6 + x0x3x5x7 + x0x3x6x7 + x0x4x5x6 + x0x4x5x7 + x0x5x6x7 + "
            "x1x2x3x5 + x1x2x3x6 + x1x2x3x7 + x1x2x4x7 + x1x2x5x7 + x1x3x4x6 + x1x3x4x7 + x1x3x5x6 + x1x4x5x6 + x1x5x6x7 + "
            "x2x3x5x7 + x2x3x6x7 + x2x4x5x6 + x2x4x5x7 + x2x5x6x7 + x4x5x6x7 + "
            "x0x1x2x3x5 + x0x1x2x4x7 + x0x1x2x5x6 + x0x1x2x6x7 + x0x1x3x4x7 + x0x1x3x5x6 + x0x1x3x5x7 + x0x1x4x6x7 + x0x1x5x6x7 + "
            "x0x2x3x4x5 + x0x2x3x4x6 + x0x2x3x4x7 + x0x2x3x5x7 + x0x2x4x5x6 + x0x2x4x6x7 + x0x2x5x6x7 + x0x3x5x6x7 + x0x4x5x6x7 + "
            "x1x2x3x4x5 + x1x2x3x4x7 + x1x2x3x5x6 + x1x2x3x6x7 + x1x2x4x5x6 + x1x2x4x6x7 + x1x3x4x5x7 + x1x3x4x6x7 + x1x4x5x6x7 + "
            "x2x3x4x5x7 + x2x3x5x6x7 + x3x4x5x6x7 + "
            "x0x1x2x3x4x5 + x0x1x2x3x4x6 + x0x1x2x3x5x6 + x0x1x2x3x6x7 + x0x1x2x4x5x7 + x0x1x2x5x6x7 + x0x1x3x4x5x6 + x0x1x3x4x6x7 + "
            "x0x1x4x5x6x7 + x0x2x3x4x5x7 + x0x2x3x5x6x7 + x0x2x4x5x6x7 + x0x3x4x5x6x7 + x1x2x3x4x5x7 + x1x3x4x5x6x7 + "
            "x0x1x2x3x4x5x7 + x0x1x2x3x5x6x7 + x0x1x3x4x5x6x7";

    std::string poly5 =
            "x2 + x4 + x5 + x6 + x7 + x0x1 + x0x2 + x0x4 + x0x7 + x1x3 + x1x6 + x1x7 + x2x3 + x2x4 + x2x5 + x2x6 + x2x7 + x3x4 + x3x6 + x3x7 + x4x5 + x6x7 + "
            "x0x1x2 + x0x1x3 + x0x1x4 + x0x1x5 + x0x1x6 + x0x1x7 + x0x2x3 + x0x2x4 + x0x2x6 + x0x3x4 + x0x3x5 + x0x3x7 + "
            "x1x2x4 + x1x2x5 + x1x3x4 + x1x3x7 + x1x4x5 + x1x5x7 + "
            "x2x3x4 + x2x3x5 + x2x3x6 + x2x3x7 + x2x4x5 + x2x4x6 + x2x4x7 + x3x4x5 + x3x4x7 + x3x5x6 + x4x5x7 + "
            "x0x1x2x4 + x0x1x3x4 + x0x1x3x7 + x0x1x4x7 + x0x1x5x6 + x0x1x6x7 + x0x2x3x4 + x0x2x4x5 + x0x2x4x7 + x0x2x5x6 + x0x2x5x7 + x0x2x6x7 + "
            "x0x3x4x5 + x0x3x4x6 + x0x3x4x7 + x0x4x6x7 + "
            "x1x2x3x7 + x1x2x4x5 + x1x2x4x7 + x1x2x6x7 + x1x3x4x7 + x1x3x5x6 + x1x3x6x7 + x1x4x6x7 + x1x5x6x7 + "
            "x2x3x4x6 + x2x3x4x7 + x2x3x6x7 + x2x4x5x6 + x2x4x6x7 + x3x4x5x6 + x3x4x5x7 + "
            "x0x1x2x3x4 + x0x1x2x3x5 + x0x1x2x3x6 + x0x1x2x3x7 + x0x1x2x4x6 + x0x1x2x4x7 + x0x1x2x5x6 + x0x1x2x6x7 + "
            "x0x1x3x4x5 + x0x1x3x4x7 + x0x1x3x5x7 + x0x1x4x6x7 + x0x1x5x6x7 + "
            "x0x2x3x4x5 + x0x2x3x4x6 + x0x2x3x5x6 + x0x2x4x5x6 + x0x2x4x5x7 + x0x2x4x6x7 + x0x2x5x6x7 + x0x4x5x6x7 + "
            "x1x2x3x4x5 + x1x2x3x4x6 + x1x2x3x5x6 + x1x2x3x6x7 + x1x2x4x5x6 + x1x2x4x5x7 + x1x3x4x5x6 + x1x3x4x5x7 + x1x3x4x6x7 + x1x4x5x6x7 + "
            "x2x3x4x5x7 + x2x3x5x6x7 + x3x4x5x6x7 + "
            "x0x1x2x3x4x7 + x0x1x2x4x5x6 + x0x1x3x4x5x6 + x0x2x3x4x5x6 + x0x2x3x4x5x7 + x0x3x4x5x6x7 + "
            "x1x2x3x4x5x6 + x1x2x3x4x6x7 + x1x2x3x5x6x7 + x1x2x4x5x6x7 + x2x3x4x5x6x7 + "
            "x0x1x2x3x4x5x7 + x0x1x3x4x5x6x7 + x0x2x3x4x5x6x7";

    std::string poly6 =
            "1 + x0 + x1 + x3 + x0x2 + x1x3 + x1x6 + x2x6 + x3x4 + x3x5 + x4x7 + "
            "x0x1x2 + x0x1x5 + x0x1x6 + x0x2x3 + x0x2x4 + x0x2x5 + x0x2x6 + x0x3x5 + x0x3x6 + x0x3x7 + x0x4x5 + x0x4x6 + x0x6x7 + "
            "x1x2x6 + x1x2x7 + x1x3x6 + x1x4x5 + x1x4x6 + x1x5x6 + x1x5x7 + x1x6x7 + x2x3x4 + x2x3x5 + x2x4x6 + x2x4x7 + x2x5x6 + x2x6x7 + "
            "x3x5x6 + x3x5x7 + x3x6x7 + x4x6x7 + x5x6x7 + "
            "x0x1x2x7 + x0x1x3x4 + x0x1x3x7 + x0x2x3x4 + x0x2x3x5 + x0x2x3x6 + x0x2x4x6 + x0x2x6x7 + x0x3x4x5 + x0x3x4x6 + x0x3x4x7 + x0x3x5x6 + x0x3x6x7 + x0x4x6x7 + "
            "x1x2x3x4 + x1x2x4x5 + x1x2x4x6 + x1x2x6x7 + x1x3x4x5 + x1x3x4x6 + x1x3x6x7 + x1x4x5x6 + "
            "x2x3x4x5 + x2x3x5x6 + x2x3x5x7 + x2x3x6x7 + x2x4x5x6 + x2x4x5x7 + x2x5x6x7 + x3x4x5x7 + x3x4x6x7 + x4x5x6x7 + "
            "x0x1x2x3x5 + x0x1x2x3x7 + x0x1x2x4x5 + x0x1x2x6x7 + x0x1x3x4x6 + x0x1x3x6x7 + x0x1x4x5x7 + x0x1x5x6x7 + "
            "x0x2x3x4x7 + x0x2x3x5x7 + x0x2x3x6x7 + x0x2x4x5x7 + x0x2x4x6x7 + x0x3x4x5x6 + x0x3x4x5x7 + x0x4x5x6x7 + "
            "x1x2x3x4x5 + x1x2x3x4x6 + x1x2x3x4x7 + x1x2x3x5x6 + x1x2x3x6x7 + x1x3x4x5x6 + x1x3x5x6x7 + x1x4x5x6x7 + "
            "x2x3x5x6x7 + x2x4x5x6x7 + x3x4x5x6x7 + "
            "x0x1x2x3x5x6 + x0x1x2x3x5x7 + x0x1x2x4x5x6 + x0x1x3x4x5x6 + x0x1x3x4x5x7 + x0x2x3x5x6x7 + x0x2x4x5x6x7 + "
            "x1x2x3x5x6x7 + x1x2x4x5x6x7 + x2x3x4x5x6x7 + x0x1x2x3x5x6x7 + x0x1x2x4x5x6x7";

    std::string poly7 =
            "1 + x1 + x2 + x4 + x0x2 + x0x4 + x0x6 + x0x7 + x1x3 + x2x4 + x2x7 + x3x7 + x4x5 + x4x6 + "
            "x0x1x2 + x0x1x4 + x0x1x6 + x0x2x4 + x0x2x7 + x0x3x5 + x0x3x6 + x0x3x7 + x0x4x5 + x0x5x6 + "
            "x1x2x3 + x1x2x6 + x1x2x7 + x1x3x4 + x1x3x5 + x1x3x6 + x1x3x7 + x1x4x6 + x1x4x7 + x1x5x6 + x1x5x7 + "
            "x2x3x7 + x2x4x7 + x2x5x6 + x2x5x7 + x2x6x7 + x3x4x5 + x3x4x6 + x3x5x7 + x3x6x7 + x4x6x7 + "
            "x0x1x2x3 + x0x1x3x6 + x0x1x3x7 + x0x1x6x7 + x0x2x3x6 + x0x2x4x5 + x0x2x4x6 + x0x2x5x7 + "
            "x0x3x4x6 + x0x3x4x7 + x0x3x5x6 + x0x4x5x7 + x0x4x6x7 + "
            "x1x2x4x5 + x1x3x4x5 + x1x3x4x6 + x1x3x4x7 + x1x3x5x7 + x1x4x5x6 + x1x4x5x7 + x1x4x6x7 + "
            "x2x3x4x5 + x2x3x5x6 + x2x3x5x7 + x2x4x5x6 + x2x4x5x7 + x2x5x6x7 + "
            "x3x4x5x6 + x3x4x6x7 + x3x5x6x7 + "
            "x0x1x2x3x4 + x0x1x2x3x6 + x0x1x2x3x7 + x0x1x2x4x5 + x0x1x2x5x6 + x0x1x2x5x7 + x0x1x3x4x5 + x0x1x3x4x7 + x0x1x3x5x7 + x0x1x4x6x7 + "
            "x0x2x3x4x6 + x0x2x3x5x7 + x0x2x3x6x7 + x0x2x4x5x6 + x0x3x4x5x6 + x0x4x5x6x7 + "
            "x1x2x3x4x6 + x1x2x3x5x6 + x1x2x4x5x7 + x1x4x5x6x7 + x2x3x4x5x6 + x2x3x4x5x7 + x2x3x4x6x7 + x2x4x5x6x7 + "
            "x0x1x2x3x4x6 + x0x1x2x4x6x7 + x0x1x2x5x6x7 + x0x1x3x4x6x7 + x0x1x3x5x6x7 + x0x2x3x4x6x7 + x0x2x3x5x6x7 + x0x3x4x5x6x7 + "
            "x1x2x3x4x6x7 + x1x2x3x5x6x7 + x1x2x4x5x6x7 + x0x1x2x3x4x6x7 + x0x1x2x3x5x6x7";

    std::string poly8 =
            "x0 + x2 + x3 + x5 + x0x2 + x0x6 + x0x7 + x1x3 + x1x5 + x1x7 + x2x4 + x3x5 + x5x6 + x5x7 + "
            "x0x1x3 + x0x1x5 + x0x1x7 + x0x2x3 + x0x2x4 + x0x3x6 + x0x4x7 + x0x5x7 + "
            "x1x2x3 + x1x2x5 + x1x2x7 + x1x3x5 + x1x4x6 + x1x4x7 + x1x5x6 + x1x6x7 + "
            "x2x3x4 + x2x3x7 + x2x4x5 + x2x4x6 + x2x4x7 + x2x5x7 + x2x6x7 + x3x6x7 + x4x5x6 + x4x5x7 + "
            "x0x1x2x3 + x0x1x2x5 + x0x1x2x6 + x0x1x3x5 + x0x1x3x6 + x0x1x3x7 + x0x1x4x6 + x0x1x4x7 + "
            "x0x2x3x6 + x0x2x4x7 + x0x3x4x5 + x0x3x4x6 + x0x3x4x7 + x0x3x5x7 + x0x3x6x7 + x0x4x5x6 + x0x4x5x7 + x0x5x6x7 + "
            "x1x2x3x4 + x1x2x4x7 + x1x3x4x7 + x1x3x5x6 + x1x3x5x7 + x1x4x5x7 + x1x4x6x7 + "
            "x2x3x5x6 + x2x4x5x6 + x2x4x5x7 + x2x5x6x7 + x3x4x5x6 + x3x4x6x7 + x3x5x6x7 + x4x5x6x7 + "
            "x0x1x2x3x4 + x0x1x2x3x6 + x0x1x2x4x6 + x0x1x2x4x7 + x0x1x2x5x6 + x0x1x2x6x7 + x0x1x3x4x7 + x0x1x3x5x6 + x0x1x5x6x7 + "
            "x0x2x3x6x7 + x0x2x5x6x7 + x0x3x4x5x7 + x0x3x4x6x7 + "
            "x1x2x3x4x5 + x1x2x3x4x7 + x1x2x3x5x6 + x1x2x3x6x7 + x1x2x4x5x6 + x1x3x4x5x7 + x1x3x5x6x7 + x1x4x5x6x7 + "
            "x2x3x4x5x7 + x2x3x4x6x7 + x3x4x5x6x7 + "
            "x0x1x2x3x4x6 + x0x1x2x3x5x7 + x0x1x2x4x5x7 + x0x1x2x4x6x7 + x0x1x3x4x5x7 + x0x1x3x4x6x7 + x0x1x4x5x6x7 + "
            "x0x2x3x4x5x7 + x0x2x3x4x6x7 + x0x2x3x5x6x7 + x1x2x3x4x5x7 + x0x1x2x3x4x5x7 + x0x1x2x3x4x6x7";

    // ====== 依次运行 8 个函数 ======
    // 复用参考集合（累积前面所有函数得到的 pairs）
    std::vector<CircuitOptimizer::OptimizedPair> reuseSoFar;

    // 1) 第一个函数：不复用，建字典
    {
        std::vector<CircuitOptimizer::OptimizedPair> pairs;
        std::vector<std::string> genQ;
        runAndReportOneFunction("F1 (poly1)", optimizer, poly1, pairs, genQ, /*tryReuseFirst=*/false, {});
        // 加入全局复用池
        reuseSoFar.insert(reuseSoFar.end(), pairs.begin(), pairs.end());
    }
    optimizer.clearPairDepthInfo(); // 仅清统计

    // 2) 第二个函数及其后：先复用再优化；每次把本轮 pairs 也加入复用池
    auto run_with_reuse = [&](const char* title, const std::string& poly) {
        std::vector<CircuitOptimizer::OptimizedPair> pairs;
        std::vector<std::string> genQ;
        runAndReportOneFunction(title, optimizer, poly, pairs, genQ, /*tryReuseFirst=*/true, reuseSoFar);
        reuseSoFar.insert(reuseSoFar.end(), pairs.begin(), pairs.end());
        optimizer.clearPairDepthInfo(); // 仅清统计（电路与复用目录保留）
    };

    run_with_reuse("F2 (poly2)", poly2);
    run_with_reuse("F3 (poly3)", poly3);
    run_with_reuse("F4 (poly4)", poly4);
    run_with_reuse("F5 (poly5)", poly5);
    run_with_reuse("F6 (poly6)", poly6);
    run_with_reuse("F7 (poly7)", poly7);
    run_with_reuse("F8 (poly8)", poly8);

    optimizer.exportVerilog("aes_sbox_tgates.v", "AES_SBOX_TGATES");

    return 0;
}
