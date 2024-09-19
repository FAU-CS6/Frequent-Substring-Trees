using namespace std;

#include <cstring>
#include <iostream>
#include <list>
#include <stdexcept>

#include "classes/node.h"
#include "classes/fst.h"

// ---------------------------------------------------------------------------------------------
// Main function
// ---------------------------------------------------------------------------------------------
// FST is short for the temporary name of the project "Frequent Substring Trees".
//
// This is a standalone wrapper, that enables the user to interactively create and use an
// FST without the requirement of FSST or any other symbol-table-based compression algorithm.
//
int main()
{
     // Make an FST
     FST *fstc = new FST();

     // Build tree boolean
     bool build_tree = true;

     while (build_tree == true)
     {
          // Wait for user input
          std::string inp;
          cout << endl
               << "Enter a string to add to the FST: " << endl;
          std::cin >> inp;

          // Convert inp to a char array
          char *inp_char = new char[inp.length() + 1];
          strcpy(inp_char, inp.c_str());

          // Add a string to the FST
          fstc->addString(inp_char, strlen(inp_char));

          // Print the FST
          cout << endl
               << "The new FST: " << endl;
          fstc->printPaths();

          // Ask whether to add another string
          cout << endl
               << "Do you want to add another string? (y/n)" << endl;
          std::cin >> inp;

          // If the user does not want to add another string, stop the loop
          if (inp == "n")
               build_tree = false;
     }

     // Get the count of dictionary entries to find
     std::string inp;
     cout << endl
          << "Enter how many dictionary entries should be found: " << endl;
     std::cin >> inp;

     // Convert inp to int
     int nr_dict_entries = atoi(inp.c_str());

     // Get the count of dictionary entries to find
     std::string inp_lvl;
     cout << endl
          << "Enter up to which level nodes should be considered for replacement:"
          << endl
          << "(If only dictionary entries up to length 8 can be created, this "
             "should be 7 (length - 1))"
          << endl;
     std::cin >> inp_lvl;

     // Convert inp to int
     int max_level = atoi(inp_lvl.c_str());

     // Save the dictionary entries in a list of strings
     list<string> dict_entries;

     // Find nr_dict_entries dictonary entries
     for (int i = 0; i < nr_dict_entries; i++)
     {
          // Print the sorted gain list
          cout << endl
               << "The sorted gain list before finding the " << i + 1
               << "th dictionary entry: " << endl;
          list<Node *> sorted_gain_list = fstc->getSortedGainList(max_level);

          for (list<Node *>::iterator it = sorted_gain_list.begin();
               it != sorted_gain_list.end(); it++)
               cout << (*it)->getGain() << " " << (*it)->getFrequency() << " "
                    << (*it)->getSymbol() << " " << (*it)->getSubstring() << endl;

          // If the sorted gain list is empty, break the loop
          if (sorted_gain_list.size() < 1)
          {
               cout << "Not enough candidates => End the search" << endl;
               break;
          }

          // Get the node with the highest gain
          Node *highest_gain_node = sorted_gain_list.front();

          // Print the highest gain node
          cout << endl
               << "The highest gain node: " << endl;
          cout << highest_gain_node->getGain() << " "
               << highest_gain_node->getSymbol() << " "
               << highest_gain_node->getSubstring() << endl;

          // Add the highest gain node to the dictionary
          dict_entries.push_back(highest_gain_node->getSubstring());

          // Remove the highest gain node from the fstc list
          fstc->handleSubstringAddedToDict(highest_gain_node);

          // Print the FST after the handleSubstringAddedToDict function
          cout << endl
               << "The FrequentSubstringTree after the replacing the "
               << i + 1 << "th dictonary entry: " << endl;
          fstc->printPaths();
     }

     // Print the dictionary entries
     cout << endl
          << "The dictionary entries: " << endl;
     for (list<string>::iterator it = dict_entries.begin();
          it != dict_entries.end(); it++)
          cout << *it << endl;
}