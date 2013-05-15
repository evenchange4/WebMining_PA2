#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include "getopt.h"

using namespace std;

template<typename K, typename V, typename Inserter>
void loadPair(const string& filename, Inserter iter) {
    ifstream in(filename.c_str());
    K key;
    V value; 
    char colon;

    while (in >> key >> colon >> value) *iter++ = make_pair(key, value);
}

template<typename Iterator1, typename Iterator2, typename Type>
Type l1_norm(Iterator1 first1, Iterator1 last1, Iterator2 first2, Type init) {
    Type result = init;
    while (first1 != last1) {
	if (first1->first != first2->first) throw runtime_error("Two postings do not match");
	Type diff = first1++->second - first2++->second;
	result += (diff > 0)? diff: -diff;
    }
    return result;
}

template<typename Iterator1, typename Iterator2, typename Type>
Type sse(Iterator1 first1, Iterator1 last1, Iterator2 first2, Type init) {
    Type result = init;
    while (first1 != last1) {
	if (first1->first != first2->first) throw runtime_error("Two postings do not match");
	Type diff = first1++->second - first2++->second;
	result += diff * diff;
    }
    return result;
}

template<typename Iterator1, typename Iterator2, typename Type>
Type rho(Iterator1 first1, Iterator1 last1, Iterator2 first2, Type init) {
    unsigned int n = last1 - first1;
    Type d2sum = sse(first1, last1, first2, init);
    return 1 - 6 * (d2sum / n / (n + 1) / (n - 1)); // Numerically safe way...
}

template<typename P> bool value_compare(const P& a, const P& b) { 
    return a.second < b.second; 
}

struct save_rank_to_value {
    unsigned int rank;
    save_rank_to_value(): rank(1) {}
    template<typename P> void operator()(P& a) { a.second = rank++; }
};

int main(int argc, char** argv) {
    irlab::Getopt g(argc, argv);
    g.setAutoHelp(true);
    g.setUsage("file1 file2");
    g.setSummary("Calculate various rank correlation coefficients.");
    g.addOption("sort", "", "s", "BOOL", "Sort the posting in advance", "0");

    map<string,string> opt;
    vector<string> cmdline;
    if (!g.getOptions(opt, cmdline)) return 1;

    if (cmdline.size() != 2) throw runtime_error("We need two postings");

    typedef unsigned int Key;
    typedef double Value;
    typedef pair<Key,Value> Pair;

    vector<pair<Key,Value> > p1, p2;
    loadPair<Key,Value>(cmdline[0], back_inserter(p1));
    loadPair<Key,Value>(cmdline[1], back_inserter(p2));

    if (opt["sort"] == "1") {
	stable_sort(p1.begin(), p1.end());
	stable_sort(p2.begin(), p2.end());
    }

    cout << "L1 norm: " << l1_norm(p1.begin(), p1.end(), p2.begin(), 0.0) << endl;

    // ``Double sort'' trick!
    stable_sort(p1.begin(), p1.end(), ptr_fun(value_compare<Pair>));
    stable_sort(p2.begin(), p2.end(), ptr_fun(value_compare<Pair>));
    for_each(p1.begin(), p1.end(), save_rank_to_value());
    for_each(p2.begin(), p2.end(), save_rank_to_value());
    stable_sort(p1.begin(), p1.end());
    stable_sort(p2.begin(), p2.end());

    cout << "Spearman's rho: " << rho(p1.begin(), p1.end(), p2.begin(), 0.0) << endl;

    return 0;
}
