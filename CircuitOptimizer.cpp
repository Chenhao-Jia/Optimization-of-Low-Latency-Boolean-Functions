#include "CircuitOptimizer.h"
#include <cmath>
#include <fstream>
#include <regex>
#include <set>

// Term 结构体实现保持不变
// Term 结构体实现
CircuitOptimizer::Term::Term(const std::string& expr) : expression(expr), used(false) {
    std::stringstream ss(expr);
    char c;
    int num;

    while (ss >> c) {
        if (c == 'x') {
            if (ss >> num) {
                variables.insert(num);
            }
        }
    }
    degree = variables.size();
}

bool CircuitOptimizer::Term::hasCommonFactor(const Term& other, std::set<int>& commonVars) const {
    commonVars.clear();
    std::set_intersection(
            variables.begin(), variables.end(),
            other.variables.begin(), other.variables.end(),
            std::inserter(commonVars, commonVars.begin())
    );
    return commonVars.size() >= 2;
}

std::set<int> CircuitOptimizer::Term::getDifference(const Term& other) const {
    std::set<int> diff;
    std::set_difference(
            variables.begin(), variables.end(),
            other.variables.begin(), other.variables.end(),
            std::inserter(diff, diff.begin())
    );
    return diff;
}

// 新增的方法
std::set<int> CircuitOptimizer::Term::getDifferenceFromSet(const std::set<int>& otherSet) const {
    std::set<int> diff;
    std::set_difference(
            variables.begin(), variables.end(),
            otherSet.begin(), otherSet.end(),
            std::inserter(diff, diff.begin())
    );
    return diff;
}

// OptimizedPair 构造函数
CircuitOptimizer::OptimizedPair::OptimizedPair()
        : type(ONE_UNIQUE_FACTOR), highDegreeTerm(nullptr), lowDegreeTerm(nullptr) {
}

CircuitOptimizer::OptimizedPair::OptimizedPair(
        const std::shared_ptr<Term>& high,
        const std::shared_ptr<Term>& low,
        const std::set<int>& common,
        const std::set<int>& unique)
        : type(ONE_UNIQUE_FACTOR), highDegreeTerm(high), lowDegreeTerm(low),
          commonVars(common), uniqueVars(unique) {
}

// 修改两个unique factors的构造函数，包含低次项
CircuitOptimizer::OptimizedPair::OptimizedPair(
        const std::shared_ptr<Term>& high,
        const std::vector<std::shared_ptr<Term>>& middle,
        const std::shared_ptr<Term>& low,
        const std::set<int>& common,
        const std::set<int>& unique)
        : type(TWO_UNIQUE_FACTORS), highDegreeTerm(high), lowDegreeTerm(low), middleTerms(middle),
          commonVars(common), uniqueVars(unique) {
}


// 分组函数保持不变
std::map<int, std::vector<std::shared_ptr<CircuitOptimizer::Term>>>
CircuitOptimizer::groupByDegree(const std::string& polynomial) {
    std::map<int, std::vector<std::shared_ptr<Term>>> groupedTerms;
    std::stringstream ss(polynomial);
    std::string termStr;

    while (std::getline(ss, termStr, '+')) {
        termStr = cleanTerm(termStr);
        if (!termStr.empty()) {
            auto term = std::make_shared<Term>(termStr);
            groupedTerms[term->degree].push_back(term);
        }
    }
    return groupedTerms;
}

// 一个unique factor的优化函数
std::vector<CircuitOptimizer::OptimizedPair>
CircuitOptimizer::optimizeOneUniqueFactor(
        std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
        int highDegree, int lowDegree, int requiredCommonVars) {

    // 移除 clearCircuitNodes() 调用，保持电路节点的积累
    std::vector<OptimizedPair> allPairs;

    std::cout << "Optimizing ONE unique factor pairs (" << highDegree
              << "-degree + " << lowDegree << "-degree):" << std::endl;
    std::cout << "============================================" << std::endl;

    while (true) {
        auto pair = findOneUniqueFactorPair(groupedTerms, highDegree, lowDegree, requiredCommonVars);
        if (!pair.highDegreeTerm) {
            break;
        }

        pair.highDegreeTerm->used = true;
        pair.lowDegreeTerm->used = true;
        generateCircuitForOneUniqueFactor(pair);
        allPairs.push_back(pair);

        std::cout << "Found ONE-unique-factor pair: " << pair.highDegreeTerm->expression
                  << " + " << pair.lowDegreeTerm->expression << std::endl;
    }

    return allPairs;
}

// 两个unique factors的优化函数
// 两个unique factors的优化函数 - 修复版本
std::vector<CircuitOptimizer::OptimizedPair>
CircuitOptimizer::optimizeTwoUniqueFactors(
        std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
        int highDegree, int middleDegree, int lowDegree, int requiredCommonVars) {

    std::vector<OptimizedPair> allPairs;

    std::cout << "Optimizing TWO unique factors pairs (" << highDegree
              << "-degree + 2*" << middleDegree << "-degree + "
              << lowDegree << "-degree):" << std::endl;
    std::cout << "==================================================" << std::endl;

    while (true) {
        auto pair = findTwoUniqueFactorsPair(groupedTerms, highDegree, middleDegree, lowDegree, requiredCommonVars);
        if (!pair.highDegreeTerm) {
            break;
        }

        // 标记所有使用的项
        pair.highDegreeTerm->used = true;  // 高次项
        for (auto& middleTerm : pair.middleTerms) {
            if (middleTerm) middleTerm->used = true;  // 中间项
        }
        if (pair.lowDegreeTerm) {
            pair.lowDegreeTerm->used = true;  // 低次项 - 新增这行！
        }

        generateCircuitForTwoUniqueFactors(pair);
        allPairs.push_back(pair);

        std::cout << "Found TWO-unique-factors pair: " << pair.highDegreeTerm->expression
                  << " + 2 middle terms + " << (pair.lowDegreeTerm ? pair.lowDegreeTerm->expression : "null")
                  << std::endl;
    }

    return allPairs;
}
// 一个unique factor的配对查找
CircuitOptimizer::OptimizedPair
CircuitOptimizer::findOneUniqueFactorPair(
        std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
        int highDegree, int lowDegree, int requiredCommonVars) {

    if (groupedTerms.find(highDegree) == groupedTerms.end() ||
        groupedTerms.find(lowDegree) == groupedTerms.end()) {
        return OptimizedPair(nullptr, nullptr, {}, {});  // 这个保持不变，使用一个unique factor的构造函数
    }

    auto& highDegreeTerms = groupedTerms[highDegree];
    auto& lowDegreeTerms = groupedTerms[lowDegree];

    for (auto& highTerm : highDegreeTerms) {
        if (highTerm->used) continue;

        for (auto& lowTerm : lowDegreeTerms) {
            if (lowTerm->used) continue;

            std::set<int> commonVars;
            if (highTerm->hasCommonFactor(*lowTerm, commonVars)) {
                std::set<int> uniqueVars = highTerm->getDifference(*lowTerm);
                if (uniqueVars.size() >= 1 && uniqueVars.size() <= 2 &&
                    commonVars.size() == requiredCommonVars) {
                    return OptimizedPair(highTerm, lowTerm, commonVars, uniqueVars);  // 这个保持不变
                }
            }
        }
    }

    return OptimizedPair(nullptr, nullptr, {}, {});  // 这个保持不变
}

// 两个unique factors的配对查找
// 两个unique factors的配对查找 - 修复版本
CircuitOptimizer::OptimizedPair
CircuitOptimizer::findTwoUniqueFactorsPair(
        std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
        int highDegree, int middleDegree, int lowDegree, int requiredCommonVars) {

    if (groupedTerms.find(highDegree) == groupedTerms.end() ||
        groupedTerms.find(middleDegree) == groupedTerms.end() ||
        groupedTerms.find(lowDegree) == groupedTerms.end()) {
        return OptimizedPair(nullptr, std::vector<std::shared_ptr<Term>>(), nullptr, {}, {});  // 修改这里
    }

    auto& highDegreeTerms = groupedTerms[highDegree];
    auto& middleDegreeTerms = groupedTerms[middleDegree];
    auto& lowDegreeTerms = groupedTerms[lowDegree];

    for (auto& highTerm : highDegreeTerms) {
        if (highTerm->used) continue;

        for (auto& lowTerm : lowDegreeTerms) {
            if (lowTerm->used) continue;

            std::set<int> commonVars;
            if (highTerm->hasCommonFactor(*lowTerm, commonVars)) {
                if (commonVars.size() == requiredCommonVars) {
                    std::set<int> uniqueVars = highTerm->getDifference(*lowTerm);

                    if (uniqueVars.size() == 2) {
                        std::vector<std::shared_ptr<Term>> foundMiddleTerms;

                        for (auto& middleTerm : middleDegreeTerms) {
                            if (middleTerm->used) continue;

                            std::set<int> middleCommonVars;
                            if (middleTerm->hasCommonFactor(*lowTerm, middleCommonVars)) {
                                if (middleCommonVars.size() == commonVars.size()) {
                                    std::set<int> middleUniqueVars = middleTerm->getDifference(*lowTerm);
                                    if (middleUniqueVars.size() == 1 &&
                                        uniqueVars.find(*middleUniqueVars.begin()) != uniqueVars.end()) {
                                        foundMiddleTerms.push_back(middleTerm);
                                    }
                                }
                            }
                        }

                        if (foundMiddleTerms.size() == 2) {
                            return OptimizedPair(highTerm, foundMiddleTerms, lowTerm, commonVars, uniqueVars);  // 修改这里
                        }
                    }
                }
            }
        }
    }

    return OptimizedPair(nullptr, std::vector<std::shared_ptr<Term>>(), nullptr, {}, {});  // 修改这里
}

