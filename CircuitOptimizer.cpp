#include "CircuitOptimizer.h"
#include <cmath>
#include <fstream>
#include <regex>
#include <set>


// Term struct implementation
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


std::set<int> CircuitOptimizer::Term::getDifferenceFromSet(const std::set<int>& otherSet) const {
    std::set<int> diff;
    std::set_difference(
            variables.begin(), variables.end(),
            otherSet.begin(), otherSet.end(),
            std::inserter(diff, diff.begin())
    );
    return diff;
}

// OptimizedPair constructors
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

// Constructor for TWO_UNIQUE_FACTORS: includes the low-degree term as well
CircuitOptimizer::OptimizedPair::OptimizedPair(
        const std::shared_ptr<Term>& high,
        const std::vector<std::shared_ptr<Term>>& middle,
        const std::shared_ptr<Term>& low,
        const std::set<int>& common,
        const std::set<int>& unique)
        : type(TWO_UNIQUE_FACTORS), highDegreeTerm(high), lowDegreeTerm(low), middleTerms(middle),
          commonVars(common), uniqueVars(unique) {
}


// Group terms by degree (unchanged)
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

// Optimize ONE_UNIQUE_FACTOR pattern
std::vector<CircuitOptimizer::OptimizedPair>
CircuitOptimizer::optimizeOneUniqueFactor(
        std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
        int highDegree, int lowDegree, int requiredCommonVars) {

    // Do NOT clear circuit nodes here; we want to accumulate reusable gates
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

// Optimize TWO_UNIQUE_FACTORS pattern
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

        // Mark all used terms in this pair
        pair.highDegreeTerm->used = true;  // high degree term
        for (auto& middleTerm : pair.middleTerms) {
            if (middleTerm) middleTerm->used = true;  // middle terms
        }
        if (pair.lowDegreeTerm) {
            pair.lowDegreeTerm->used = true;  // low degree term
        }

        generateCircuitForTwoUniqueFactors(pair);
        allPairs.push_back(pair);

        std::cout << "Found TWO-unique-factors pair: " << pair.highDegreeTerm->expression
                  << " + 2 middle terms + " << (pair.lowDegreeTerm ? pair.lowDegreeTerm->expression : "null")
                  << std::endl;
    }

    return allPairs;
}

// Find ONE_UNIQUE_FACTOR pair
CircuitOptimizer::OptimizedPair
CircuitOptimizer::findOneUniqueFactorPair(
        std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
        int highDegree, int lowDegree, int requiredCommonVars) {

    if (groupedTerms.find(highDegree) == groupedTerms.end() ||
        groupedTerms.find(lowDegree) == groupedTerms.end()) {
        return OptimizedPair(nullptr, nullptr, {}, {});
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
                    return OptimizedPair(highTerm, lowTerm, commonVars, uniqueVars);
                }
            }
        }
    }

    return OptimizedPair(nullptr, nullptr, {}, {});
}

// Find TWO_UNIQUE_FACTORS pair
CircuitOptimizer::OptimizedPair
CircuitOptimizer::findTwoUniqueFactorsPair(
        std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
        int highDegree, int middleDegree, int lowDegree, int requiredCommonVars) {

    if (groupedTerms.find(highDegree) == groupedTerms.end() ||
        groupedTerms.find(middleDegree) == groupedTerms.end() ||
        groupedTerms.find(lowDegree) == groupedTerms.end()) {
        return OptimizedPair(nullptr, std::vector<std::shared_ptr<Term>>(), nullptr, {}, {});
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
                            return OptimizedPair(highTerm, foundMiddleTerms, lowTerm, commonVars, uniqueVars);
                        }
                    }
                }
            }
        }
    }

    return OptimizedPair(nullptr, std::vector<std::shared_ptr<Term>>(), nullptr, {}, {});
}

// Optimize SPECIAL_TWO_UNIQUE_FACTORS (4-degree + 2*3-degree + virtual 2-degree)
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

        // Generate circuit and record the virtual quadratic term
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

