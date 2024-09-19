using namespace std;

#include "fst.h"
#include "../helpers/string_helpers.h"
#include "node.h"
#include <cstring>
#include <iostream>
#include <list>
#include <sstream>
#include <stdexcept>
#include <string>

// ---------------------------------------------------------------------------------------------
// Class FST
// ---------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------
// Private functions
// -----------------------------------------------------------------------------------------

// --------------------------------------------------
// Simple setters
// --------------------------------------------------

void FST::addPath(Node *node)
{
  // Add a new path
  this->paths.push_back(node);
}

void FST::removePath(Node *node)
{
  // Remove a child from the FST
  for (unsigned int i = 0; i < this->paths.size(); i++)
    if (this->paths.at(i) == node)
      this->paths.erase(this->paths.begin() + i);

  // Delete the Node
  delete (node);
}

// --------------------------------------------------
// Adders
// --------------------------------------------------

void FST::addSubstring(char *substring, unsigned int length)
{
  // Check if there is a node for the first character of the substring
  Node *root_node = this->getPath(substring[0]);

  // If there is no node for the first character of the substring, create one
  if (root_node == NULL)
  {
    root_node = new Node(substring[0], 0, 0, NULL);
    this->addPath(root_node);
  }

  // Call the addSubstring function of the root node
  root_node->addSubstring(substring, length);
}

void FST::addOverlaps(string string)
{
  // Get the symbols contained in the string
  set<char> symbols = string_helpers::get_contained_chars(string);

  // For each symbol
  for (set<char>::iterator it = symbols.begin(); it != symbols.end(); it++)
  {
    // Get the root node of the path with the symbol
    Node *root_node = this->getPath(*it);

    // If there is a root node for the symbol
    if (root_node != NULL)
    {
      // Call the addOverlaps function of the root node
      root_node->addOverlaps(string);
    }
  }
}

// --------------------------------------------------
// Subtractors
// --------------------------------------------------

void FST::subtractOverlaps(string string)
{
  // Get the symbols contained in the string
  set<char> symbols = string_helpers::get_contained_chars(string);

  // For each symbol
  for (set<char>::iterator it = symbols.begin(); it != symbols.end(); it++)
  {
    // Get the root node of the path with the symbol
    Node *root_node = this->getPath(*it);

    // If there is a root node for the symbol
    if (root_node != NULL)
    {
      // Call the subtractOverlaps function of the root node
      root_node->subtractOverlaps(string);
    }
  }
}

// -----------------------------------------------------------------------------------------
// Public functions
// -----------------------------------------------------------------------------------------

// --------------------------------------------------
// Constructor and destructor
// --------------------------------------------------

FST::FST() {}

FST::~FST()
{
  // Delete all paths
  for (unsigned int i = 0; i < this->paths.size(); i++)
    delete (this->paths.at(i));
}

// --------------------------------------------------
// Getters
// --------------------------------------------------

unsigned int FST::getNrPaths() const
{
  // Get the number of paths
  return this->paths.size();
}

Node *FST::getPath(unsigned int i) const
{
  // Get the i-th path
  return this->paths.at(i);
}

Node *FST::getPath(const char &symbol) const
{
  // Search through all paths and return the one with the given symbol
  for (unsigned int j = 0; j < this->paths.size(); j++)
    if (this->paths.at(j)->getSymbol() == symbol)
      return this->paths.at(j);

  // If no path with the given symbol was found, return NULL
  return NULL;
}

Node *FST::getNodeRepresentingSubstring(string substring) const
{
  // Check if there is a Node for the first character of the substring
  Node *root_node = this->getPath(substring[0]);

  // If there is a Node for the first character of the substring, call the
  // getNodeRepresentingSubstring function of the root node
  if (root_node != NULL)
    return root_node->getNodeRepresentingSubstring(substring);
  else
    return NULL;
}

list<Node *> FST::getSortedGainList(unsigned int max_level) const
{
  // Create a list of all Nodes
  list<Node *> nodes;

  // For all paths get the descendents and add them to the list
  for (unsigned int i = 0; i < this->getNrPaths(); i++)
  {
    nodes.splice(nodes.end(), this->getPath(i)->getDescendants(max_level));
  }

  // Sort the list
  nodes.sort(NodeCompare());

  // Return the sorted list
  return nodes;
}

// --------------------------------------------------
// Get dictionary entries (and remove them from the FST)
// --------------------------------------------------

list<string> FST::getDictionaryEntries(unsigned int x,
                                       unsigned int max_level)
{
  // Save the dictionary entries in a list of strings
  list<string> dict_entries;

  // Find x dictionary entries
  for (unsigned int i = 0; i < x; i++)
  {
    // Get the sorted gain list
    list<Node *> sorted_gain_list = this->getSortedGainList(max_level);

    // If the sorted gain list is empty, break the loop
    if (sorted_gain_list.size() < 1)
    {
      break;
    }

    // Get the node with the highest gain
    Node *highest_gain_node = sorted_gain_list.front();

    // Add the highest gain node to the dictionary
    dict_entries.push_back(highest_gain_node->getSubstring());

    // Remove the highest gain node from the FST
    this->handleSubstringAddedToDict(highest_gain_node);
  }

  // Return the list of dictionary entries
  return dict_entries;
}

// --------------------------------------------------
// Remove substring from the FST
// --------------------------------------------------