// 特殊的两个unique factors优化
std::vector<CircuitOptimizer::OptimizedPair>
CircuitOptimizer::optimizeSpecialTwoUniqueFactors(
        std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
        int highDegree, int middleDegree, int lowDegree, int requiredCommonVars,
        std::vector<std::string>& generatedQuadraticTerms) {

    std::vector<OptimizedPair> allPairs;

    std::cout << "Optimizing SPECIAL TWO unique factors pairs (" << highDegree
              << "-degree + 2*" << middleDegree << "-degree + virtual "
              << lowDegree << "-degree):" << std::endl;
    std::cout << "==========================================================" << std::endl;

    while (true) {
        auto pair = findSpecialTwoUniqueFactorsPair(groupedTerms, highDegree, middleDegree, lowDegree, requiredCommonVars);
        if (!pair.highDegreeTerm) {
            break;
        }

        pair.highDegreeTerm->used = true;
        for (auto& middleTerm : pair.middleTerms) {
            if (middleTerm) middleTerm->used = true;
        }

        // 生成电路并记录虚拟的二次项
        std::string quadraticTerm = generateCircuitForSpecialTwoUniqueFactors(pair);
        generatedQuadraticTerms.push_back(quadraticTerm);

        allPairs.push_back(pair);

        std::cout << "Found SPECIAL TWO-unique-factors pair: " << pair.highDegreeTerm->expression
                  << " | middle: " << (pair.middleTerms.size() > 0 ? pair.middleTerms[0]->expression : "NA")
                  << ", "        << (pair.middleTerms.size() > 1 ? pair.middleTerms[1]->expression : "NA")
                  << " -> virtual quadratic: " << quadraticTerm << std::endl;
    }

    return allPairs;
}

// 替换 CircuitOptimizer.cpp 中现有的 findSpecialTwoUniqueFactorsPair 实现
CircuitOptimizer::OptimizedPair
CircuitOptimizer::findSpecialTwoUniqueFactorsPair(
        std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
        int highDegree, int middleDegree, int /*lowDegree*/, int requiredCommonVars) {

    // 只需要4次项和3次项
    if (groupedTerms.find(highDegree) == groupedTerms.end() ||
        groupedTerms.find(middleDegree) == groupedTerms.end()) {
        return OptimizedPair(nullptr, std::vector<std::shared_ptr<Term>>(), nullptr, {}, {});
    }

    auto& highDegreeTerms   = groupedTerms[highDegree];    // 期望：4次项组
    auto& middleDegreeTerms = groupedTerms[middleDegree];  // 期望：3次项组

    // 仅支持 requiredCommonVars == 2 的场景（你的设计就是公共2比特）
    if (requiredCommonVars != 2) {
        return OptimizedPair(nullptr, std::vector<std::shared_ptr<Term>>(), nullptr, {}, {});
    }

    // 遍历每一个4次项
    for (auto& highTerm : highDegreeTerms) {
        if (highTerm->used) continue;
        if (highTerm->variables.size() != static_cast<size_t>(highDegree)) continue; // 稳妥起见

        // 把4次项变量拷出来，准备在内部枚举“公共2变量”C
        std::vector<int> hv(highTerm->variables.begin(), highTerm->variables.end());

        // 枚举 C = {a,b} (a<b)，unique = highTerm \ C = {u1,u2}
        for (size_t i = 0; i < hv.size(); ++i) {
            for (size_t j = i + 1; j < hv.size(); ++j) {
                std::set<int> commonVars = { hv[i], hv[j] };
                if (static_cast<int>(commonVars.size()) != requiredCommonVars) continue;

                // 求 unique = high \ common
                std::set<int> uniqueVars = highTerm->getDifferenceFromSet(commonVars);
                if (uniqueVars.size() != 2) continue;

                // 目标三次因子集合：C ∪ {u1} 和 C ∪ {u2}
                auto it = uniqueVars.begin();
                int u1 = *it++;
                int u2 = *it;

                std::set<int> need1 = commonVars; need1.insert(u1);
                std::set<int> need2 = commonVars; need2.insert(u2);

                // 在 middleDegreeTerms 里找“变量集合恰好等于 need1/need2”的三次项（且未被使用）
                std::shared_ptr<Term> m1 = nullptr, m2 = nullptr;
                for (auto& mid : middleDegreeTerms) {
                    if (mid->used) continue;
                    if (mid->variables == need1) { m1 = mid; }
                    else if (mid->variables == need2) { m2 = mid; }
                    if (m1 && m2) break;
                }

                // 两个都存在，才算一个合法的 special pair
                if (m1 && m2) {
                    std::vector<std::shared_ptr<Term>> mids = { m1, m2 };
                    return OptimizedPair(highTerm, mids, /*low=*/nullptr, commonVars, uniqueVars);
                }
            }
        }
    }

    return OptimizedPair(nullptr, std::vector<std::shared_ptr<Term>>(), nullptr, {}, {});
}


// 特殊的电路生成函数（返回生成的虚拟二次项）
// 特殊的电路生成函数（返回生成的虚拟二次项）
// 特殊的电路生成函数（返回生成的虚拟二次项）（完整替换版：设置 outputGateName，正确记录最终深度/输出）
// 规则：只要求 4 次项 + 两个 3 次项同时存在；这里不检查“虚拟二次项”是否真实在原式中
std::string CircuitOptimizer::generateCircuitForSpecialTwoUniqueFactors(OptimizedPair& pair) {
    if (pair.uniqueVars.size() != 2) return "";
    // 明确类型给复用目录用
    pair.kind = PairKind::SPECIAL_TWO_UNIQUE_FACTORS;

    auto extractLastNode = [](const std::vector<std::string>& ops) -> std::string {
        for (int i = static_cast<int>(ops.size()) - 1; i >= 0; --i) {
            const std::string& s = ops[i];
            auto lb = s.rfind('[');
            auto rb = s.rfind(']');
            if (lb != std::string::npos && rb != std::string::npos && rb > lb + 1) {
                return s.substr(lb + 1, rb - lb - 1);
            }
        }
        return "";
    };

    auto uniqueIt = pair.uniqueVars.begin();
    int uniqueVar1 = *uniqueIt;
    int uniqueVar2 = *(++uniqueIt);

    // 深度0：两个NOT操作，对两个“unique”比特取反
    std::string notOp1 = "NOT(x" + std::to_string(uniqueVar1) + ")";
    std::string notResult1 = "(x" + std::to_string(uniqueVar1) + "+1)";
    std::string notNode1 = findOrCreateNode(notOp1, notResult1, 0);

    std::string notOp2 = "NOT(x" + std::to_string(uniqueVar2) + ")";
    std::string notResult2 = "(x" + std::to_string(uniqueVar2) + "+1)";
    std::string notNode2 = findOrCreateNode(notOp2, notResult2, 0);

    pair.circuitOperations.push_back("Depth 0: " + notOp1 + " -> " + notResult1 + " [" + notNode1 + "]");
    pair.circuitOperations.push_back("Depth 0: " + notOp2 + " -> " + notResult2 + " [" + notNode2 + "]");

    // 深度1：统一处理所有变量（公共变量 + 两个 NOT 结果）
    std::vector<std::string> depth1Inputs;

    // 添加公共变量（即 4 次项的“公共 2 比特”）
    std::vector<int> commonVars(pair.commonVars.begin(), pair.commonVars.end());
    for (int var : commonVars) {
        depth1Inputs.push_back("x" + std::to_string(var));
    }

    // 添加NOT结果
    depth1Inputs.push_back(notNode1);
    depth1Inputs.push_back(notNode2);

    // 深度1：两两AND操作
    std::vector<std::string> depth1Results;
    for (size_t i = 0; i < depth1Inputs.size(); i += 2) {
        if (i + 1 < depth1Inputs.size()) {
            std::string left = depth1Inputs[i];
            std::string right = depth1Inputs[i+1];
            std::string leftExpr, rightExpr;
            for (const auto& node : circuitNodes) {
                if (node.name == left) leftExpr = node.expression;
                if (node.name == right) rightExpr = node.expression;
            }
            if (leftExpr.empty()) leftExpr = left;
            if (rightExpr.empty()) rightExpr = right;

            std::string andOp = "AND(" + left + ", " + right + ")";
            std::string result = "(" + leftExpr + rightExpr + ")";
            std::string nodeName = findOrCreateNode(andOp, result, 1);
            pair.circuitOperations.push_back("Depth 1: " + andOp + " -> " + result + " [" + nodeName + "]");
            depth1Results.push_back(nodeName);
        } else {
            depth1Results.push_back(depth1Inputs[i]);
        }
    }

    // 继续组合并记录最终深度
    int finalDepth = combineOperationsWithReuse(pair, depth1Results, 2);

    // === 关键：抽取最终门名 ===
    std::string finalNode = extractLastNode(pair.circuitOperations);
    pair.outputGateName = finalNode;

    // 记录深度信息
    PairDepthInfo info;
    info.pairIndex = static_cast<int>(pairDepthInfo.size()) + 1;
    info.finalDepth = finalDepth;
    info.finalOutput = finalNode;
    info.pairType = "SPECIAL_TWO_UNIQUE_FACTORS";
    info.expression.clear();
    info.isSingleTerm = false;
    pairDepthInfo.push_back(info);

    // 生成“虚拟二次项”：由公共比特拼出，例如 C={a,b} -> "xaxb"
    std::string virtualQuadratic;
    for (int var : pair.commonVars) {
        virtualQuadratic += "x" + std::to_string(var);
    }
    return virtualQuadratic;
}


