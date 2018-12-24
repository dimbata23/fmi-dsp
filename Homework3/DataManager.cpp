#include <iostream>
#include "DataManager.hpp"

const std::string NO_KEY = "\n";


void DataManager::fill(std::ifstream& in) {

    std::string buffer;
    do {

        std::getline(in, buffer);

        if (!buffer.compare("[zones]")) {

            fillZones(in);

        } else if (!buffer.compare("[keys]")) {

            fillKeys(in);

        }

    } while(!in.eof());

}


void DataManager::fillZones(std::ifstream& in) {

    std::string zoneName1;
    std::string zoneName2;
    std::string keyName;
    std::string buffer;
    char ch;

    for (;!in.eof();) {

        if (in.peek() == '\n')
            break;

        zoneName1.clear();
        zoneName2.clear();
        keyName.clear();

        in >> zoneName1;

        for (;;) {
            in >> buffer;

            if (!buffer.compare("->"))
                break;

            zoneName1 += ' ';
            zoneName1 += buffer;
        }

        do {
            in.get(ch);
        } while(ch == ' ');

        zoneName2 += ch;
        do {
            in.get(ch);
            zoneName2 += ch;
        } while(ch != '[' && ch != '\n');
        zoneName2.pop_back();

        if (ch == '[') {
            zoneName2.pop_back();
            do {
                in.get(ch);
                keyName += ch;
            } while(ch != ']');
            keyName.pop_back();

            do {
                in.get(ch);
            } while(ch != '\n');
        }

        auto result = dict.insert(std::make_pair(zoneName1, Zone())).first;
        dict.insert(std::make_pair(zoneName2, Zone()));
        result->second.children.push_front(std::make_pair(zoneName2, (keyName.length() > 0) ? (keyName) : (NO_KEY)));

    }

}


void DataManager::fillKeys(std::ifstream& in) {

    std::string zoneName;
    std::string keyName;
    std::string buffer;
    char ch;

    for (;!in.eof();) {

        if (in.peek() == '\n')
            break;

        zoneName.clear();
        keyName.clear();

        in >> keyName;

        for (;;) {
            in >> buffer;

            if (!buffer.compare("->"))
                break;

            keyName += ' ';
            keyName += buffer;
        }

        do {
            in.get(ch);
        } while(in.peek() == ' ');

        std::getline(in, zoneName);

        auto result = dict.find(zoneName);
        if (result != dict.end())
            result->second.keys.push_back(keyName);

    }

}


bool DataManager::zoneExists(const std::string& name) {
    return (dict.find(name) != dict.end());
}


void DataManager::exploreZones(const std::string& starting) {

    std::unordered_set<std::string> inventory;
    std::queue<std::string> startingPoints;
    startingPoints.push(starting);

    BFS(startingPoints, inventory);

}


void DataManager::BFS(std::queue<std::string>& startingPoints, std::unordered_set<std::string>& inventory) {

    std::unordered_set<std::string> visited;
    std::queue<std::string> wave;
    wave.push(startingPoints.front());

    while (!wave.empty()) {

        std::string& curr = wave.front();
        visited.insert(curr);

        auto elem = dict.find(curr);
        elem->second.reachable = true;

        // get all the keys in the current zone
        for (const std::string& key : elem->second.keys) {
            // Maybe this will break the algorithm
            if (inventory.find(key) == inventory.end())
                startingPoints.push(curr);
            inventory.insert(key);
        }

        for (std::pair<std::string, std::string> child : elem->second.children) {

            // If it hasn't been visited
            if (visited.find(child.first) == visited.end()) {
                // If it doesn't require key or it requires key and we have the key
                if ((child.second == NO_KEY) ||
                    (child.second != NO_KEY && inventory.find(child.second) != inventory.end())) {

                   visited.insert(child.first);
                   wave.push(child.first);

                }

            }

        }
        wave.pop();
    }

    startingPoints.pop();
    if (!startingPoints.empty())
        BFS(startingPoints, inventory);

}


void DataManager::generateDotFile() const {

    std::ofstream out("output.gv");
    if (!out) {
        std::cout << "Couldn't create file output.gv!" << std::endl;
        return;
    }

    out << "digraph {" << std::endl;
    for ( const std::pair<std::string, Zone>& curr : dict ) {

        if (!curr.second.reachable || curr.second.keys.size() > 0) {
            out << curr.first << " [";

            if (curr.second.keys.size() > 0) {
                out << "label=\"" << curr.first;
                for ( const std::string& key : curr.second.keys )
                    out << "\\n" << key;
                out << '\"';
            }

            if (!curr.second.reachable) {
                if (curr.second.keys.size() > 0)
                    out << ',';
                out << "color=red,style=filled,fillcolor=\"#ffefef\"";
            }
            out << ']' << std::endl;

        }

        if (curr.second.children.size() > 0) {
            for ( const std::pair<std::string, std::string>& child : curr.second.children ) {
                out << curr.first << " -> " << child.first;
                if (child.second != NO_KEY)
                    out << " [label=\"" << child.second << "\"]";
                out << ';' << std::endl;
            }
        }

    }

    out << '}';
    out.close();

}
