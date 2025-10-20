#include "autocomplete.h"
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

void testBSTAll();

void Autocomplete::readFile(const string &fileName) {
  ifstream ifs(fileName);

  if (!ifs.is_open()) {
    cerr << "Error: could not open " << fileName << endl;
    return;
  }

  string line;
  while (getline(ifs, line)) {
    if (line.empty()) {
      continue;
    }
    stringstream ss(line);

    uint64_t weight = 0;
    if (!(ss >> weight)) {
      continue;
    }
    string key;

    getline(ss >> ws, key);
    if (key.empty()) {
      continue;
    }
    phrases[key] = weight;
  }
  ifs.close();
  phrases.rebalance();
}

vector<BSTMap::value_type> Autocomplete::complete(const string &prefix) const {
  auto v = phrases.getAll(prefix);
  sort(v.begin(), v.end(), sortByWeight);
  return v;
}

bool Autocomplete::sortByWeight(BSTMap::value_type &a, BSTMap::value_type &b) {
  return a.second > b.second;
}