// Find SPECIAL_TWO_UNIQUE_FACTORS pair (4-degree + two 3-degree; 2-degree virtual not required)
CircuitOptimizer::OptimizedPair
CircuitOptimizer::findSpecialTwoUniqueFactorsPair(
        std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
        int highDegree, int middleDegree, int /*lowDegree*/, int requiredCommonVars) {

    // Need only degree-4 and degree-3 groups
    if (groupedTerms.find(highDegree) == groupedTerms.end() ||
        groupedTerms.find(middleDegree) == groupedTerms.end()) {
        return OptimizedPair(nullptr, std::vector<std::shared_ptr<Term>>(), nullptr, {}, {});
    }

    auto& highDegreeTerms   = groupedTerms[highDegree];    // expected: degree-4 terms
    auto& middleDegreeTerms = groupedTerms[middleDegree];  // expected: degree-3 terms

    // Only support requiredCommonVars == 2 (your design uses two common variables)
    if (requiredCommonVars != 2) {
        return OptimizedPair(nullptr, std::vector<std::shared_ptr<Term>>(), nullptr, {}, {});
    }

    // Iterate over each degree-4 term
    for (auto& highTerm : highDegreeTerms) {
        if (highTerm->used) continue;
        if (highTerm->variables.size() != static_cast<size_t>(highDegree)) continue;

        // Copy variables and enumerate all 2-variable common sets
        std::vector<int> hv(highTerm->variables.begin(), highTerm->variables.end());

        for (size_t i = 0; i < hv.size(); ++i) {
            for (size_t j = i + 1; j < hv.size(); ++j) {
                std::set<int> commonVars = { hv[i], hv[j] };
                if (static_cast<int>(commonVars.size()) != requiredCommonVars) continue;

                // Compute unique = high \ common
                std::set<int> uniqueVars = highTerm->getDifferenceFromSet(commonVars);
                if (uniqueVars.size() != 2) continue;

                // Target C ∪ {u1} and C ∪ {u2} as the two degree-3 terms
                auto it = uniqueVars.begin();
                int u1 = *it++;
                int u2 = *it;

                std::set<int> need1 = commonVars; need1.insert(u1);
                std::set<int> need2 = commonVars; need2.insert(u2);

                // Find exact degree-3 terms with variable set equal to need1/need2
                std::shared_ptr<Term> m1 = nullptr, m2 = nullptr;
                for (auto& mid : middleDegreeTerms) {
                    if (mid->used) continue;
                    if (mid->variables == need1) { m1 = mid; }
                    else if (mid->variables == need2) { m2 = mid; }
                    if (m1 && m2) break;
                }

                if (m1 && m2) {
                    std::vector<std::shared_ptr<Term>> mids = { m1, m2 };
                    return OptimizedPair(highTerm, mids, /*low=*/nullptr, commonVars, uniqueVars);
                }
            }
        }
    }

    return OptimizedPair(nullptr, std::vector<std::shared_ptr<Term>>(), nullptr, {}, {});
}


// Generate circuit for SPECIAL_TWO_UNIQUE_FACTORS and return the virtual quadratic term
// Rule: only require Q4 + two C3 presence; the virtual quadratic existence in the original poly is not checked here
std::string CircuitOptimizer::generateCircuitForSpecialTwoUniqueFactors(OptimizedPair& pair) {
    if (pair.uniqueVars.size() != 2) return "";
    // Set the kind so the reuse catalog can recognize this pair
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

    // Depth 0: two NOT gates for the two "unique" bits
    std::string notOp1 = "NOT(x" + std::to_string(uniqueVar1) + ")";
    std::string notResult1 = "(x" + std::to_string(uniqueVar1) + "+1)";
    std::string notNode1 = findOrCreateNode(notOp1, notResult1, 0);

    std::string notOp2 = "NOT(x" + std::to_string(uniqueVar2) + ")";
    std::string notResult2 = "(x" + std::to_string(uniqueVar2) + "+1)";
    std::string notNode2 = findOrCreateNode(notOp2, notResult2, 0);

    pair.circuitOperations.push_back("Depth 0: " + notOp1 + " -> " + notResult1 + " [" + notNode1 + "]");
    pair.circuitOperations.push_back("Depth 0: " + notOp2 + " -> " + notResult2 + " [" + notNode2 + "]");

    // Depth 1: prepare inputs (common variables + the two NOT results)
    std::vector<std::string> depth1Inputs;

    // Add the common variables (the 2-bit common part of Q4)
    std::vector<int> commonVars(pair.commonVars.begin(), pair.commonVars.end());
    for (int var : commonVars) {
        depth1Inputs.push_back("x" + std::to_string(var));
    }

    // Add NOT results
    depth1Inputs.push_back(notNode1);
    depth1Inputs.push_back(notNode2);

    // Depth 1: AND pairwise
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

    // Combine as an AND tree and record the final depth (AND tree continues from depth 2)
    int finalDepth = combineOperationsWithReuse(pair, depth1Results, 2);

    // Extract the final gate name created in this function
    std::string finalNode = extractLastNode(pair.circuitOperations);
    pair.outputGateName = finalNode;

    // Record depth info for statistics
    PairDepthInfo info;
    info.pairIndex = static_cast<int>(pairDepthInfo.size()) + 1;
    info.finalDepth = finalDepth;
    info.finalOutput = finalNode;
    info.pairType = "SPECIAL_TWO_UNIQUE_FACTORS";
    info.expression.clear();
    info.isSingleTerm = false;
    pairDepthInfo.push_back(info);

    // Build the "virtual quadratic" from the two common variables: e.g., {a,b} -> "xaxb"
    std::string virtualQuadratic;
    for (int var : pair.commonVars) {
        virtualQuadratic += "x" + std::to_string(var);
    }
    return virtualQuadratic;
}


