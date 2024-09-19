using namespace std;

#include "../helpers/string_helpers.h"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <list>
#include <set>
#include <sstream>
#include <stdexcept>
#include <tuple>
#include <vector>

#include "node.h"

// ---------------------------------------------------------------------------------------------
// Class Node
// ---------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------
// Private functions
// -----------------------------------------------------------------------------------------

// --------------------------------------------------
// Setters
// --------------------------------------------------

void Node::setSymbol(char symbol)
{
  // Set the symbol of this Node
  this->symbol = symbol;
}

void Node::setFrequency(unsigned int frequency)
{
  // Set the frequency of the symbol of this Node
  this->frequency = frequency;
}

void Node::setLevel(unsigned int level)
{
  // Set the level of this Node
  this->level = level;
}

void Node::setOverlaps(unsigned int overlaps)
{
  // Set the overlaps of this Node
  this->overlaps = overlaps;
}

void Node::addChild(Node *node)
{
  // Add a child to the Node
  this->children.push_back(node);
}

void Node::removeChild(Node *node)
{
  // Remove a child from the Node
  for (unsigned int i = 0; i < this->children.size(); i++)
    if (this->children.at(i) == node)
      this->children.erase(this->children.begin() + i);

  // Delete the Node
  delete (node);
}

void Node::setParent(Node *node)
{
  // Set the parent of this Node
  this->parent = node;
}

// -----------------------------------------------------------------------------------------
// Public functions
// -----------------------------------------------------------------------------------------

// --------------------------------------------------
// Constructor and destructor
// --------------------------------------------------

Node::Node(const char &symbol, const unsigned int frequency,
           const unsigned int level, Node *parent)
{
  // Set all private variables
  setSymbol(symbol);
  setFrequency(frequency);
  setLevel(level);
  setParent(parent);
  setOverlaps(0);
}

Node::~Node()
{
  // Delete all children
  for (unsigned int i = 0; i < this->children.size(); i++)
    delete (this->children.at(i));
}

// --------------------------------------------------
// Getters
// --------------------------------------------------

char Node::getSymbol() const
{
  // Get the symbol of this Node
  return this->symbol;
}

unsigned int Node::getFrequency() const
{
  // Get the frequency of the symbol of this Node
  return this->frequency;
}

unsigned int Node::getLevel() const
{
  // Get the level of this node
  return this->level;
}

unsigned int Node::getOverlaps() const
{
  // Get the overlaps of this Node
  return this->overlaps;
}

unsigned int Node::getNrChildren() const
{
  // Get the number of children of this Node
  return this->children.size();
}

Node *Node::getChild(unsigned int i) const
{
  // Get the i-th child of this Node
  return this->children.at(i);
}

Node *Node::getChild(const char &symbol) const
{
  // Get the child of this Node with the given symbol
  for (unsigned int i = 0; i < this->children.size(); i++)
    if (this->children.at(i)->getSymbol() == symbol)
      return this->children.at(i);

  // If no Node with the given symbol was found, return NULL
  return nullptr;
}

list<Node *> Node::getDescendants(unsigned int max_level) const
{
  // Get all descendants of this Node
  list<Node *> descendants;

  // If the maximum level is not reached yet
  if (this->getLevel() < max_level)
  {
    // For all children of this Node
    for (unsigned int i = 0; i < this->children.size(); i++)
    {
      // Add the child to the list of descendants
      descendants.push_back(this->children.at(i));

      // Get the descendants of the child
      list<Node *> childDescendants =
          this->children.at(i)->getDescendants(max_level);

      // Add the descendants of the child to the list of descendants
      descendants.insert(descendants.end(), childDescendants.begin(),
                         childDescendants.end());
    }
  }

  return descendants;
}

Node *Node::getParent() const
{
  // Get the parent of this Node
  return this->parent;
}

list<Node *> Node::getAncestors() const
{
  // Get the ancestors of the Node
  list<Node *> ancestors;

  // Get the parent of the Node
  Node *parent = this->getParent();

  // If the parent is not NULL
  if (parent != NULL)
  {
    // Get the ancestors of the parent
    list<Node *> parentAncestors = parent->getAncestors();

    // Add the ancestors of the parent to the list of ancestors
    ancestors.insert(ancestors.end(), parentAncestors.begin(),
                     parentAncestors.end());

    // Add the parent to the list of ancestors
    ancestors.push_back(parent);
  }

  return ancestors;
}

