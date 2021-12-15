#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <stdint.h>

using std::vector;


/*============
 * RULE
 ==========*/

struct Rule
{
    int a;
    int b;
    int c;
};

/*============
 * CHAIN
 ==========*/

class Chain
{
public:
    Chain(vector<uint64_t> initial);
    void apply(vector<Rule> rules);
    uint64_t length() const;
    void print() const;
    void printSolution() const;

private:
    int first;
    vector<vector<uint64_t>> link;
};

Chain::Chain(std::vector<uint64_t> initial):
    first(*initial.begin()),
    link(vector<vector<uint64_t>>(26, vector<uint64_t>(26, 0)))
{
    for (int i=1; i<(int)initial.size(); i++) {
        int a = initial[i-1];
        int b = initial[i];
        link[a][b]++;
    }
}

void Chain::apply(vector<Rule> rules)
{
    vector<vector<uint64_t>> start = link;

    for (Rule rule: rules) {
        const int a = rule.a;
        const int b = rule.b;
        const int c = rule.c;

        const uint64_t ab = start[a][b];

        if (ab) {
            link[a][b] -= ab;
            link[a][c] += ab;
            link[c][b] += ab;
        }
    }
}

uint64_t Chain::length() const
{
    uint64_t length = 1;
    for (auto v: link) {
        for (auto c: v) {
            length += c;
        }
    }

    return length;
}

void Chain::print() const
{
    printf("     ");
    for (int i=0; i<26; i++) {
        printf("%c  ", i+'A');
    }
    printf("\n");
    printf("   +");
    for (int i=0; i<26; i++) {
        printf("---");
    }
    printf("\n");

    for (int from=0; from<26; from++) {
        printf("%c  |", from+'A');
        for (int to=0; to<26; to++) {
            int n = link[from][to];
            if (n) {
                printf("%2lld ", link[from][to]);
            } else {
                printf("   ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

void Chain::printSolution() const
{
    vector<uint64_t> counts(26, 0);
    counts[first] = 1;

    for (int i=0; i<26; i++) {
        for (int j=0; j<26; j++) {
            counts[j] += link[i][j];
        }
    }

    uint64_t minC = ~0;
    int minI = -1;
    uint64_t maxC = counts[0];
    int maxI = 0;
    for (int i=0; i<26; i++) {
        if (counts[i] && counts[i] < minC) {
            minC = counts[i];
            minI = i;
        }

        if (counts[i] && counts[i] > maxC) {
            maxC = counts[i];
            maxI = i;
        }
    }

    printf("most common: %c (%lld)\n", maxI+'A', maxC);
    printf("least common: %c (%lld)\n", minI+'A', minC);
    printf("result: %lld\n", maxC - minC);
}

/*============
 * TASK
 ==========*/

Chain readChain(std::ifstream &file)
{
    std::string line;
    std::getline(file, line);

    vector<uint64_t> initial;
    for (char ch: line) {
        initial.push_back(ch - 'A');
    }

    std::getline(file, line);
    return Chain(initial);
}

vector<Rule> readRules(std::ifstream &file)
{
    vector<Rule> rules;
    std::string line;
    while (std::getline(file, line)) {
        Rule rule;
        char ch;

        std::istringstream ss(line);
        ss >> ch;
        rule.a = ch - 'A';
        ss >> ch;
        rule.b = ch - 'A';
        ss >> ch >> ch >> ch >> ch;
        ss >> ch;
        rule.c = ch - 'A';
        rules.push_back(rule);
    }

    return rules;
}

int main()
{
    std::ifstream file("input");

    Chain chain = readChain(file);
    vector<Rule> rules = readRules(file);

    for (int i=0; i<40; i++) {
        chain.apply(rules);
    }

    chain.printSolution();
}