// Build XOR result string for quadratic terms (cancel even multiplicities in GF(2))
std::string CircuitOptimizer::getQuadraticXORResult(
        const std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
        const std::vector<std::string>& generatedQuadraticTerms) {

    // Collect original degree-2 terms still unused in the polynomial
    std::vector<std::string> originalQuadraticTerms;
    if (groupedTerms.find(2) != groupedTerms.end()) {
        for (const auto& term : groupedTerms.at(2)) {
            if (!term->used) {
                originalQuadraticTerms.push_back(term->expression);
            }
        }
    }

    // Merge generated + original
    std::vector<std::string> allTerms;
    for (const auto& term : generatedQuadraticTerms) {
        allTerms.push_back(term);
    }
    for (const auto& term : originalQuadraticTerms) {
        allTerms.push_back(term);
    }

    // Count multiplicities in GF(2)
    std::map<std::string, int> termCount;

    for (const auto& term : allTerms) {
        termCount[term]++;
    }

    // Keep only those appearing odd number of times
    std::vector<std::string> resultTerms;
    for (const auto& pair : termCount) {
        if (pair.second % 2 == 1) {
            resultTerms.push_back(pair.first);
        }
    }

    // Build final text
    std::string result;
    for (size_t i = 0; i < resultTerms.size(); i++) {
        if (i > 0) result += " + ";
        result += resultTerms[i];
    }

    // If nothing remains, return "0"
    if (result.empty()) {
        result = "0";
    }

    return result;
}

// Generate circuit for ONE_UNIQUE_FACTOR
void CircuitOptimizer::generateCircuitForOneUniqueFactor(OptimizedPair& pair) {
    if (pair.uniqueVars.empty()) return;
    // Mark kind for reuse catalog
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

    // Depth 0: NOT gate for the unique factor
    std::string notOp = "NOT(x" + std::to_string(uniqueVar) + ")";
    std::string notResult = "(x" + std::to_string(uniqueVar) + "+1)";
    std::string notNode = findOrCreateNode(notOp, notResult, 0);
    pair.circuitOperations.push_back("Depth 0: " + notOp + " -> " + notResult + " [" + notNode + "]");

    // Depth 1: prepare inputs (common variables + NOT(unique))
    std::vector<std::string> depth1Inputs;
    std::vector<int> commonVars(pair.commonVars.begin(), pair.commonVars.end());
    for (int var : commonVars) {
        depth1Inputs.push_back("x" + std::to_string(var));
    }
    depth1Inputs.push_back(notNode);

    // Depth 1: pairwise AND
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

    // Continue combining as an AND tree; record final depth
    int finalDepth = combineOperationsWithReuse(pair, depth1Results, 2);

    // Extract last node name created here
    std::string finalNode = extractLastNode(pair.circuitOperations);
    pair.outputGateName = finalNode;

    // Record depth information
    PairDepthInfo info;
    info.pairIndex = static_cast<int>(pairDepthInfo.size()) + 1;
    info.finalDepth = finalDepth;
    info.finalOutput = finalNode;
    info.pairType = "ONE_UNIQUE_FACTOR";
    info.expression.clear();
    info.isSingleTerm = false;
    pairDepthInfo.push_back(info);
}


// Generate circuit for TWO_UNIQUE_FACTORS
void CircuitOptimizer::generateCircuitForTwoUniqueFactors(OptimizedPair& pair) {
    if (pair.uniqueVars.size() != 2) return;
    // Set kind for reuse catalog
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

    // Depth 0: two NOT gates for two unique factors
    std::string notOp1 = "NOT(x" + std::to_string(uniqueVar1) + ")";
    std::string notResult1 = "(x" + std::to_string(uniqueVar1) + "+1)";
    std::string notNode1 = findOrCreateNode(notOp1, notResult1, 0);

    std::string notOp2 = "NOT(x" + std::to_string(uniqueVar2) + ")";
    std::string notResult2 = "(x" + std::to_string(uniqueVar2) + "+1)";
    std::string notNode2 = findOrCreateNode(notOp2, notResult2, 0);

    pair.circuitOperations.push_back("Depth 0: " + notOp1 + " -> " + notResult1 + " [" + notNode1 + "]");
    pair.circuitOperations.push_back("Depth 0: " + notOp2 + " -> " + notResult2 + " [" + notNode2 + "]");

    // Depth 1: prepare inputs (common variables + two NOT results)
    std::vector<std::string> depth1Inputs;
    std::vector<int> commonVars(pair.commonVars.begin(), pair.commonVars.end());
    for (int var : commonVars) {
        depth1Inputs.push_back("x" + std::to_string(var));
    }
    depth1Inputs.push_back(notNode1);
    depth1Inputs.push_back(notNode2);

    // Depth 1: pairwise AND
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

    // Combine and record final depth
    int finalDepth = combineOperationsWithReuse(pair, depth1Results, 2);

    // Extract the final node created here
    std::string finalNode = extractLastNode(pair.circuitOperations);
    pair.outputGateName = finalNode;

    // Record depth info
    PairDepthInfo info;
    info.pairIndex = static_cast<int>(pairDepthInfo.size()) + 1;
    info.finalDepth = finalDepth;
    info.finalOutput = finalNode;
    info.pairType = "TWO_UNIQUE_FACTORS";
    info.expression.clear();
    info.isSingleTerm = false;
    pairDepthInfo.push_back(info);
}



