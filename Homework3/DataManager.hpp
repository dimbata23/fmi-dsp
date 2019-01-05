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
        // Pairs of zone's name and the key needed to go there
        std::list<std::pair<std::string, std::string>> children;
        // All the keys in the zone
        std::vector<std::string> keys;
        // Can be reached by the algorithm
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
    void BFS(const std::string& startingPoint, const std::unordered_set<std::string>& inventory);

    void readFirstLabel(std::ifstream& in, std::string& name) const;
    void readSecondLabel(std::ifstream& in, std::string& name) const;
    void readBracketsLabel(std::ifstream& in, std::string& name) const;

private:

    std::unordered_map<std::string, Zone> dict;

};