// 获取二次项异或结果
std::string CircuitOptimizer::getQuadraticXORResult(
        const std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
        const std::vector<std::string>& generatedQuadraticTerms) {

    // 获取原始多项式中的二次项
    std::vector<std::string> originalQuadraticTerms;
    if (groupedTerms.find(2) != groupedTerms.end()) {
        for (const auto& term : groupedTerms.at(2)) {
            if (!term->used) {
                originalQuadraticTerms.push_back(term->expression);
            }
        }
    }

    // 合并所有项（生成的 + 原始的）
    std::vector<std::string> allTerms;
    for (const auto& term : generatedQuadraticTerms) {
        allTerms.push_back(term);
    }
    for (const auto& term : originalQuadraticTerms) {
        allTerms.push_back(term);
    }

    // 在二元域上消去重复项
    std::map<std::string, int> termCount;  // 记录每个项出现的次数

    for (const auto& term : allTerms) {
        termCount[term]++;  // 统计每个项出现的次数
    }

    // 只保留出现奇数次的项（在二元域中，偶数次会消去）
    std::vector<std::string> resultTerms;
    for (const auto& pair : termCount) {
        if (pair.second % 2 == 1) {  // 只保留奇数次出现的项
            resultTerms.push_back(pair.first);
        }
    }

    // 构建结果字符串
    std::string result;
    for (size_t i = 0; i < resultTerms.size(); i++) {
        if (i > 0) result += " + ";
        result += resultTerms[i];
    }

    // 如果没有剩余项，返回"0"
    if (result.empty()) {
        result = "0";
    }

    return result;
}

// 生成一个unique factor的电路
// 修改生成电路函数，记录最终深度
// 生成一个unique factor的电路（完整替换版：设置 outputGateName，正确记录最终深度/输出）
void CircuitOptimizer::generateCircuitForOneUniqueFactor(OptimizedPair& pair) {
    if (pair.uniqueVars.empty()) return;
    // 明确类型给复用目录用（rememberPairForReuse 依赖 pair.kind）
    pair.kind = PairKind::ONE_UNIQUE_FACTOR;

    auto extractLastNode = [](const std::vector<std::string>& ops) -> std::string {
        for (int i = static_cast<int>(ops.size()) - 1; i >= 0; --i) {
            const std::string& s = ops[i];
            auto lb = s.rfind('[');
            auto rb = s.rfind(']');
            if (lb != std::string::npos && rb != std::string::npos && rb > lb + 1) {
                return s.substr(lb + 1, rb - lb - 1);
            }
        }
        return "";
    };

    int uniqueVar = *pair.uniqueVars.begin();

    // 深度0：NOT操作
    std::string notOp = "NOT(x" + std::to_string(uniqueVar) + ")";
    std::string notResult = "(x" + std::to_string(uniqueVar) + "+1)";
    std::string notNode = findOrCreateNode(notOp, notResult, 0);
    pair.circuitOperations.push_back("Depth 0: " + notOp + " -> " + notResult + " [" + notNode + "]");

    // 深度1：统一处理所有变量
    std::vector<std::string> depth1Inputs;
    std::vector<int> commonVars(pair.commonVars.begin(), pair.commonVars.end());
    for (int var : commonVars) {
        depth1Inputs.push_back("x" + std::to_string(var));
    }
    depth1Inputs.push_back(notNode);

    // 深度1：两两AND操作
    std::vector<std::string> depth1Results;
    for (size_t i = 0; i < depth1Inputs.size(); i += 2) {
        if (i + 1 < depth1Inputs.size()) {
            std::string left = depth1Inputs[i];
            std::string right = depth1Inputs[i+1];
            std::string leftExpr, rightExpr;
            for (const auto& node : circuitNodes) {
                if (node.name == left) leftExpr = node.expression;
                if (node.name == right) rightExpr = node.expression;
            }
            if (leftExpr.empty()) leftExpr = left;
            if (rightExpr.empty()) rightExpr = right;

            std::string andOp = "AND(" + left + ", " + right + ")";
            std::string result = "(" + leftExpr + rightExpr + ")";
            std::string nodeName = findOrCreateNode(andOp, result, 1);
            pair.circuitOperations.push_back("Depth 1: " + andOp + " -> " + result + " [" + nodeName + "]");
            depth1Results.push_back(nodeName);
        } else {
            depth1Results.push_back(depth1Inputs[i]);
        }
    }

    // 继续组合并记录最终深度（AND 树从深度 2 开始）
    int finalDepth = combineOperationsWithReuse(pair, depth1Results, 2);

    // === 关键：从本函数产生的最后一条电路操作里抽取最终门名 ===
    std::string finalNode = extractLastNode(pair.circuitOperations);
    pair.outputGateName = finalNode;

    // 记录深度信息
    PairDepthInfo info;
    info.pairIndex = static_cast<int>(pairDepthInfo.size()) + 1;
    info.finalDepth = finalDepth;
    info.finalOutput = finalNode;
    info.pairType = "ONE_UNIQUE_FACTOR";
    info.expression.clear();
    info.isSingleTerm = false;
    pairDepthInfo.push_back(info);
}


// 生成两个unique factors的电路
// 同样修改 generateCircuitForTwoUniqueFactors 函数
// 生成两个 unique factors 的电路（完整替换版：设置 outputGateName，正确记录最终深度/输出）
void CircuitOptimizer::generateCircuitForTwoUniqueFactors(OptimizedPair& pair) {
    if (pair.uniqueVars.size() != 2) return;
    // 明确类型给复用目录用
    pair.kind = PairKind::TWO_UNIQUE_FACTORS;

    auto extractLastNode = [](const std::vector<std::string>& ops) -> std::string {
        for (int i = static_cast<int>(ops.size()) - 1; i >= 0; --i) {
            const std::string& s = ops[i];
            auto lb = s.rfind('[');
            auto rb = s.rfind(']');
            if (lb != std::string::npos && rb != std::string::npos && rb > lb + 1) {
                return s.substr(lb + 1, rb - lb - 1);
            }
        }
        return "";
    };

    auto uniqueIt = pair.uniqueVars.begin();
    int uniqueVar1 = *uniqueIt;
    int uniqueVar2 = *(++uniqueIt);

    // 深度0：两个NOT操作
    std::string notOp1 = "NOT(x" + std::to_string(uniqueVar1) + ")";
    std::string notResult1 = "(x" + std::to_string(uniqueVar1) + "+1)";
    std::string notNode1 = findOrCreateNode(notOp1, notResult1, 0);

    std::string notOp2 = "NOT(x" + std::to_string(uniqueVar2) + ")";
    std::string notResult2 = "(x" + std::to_string(uniqueVar2) + "+1)";
    std::string notNode2 = findOrCreateNode(notOp2, notResult2, 0);

    pair.circuitOperations.push_back("Depth 0: " + notOp1 + " -> " + notResult1 + " [" + notNode1 + "]");
    pair.circuitOperations.push_back("Depth 0: " + notOp2 + " -> " + notResult2 + " [" + notNode2 + "]");

    // 深度1：统一处理所有变量
    std::vector<std::string> depth1Inputs;
    std::vector<int> commonVars(pair.commonVars.begin(), pair.commonVars.end());
    for (int var : commonVars) {
        depth1Inputs.push_back("x" + std::to_string(var));
    }
    depth1Inputs.push_back(notNode1);
    depth1Inputs.push_back(notNode2);

    // 深度1：两两AND操作
    std::vector<std::string> depth1Results;
    for (size_t i = 0; i < depth1Inputs.size(); i += 2) {
        if (i + 1 < depth1Inputs.size()) {
            std::string left = depth1Inputs[i];
            std::string right = depth1Inputs[i+1];
            std::string leftExpr, rightExpr;
            for (const auto& node : circuitNodes) {
                if (node.name == left) leftExpr = node.expression;
                if (node.name == right) rightExpr = node.expression;
            }
            if (leftExpr.empty()) leftExpr = left;
            if (rightExpr.empty()) rightExpr = right;

            std::string andOp = "AND(" + left + ", " + right + ")";
            std::string result = "(" + leftExpr + rightExpr + ")";
            std::string nodeName = findOrCreateNode(andOp, result, 1);
            pair.circuitOperations.push_back("Depth 1: " + andOp + " -> " + result + " [" + nodeName + "]");
            depth1Results.push_back(nodeName);
        } else {
            depth1Results.push_back(depth1Inputs[i]);
        }
    }

    // 继续组合并记录最终深度
    int finalDepth = combineOperationsWithReuse(pair, depth1Results, 2);

    // === 关键：抽取最终门名 ===
    std::string finalNode = extractLastNode(pair.circuitOperations);
    pair.outputGateName = finalNode;

    // 记录深度信息
    PairDepthInfo info;
    info.pairIndex = static_cast<int>(pairDepthInfo.size()) + 1;
    info.finalDepth = finalDepth;
    info.finalOutput = finalNode;
    info.pairType = "TWO_UNIQUE_FACTORS";
    info.expression.clear();
    info.isSingleTerm = false;
    pairDepthInfo.push_back(info);
}