// Parse degree-2 term like "x3x7" -> ["x3", "x7"]
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


// Generate circuit for all the remaining terms (those not consumed by patterns)
void CircuitOptimizer::generateRemainingTermsCircuit(
        const std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
        const std::vector<std::string>& quadraticTerms) {

    std::cout << "\nGenerating circuit for remaining terms:" << std::endl;
    std::cout << "========================================" << std::endl;

    // Implement each monomial in the quadratic XOR result
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

                // Record depth info even when reusing
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

                // Record depth info
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

    // Handle higher-degree remaining terms (Degree 3, 4, 5, 6)
    std::vector<int> degreesToProcess = {3, 4, 5, 6};

    for (int degree : degreesToProcess) {
        if (groupedTerms.find(degree) != groupedTerms.end()) {
            auto& terms = groupedTerms.at(degree);
            std::vector<std::shared_ptr<Term>> remainingTerms;

            // Collect unused terms
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

                    // Sort variables for deterministic tree
                    std::vector<int> vars(term->variables.begin(), term->variables.end());
                    std::sort(vars.begin(), vars.end());

                    // Build input names xK
                    std::vector<std::string> varNames;
                    for (int var : vars) {
                        varNames.push_back("x" + std::to_string(var));
                    }

                    // Build an AND tree layer by layer
                    std::vector<std::string> currentLevel = varNames;
                    int depth = 1;
                    std::string finalOutput;

                    while (currentLevel.size() > 1) {
                        std::vector<std::string> nextLevel;

                        for (size_t i = 0; i < currentLevel.size(); i += 2) {
                            if (i + 1 < currentLevel.size()) {
                                std::string left = currentLevel[i];
                                std::string right = currentLevel[i + 1];

                                // Fetch readable expressions
                                std::string leftExpr, rightExpr;
                                for (const auto& node : circuitNodes) {
                                    if (node.name == left) leftExpr = node.expression;
                                    if (node.name == right) rightExpr = node.expression;
                                }
                                if (leftExpr.empty()) leftExpr = left;
                                if (rightExpr.empty()) rightExpr = right;

                                std::string andOp = "AND(" + left + ", " + right + ")";
                                std::string result = "(" + leftExpr + rightExpr + ")";

                                // Check reuse
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

                    // Record the final output
                    if (currentLevel.size() == 1) {
                        finalOutput = currentLevel[0];
                        std::cout << "Final result for " << term->expression << ": " << finalOutput << std::endl;

                        // Record depth info
                        PairDepthInfo info;
                        info.pairIndex = pairDepthInfo.size() + 1;
                        info.finalDepth = depth - 1; // subtract one because depth was incremented after the last combine
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

    // Handle degree-1 terms: use inputs directly
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

                // depth = 0 for inputs
                PairDepthInfo info;
                info.pairIndex = pairDepthInfo.size() + 1;
                info.finalDepth = 0;
                info.finalOutput = term->expression; // directly use xK
                info.pairType = "DEGREE_1_TERM";
                info.expression = term->expression;
                info.isSingleTerm = true;
                pairDepthInfo.push_back(info);
            }
        }
    }

    // Some Boolean functions (e.g., poly3/4/6/7) contain a constant "1".
    // Record it as a depth-0 single item so it appears in "Initial items by depth".
    if (groupedTerms.find(0) != groupedTerms.end()) {
        const auto& terms0 = groupedTerms.at(0);
        bool recorded = false;
        for (const auto& t : terms0) {
            // Record once when the literal is exactly "1" and not used
            if (!t->used && t->expression == "1") {
                std::cout << "\nDegree 0 constant implementation:" << std::endl;
                std::cout << "---------------------------------" << std::endl;
                std::cout << "Constant 1 is available as logic constant at Depth 0." << std::endl;

                PairDepthInfo info;
                info.pairIndex = static_cast<int>(pairDepthInfo.size()) + 1;
                info.finalDepth = 0;
                info.finalOutput = "1";        // logic constant 1
                info.pairType = "CONST_1";
                info.expression = "1";
                info.isSingleTerm = true;
                pairDepthInfo.push_back(info);

                // If you want to avoid it being considered "unused" later, you can mark used:
                // t->used = true;
                recorded = true;
                break; // record constant once
            }
        }
        if (!recorded) {
            // If degree-0 terms are not the literal "1", we skip
        }
    }
}


// Canonicalize expression: "x3x1x2" -> "x1x2x3"
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


// Make an unordered XOR key: min(A,B) + "&" + max(A,B) after canonicalization
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
    // Possibly an input variable "xk"
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

    // Key = depth + '|' + makeXorKey(A,B)
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
    if (it == xorCatalog_.end()) return "";
    return it->second.outNode;
}

void CircuitOptimizer::clearXorCatalog() { xorCatalog_.clear(); } // typically not needed




int CircuitOptimizer::degreeOfExpr(const std::string& expr) {
    int cnt = 0;
    for (size_t i=0;i<expr.size();++i) if (expr[i]=='x') ++cnt;
    return cnt;
}

// THREE signatures for pair reuse (all components canonicalized before concatenation)
std::string CircuitOptimizer::makeSignatureTwoUnique(const std::string& high,
                                                     const std::string& m1,
                                                     const std::string& m2,
                                                     const std::string& low) {
    std::string cH = canonicalExpr(high);
    std::string cM1 = canonicalExpr(m1);
    std::string cM2 = canonicalExpr(m2);
    if (cM2 < cM1) std::swap(cM1, cM2); // two middle terms are unordered
    std::string cL = canonicalExpr(low);
    return "two|H:" + cH + "|M:" + cM1 + "," + cM2 + "|L:" + cL;
}

std::string CircuitOptimizer::makeSignatureOneUnique(const std::string& high,
                                                     const std::string& low) {
    std::string cH = canonicalExpr(high);
    std::string cL = canonicalExpr(low);
    return "one|H:" + cH + "|L:" + cL;
}

// Special: only check presence of (Q4) and two (C3) terms; 2-degree presence is not required
std::string CircuitOptimizer::makeSignatureSpecialTwo(const std::string& q4,
                                                      const std::string& c31,
                                                      const std::string& c32) {
    std::string cQ4 = canonicalExpr(q4);
    std::string c31c = canonicalExpr(c31);
    std::string c32c = canonicalExpr(c32);
    if (c32c < c31c) std::swap(c31c, c32c);
    return "special|Q4:" + cQ4 + "|C3:" + c31c + "," + c32c;
}



// Other helpers (unchanged)
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

// Combine a list of inputs into an AND tree with reuse; return final depth
int CircuitOptimizer::combineOperationsWithReuse(OptimizedPair& pair,
                                                 std::vector<std::string>& inputs,
                                                 int depth) {
    if (inputs.size() <= 1) {
        // When only one node remains, the final depth is depth-1
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

    // Recurse and return final depth
    return combineOperationsWithReuse(pair, nextLevel, depth + 1);
}


// Group all recorded items by their final depth
std::map<int, std::vector<std::string>> CircuitOptimizer::getAllItemsByDepth() const {
    std::map<int, std::vector<std::string>> depthItems;

    for (const auto& info : pairDepthInfo) {
        depthItems[info.finalDepth].push_back(info.finalOutput);
    }

    return depthItems;
}

// XOR-merge all items at a given depth; tries XOR reuse first
std::string CircuitOptimizer::mergeItemsAtDepth(int depth, std::vector<std::string> items, int startDepth,
                                                const std::map<int, std::pair<std::string, int>>& availableResults) {
    if (items.empty()) return "";
    if (items.size() == 1) return items[0];

    std::vector<std::string> currentLevel = items;
    int currentDepth = startDepth;

    std::cout << "  Merging " << items.size() << " items from depth " << depth << " starting at depth " << startDepth << ":" << std::endl;

    while (currentLevel.size() > 1) {
        std::vector<std::string> nextLevel;

        // At this start depth, see if there are newly available results to inject
        for (const auto& result : availableResults) {
            int resultDepth = result.first;
            const auto& resultInfo = result.second;
            std::string resultNode = resultInfo.first;
            int completionDepth = resultInfo.second;

            if (completionDepth + 1 == currentDepth &&
                std::find(currentLevel.begin(), currentLevel.end(), resultNode) == currentLevel.end()) {
                currentLevel.push_back(resultNode);
                std::cout << "    Depth " << currentDepth << ": Adding available result " << resultNode
                          << " from Processing Depth " << resultDepth << std::endl;
            }
        }

        for (size_t i = 0; i < currentLevel.size(); i += 2) {
            if (i + 1 < currentLevel.size()) {
                // XOR2
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

                // Reuse if possible
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
                // Odd count: pass-through
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


// Hierarchical merge for all recorded terms/pairs
void CircuitOptimizer::hierarchicalMergeAllTerms() {
    hierarchicalOperations.clear();
    depthMergeResults.clear();

    std::cout << "\nHierarchical Merge of All Terms:" << std::endl;
    std::cout << "================================" << std::endl;

    // 1) Collect all items: key = finalDepth, value = node name (xK or tNN)
    auto itemsByDepth = getAllItemsByDepth();

    // Print initial statistics
    std::cout << "Initial items by depth:" << std::endl;
    for (const auto& kv : itemsByDepth) {
        std::cout << "  Depth " << kv.first << ": " << kv.second.size() << " items" << std::endl;
    }

    // 2) Helper: XOR merge on one level with reuse preference
    auto xorMergeLevelWithReuse = [&](std::vector<std::string>& items,
                                      int startDepth,
                                      std::vector<std::string>& outNextLevel,
                                      std::vector<std::string>& opLogs) {
        for (size_t i = 0; i < items.size(); i += 2) {
            if (i + 1 < items.size()) {
                const std::string& L = items[i];
                const std::string& R = items[i + 1];

                // Fetch readable expressions for both sides
                std::string Lexpr, Rexpr;
                getNodeExpr(L, Lexpr, nullptr);
                getNodeExpr(R, Rexpr, nullptr);
                if (Lexpr.empty()) Lexpr = L;
                if (Rexpr.empty()) Rexpr = R;

                // Try reuse first: same merge depth and same pair of inputs (unordered)
                std::string reusedNode = tryReuseXorAtDepth(startDepth, Lexpr, Rexpr);
                if (!reusedNode.empty()) {
                    outNextLevel.push_back(reusedNode);
                    opLogs.push_back("Depth " + std::to_string(startDepth) + ": XOR2(" + L + ", " + R +
                                     ") -> (" + Lexpr + " + " + Rexpr + ") [" + reusedNode + "] (reused)");
                } else {
                    // Not reusable: create new XOR and remember it
                    std::string xorOp = "XOR2(" + L + ", " + R + ")";
                    std::string result = "(" + Lexpr + " + " + Rexpr + ")";
                    std::string node   = findOrCreateNode(xorOp, result, startDepth);
                    outNextLevel.push_back(node);
                    opLogs.push_back("Depth " + std::to_string(startDepth) + ": " + xorOp + " -> " + result +
                                     " [" + node + "]");

                    // Remember this merge for reuse (same depth and operands)
                    rememberXorMerge(startDepth, L, R, node);
                }
            } else {
                // Odd count: last item passes through
                outNextLevel.push_back(items[i]);
                std::string expr; getNodeExpr(items[i], expr, nullptr);
                if (expr.empty()) expr = items[i];
                opLogs.push_back("Depth " + std::to_string(startDepth) + ": " + items[i] + " passed through");
            }
        }
    };

    // 3) Process depths in ascending order so logs look clear
    std::map<int, std::pair<std::string,int>> depthMerged; // depth -> (mergedNode, completionDepth)

    for (const auto& kv : itemsByDepth) {
        int depth = kv.first;
        const auto& items = kv.second;

        if (items.empty()) continue;

        // Merge items from "depth"; start merging at depth+1 (consistent with previous style)
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

        // Print merge logs
        for (auto& s : logs) std::cout << "    " << s << std::endl;

        // Summarize: the merged node for this depth and the completion depth
        std::string mergedNode   = current.front();
        int completionDepth      = startDepth - 1;

        depthMerged[depth]       = std::make_pair(mergedNode, completionDepth);
        depthMergeResults[depth] = mergedNode;

        std::cout << "  Merge completed at depth " << completionDepth << ": " << mergedNode << std::endl;
        std::cout << "  Result will be available at depth " << (completionDepth + 1) << std::endl;
    }

    // 4) Final overall result (pick the last processed depth result)
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
    rec.kind = pair.kind;                       // ensure pair.kind is set before calling
    rec.outputGateName = pair.outputGateName;

    if (pair.kind == PairKind::TWO_UNIQUE_FACTORS) {
        // Components: high + two middle + low
        if (!pair.highDegreeTerm || pair.middleTerms.size()<2 || !pair.lowDegreeTerm) return;
        rec.highExpr = canonicalExpr(pair.highDegreeTerm->expression);
        rec.middleExprs = { canonicalExpr(pair.middleTerms[0]->expression),
                            canonicalExpr(pair.middleTerms[1]->expression) };
        if (rec.middleExprs[1] < rec.middleExprs[0]) std::swap(rec.middleExprs[0], rec.middleExprs[1]);
        rec.lowExpr = canonicalExpr(pair.lowDegreeTerm->expression);
        rec.signature = makeSignatureTwoUnique(rec.highExpr, rec.middleExprs[0], rec.middleExprs[1], rec.lowExpr);
    } else if (pair.kind == PairKind::ONE_UNIQUE_FACTOR) {
        // Components: high + low (single middle/low in this pattern)
        if (!pair.highDegreeTerm || !pair.lowDegreeTerm) return;
        rec.highExpr = canonicalExpr(pair.highDegreeTerm->expression);
        rec.lowExpr  = canonicalExpr(pair.lowDegreeTerm->expression);
        rec.signature = makeSignatureOneUnique(rec.highExpr, rec.lowExpr);
    } else if (pair.kind == PairKind::SPECIAL_TWO_UNIQUE_FACTORS) {
        // Components: Q4 + two C3 (we don't check for a degree-2 term existence)
        if (!pair.lowDegreeTerm || pair.middleTerms.size()<2) return;
        // In this mode, lowDegreeTerm stores Q4; middleTerms store two C3 terms
        rec.highExpr.clear();
        rec.lowExpr  = canonicalExpr(pair.lowDegreeTerm->expression); // Q4
        rec.middleExprs = { canonicalExpr(pair.middleTerms[0]->expression),
                            canonicalExpr(pair.middleTerms[1]->expression) };
        if (rec.middleExprs[1] < rec.middleExprs[0]) std::swap(rec.middleExprs[0], rec.middleExprs[1]);
        rec.signature = makeSignatureSpecialTwo(rec.lowExpr, rec.middleExprs[0], rec.middleExprs[1]);
    } else {
        return;
    }

    // Insert/overwrite into catalog (overwriting is fine as same output gate)
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
    info.finalOutput  = out;          // tNN or xk
    info.pairType     = type;         // "DEGREE_1_TERM" / "QUADRATIC_TERM" / ...
    info.expression   = expr;         // e.g., "x0x1x2", "x0x4x5x7"
    info.isSingleTerm = true;
    pairDepthInfo.push_back(std::move(info));
}

bool CircuitOptimizer::findTermByExpr(
        std::map<int, std::vector<std::shared_ptr<Term>>>& groupedTerms,
        const std::string& expr,
        std::shared_ptr<Term>& outTerm) const
{
    std::string key = canonicalExpr(expr);

    // Estimate degree by counting 'x' (fast path)
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
        // These referencePairs come from the first pass (kind/outputGateName/components are set)
        OptimizedPair p;
        p.kind = ref.kind;
        p.outputGateName = ref.outputGateName; // reuse the existing t_xx
        p.circuitOperations.clear();           // no pipeline rebuild when reusing

        bool ok = true;

        if (ref.kind == PairKind::TWO_UNIQUE_FACTORS) {
            // Need high + two middle + low all present
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
            // Need high + low
            if (!ref.highDegreeTerm || !ref.lowDegreeTerm) continue;

            std::shared_ptr<Term> th, tl;
            ok = ok && findTermByExpr(groupedTerms, ref.highDegreeTerm->expression, th);
            ok = ok && findTermByExpr(groupedTerms, ref.lowDegreeTerm->expression,  tl);
            if (!ok) continue;

            p.highDegreeTerm = th;
            p.lowDegreeTerm  = tl;

        } else if (ref.kind == PairKind::SPECIAL_TWO_UNIQUE_FACTORS) {
            // Need Q4 + two C3 (2-degree virtual term not required)
            if (!ref.lowDegreeTerm || ref.middleTerms.size() < 2) continue;

            std::shared_ptr<Term> tq4, tc31, tc32;
            ok = ok && findTermByExpr(groupedTerms, ref.lowDegreeTerm->expression,  tq4);   // Q4
            ok = ok && findTermByExpr(groupedTerms, ref.middleTerms[0]->expression, tc31);  // C3
            ok = ok && findTermByExpr(groupedTerms, ref.middleTerms[1]->expression, tc32);  // C3
            if (!ok) continue;

            p.lowDegreeTerm = tq4;
            p.middleTerms   = { tc31, tc32 };
        } else {
            continue;
        }

        // Mark these as used to avoid re-implementing in "remaining terms"
        if (p.highDegreeTerm) p.highDegreeTerm->used = true;
        for (auto& m : p.middleTerms) if (m) m->used = true;
        if (p.lowDegreeTerm)  p.lowDegreeTerm->used  = true;

        // Depth of the reused output node
        int depthFound = 0;
        if (!p.outputGateName.empty()) {
            for (const auto& node : circuitNodes) {
                if (node.name == p.outputGateName) { depthFound = node.depth; break; }
            }
        }

        // Count reuse in the statistics of this pass
        PairDepthInfo info;
        info.pairIndex    = static_cast<int>(pairDepthInfo.size()) + 1;
        info.finalDepth   = depthFound;
        info.finalOutput  = p.outputGateName;
        info.pairType     = (ref.kind == PairKind::TWO_UNIQUE_FACTORS) ? "TWO_UNIQUE_FACTORS" :
                            (ref.kind == PairKind::ONE_UNIQUE_FACTOR)  ? "ONE_UNIQUE_FACTOR"  :
                            "SPECIAL_TWO_UNIQUE_FACTORS";
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




// Print hierarchical merge operations and summary
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



// Remove spaces from a term like "x1 x2" -> "x1x2"
std::string CircuitOptimizer::cleanTerm(const std::string& term) {
    std::string cleaned;
    for (char c : term) {
        if (c != ' ') cleaned += c;
    }
    return cleaned;
}

// Print unused terms (by degree)
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

// Print optimization statistics for a batch of pairs
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
            optimizedTerms += 3; // high + two middle
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

// Convert internal node names to Verilog signals: x0->X0, t12->t12
static std::string toVerilogSignal(const std::string& s) {
    if (s.empty()) return s;
    if (s[0] == 'x' || s[0] == 'X') {
        // x0...x7 -> X0...X7
        return "X" + s.substr(1);
    }
    // tXX is kept as-is
    return s;
}

// Parse operands inside "AND(a, b)" / "XOR2(a, b)" / "NOT(a)"
static std::vector<std::string> parseOperands(const std::string& op) {
    std::vector<std::string> out;
    auto l = op.find('(');
    auto r = op.rfind(')');
    if (l == std::string::npos || r == std::string::npos || r <= l) return out;

    std::string inside = op.substr(l + 1, r - l - 1);
    // split by comma
    size_t p = 0;
    while (p < inside.size()) {
        while (p < inside.size() && isspace(static_cast<unsigned char>(inside[p]))) ++p;
        size_t q = p;
        while (q < inside.size() && inside[q] != ',') ++q;
        std::string token = inside.substr(p, q - p);
        while (!token.empty() && isspace(static_cast<unsigned char>(token.front()))) token.erase(token.begin());
        while (!token.empty() && isspace(static_cast<unsigned char>(token.back())))  token.pop_back();
        if (!token.empty()) out.push_back(token);
        p = (q == inside.size()) ? q : q + 1;
    }
    return out;
}

// Export all recorded gates as a Verilog netlist (X0..X7 inputs, Y0..Y7 left for manual connection)
void CircuitOptimizer::exportVerilog(const std::string& filepath,
                                     const std::string& moduleName) const
{
    std::ofstream ofs(filepath);
    if (!ofs) {
        std::cerr << "[exportVerilog] Cannot open file: " << filepath << std::endl;
        return;
    }

    // Sort nodes by t-index for readability: t0, t1, t2, ...
    std::vector<CircuitNode> nodes = circuitNodes;
    auto tIndex = [](const std::string& name)->int {
        // name like t123
        if (name.size() >= 2 && (name[0]=='t' || name[0]=='T')) {
            return std::atoi(name.c_str() + 1);
        }
        return INT_MAX; // non-t put to the end
    };
    std::sort(nodes.begin(), nodes.end(),
              [&](const CircuitNode& a, const CircuitNode& b) {
                  int ia = tIndex(a.name), ib = tIndex(b.name);
                  if (ia != ib) return ia < ib;
                  // fallback: depth then name
                  if (a.depth != b.depth) return a.depth < b.depth;
                  return a.name < b.name;
              });

    // File header
    ofs << "// Auto-generated by CircuitOptimizer::exportVerilog\n";
    ofs << "// This netlist includes all gates (tXX) recorded so far.\n\n";

    // Module header
    ofs << "module " << moduleName << "(\n";
    ofs << "    input  wire X0, X1, X2, X3, X4, X5, X6, X7,\n";
    ofs << "    output wire Y0, Y1, Y2, Y3, Y4, Y5, Y6, Y7\n";
    ofs << ");\n\n";

    // Declare internal wires for all tXX nodes
    std::vector<std::string> tnames;
    tnames.reserve(nodes.size());
    for (const auto& n : nodes) {
        if (!n.name.empty() && (n.name[0]=='t' || n.name[0]=='T')) {
            tnames.push_back(n.name);
        }
    }
    // unique & sort
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

    // Emit each node as a Verilog assign
    // NOT(a) -> ~a, AND(a,b) -> (a & b), XOR2(a,b) -> (a ^ b)
    ofs << "  // ========== Gates ==========\n";
    for (const auto& n : nodes) {
        if (n.name.empty()) continue;
        // Only export tXX; remove this check if you also want to export temp non-t nodes
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
            // Skip unsupported op kinds
            ofs << "  // [INFO] skip unsupported op: " << n.operation
                << "  node: " << n.name << "\n\n";
        }
    }

    // Leave outputs for you to wire
    ofs << "  // ========== Outputs (connect by yourself) ==========\n";
    ofs << "  // Example:\n";
    ofs << "  // assign Y0 = t123;  // TODO\n";
    ofs << "  // assign Y1 = t456;  // TODO\n";
    ofs << "  // ...\n\n";

    ofs << "endmodule\n";
    ofs.close();

    std::cout << "[exportVerilog] Wrote Verilog netlist to: " << filepath << std::endl;

}