Node *Node::getRoot() const
{
  // If this Node is the root, return this Node
  if (this->getParent() == NULL)
    return (Node *)this;

  // Get the root of the Node
  return this->getParent()->getRoot();
}

Node *Node::getNodeRepresentingSubstring(string substring) const
{
  // Check if the character of this node is the first character of the substring
  if (this->getSymbol() == substring.at(0))
  {
    // If the substring is only one character long, return this Node
    if (substring.length() == 1)
      return (Node *)this;

    // If the substring is longer than one character
    else
    {
      // Check if there is a child representing the substring the second
      // character of the substring
      Node *child_representing_substring = this->getChild(substring.at(1));

      if (child_representing_substring != NULL)
        return child_representing_substring->getNodeRepresentingSubstring(
            substring.substr(1));
      else
        return NULL;
    }
  }
  else
  {
    // If the character of this Node is not the first character of the
    // substring, return NULL
    return NULL;
  }
}

int Node::getGain() const
{
  // Compute and return the gain if the substring represented by this Node is
  // replaced Frequency * (level * size of a character - size of index value) -
  // (cost for dictonary entry) return (this->getFrequency() -
  // this->getOverlaps()) * (this->getLevel() * 1 - 1) - 9;
  return (this->getFrequency() - this->getOverlaps()) *
         (this->getLevel() * 1 - 1);
}

string Node::getSubstring() const
{
  // Get the substring represented by this Node
  string substring = "";

  // Get the parent of this Node
  Node *parent = this->getParent();

  // If the parent is not NULL
  if (parent != NULL)
  {
    // Get the substring of the parent
    substring = parent->getSubstring();
  }

  // Add the symbol of this Node to the substring
  substring += this->getSymbol();

  // Return the substring
  return substring;
}

// --------------------------------------------------
// Special getter to determine whether overlaps are possible
// --------------------------------------------------

bool Node::overlapsPossible() const
{
  // If this node is the root node, overlaps are not possible
  if (this->getParent() == NULL)
    return false;

  // Get the symbol of the root node
  char rootSymbol = this->getRoot()->getSymbol();

  // If the symbol of the root node is the same as the symbol of this node
  // overlaps are possible
  if (rootSymbol == this->getSymbol())
    return true;

  // Otherwise we have to check the ancestors of this node
  return this->getParent()->overlapsPossible();
}

// --------------------------------------------------
// Setters
// --------------------------------------------------

void Node::raiseFrequency(unsigned int increase)
{
  // Raise the frequency of this Node
  this->frequency += increase;
}

void Node::lowerFrequency(unsigned int decrease)
{
  // Lower the frequency of this Node
  // The frequency of a node can never be negative
  if (this->frequency < decrease)
    this->setFrequencyToZero();
  else
    this->frequency -= decrease;
}

void Node::setFrequencyToZero()
{
  // Delete the node
  this->frequency = 0;
}

void Node::raiseOverlaps(unsigned int increase)
{
  // Raise the overlaps of this Node
  this->overlaps += increase;
}

void Node::lowerOverlaps(unsigned int decrease)
{
  // Lower the overlaps of this Node
  // The overlaps of a node can never be negative
  if (this->overlaps < decrease)
    this->setOverlapsToZero();
  else
    this->overlaps -= decrease;
}

void Node::setOverlapsToZero()
{
  // Set the overlaps of this Node to zero
  this->overlaps = 0;
}

// --------------------------------------------------
// Adders
// --------------------------------------------------