// 从字符串解析二次项
std::vector<std::string> CircuitOptimizer::parseQuadraticTerm(const std::string& quadraticTerm) {
    std::vector<std::string> variables;
    std::string currentVar;

    for (char c : quadraticTerm) {
        if (c == 'x') {
            if (!currentVar.empty()) {
                variables.push_back(currentVar);
                currentVar.clear();
            }
            currentVar += 'x';
        } else if (isdigit(c)) {
            currentVar += c;
        }
    }

    if (!currentVar.empty()) {
        variables.push_back(currentVar);
    }

    return variables;
}


// 生成剩余项的电路
// 修改 generateRemainingTermsCircuit 函数，打印所有实现（包括已存在的）
// 在 generateRemainingTermsCircuit 函数中为每个单项式记录深度信息
void CircuitOptimizer::generateRemainingTermsCircuit(
        const std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
        const std::vector<std::string>& quadraticTerms) {

    std::cout << "\nGenerating circuit for remaining terms:" << std::endl;
    std::cout << "========================================" << std::endl;

    // 处理二次项 XOR 结果中的每个单项式
    std::cout << "\nQuadratic Terms XOR Result implementation:" << std::endl;
    std::cout << "------------------------------------------" << std::endl;

    for (const auto& quadraticTerm : quadraticTerms) {
        if (quadraticTerm.empty() || quadraticTerm == "0") continue;

        auto variables = parseQuadraticTerm(quadraticTerm);
        if (variables.size() == 2) {
            std::string andOp = "AND(" + variables[0] + ", " + variables[1] + ")";
            std::string result = "(" + variables[0] + variables[1] + ")";

            bool exists = false;
            int existingDepth = -1;
            std::string existingNode;
            std::string finalOutput;

            for (const auto& node : circuitNodes) {
                if (node.operation == andOp) {
                    exists = true;
                    existingDepth = node.depth;
                    existingNode = node.name;
                    finalOutput = node.name;
                    break;
                }
            }

            if (exists) {
                std::cout << "Depth " << existingDepth << ": " + andOp + " -> " + result + " [" + existingNode + "] (reused)" << std::endl;

                // 记录深度信息（即使是重用的也记录）
                PairDepthInfo info;
                info.pairIndex = pairDepthInfo.size() + 1;
                info.finalDepth = existingDepth;
                info.finalOutput = finalOutput;
                info.pairType = "QUADRATIC_TERM";
                info.expression = quadraticTerm;
                info.isSingleTerm = true;
                pairDepthInfo.push_back(info);
            } else {
                std::string nodeName = findOrCreateNode(andOp, result, 1);
                std::cout << "Depth 1: " + andOp + " -> " + result + " [" + nodeName + "]" << std::endl;

                // 记录深度信息
                PairDepthInfo info;
                info.pairIndex = pairDepthInfo.size() + 1;
                info.finalDepth = 1;
                info.finalOutput = nodeName;
                info.pairType = "QUADRATIC_TERM";
                info.expression = quadraticTerm;
                info.isSingleTerm = true;
                pairDepthInfo.push_back(info);
            }
        }
    }

    // 处理高次剩余项（Degree 3, 4, 5, 6）
    std::vector<int> degreesToProcess = {3, 4, 5, 6};

    for (int degree : degreesToProcess) {
        if (groupedTerms.find(degree) != groupedTerms.end()) {
            auto& terms = groupedTerms.at(degree);
            std::vector<std::shared_ptr<Term>> remainingTerms;

            // 收集未使用的项
            for (const auto& term : terms) {
                if (!term->used) {
                    remainingTerms.push_back(term);
                }
            }

            if (!remainingTerms.empty()) {
                std::cout << "\nDegree " << degree << " remaining terms implementation:" << std::endl;
                std::cout << "--------------------------------------" << std::endl;

                for (const auto& term : remainingTerms) {
                    std::cout << "Implementing: " << term->expression << std::endl;

                    // 将变量排序以确保一致性
                    std::vector<int> vars(term->variables.begin(), term->variables.end());
                    std::sort(vars.begin(), vars.end());

                    // 构建变量名列表
                    std::vector<std::string> varNames;
                    for (int var : vars) {
                        varNames.push_back("x" + std::to_string(var));
                    }

                    // 分层实现AND操作
                    std::vector<std::string> currentLevel = varNames;
                    int depth = 1;
                    std::string finalOutput;

                    while (currentLevel.size() > 1) {
                        std::vector<std::string> nextLevel;

                        for (size_t i = 0; i < currentLevel.size(); i += 2) {
                            if (i + 1 < currentLevel.size()) {
                                std::string left = currentLevel[i];
                                std::string right = currentLevel[i + 1];

                                // 获取表达式
                                std::string leftExpr, rightExpr;
                                for (const auto& node : circuitNodes) {
                                    if (node.name == left) leftExpr = node.expression;
                                    if (node.name == right) rightExpr = node.expression;
                                }
                                if (leftExpr.empty()) leftExpr = left;
                                if (rightExpr.empty()) rightExpr = right;

                                std::string andOp = "AND(" + left + ", " + right + ")";
                                std::string result = "(" + leftExpr + rightExpr + ")";

                                // 检查是否已存在
                                bool exists = false;
                                int existingDepth = -1;
                                std::string existingNode;

                                for (const auto& node : circuitNodes) {
                                    if (node.operation == andOp) {
                                        exists = true;
                                        existingDepth = node.depth;
                                        existingNode = node.name;
                                        break;
                                    }
                                }

                                if (exists) {
                                    std::cout << "Depth " << existingDepth << ": " + andOp + " -> " + result + " [" + existingNode + "] (reused)" << std::endl;
                                    nextLevel.push_back(existingNode);
                                    finalOutput = existingNode;
                                } else {
                                    std::string nodeName = findOrCreateNode(andOp, result, depth);
                                    std::cout << "Depth " << depth << ": " + andOp + " -> " + result + " [" + nodeName + "]" << std::endl;
                                    nextLevel.push_back(nodeName);
                                    finalOutput = nodeName;
                                }
                            } else {
                                nextLevel.push_back(currentLevel[i]);
                                finalOutput = currentLevel[i];
                            }
                        }

                        currentLevel = nextLevel;
                        depth++;
                    }

                    // 记录最终输出
                    if (currentLevel.size() == 1) {
                        finalOutput = currentLevel[0];
                        std::cout << "Final result for " << term->expression << ": " << finalOutput << std::endl;

                        // 记录深度信息
                        PairDepthInfo info;
                        info.pairIndex = pairDepthInfo.size() + 1;
                        info.finalDepth = depth - 1; // 因为depth在循环后已经+1了
                        info.finalOutput = finalOutput;
                        info.pairType = "DEGREE_" + std::to_string(degree) + "_TERM";
                        info.expression = term->expression;
                        info.isSingleTerm = true;
                        pairDepthInfo.push_back(info);
                    }
                }
            }
        }
    }

    // 处理一次项（Degree 1） - 直接使用变量本身
    if (groupedTerms.find(1) != groupedTerms.end()) {
        auto& terms = groupedTerms.at(1);
        std::vector<std::shared_ptr<Term>> remainingTerms;

        for (const auto& term : terms) {
            if (!term->used) {
                remainingTerms.push_back(term);
            }
        }

        if (!remainingTerms.empty()) {
            std::cout << "\nDegree 1 remaining terms (direct variables):" << std::endl;
            std::cout << "--------------------------------------------" << std::endl;

            for (const auto& term : remainingTerms) {
                std::cout << term->expression << " (already available as input)" << std::endl;

                // 记录深度信息（深度为0，因为是输入变量）
                PairDepthInfo info;
                info.pairIndex = pairDepthInfo.size() + 1;
                info.finalDepth = 0;
                info.finalOutput = term->expression; // 直接使用变量名
                info.pairType = "DEGREE_1_TERM";
                info.expression = term->expression;
                info.isSingleTerm = true;
                pairDepthInfo.push_back(info);
            }
        }
    }

    // ===== 新增：处理零次项（常数 1）—— 计入 Depth 0 =====
    // 有些布尔函数（如 poly3/4/6/7）带有常数 1；将其作为深度 0 的单项式输出记录，便于统计“Initial items by depth”
    if (groupedTerms.find(0) != groupedTerms.end()) {
        const auto& terms0 = groupedTerms.at(0);
        bool recorded = false;
        for (const auto& t : terms0) {
            // 只在表达式字面是 "1" 且尚未标记 used 时登记一次
            if (!t->used && t->expression == "1") {
                std::cout << "\nDegree 0 constant implementation:" << std::endl;
                std::cout << "---------------------------------" << std::endl;
                std::cout << "Constant 1 is available as logic constant at Depth 0." << std::endl;

                PairDepthInfo info;
                info.pairIndex = static_cast<int>(pairDepthInfo.size()) + 1;
                info.finalDepth = 0;
                info.finalOutput = "1";        // 逻辑常量 1
                info.pairType = "CONST_1";
                info.expression = "1";
                info.isSingleTerm = true;
                pairDepthInfo.push_back(info);

                // 如需避免在后续流程被误当作“未用项”，可标记：
                // t->used = true;
                recorded = true;
                break; // 常数项只记一次
            }
        }
        if (!recorded) {
            // 如果 parse/group 的零次项不是字面 "1"（极少见），这里就不额外登记
        }
    }
}