void FST::subtractSubstring(string substring)
{
  // Check if there is a Node for the first character of the substring
  Node *root_node = this->getPath(substring[0]);

  // If there is a Node for the first character of the substring, call the
  // removeSubstring function of the root Node
  if (root_node != NULL)
  {
    root_node->subtractSubstring(substring);

    // If the root node has a frequency of 0, remove it
    if (root_node->getFrequency() == 0)
      this->removePath(root_node);
  }
  else
  {
    // Throw an exception
    throw runtime_error(
        "FST::subtractSubstring: No path for substring " +
        substring);
  }
}

// --------------------------------------------------
// Add string to the FST
// --------------------------------------------------

void FST::addString(char *string, unsigned int length)
{
  // For each character in the string create a substring (starting the the i-th
  // character)
  for (unsigned int i = 0; i < length; i++)
  {
    // Create a substring starting with the i-th character
    char *chars_to_add = new char[length - i];
    strncpy(chars_to_add, (string + i), length - i);

    // Add the substring to the FST
    this->addSubstring(chars_to_add, length - i);
  }

  // Add overlaps
  this->addOverlaps(string);
}

void FST::addString(string str)
{
  // Convert the string to a char array
  char *string = new char[str.length() + 1];
  strcpy(string, str.c_str());

  // Add the string to the FST
  this->addString(string, str.length());

  // Delete the char array
  delete (string);
}

// --------------------------------------------------
// Add a list of strings to the FST
// --------------------------------------------------

void FST::addStrings(list<string> strings)
{
  // For each string in the list
  for (list<string>::iterator it = strings.begin(); it != strings.end(); it++)
  {
    // Add the string to the FST
    this->addString(*it);
  }
}

// --------------------------------------------------
// Add a substring to the dictionary
// (remove the corresponding occurrences from the FST)
// --------------------------------------------------

void FST::deleteFullStringOccurrences(string substring)
{
  // For all paths in the FST
  for (Node *path : this->paths)
  {
    // Call the deleteFullStringOccurrences function of the path
    path->deleteFullStringOccurrences(substring);
  }

  // Remove all paths with a frequency of 0
  for (unsigned int i = 0; i < this->getNrPaths(); i++)
  {
    // Get the i-th path
    Node *path = this->getPath(i);

    // If the path has a frequency of 0, remove it
    if (path->getFrequency() == 0)
    {
      // Remove the path
      this->removePath(path);

      // Reset i to 0 to start over
      i = 0;
    }
  }
}

void FST::handleSubstringAddedToDict(Node *node)
{
  // Get the substring the node ends
  string substring = node->getSubstring();

  // Get the represented substrings of the node (so all substrings starting with
  // "substring" but potentially longer)
  list<string> substrings = node->getRepresentedSubstrings();

  // Get the original strings by removing contained substrings
  list<string> original_strings =
      string_helpers::delete_contained_substrings(substrings);

  // For all original_strings in the list subtract the overlaps
  for (string original_string : original_strings)
  {
    // Subtract the overlaps
    this->subtractOverlaps(original_string);
  }

  // Get all substrings that need to be deleted
  list<string> substrings_to_delete =
      string_helpers::get_substrings_to_delete(substring, original_strings);

  // For all substrings_to_delete substract the substring
  for (string substring_to_delete : substrings_to_delete)
  {
    // For each character in the modified represented substring subtract the
    // corresponding subsubstring (starting the the i-th character) However, do
    // not subtract more than the length of the substring itself
    for (unsigned int i = 1;
         i < min(substring_to_delete.length(), substring.length()); i++)
    {
      // Subtract the substring_to_delete starting with the i-th character
      this->subtractSubstring(substring_to_delete.substr(i));
    }
  }

  // Delete all occurrences of the substring from the FST
  this->deleteFullStringOccurrences(substring);
}

// --------------------------------------------------
// Printers
// --------------------------------------------------

void FST::printPaths()
{
  // Print all paths
  for (unsigned int i = 0; i < this->getNrPaths(); i++)
  {
    // Use the nodes print function to print the FST
    this->getPath(i)->printTree();

    // Add a extra line between the paths
    cout << endl;
  }
}

string FST::toString()
{
  stringstream ss;

  // Print all paths
  for (unsigned int i = 0; i < this->getNrPaths(); i++)
  {
    // Use the nodes toString function to print the path
    ss << this->getPath(i)->toString();

    // Add a extra line between the paths
    ss << "\n";
  }

  return ss.str();
}

// --------------------------------------------------
// Comparison operators
// --------------------------------------------------

bool FST::operator==(const FST &tc) const
{
  // Is true the paths of the FSTs are equal
  // Check if the number of paths is equal
  if (this->getNrPaths() != tc.getNrPaths())
    return false;

  // Check if all paths are equal
  for (unsigned int i = 0; i < this->getNrPaths(); i++)
  {
    // Get the i-th path of the FST
    Node *path = this->getPath(i);

    // Get the path with the same symbol
    Node *path_tc = tc.getPath(path->getSymbol());

    // If the path is not equal, the FSTs are not equal
    if (*path != *path_tc)
      return false;
  }

  // If all attributes are equal and the children are equal, the Nodes are equal
  return true;
}

bool FST::operator!=(const FST &tc) const
{
  // Is true if the FSTs are not equal
  return !(*this == tc);
}