void Node::addSubstring(char *substring, const unsigned int length)
{
  // If the substring is only as long as the current Node, raise the frequency
  if (length - 1 <= this->getLevel())
  {
    // Raise
    this->raiseFrequency(1);
    return;
  }
  else
  {
    // Raise the frequency of the Node
    this->raiseFrequency(1);

    // Check if there is a Node for the next character of the substring
    Node *node = this->getChild(substring[this->getLevel() + 1]);

    // If there is no Node for the first character of the substring, create one
    if (node == NULL)
    {
      // Create a new Node at level + 1
      node = new Node(substring[this->getLevel() + 1], 0, this->getLevel() + 1,
                      this);

      // Add the Node to the Node
      this->addChild(node);
    }

    // Call the addSubstring function of the Node
    node->addSubstring(substring, length);
  }
}

void Node::addSubstring(string substring)
{
  // Get the length of the substring
  unsigned int length = substring.length();

  // Get the char array of the substring
  char *charArray = new char[length + 1];
  strcpy(charArray, substring.c_str());

  // Call the addSubstring function
  this->addSubstring(charArray, length);

  // Delete the char array
  delete[] charArray;
}

void Node::addOverlaps(string str)
{
  // Check if overlaps are at all possible in this Node
  if (this->overlapsPossible())
  {
    // If yes get the substring of this Node
    string substring = this->getSubstring();

    // Check how often the substring can occur with and without overlaps within
    // the string
    unsigned int withoutOverlaps =
        string_helpers::number_of_occurrences_without_overlap(str, substring);
    unsigned int withOverlaps =
        string_helpers::number_of_occurrences_with_overlap(str, substring);

    // Compute the number of overlaps
    unsigned int overlaps = withOverlaps - withoutOverlaps;

    // Raise the overlaps of this Node
    this->raiseOverlaps(overlaps);
  }

  // Get the number of children of this Node
  unsigned int numberOfChildren = this->getNrChildren();

  // If there are children
  if (numberOfChildren > 0)
  {
    // Get the symbols contained in the string
    set<char> symbols = string_helpers::get_contained_chars(str);

    // For each symbol
    for (set<char>::iterator it = symbols.begin(); it != symbols.end(); it++)
    {
      // Get the child with the symbol
      Node *child = this->getChild(*it);

      // If there is a child with the symbol
      if (child != NULL)
      {
        // Call the addOverlaps function of the child
        child->addOverlaps(str);
      }
    }
  }
}

// --------------------------------------------------
// Subtractors
// --------------------------------------------------

void Node::subtractSubstring(string substring)
{
  // cout << "Node::subtractSubstring: " << substring << endl;

  // If the first character of the substring is the same as the symbol of this
  // Node
  if (substring[0] == this->getSymbol())
  {
    // If the substring is longer than the symbol of this Node
    if (substring.length() > 1)
    {
      // Get the child of this Node with the first character of the substring
      Node *child = this->getChild(substring[1]);

      // If there is a child with the first character of the substring
      if (child != NULL)
      {
        // Call the subtractSubstring function of the child
        child->subtractSubstring(substring.substr(1));

        // If the frequency of the child is zero, delete the child
        if (child->getFrequency() == 0)
        {
          this->removeChild(child);
        }
      }
      else
      {
        // Throw an exception
        throw runtime_error("Node::subtractSubstring: The substring " +
                            substring + " is not part of the tree");
      }
    }

    // Remove one frequency from this Node (after the child has been removed to
    // be sure that there was no exception)
    this->lowerFrequency(1);
  }
  else
  {
    // Throw an exception
    throw runtime_error("Node::subtractSubstring: The substring " + substring +
                        " is not part of the tree");
  }
}

void Node::subtractOverlaps(string str)
{
  // Check if overlaps are at all possible in this Node
  if (this->overlapsPossible())
  {
    // If yes get the substring of this Node
    string substring = this->getSubstring();

    // Check how often the substring can occur with and without overlaps within
    // the string
    unsigned int withoutOverlaps =
        string_helpers::number_of_occurrences_without_overlap(str, substring);
    unsigned int withOverlaps =
        string_helpers::number_of_occurrences_with_overlap(str, substring);

    // Compute the number of overlaps
    unsigned int overlaps = withOverlaps - withoutOverlaps;

    // Lower the overlaps of this Node
    this->lowerOverlaps(overlaps);
  }

  // Get the number of children of this Node
  unsigned int numberOfChildren = this->getNrChildren();

  // If there are children
  if (numberOfChildren > 0)
  {
    // Get the symbols contained in the string
    set<char> symbols = string_helpers::get_contained_chars(str);

    // For each symbol
    for (set<char>::iterator it = symbols.begin(); it != symbols.end(); it++)
    {
      // Get the child with the symbol
      Node *child = this->getChild(*it);

      // If there is a child with the symbol
      if (child != NULL)
      {
        // Call the subtractOverlaps function of the child
        child->subtractOverlaps(str);
      }
    }
  }
}