// 把 "x3x1x2" 变为 "x1x2x3"，保证可比性
std::string CircuitOptimizer::canonicalExpr(const std::string& expr) {
    std::vector<int> vs;
    for (size_t i = 0; i < expr.size();) {
        if (expr[i] == 'x') {
            ++i;
            int v = 0;
            while (i < expr.size() && isdigit(static_cast<unsigned char>(expr[i]))) {
                v = v * 10 + (expr[i] - '0');
                ++i;
            }
            vs.push_back(v);
        } else {
            ++i;
        }
    }
    std::sort(vs.begin(), vs.end());
    std::string out;
    for (int v : vs) out += "x" + std::to_string(v);
    return out;
}


// 生成 XOR 的无序键：min(exprA,exprB) + "&" + max(exprA,exprB)
std::string CircuitOptimizer::makeXorKey(const std::string& exprA, const std::string& exprB) {
    std::string a = canonicalExpr(exprA);
    std::string b = canonicalExpr(exprB);
    if (b < a) std::swap(a, b);
    return a + "&" + b;
}

bool CircuitOptimizer::getNodeExpr(const std::string& nodeName, std::string& outExpr, int* outDepth) const {
    for (const auto& n : circuitNodes) {
        if (n.name == nodeName) {
            outExpr = n.expression;
            if (outDepth) *outDepth = n.depth;
            return true;
        }
    }
    // 可能是输入变量 "xk"
    if (!nodeName.empty() && nodeName[0] == 'x') {
        outExpr = nodeName;
        if (outDepth) *outDepth = 0;
        return true;
    }
    return false;
}


void CircuitOptimizer::rememberXorMerge(int depth, const std::string& leftNode,
                                        const std::string& rightNode, const std::string& outNode) {
    std::string aExpr, bExpr;
    int dL = -1, dR = -1;
    if (!getNodeExpr(leftNode, aExpr, &dL))  return;
    if (!getNodeExpr(rightNode, bExpr, &dR)) return;

    // 键 = depth + '|' + makeXorKey(A,B)
    std::string key = std::to_string(depth) + "|" + makeXorKey(aExpr, bExpr);
    ReusableXorMerge rec;
    rec.depth   = depth;
    rec.inKey   = key;
    rec.inAExpr = canonicalExpr(aExpr);
    rec.inBExpr = canonicalExpr(bExpr);
    rec.outNode = outNode;
    xorCatalog_[key] = std::move(rec);
}


std::string CircuitOptimizer::tryReuseXorAtDepth(int depth, const std::string& leftExprRaw,
                                                 const std::string& rightExprRaw) const {
    std::string key = std::to_string(depth) + "|" + makeXorKey(leftExprRaw, rightExprRaw);
    auto it = xorCatalog_.find(key);
    if (it == xorCatalog_.end()) return "";        // 没记录：无法复用
    return it->second.outNode;                     // 直接返回第一轮该 XOR 的输出门名 tXX
}

void CircuitOptimizer::clearXorCatalog() { xorCatalog_.clear(); } // 一般不用




int CircuitOptimizer::degreeOfExpr(const std::string& expr) {
    int cnt = 0;
    for (size_t i=0;i<expr.size();++i) if (expr[i]=='x') ++cnt;
    return cnt;
}

// THREE kinds of signatures (固定字段顺序，全部 canonical 之后再拼)
std::string CircuitOptimizer::makeSignatureTwoUnique(const std::string& high,
                                                     const std::string& m1,
                                                     const std::string& m2,
                                                     const std::string& low) {
    std::string cH = canonicalExpr(high);
    std::string cM1 = canonicalExpr(m1);
    std::string cM2 = canonicalExpr(m2);
    if (cM2 < cM1) std::swap(cM1, cM2); // middle 两个无序
    std::string cL = canonicalExpr(low);
    return "two|H:" + cH + "|M:" + cM1 + "," + cM2 + "|L:" + cL;
}

std::string CircuitOptimizer::makeSignatureOneUnique(const std::string& high,
                                                     const std::string& low) {
    std::string cH = canonicalExpr(high);
    std::string cL = canonicalExpr(low);
    return "one|H:" + cH + "|L:" + cL;
}

// special: 只要求 (4次项) 与 两个(3次项因子) 同时存在；2次项是否存在不检查
std::string CircuitOptimizer::makeSignatureSpecialTwo(const std::string& q4,
                                                      const std::string& c31,
                                                      const std::string& c32) {
    std::string cQ4 = canonicalExpr(q4);
    std::string c31c = canonicalExpr(c31);
    std::string c32c = canonicalExpr(c32);
    if (c32c < c31c) std::swap(c31c, c32c);
    return "special|Q4:" + cQ4 + "|C3:" + c31c + "," + c32c;
}



// 其余辅助函数保持不变
void CircuitOptimizer::clearCircuitNodes() {
    circuitNodes.clear();
    nodeCounter = 0;
}

std::string CircuitOptimizer::findOrCreateNode(const std::string& operation,
                                               const std::string& expression,
                                               int depth) {
    for (const auto& node : circuitNodes) {
        if (node.operation == operation) {
            return node.name;
        }
    }
    std::string nodeName = "t" + std::to_string(nodeCounter++);
    circuitNodes.emplace_back(nodeName, operation, expression, depth);
    return nodeName;
}

void CircuitOptimizer::printCircuitNodes() const {
    std::cout << "\nCircuit Nodes (Reusable):" << std::endl;
    std::cout << "=========================" << std::endl;
    for (const auto& node : circuitNodes) {
        std::cout << node.name << " = " << node.operation
                  << " -> " << node.expression
                  << " (Depth " << node.depth << ")" << std::endl;
    }
}

// 修改 combineOperationsWithReuse 实现，返回最终深度
int CircuitOptimizer::combineOperationsWithReuse(OptimizedPair& pair,
                                                 std::vector<std::string>& inputs,
                                                 int depth) {
    if (inputs.size() <= 1) {
        // 当只剩下一个节点时，返回当前深度-1（因为这是最终结果）
        return depth - 1;
    }

    std::vector<std::string> nextLevel;

    for (size_t i = 0; i < inputs.size(); i += 2) {
        if (i + 1 < inputs.size()) {
            std::string leftExpr, rightExpr;
            for (const auto& node : circuitNodes) {
                if (node.name == inputs[i]) leftExpr = node.expression;
                if (node.name == inputs[i+1]) rightExpr = node.expression;
            }
            std::string andOp = "AND(" + inputs[i] + ", " + inputs[i+1] + ")";
            std::string result = "(" + leftExpr + rightExpr + ")";
            std::string nodeName = findOrCreateNode(andOp, result, depth);
            pair.circuitOperations.push_back("Depth " + std::to_string(depth) +
                                             ": " + andOp + " -> " + result + " [" + nodeName + "]");
            nextLevel.push_back(nodeName);
        } else {
            nextLevel.push_back(inputs[i]);
        }
    }

    // 递归调用并返回最终深度
    return combineOperationsWithReuse(pair, nextLevel, depth + 1);
}


// 获取所有项的深度分类
std::map<int, std::vector<std::string>> CircuitOptimizer::getAllItemsByDepth() const {
    std::map<int, std::vector<std::string>> depthItems;

    for (const auto& info : pairDepthInfo) {
        depthItems[info.finalDepth].push_back(info.finalOutput);
    }

    return depthItems;
}

