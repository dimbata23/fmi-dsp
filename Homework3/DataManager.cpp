#include <iostream>
#include "DataManager.hpp"
#include "HelperFunctions.hpp"


const std::string NO_KEY = "\n";


void DataManager::fill(std::ifstream& in) {

    std::string buffer;
    do {

        std::getline(in, buffer);
        help::trim(buffer);

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

    while (!in.eof()) {

        while (std::isspace(in.peek())) {
            // Stop filling zones if new line is encountered
            if (in.peek() == '\n')
                return;
            in.get();
        }

        zoneName1.clear();
        zoneName2.clear();
        keyName.clear();

        readFirstLabel(in, zoneName1);
        readSecondLabel(in, zoneName2);

        // If there is a key needed for the zone
        if (in.peek() == '[')
            readBracketsLabel(in, keyName);

        // Insert the first zone in the hash map
        auto result = dict.insert(std::make_pair(zoneName1, Zone())).first;
        // Insert the first second zone in the hash map
        dict.insert(std::make_pair(zoneName2, Zone()));
        // Insert the second zone as a possible path from zone1 to zone2
        result->second.children.push_front(
            std::make_pair(
                zoneName2, 
                (keyName.length() > 0) ? (keyName) : (NO_KEY)
            )
        );

        // Get rid of anything else left on that line (esp. the ';')
        std::getline(in, zoneName1);

    }

}


void DataManager::fillKeys(std::ifstream& in) {

    std::string keyName;
    std::string zoneName;

    while (!in.eof()) {

        while (std::isspace(in.peek())) {
            // Stop filling keys if new line is encountered
            if (in.peek() == '\n' || in.eof())
                return;
            in.get();
        }

        // Stop filling keys if we've reached the end of the file
        if (in.eof())
            return;

        keyName.clear();
        zoneName.clear();

        // Reading key name
        readFirstLabel(in, keyName);

        // Read key name until ';'
        std::getline(in, zoneName, ';');
        // Remove quotations incase there were any
        if (zoneName[0] == '\"')
            zoneName = zoneName.substr(1, zoneName.length() - 2);

        // Find the zone..
        auto result = dict.find(zoneName);
        // ..and add the key to the zone
        if (result != dict.end())
            result->second.keys.push_back(keyName);

        // Get rid of anything else left on that line
        std::getline(in, zoneName);

    }

}


bool DataManager::zoneExists(const std::string& name) {
    return (dict.find(name) != dict.end());
}


void DataManager::exploreZones(const std::string& starting) {

    std::unordered_set<std::string> inventory;
    BFS(starting, inventory);

}


void DataManager::BFS(const std::string& startingPoint, const std::unordered_set<std::string>& inventory) {

    std::unordered_set<std::string> visited;
    std::queue<std::string> wave;
    wave.push(startingPoint);

    while (!wave.empty()) {

        // Get the current vertex and add it to visited
        std::string& curr = wave.front();
        visited.insert(curr);

        // Set it's status to reachable
        auto elem = dict.find(curr);
        elem->second.reachable = true;

        // Get all the keys in the current zone
        for (const std::string& key : elem->second.keys) {
            // If we don't already have the key
            if (inventory.find(key) == inventory.end()) {
                // Start the algorithm again from the current zone
                // with the new key added to the inventory
                std::unordered_set<std::string> newInventory = inventory;
                newInventory.insert(key);
                BFS(curr, newInventory);
            }
        }

        // Going through all the children of the current zone
        for (const std::pair<std::string, std::string>& child : elem->second.children) {
            // If it hasn't been visited
            if (visited.find(child.first) == visited.end()) {
                // If it doesn't require a key or it requires a key and we have the key
                if ( (child.second == NO_KEY) ||
                     (child.second != NO_KEY && 
                      inventory.find(child.second) != inventory.end()
                    ) ) {
		                visited.insert(child.first);
		                wave.push(child.first);
                }
            }
        }

        // Remove the zone we finished working with
        wave.pop();
    }

}


void DataManager::generateDotFile() const {

    std::ofstream out("output.gv");
    if (!out) {
        std::cout << "Couldn't create file output.gv!" << std::endl;
        return;
    }

    out << "digraph {" << std::endl << std::endl;
    for ( const std::pair<std::string, Zone>& curr : dict ) {

        if (!curr.second.reachable || curr.second.keys.size() > 0) {
            out << "\t\"" << curr.first << "\" [";

            if (curr.second.keys.size() > 0) {
                out << "label = \"" << curr.first;
                for ( const std::string& key : curr.second.keys )
                    out << "\\n" << key;
                out << '\"';
            }

            if (!curr.second.reachable) {
                if (curr.second.keys.size() > 0)
                    out << ", ";
                out << "color = red, style = filled, fillcolor = \"#ffefef\"";
            }
            out << "];" << std::endl;

        }

        if (curr.second.children.size() > 0) {
            for ( const std::pair<std::string, std::string>& child : curr.second.children ) {
                out << "\t\"" << curr.first << "\" -> \"" << child.first << '\"';
                if (child.second != NO_KEY)
                    out << " [label = \"" << child.second << "\"]";
                out << ';' << std::endl;
            }
        }

        out << std::endl;

    }

    out << '}';
    out.close();

}


void DataManager::readFirstLabel(std::ifstream& in, std::string& name) const {

    std::string buffer;

    // If we have a quoted name
    if (in.peek() == '\"') {
        // Remove the quote
        in.get();
        // Read the name until '"'
        std::getline(in, name, '\"');
    } else {
        // If it's not then we just read the name as one word
        in >> name;
    }

    for (;;) {
        in >> buffer;

        // If we encounter a "->" we stop reading 
        if (!buffer.compare("->"))
            break;

        // Just in case if the have read anything else
        // other than "->", we'll just add it to the name
        name += ' ';
        name += buffer;
    }

    // Remove any white spaces before the next label
    while(std::isspace(in.peek()))
        in.get();

}


void DataManager::readSecondLabel(std::ifstream& in, std::string& name) const {

    // If we have a quoted name
    if (in.peek() == '\"') {
        // Remove the quote
        in.get();
        // Read the name until a '"'
        std::getline(in, name, '\"');

        // Remove anything else until a '[' or a ';'
        while(in.peek() != '[' && in.peek() != ';') {
            in.get();
        }

        return;
    }

    // If it's not a quoted name
    char ch;

    // Read anything until a '[' or a ';' and add it to the name
    do {
        in.get(ch);
        name += ch;
    } while(in.peek() != '[' && in.peek() != ';');

    // Trim any unwanted spaces (from the front or back)
    help::trim(name);

}


void DataManager::readBracketsLabel(std::ifstream& in, std::string& name) const {

    in.get(); // Getting rid of '['
    std::getline(in, name, ']');

}
