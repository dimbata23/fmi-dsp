#pragma once
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <queue>
#include <vector>
#include <string>
#include <fstream>

class DataManager {

private:

    struct Zone {
        Zone() : reachable(false) {}
        /// Pairs of zoneName and keyNeeded to go there
        std::list<std::pair<std::string, std::string>> children;
        std::vector<std::string> keys;
        bool reachable;
    };

public:

    void fill(std::ifstream& in);
    void exploreZones(const std::string& starting);
    void generateDotFile() const;
    bool zoneExists(const std::string& name);

private:

    void fillZones(std::ifstream& in);
    void fillKeys(std::ifstream& in);
    void BFS(std::queue<std::string>& startingPoints, std::unordered_set<std::string>& inventory);

private:

    std::unordered_map<std::string, Zone> dict;

};