// 在指定深度合并一组项（使用XOR2）
std::string CircuitOptimizer::mergeItemsAtDepth(int depth, std::vector<std::string> items, int startDepth,
                                                const std::map<int, std::pair<std::string, int>>& availableResults) {
    if (items.empty()) return "";
    if (items.size() == 1) return items[0];

    std::vector<std::string> currentLevel = items;
    int currentDepth = startDepth;

    std::cout << "  Merging " << items.size() << " items from depth " << depth << " starting at depth " << startDepth << ":" << std::endl;

    while (currentLevel.size() > 1) {
        std::vector<std::string> nextLevel;

        // 在当前深度检查是否有新的可用结果可以添加
        for (const auto& result : availableResults) {
            int resultDepth = result.first;
            const auto& resultInfo = result.second;
            std::string resultNode = resultInfo.first;
            int completionDepth = resultInfo.second;

            // 如果这个结果在当前深度可用，并且还没有在当前层级中
            if (completionDepth + 1 == currentDepth &&
                std::find(currentLevel.begin(), currentLevel.end(), resultNode) == currentLevel.end()) {
                currentLevel.push_back(resultNode);
                std::cout << "    Depth " << currentDepth << ": Adding available result " << resultNode
                          << " from Processing Depth " << resultDepth << std::endl;
            }
        }

        for (size_t i = 0; i < currentLevel.size(); i += 2) {
            if (i + 1 < currentLevel.size()) {
                // XOR2操作
                std::string left = currentLevel[i];
                std::string right = currentLevel[i + 1];

                std::string leftExpr, rightExpr;
                for (const auto& node : circuitNodes) {
                    if (node.name == left) leftExpr = node.expression;
                    if (node.name == right) rightExpr = node.expression;
                }
                if (leftExpr.empty()) leftExpr = left;
                if (rightExpr.empty()) rightExpr = right;

                std::string xorOp = "XOR2(" + left + ", " + right + ")";
                std::string result = "(" + leftExpr + " + " + rightExpr + ")";

                // 检查是否已存在
                bool exists = false;
                std::string existingNode;
                for (const auto& node : circuitNodes) {
                    if (node.operation == xorOp) {
                        exists = true;
                        existingNode = node.name;
                        break;
                    }
                }

                if (exists) {
                    hierarchicalOperations.push_back("Depth " + std::to_string(currentDepth) +
                                                     ": " + xorOp + " -> " + result + " [" + existingNode + "] (reused)");
                    nextLevel.push_back(existingNode);
                    std::cout << "    Depth " << currentDepth << ": " << xorOp << " -> " + result + " [" + existingNode + "] (reused)" << std::endl;
                } else {
                    std::string nodeName = findOrCreateNode(xorOp, result, currentDepth);
                    hierarchicalOperations.push_back("Depth " + std::to_string(currentDepth) +
                                                     ": " + xorOp + " -> " + result + " [" + nodeName + "]");
                    nextLevel.push_back(nodeName);
                    std::cout << "    Depth " << currentDepth << ": " + xorOp + " -> " + result + " [" + nodeName + "]" << std::endl;
                }
            } else {
                // 剩余单个项传递到下一层
                nextLevel.push_back(currentLevel[i]);
                std::cout << "    Depth " << currentDepth << ": " + currentLevel[i] << " passed through" << std::endl;
            }
        }

        currentLevel = nextLevel;
        currentDepth++;
    }

    std::cout << "  Final merged result: " << currentLevel[0] << " at depth " << (currentDepth - 1) << std::endl;
    return currentLevel[0];
}


// 层次化合并所有项
// 层次化合并所有项（完全修正深度依赖版本）
// 层次化合并所有项（尽早参与版本）
// 层次化合并所有项（保持深度约束的尽早参与版本）
// 层次化合并所有项（修正可用深度版本）
// 层次化合并所有项（动态添加可用结果版本）
// 层次化合并所有项（只添加直接前一个深度的结果）
// 层次化合并所有项（在合并过程中动态检查可用性）
// 层次化合并所有项（自定义处理顺序）
// 层次化合并所有项（Depth2独立合并）
// 层次化合并所有项（带 XOR 复用第一函数记录的同深度同输入归并）
void CircuitOptimizer::hierarchicalMergeAllTerms() {
    hierarchicalOperations.clear();
    depthMergeResults.clear();

    std::cout << "\nHierarchical Merge of All Terms:" << std::endl;
    std::cout << "================================" << std::endl;

    // 1) 收集所有项：key = finalDepth, value = 本深度要参与 XOR 的节点名（xK 或 tNN）
    auto itemsByDepth = getAllItemsByDepth();

    // 打印初始统计
    std::cout << "Initial items by depth:" << std::endl;
    for (const auto& kv : itemsByDepth) {
        std::cout << "  Depth " << kv.first << ": " << kv.second.size() << " items" << std::endl;
    }

    // 2) 归并单层的工具：两两 XOR；优先复用（第一函数中记录的）同深度同输入的 XOR
    auto xorMergeLevelWithReuse = [&](std::vector<std::string>& items,
                                      int startDepth,
                                      std::vector<std::string>& outNextLevel,
                                      std::vector<std::string>& opLogs) {
        for (size_t i = 0; i < items.size(); i += 2) {
            if (i + 1 < items.size()) {
                const std::string& L = items[i];
                const std::string& R = items[i + 1];

                // 取两侧节点的可读表达式（如果是 tNN 则查节点表达式；如果是 xK 直接用）
                std::string Lexpr, Rexpr;
                getNodeExpr(L, Lexpr, nullptr);
                getNodeExpr(R, Rexpr, nullptr);
                if (Lexpr.empty()) Lexpr = L;
                if (Rexpr.empty()) Rexpr = R;

                // 先尝试复用：同“当前 XOR 深度（startDepth）” + 同一对输入表达式
                std::string reusedNode = tryReuseXorAtDepth(startDepth, Lexpr, Rexpr);
                if (!reusedNode.empty()) {
                    outNextLevel.push_back(reusedNode);
                    opLogs.push_back("Depth " + std::to_string(startDepth) + ": XOR2(" + L + ", " + R +
                                     ") -> (" + Lexpr + " + " + Rexpr + ") [" + reusedNode + "] (reused)");
                } else {
                    // 未命中：新建 XOR，并登记到复用目录
                    std::string xorOp = "XOR2(" + L + ", " + R + ")";
                    std::string result = "(" + Lexpr + " + " + Rexpr + ")";
                    std::string node   = findOrCreateNode(xorOp, result, startDepth);
                    outNextLevel.push_back(node);
                    opLogs.push_back("Depth " + std::to_string(startDepth) + ": " + xorOp + " -> " + result +
                                     " [" + node + "]");

                    // 记录本次 XOR：供后续（比如第二个/第三个函数）在“同深度同输入”场景下复用
                    rememberXorMerge(startDepth, L, R, node);
                }
            } else {
                // 奇数：最后一个直接上推
                outNextLevel.push_back(items[i]);
                std::string expr; getNodeExpr(items[i], expr, nullptr);
                if (expr.empty()) expr = items[i];
                opLogs.push_back("Depth " + std::to_string(startDepth) + ": " + items[i] + " passed through");
            }
        }
    };

    // 3) 逐深度进行层级合并
    //    处理顺序按深度从小到大（0 -> 1 -> 2 -> 3），这样打印与统计直观
    std::map<int, std::pair<std::string,int>> depthMerged; // depth -> (mergedNode, completionDepth)

    for (const auto& kv : itemsByDepth) {
        int depth = kv.first;
        const auto& items = kv.second;

        if (items.empty()) continue;

        // 对“来自 depth 的这些项”进行分层 XOR：
        // 约定：合并从 depth+1 开始（与之前输出风格一致）
        int startDepth = depth + 1;

        std::vector<std::string> current = items;
        std::vector<std::string> logs;

        std::cout << "\n=== Processing Depth " << depth << " ===" << std::endl;
        std::cout << "  Merging " << items.size() << " items starting at depth " << startDepth << ":" << std::endl;

        while (current.size() > 1) {
            std::vector<std::string> next;
            xorMergeLevelWithReuse(current, startDepth, next, logs);
            current.swap(next);
            ++startDepth;
        }

        // 打印过程日志
        for (auto& s : logs) std::cout << "    " << s << std::endl;

        // 汇总：该深度的合并结果节点与完成深度（最后一次新建/复用 XOR 的深度）
        std::string mergedNode   = current.front();
        int completionDepth      = startDepth - 1;

        depthMerged[depth]       = std::make_pair(mergedNode, completionDepth);
        depthMergeResults[depth] = mergedNode;

        std::cout << "  Merge completed at depth " << completionDepth << ": " << mergedNode << std::endl;
        std::cout << "  Result will be available at depth " << (completionDepth + 1) << std::endl;
    }

    // 4) 确定全局最终结果（选最后一个处理到的深度结果即可）
    if (!depthMerged.empty()) {
        auto last = depthMerged.rbegin();
        finalMergeResult = last->second.first;
        std::cout << "\nFinal result (from depth " << last->first
                  << ", completed at depth " << last->second.second << "): "
                  << finalMergeResult << std::endl;
    } else {
        finalMergeResult = "0";
    }
}



static std::string setToStr(const std::set<int>& s) {
    std::ostringstream os; os << "{";
    bool first = true;
    for (int v : s) { if(!first) os << ","; os << v; first=false; }
    os << "}";
    return os.str();
}
static std::string vecTermsToStr(const std::vector<std::shared_ptr<CircuitOptimizer::Term>>& v) {
    std::vector<std::string> bags;
    for (auto& t : v) bags.push_back(setToStr(t->variables));
    std::sort(bags.begin(), bags.end());
    std::ostringstream os;
    for (size_t i=0;i<bags.size();++i) {
        if (i) os << ",";
        os << bags[i];
    }
    return os.str();
}

std::string CircuitOptimizer::OptimizedPair::patternKey() const {
    std::ostringstream key;
    switch (kind) {
        case PairKind::TWO_UNIQUE_FACTORS: key << "TWO_UNIQUE|"; break;
        case PairKind::ONE_UNIQUE_FACTOR:  key << "ONE_UNIQUE|"; break;
        case PairKind::SPECIAL_TWO_UNIQUE_FACTORS: key << "SPECIAL_TWO_UNIQUE|"; break;
        default: key << "UNKNOWN|"; break;
    }
    key << "H:" << (highDegreeTerm ? setToStr(highDegreeTerm->variables) : "{}");
    if (!middleTerms.empty()) key << "|M:" << vecTermsToStr(middleTerms);
    if (lowDegreeTerm) key << "|L:" << setToStr(lowDegreeTerm->variables);
    if (!commonVars.empty()) key << "|C:" << setToStr(commonVars);
    if (!uniqueVars.empty()) key << "|U:" << setToStr(uniqueVars);
    return key.str();
}