// --------------------------------------------------
// Functions to modify occurrence counts when a substring is added to the
// dictionary
// --------------------------------------------------

list<string> Node::getRepresentedSubstrings() const
{
  // Get the strings represented by this Node
  list<string> strings;

  // If the Node is a leaf
  if (this->getNrChildren() == 0)
  {
    // Cache the substring
    string substring = this->getSubstring();

    // Add the substring frequency times to the list of strings
    for (unsigned int i = 0; i < this->getFrequency(); i++)
    {
      strings.push_back(substring);
    }
  }
  else
  {
    // For all children of this Node
    for (unsigned int i = 0; i < this->getNrChildren(); i++)
    {
      // Get the strings represented by the child
      list<string> childStrings = this->getChild(i)->getRepresentedSubstrings();

      // Add the strings represented by the child to the list of strings
      strings.splice(strings.end(), childStrings);
    }

    // There might also be substrings ending at this Node
    // Cache the own substring
    string substring = this->getSubstring();

    // Get the size of strings
    unsigned int size = strings.size();

    // Add the substring (frequency - size) times to the list of strings
    for (unsigned int i = 0; i < (this->getFrequency() - size); i++)
    {
      strings.push_back(substring);
    }
  }

  return strings;
}

tuple<int, list<string>>
Node::deleteFullStringOccurrencesStartingAtThisNode(string substring,
                                                    unsigned int pos)
{
  // Throw an exception if pos is higher than the length of the substring
  if (pos > substring.length())
  {
    throw runtime_error("Node::deleteFullStringOccurrencesHelper: pos is higher "
                        "than the length of the substring");
  }

  // If the pos-th character of the substring is the symbol of this Node
  if (substring[pos] == this->getSymbol())
  {
    // If pos is the last character of the substring
    if (pos == substring.length() - 1)
    {
      // Get the frequency of this Node
      int frequency = this->getFrequency();

      // Get all the substrings represented by this Node
      list<string> strings = this->getRepresentedSubstrings();

      // Set the frequency of this Node to zero
      this->setFrequencyToZero();

      // Return the frequency
      return {frequency, strings};
    }
    else
    {
      // Get the child of this Node with the next character of the substring
      Node *child = this->getChild(substring[pos + 1]);

      // If there is a child with the next character of the substring
      if (child != NULL)
      {
        // Call the deleteFullStringOccurrencesStartingAtThisNode function of the
        // child with the substring and the next position
        tuple<int, list<string>> return_value =
            child->deleteFullStringOccurrencesStartingAtThisNode(substring,
                                                                 pos + 1);

        // If the substring was found
        if (get<0>(return_value) > 0)
        {
          // Lower the frequency of this Node by the frequency of the substring
          this->lowerFrequency(get<0>(return_value));

          // If the frequency of the child is zero, delete the child
          if (child->getFrequency() == 0)
          {
            this->removeChild(child);
          }

          // Return the frequency and the represented_substrings of the
          // substring
          return return_value;
        }
      }
    }
  }

  // Return -1 if the substring was not found
  return {-1, list<string>()};
}