void CircuitOptimizer::rememberPairForReuse(const OptimizedPair& pair) {
    ReusablePairRecord rec;
    rec.kind = pair.kind;                       // 记得你要在创建时给 pair.kind 赋值
    rec.outputGateName = pair.outputGateName;

    if (pair.kind == PairKind::TWO_UNIQUE_FACTORS) {
        // 组件：high + 两个 middle + 低次项 low
        if (!pair.highDegreeTerm || pair.middleTerms.size()<2 || !pair.lowDegreeTerm) return;
        rec.highExpr = canonicalExpr(pair.highDegreeTerm->expression);
        rec.middleExprs = { canonicalExpr(pair.middleTerms[0]->expression),
                            canonicalExpr(pair.middleTerms[1]->expression) };
        if (rec.middleExprs[1] < rec.middleExprs[0]) std::swap(rec.middleExprs[0], rec.middleExprs[1]);
        rec.lowExpr = canonicalExpr(pair.lowDegreeTerm->expression);
        rec.signature = makeSignatureTwoUnique(rec.highExpr, rec.middleExprs[0], rec.middleExprs[1], rec.lowExpr);
    } else if (pair.kind == PairKind::ONE_UNIQUE_FACTOR) {
        // 组件：high + 低次项（本模式只有一个 middle/low）
        if (!pair.highDegreeTerm || !pair.lowDegreeTerm) return;
        rec.highExpr = canonicalExpr(pair.highDegreeTerm->expression);
        rec.lowExpr  = canonicalExpr(pair.lowDegreeTerm->expression);
        rec.signature = makeSignatureOneUnique(rec.highExpr, rec.lowExpr);
    } else if (pair.kind == PairKind::SPECIAL_TWO_UNIQUE_FACTORS) {
        // 组件：4次项 + 两个3次项（不检查2次项是否存在）
        if (!pair.lowDegreeTerm || pair.middleTerms.size()<2) return;
        // 这里沿用你的存放方式：lowDegreeTerm 存 4次项；middleTerms 存两个 3次项
        rec.highExpr.clear();
        rec.lowExpr  = canonicalExpr(pair.lowDegreeTerm->expression); // 4次项
        rec.middleExprs = { canonicalExpr(pair.middleTerms[0]->expression),
                            canonicalExpr(pair.middleTerms[1]->expression) };
        if (rec.middleExprs[1] < rec.middleExprs[0]) std::swap(rec.middleExprs[0], rec.middleExprs[1]);
        rec.signature = makeSignatureSpecialTwo(rec.lowExpr, rec.middleExprs[0], rec.middleExprs[1]);
    } else {
        return;
    }

    // 录入字典（后写覆盖先写没关系，因为输出门相同）
    reuseCatalog_[rec.signature] = rec;
}

static std::shared_ptr<CircuitOptimizer::Term> findExactTerm(
        std::vector<std::shared_ptr<CircuitOptimizer::Term>>& bucket,
        const std::set<int>& need)
{
    for (auto& t : bucket) {
        if (!t->used && t->variables == need) return t;
    }
    return nullptr;
}




void CircuitOptimizer::pushDepthInfoSingle(const std::string& expr,
                                           const std::string& out,
                                           int finalDepth,
                                           const std::string& type) {
    PairDepthInfo info;
    info.pairIndex    = static_cast<int>(pairDepthInfo.size()) + 1;
    info.finalDepth   = finalDepth;
    info.finalOutput  = out;          // tNN 或者直接 xk
    info.pairType     = type;         // "DEGREE_1_TERM" / "QUADRATIC_TERM" / "DEGREE_3_TERM" / ...
    info.expression   = expr;         // 例如 "x0x1x2"、"x0x4x5x7"
    info.isSingleTerm = true;
    pairDepthInfo.push_back(std::move(info));
}

bool CircuitOptimizer::findTermByExpr(
        std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
        const std::string& expr,
        std::shared_ptr<Term>& outTerm) const
{
    std::string key = canonicalExpr(expr);

    // 通过统计 'x' 的个数估计次数（也可写一个解析器）
    int deg = 0; for (char c : expr) if (c == 'x') ++deg;

    auto it = groupedTerms.find(deg);
    if (it == groupedTerms.end()) return false;

    for (auto& t : it->second) {
        if (!t->used && canonicalExpr(t->expression) == key) {
            outTerm = t;
            return true;
        }
    }
    return false;
}

std::vector<CircuitOptimizer::OptimizedPair>
CircuitOptimizer::tryReusePairsForPattern(
        std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
        const std::vector<OptimizedPair>& referencePairs)
{
    std::vector<OptimizedPair> reused;

    for (const auto& ref : referencePairs) {
        // 这些 referencePairs 来自第一轮（已经有 kind / outputGateName / 各组成项）
        OptimizedPair p;
        p.kind = ref.kind;
        p.outputGateName = ref.outputGateName; // 直接复用这个 t_xx
        p.circuitOperations.clear();           // 复用无需重建流水

        bool ok = true;

        if (ref.kind == PairKind::TWO_UNIQUE_FACTORS) {
            // 需要：高次项 + 两个中间项 + 低次项 都存在
            if (!ref.highDegreeTerm || ref.middleTerms.size() < 2 || !ref.lowDegreeTerm) continue;

            std::shared_ptr<Term> th, tm1, tm2, tl;
            ok = ok && findTermByExpr(groupedTerms, ref.highDegreeTerm->expression, th);
            ok = ok && findTermByExpr(groupedTerms, ref.middleTerms[0]->expression, tm1);
            ok = ok && findTermByExpr(groupedTerms, ref.middleTerms[1]->expression, tm2);
            ok = ok && findTermByExpr(groupedTerms, ref.lowDegreeTerm->expression,  tl);
            if (!ok) continue;

            p.highDegreeTerm = th;
            p.middleTerms = { tm1, tm2 };
            p.lowDegreeTerm = tl;

        } else if (ref.kind == PairKind::ONE_UNIQUE_FACTOR) {
            // 需要：高次项 + 低次项
            if (!ref.highDegreeTerm || !ref.lowDegreeTerm) continue;

            std::shared_ptr<Term> th, tl;
            ok = ok && findTermByExpr(groupedTerms, ref.highDegreeTerm->expression, th);
            ok = ok && findTermByExpr(groupedTerms, ref.lowDegreeTerm->expression,  tl);
            if (!ok) continue;

            p.highDegreeTerm = th;
            p.lowDegreeTerm  = tl;

        } else if (ref.kind == PairKind::SPECIAL_TWO_UNIQUE_FACTORS) {
            // 需要：4次项 + 两个3次项（不检查“虚拟二次项”是否存在）
            if (!ref.lowDegreeTerm || ref.middleTerms.size() < 2) continue;

            std::shared_ptr<Term> tq4, tc31, tc32;
            ok = ok && findTermByExpr(groupedTerms, ref.lowDegreeTerm->expression,  tq4);   // 4次项
            ok = ok && findTermByExpr(groupedTerms, ref.middleTerms[0]->expression, tc31);  // 3次项
            ok = ok && findTermByExpr(groupedTerms, ref.middleTerms[1]->expression, tc32);  // 3次项
            if (!ok) continue;

            p.lowDegreeTerm = tq4;
            p.middleTerms   = { tc31, tc32 };
        } else {
            continue;
        }

        // 标记本轮这些项为已用，避免后面被“剩余项电路”再实现
        if (p.highDegreeTerm) p.highDegreeTerm->used = true;
        for (auto& m : p.middleTerms) if (m) m->used = true;
        if (p.lowDegreeTerm)  p.lowDegreeTerm->used  = true;

        // 记录这个“复用 pair”的深度（查它的输出节点深度）
        int depthFound = 0;
        if (!p.outputGateName.empty()) {
            for (const auto& node : circuitNodes) {
                if (node.name == p.outputGateName) { depthFound = node.depth; break; }
            }
        }

        // 把“复用”也计入本轮统计
        PairDepthInfo info;
        info.pairIndex    = static_cast<int>(pairDepthInfo.size()) + 1;
        info.finalDepth   = depthFound;
        info.finalOutput  = p.outputGateName;
        info.pairType     = (ref.kind == PairKind::TWO_UNIQUE_FACTORS) ? "TWO_UNIQUE_FACTORS" :
                            (ref.kind == PairKind::ONE_UNIQUE_FACTOR)  ? "ONE_UNIQUE_FACTOR"  :
                            "SPECIAL_TWO_UNIQUE_FACTORS";
        // 也可以把表达式拼成易读文本写进 expression，这里先留空
        info.expression   = "";
        info.isSingleTerm = false;
        pairDepthInfo.push_back(std::move(info));

        reused.push_back(std::move(p));
    }

    if (!reused.empty()) {
        std::cout << "\nReused " << reused.size()
                  << " pairs from the first function for this pattern." << std::endl;
        for (const auto& p : reused) {
            std::cout << "  -> reuse output: " << (p.outputGateName.empty()? "(unknown)" : p.outputGateName) << std::endl;
        }
    }

    return reused;
}




// 打印层次化合并结果
void CircuitOptimizer::printHierarchicalMergeResult() const {
    std::cout << "\nHierarchical Merge Operations:" << std::endl;
    std::cout << "==============================" << std::endl;

    for (const auto& op : hierarchicalOperations) {
        std::cout << op << std::endl;
    }

    std::cout << "\nDepth Merge Results:" << std::endl;
    std::cout << "===================" << std::endl;
    for (const auto& result : depthMergeResults) {
        std::cout << "Depth " << result.first << " merged result: " << result.second << std::endl;
    }

    std::cout << "\nFinal Overall Result: " << finalMergeResult << std::endl;
}



std::string CircuitOptimizer::cleanTerm(const std::string& term) {
    std::string cleaned;
    for (char c : term) {
        if (c != ' ') cleaned += c;
    }
    return cleaned;
}

void CircuitOptimizer::printRemainingTerms(
        const std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms) {
    std::cout << "\nRemaining unoptimized terms:" << std::endl;
    std::cout << "==============================" << std::endl;
    for (const auto& degreePair : groupedTerms) {
        std::vector<std::string> remaining;
        for (const auto& term : degreePair.second) {
            if (!term->used) remaining.push_back(term->expression);
        }
        if (!remaining.empty()) {
            std::cout << "Degree " << degreePair.first << " (" << remaining.size() << " terms):" << std::endl;
            for (const auto& expr : remaining) {
                std::cout << "  " << expr << std::endl;
            }
        }
    }
}

void CircuitOptimizer::printOptimizationStatistics(
        const std::vector<OptimizedPair>& pairs,
        const std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms) {
    std::cout << "\nOptimization Statistics:" << std::endl;
    std::cout << "========================" << std::endl;

    int totalTerms = 0;
    int optimizedTerms = 0;
    int remainingTerms = 0;

    for (const auto& pair : pairs) {
        if (pair.type == OptimizedPair::ONE_UNIQUE_FACTOR) {
            optimizedTerms += 2;
        } else if (pair.type == OptimizedPair::TWO_UNIQUE_FACTORS) {
            optimizedTerms += 3; // 高次项 + 2个中间项
        }
    }

    for (const auto& degreePair : groupedTerms) {
        for (const auto& term : degreePair.second) {
            totalTerms++;
            if (!term->used) remainingTerms++;
        }
    }

    std::cout << "Total terms: " << totalTerms << std::endl;
    std::cout << "Optimized terms: " << optimizedTerms << " (" << pairs.size() << " pairs)" << std::endl;
    std::cout << "Remaining terms: " << remainingTerms << std::endl;
    std::cout << "Optimization rate: " << std::fixed << std::setprecision(1)
              << (optimizedTerms * 100.0 / totalTerms) << "%" << std::endl;
}

// 把内部信号名统一成 Verilog 端口/导线名：x0->X0，t12->t12
static std::string toVerilogSignal(const std::string& s) {
    if (s.empty()) return s;
    if (s[0] == 'x' || s[0] == 'X') {
        // x0...x7 -> X0...X7
        return "X" + s.substr(1);
    }
    // tXX 直接返回
    return s;
}

// 解析 operation 里的两个操作数：形如 "AND(a, b)" / "XOR2(a, b)" / "NOT(a)"
static std::vector<std::string> parseOperands(const std::string& op) {
    std::vector<std::string> out;
    auto l = op.find('(');
    auto r = op.rfind(')');
    if (l == std::string::npos || r == std::string::npos || r <= l) return out;

    std::string inside = op.substr(l + 1, r - l - 1);
    // 拆分逗号
    size_t p = 0;
    while (p < inside.size()) {
        // 跳过空白
        while (p < inside.size() && isspace(static_cast<unsigned char>(inside[p]))) ++p;
        size_t q = p;
        while (q < inside.size() && inside[q] != ',') ++q;
        std::string token = inside.substr(p, q - p);
        // 去掉空白
        while (!token.empty() && isspace(static_cast<unsigned char>(token.front()))) token.erase(token.begin());
        while (!token.empty() && isspace(static_cast<unsigned char>(token.back())))  token.pop_back();
        if (!token.empty()) out.push_back(token);
        p = (q == inside.size()) ? q : q + 1;
    }
    return out;
}

void CircuitOptimizer::exportVerilog(const std::string& filepath,
                                     const std::string& moduleName) const
{
    std::ofstream ofs(filepath);
    if (!ofs) {
        std::cerr << "[exportVerilog] Cannot open file: " << filepath << std::endl;
        return;
    }

    // 先按 t 的编号排序，保证可读性：t0, t1, t2, ...
    std::vector<CircuitNode> nodes = circuitNodes;
    auto tIndex = [](const std::string& name)->int {
        // name 形如 t123
        if (name.size() >= 2 && (name[0]=='t' || name[0]=='T')) {
            return std::atoi(name.c_str() + 1);
        }
        return INT_MAX; // 非 t 开头的排后
    };
    std::sort(nodes.begin(), nodes.end(),
              [&](const CircuitNode& a, const CircuitNode& b) {
                  int ia = tIndex(a.name), ib = tIndex(b.name);
                  if (ia != ib) return ia < ib;
                  // 次序：再按 depth / name 保底
                  if (a.depth != b.depth) return a.depth < b.depth;
                  return a.name < b.name;
              });

    // 头部注释
    ofs << "// Auto-generated by CircuitOptimizer::exportVerilog\n";
    ofs << "// This netlist includes all gates (tXX) recorded so far.\n\n";

    // 模块头
    ofs << "module " << moduleName << "(\n";
    ofs << "    input  wire X0, X1, X2, X3, X4, X5, X6, X7,\n";
    ofs << "    output wire Y0, Y1, Y2, Y3, Y4, Y5, Y6, Y7\n";
    ofs << ");\n\n";

    // 声明所有将用到的中间导线 tXX（只为 name 以 t 开头的节点）
    std::vector<std::string> tnames;
    tnames.reserve(nodes.size());
    for (const auto& n : nodes) {
        if (!n.name.empty() && (n.name[0]=='t' || n.name[0]=='T')) {
            tnames.push_back(n.name);
        }
    }
    // 去重
    std::sort(tnames.begin(), tnames.end(), [&](const std::string& a, const std::string& b){
        int ia = tIndex(a), ib = tIndex(b);
        if (ia != ib) return ia < ib;
        return a < b;
    });
    tnames.erase(std::unique(tnames.begin(), tnames.end()), tnames.end());

    if (!tnames.empty()) {
        ofs << "  // Internal wires (auto-generated)\n";
        ofs << "  wire ";
        for (size_t i = 0; i < tnames.size(); ++i) {
            ofs << tnames[i];
            ofs << ((i + 1 == tnames.size()) ? ";\n\n" : ", ");
        }
    }

    // 把每个节点的 operation 翻译成 Verilog assign
    // 支持：NOT(a) -> ~a, AND(a,b) -> (a & b), XOR2(a,b) -> (a ^ b)
    ofs << "  // ========== Gates ==========\n";
    for (const auto& n : nodes) {
        if (n.name.empty()) continue;
        // 只导出 tXX；如果你也想导出临时的非 t 节点，这里可以去掉判断
        if (!(n.name[0]=='t' || n.name[0]=='T')) continue;

        const std::string& op = n.operation;
        if (op.rfind("NOT", 0) == 0) {
            // NOT(a)
            auto ops = parseOperands(op);
            if (ops.size() == 1) {
                auto a = toVerilogSignal(ops[0]);
                ofs << "  // depth " << n.depth << "  " << n.operation
                    << " -> " << n.expression << "\n";
                ofs << "  assign " << n.name << " = ~" << a << ";\n\n";
            } else {
                ofs << "  // [WARN] malformed: " << op << "\n";
            }
        } else if (op.rfind("AND", 0) == 0) {
            // AND(a,b)
            auto ops = parseOperands(op);
            if (ops.size() == 2) {
                auto a = toVerilogSignal(ops[0]);
                auto b = toVerilogSignal(ops[1]);
                ofs << "  // depth " << n.depth << "  " << n.operation
                    << " -> " << n.expression << "\n";
                ofs << "  assign " << n.name << " = (" << a << " & " << b << ");\n\n";
            } else {
                ofs << "  // [WARN] malformed: " << op << "\n";
            }
        } else if (op.rfind("XOR2", 0) == 0) {
            // XOR2(a,b)
            auto ops = parseOperands(op);
            if (ops.size() == 2) {
                auto a = toVerilogSignal(ops[0]);
                auto b = toVerilogSignal(ops[1]);
                ofs << "  // depth " << n.depth << "  " << n.operation
                    << " -> " << n.expression << "\n";
                ofs << "  assign " << n.name << " = (" << a << " ^ " << b << ");\n\n";
            } else {
                ofs << "  // [WARN] malformed: " << op << "\n";
            }
        } else {
            // 其它类型（如果你将来扩展了门）
            ofs << "  // [INFO] skip unsupported op: " << n.operation
                << "  node: " << n.name << "\n\n";
        }
    }

    // 输出口先留空（你自己接需要的 tXX 到 Y*）
    ofs << "  // ========== Outputs (connect by yourself) ==========\n";
    ofs << "  // Example:\n";
    ofs << "  // assign Y0 = t123;  // TODO\n";
    ofs << "  // assign Y1 = t456;  // TODO\n";
    ofs << "  // ...\n\n";

    ofs << "endmodule\n";
    ofs.close();

    std::cout << "[exportVerilog] Wrote Verilog netlist to: " << filepath << std::endl;
}