void Node::deleteFullStringOccurrences(string substring)
{
  // If the first character of the substring is the symbol of this Node
  if (substring[0] == this->getSymbol())
  {
    // Check if there are occurrences of the substring starting at this Node (and
    // delete them)
    deleteFullStringOccurrencesStartingAtThisNode(substring, 0);
  }

  // Pass the call to all children
  for (int i = this->getNrChildren() - 1; i >= 0; i--)
  {
    // Get the child
    Node *child = this->getChild(static_cast<unsigned int>(i));

    // If the child is not NULL
    if (child != NULL)
    {
      // Call the deleteFullStringOccurrences function of the child
      child->deleteFullStringOccurrences(substring);

      // If the frequency of the child is zero, delete the child
      if (child->getFrequency() == 0)
      {
        this->removeChild(child);
      }
    }
  }

  // Overlaps correction
  if (this->getOverlaps() > 0)
  {
    // Set the overlaps to zero
    this->setOverlapsToZero();

    // Get all substrings this node represents
    list<string> represented_substrings = this->getRepresentedSubstrings();

    // Get the original strings by removing contained substrings
    list<string> original_strings =
        string_helpers::delete_contained_substrings(represented_substrings);

    // For all original strings
    for (string original_string : original_strings)
    {
      // Add the overlaps again
      this->addOverlaps(original_string);
    }
  }
}

// --------------------------------------------------
// Comparison operators
// --------------------------------------------------

bool Node::operator==(const Node &node) const
{
  // Is true if all attributes are equal and the children are equal
  // Check if the symbol is equal
  if (this->getSymbol() != node.getSymbol())
  {
    return false;
  }

  // Check if the frequency is equal
  if (this->getFrequency() != node.getFrequency())
  {
    return false;
  }

  // Check if the overlaps are equal
  if (this->getOverlaps() != node.getOverlaps())
  {
    return false;
  }

  // Check if the level is equal
  if (this->getLevel() != node.getLevel())
  {
    return false;
  }

  // Check if there are the same number of children
  if (this->getNrChildren() != node.getNrChildren())
  {
    return false;
  }

  // Check if the children are equal
  for (unsigned int i = 0; i < this->getNrChildren(); i++)
  {
    // Get the children of this
    Node *child1 = this->getChild(i);

    // Get the children with the same symbol of node
    Node *child2 = node.getChild(child1->getSymbol());

    // If the children are not equal
    if (*child1 != *child2)
    {
      return false;
    }
  }

  // If all attributes are equal and the children are equal, the Nodes are equal
  return true;
}

bool Node::operator!=(const Node &node) const
{
  // Is true if the Nodes are not equal
  return !(*this == node);
}

bool Node::operator<(const Node &node) const
{
  // Is compared using the gain
  return this->getGain() < node.getGain();
}

bool Node::operator>(const Node &node) const
{
  // Is compared using the gain
  return this->getGain() > node.getGain();
}

bool Node::operator<=(const Node &node) const
{
  // Is compared using the gain
  return this->getGain() <= node.getGain();
}

bool Node::operator>=(const Node &node) const
{
  // Is compared using the gain
  return this->getGain() >= node.getGain();
}

// --------------------------------------------------
// Printers
// --------------------------------------------------

void Node::printTree()
{
  // Print the Node
  for (unsigned int i = 0; i < this->getLevel(); i++)
  {
    if (i == 0)
      cout << "|";
    else
      cout << "-";

    if (i == this->getLevel() - 1)
      cout << " ";
    else
      cout << "-";
  }

  cout << this->getSymbol() << ": " << this->getFrequency() << " (-"
       << this->getOverlaps() << ")" << endl;

  // Print all children
  for (unsigned int i = 0; i < this->getNrChildren(); i++)
    this->getChild(i)->printTree();
}

string Node::toString()
{
  stringstream ss;

  // Print the Node
  for (unsigned int i = 0; i < this->getLevel(); i++)
  {
    if (i == 0)
      ss << "|";
    else
      ss << "-";

    if (i == this->getLevel() - 1)
      ss << " ";
    else
      ss << "-";
  }

  ss << this->getSymbol() << ": " << this->getFrequency() << " (-"
     << this->getOverlaps() << ")"
     << "\n";

  // Print all children
  for (unsigned int i = 0; i < this->getNrChildren(); i++)
    ss << this->getChild(i)->toString();

  return ss.str();
}

// ---------------------------------------------------------------------------------------------
// Struct NodeCompare used to sort a list of Node pointers based on the gain of
// the nodes itself
// ---------------------------------------------------------------------------------------------

bool NodeCompare::operator()(const Node *node1, const Node *node2) const
{
  // Is compared using the gain
  return *node1 > *node2;
}
