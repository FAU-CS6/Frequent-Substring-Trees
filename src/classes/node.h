#ifndef NODE_H
#define NODE_H

using namespace std;

#include <list>
#include <string>
#include <tuple>
#include <vector>

#include "../helpers/string_helpers.h"

// ---------------------------------------------------------------------------------------------
// Class Node
// ---------------------------------------------------------------------------------------------

/**
 * @class Node
 * @brief Represents a node in a tree.
 */
class Node
{
private:
  /**
   * @brief The frequency of occurrence of this symbol.
   */
  unsigned int frequency;

  /**
   * @brief Level in the tree hierarchy (zero-indexed: root = 0; child of root =
   * 1, etc.).
   */
  unsigned int level;

  /**
   * @brief Number of overlaps (= number of times the substring represented by
   * this node can not be replaced in the original data due to overlaps with
   * another occurrence of this substring).
   */
  unsigned int overlaps;

  /**
   * @brief The symbol this node represents.
   */
  char symbol;

  /**
   * @brief The children of this node.
   */
  std::vector<Node *> children;

  /**
   * @brief The parent node of this node.
   */
  Node *parent;

  // --------------------------------------------------
  // Setters
  // --------------------------------------------------

  /**
   * @brief Sets the symbol of this node.
   * @param symbol The symbol to set.
   */
  void setSymbol(char symbol);

  /**
   * @brief Sets the frequency of this node.
   * @param frequency The frequency to set.
   */
  void setFrequency(unsigned int frequency);

  /**
   * @brief Sets the level of this node.
   * @param level The level to set.
   */
  void setLevel(unsigned int level);

  /**
   * @brief Sets the number of overlaps of this node.
   * @param overlaps The number of overlaps to set.
   */
  void setOverlaps(unsigned int overlaps);

  /**
   * @brief Adds a child node to this node.
   * @param node The node to add.
   */
  void addChild(Node *node);

  /**
   * @brief Removes a child node from this node.
   * @param node The node to remove.
   */
  void removeChild(Node *node);

  /**
   * @brief Sets the parent of this node.
   * @param node The node to set as parent.
   */
  void setParent(Node *node);

public:
  /**
   * @brief Constructs a node.
   * @param symbol The symbol this node represents.
   * @param frequency The frequency of occurrence of this symbol.
   * @param level The level of this node in the tree hierarchy.
   * @param parent The parent node of this node.
   */
  Node(const char &symbol, const unsigned int frequency,
       const unsigned int level, Node *parent);

  /**
   * @brief Destructs the node.
   */
  virtual ~Node(void);

  // --------------------------------------------------
  // Getters
  // --------------------------------------------------

  /**
   * @brief Gets the symbol this node represents.
   * @return The symbol.
   */
  char getSymbol() const;

  /**
   * @brief Gets the frequency of occurrence of this symbol.
   * @return The frequency.
   */
  unsigned int getFrequency() const;

  /**
   * @brief Gets the level of this node in the tree hierarchy.
   * @return The level.
   */
  unsigned int getLevel() const;

  /**
   * @brief Gets the number of overlaps.
   * @return The number of overlaps.
   */
  unsigned int getOverlaps() const;

  /**
   * @brief Gets the number of children of this node.
   * @return The number of children.
   */
  unsigned int getNrChildren() const;

  /**
   * @brief Gets the child node by its ID.
   * @param i The ID of the child node.
   * @return The child node.
   */
  Node *getChild(unsigned int i) const;

  /**
   * @brief Gets the child node by its symbol.
   * @param symbol The symbol of the child node.
   * @return The child node.
   */
  Node *getChild(const char &symbol) const;

  /**
   * @brief Gets all descendants of this node.
   * @param max_level The maximum level of descendants to retrieve.
   * @return The list of descendant nodes.
   */
  list<Node *> getDescendants(unsigned int max_level = 7) const;

  /**
   * @brief Gets the parent node of this node.
   * @return The parent node.
   */
  Node *getParent() const;

  /**
   * @brief Gets all ancestors of this node.
   * @return The list of ancestor nodes.
   */
  list<Node *> getAncestors() const;

  /**
   * @brief Gets the root node of this node.
   * @return The root node.
   */
  Node *getRoot() const;

  /**
   * @brief Gets the node that represents the given substring.
   * @param substring The substring.
   * @return The node that represents the substring.
   */
  Node *getNodeRepresentingSubstring(string substring) const;

  /**
   * @brief Gets the gain of this node.
   * @return The gain.
   */
  int getGain() const;

  /**
   * @brief Gets the substring represented by this node.
   * @return The substring.
   */
  string getSubstring() const;

  /**
   * @brief Determines whether overlaps are possible.
   * @return True if overlaps are possible, false otherwise.
   */
  bool overlapsPossible() const;

  // --------------------------------------------------
  // Setters
  // --------------------------------------------------

  /**
   * @brief Increases the frequency of this node.
   * @param increase The amount to increase by.
   */
  void raiseFrequency(unsigned int increase = 1);

  /**
   * @brief Decreases the frequency of this node.
   * @param decrease The amount to decrease by.
   */
  void lowerFrequency(unsigned int decrease = 1);

  /**
   * @brief Sets the frequency of this node to zero.
   */
  void setFrequencyToZero();

  /**
   * @brief Increases the number of overlaps of this node.
   * @param increase The amount to increase by.
   */
  void raiseOverlaps(unsigned int increase = 1);

  /**
   * @brief Decreases the number of overlaps of this node.
   * @param decrease The amount to decrease by.
   */
  void lowerOverlaps(unsigned int decrease = 1);

  /**
   * @brief Sets the number of overlaps of this node to zero.
   */
  void setOverlapsToZero();

  // --------------------------------------------------
  // Adders
  // --------------------------------------------------

  /**
   * @brief Adds a substring to this node.
   * @param substring The substring to add.
   * @param length The length of the substring.
   */
  void addSubstring(char *substring, unsigned int length);

  /**
   * @brief Adds a substring to this node.
   * @param substring The substring to add.
   */
  void addSubstring(string substring);

  /**
   * @brief Adds overlaps to this node.
   * @param original_string The original string.
   */
  void addOverlaps(string original_string);

  // --------------------------------------------------
  // Subtractors
  // --------------------------------------------------

  /**
   * @brief Subtracts a substring from this node.
   * @param substring The substring to subtract.
   */
  void subtractSubstring(string substring);

  /**
   * @brief Subtracts overlaps from this node.
   * @param original_string The original string.
   */
  void subtractOverlaps(string original_string);

  // --------------------------------------------------
  // Functions to modify occurrence counts when a substring is added to the
  // dictionary
  // --------------------------------------------------

  /**
   * @brief Gets all substrings represented by this node.
   * @return A list of represented substrings.
   */
  list<string> getRepresentedSubstrings() const;

  /**
   * @brief Deletes all full occurrences of a substring starting at this node.
   * @param substring The substring to delete occurrences of.
   * @param pos The position to start deleting from.
   * @return A tuple containing the number of occurrences deleted and a list of
   * deleted substrings.
   */
  tuple<int, list<string>>
  deleteFullStringOccurrencesStartingAtThisNode(string substring,
                                                unsigned int pos);

  /**
   * @brief Deletes all full occurrences of a substring in this node.
   * @param substring The substring to delete occurrences of.
   */
  void deleteFullStringOccurrences(string substring);

  // --------------------------------------------------
  // Comparison operators
  // --------------------------------------------------

  /**
   * @brief Checks if this node is equal to the given node.
   * @param node The node to compare with.
   * @return True if the nodes are equal, false otherwise.
   */
  bool operator==(const Node &node) const;

  /**
   * @brief Checks if this node is not equal to the given node.
   * @param node The node to compare with.
   * @return True if the nodes are not equal, false otherwise.
   */
  bool operator!=(const Node &node) const;

  /**
   * @brief Checks if this node is less than the given node.
   * @param node The node to compare with.
   * @return True if this node is less than the given node, false otherwise.
   */
  bool operator<(const Node &node) const;

  /**
   * @brief Checks if this node is greater than the given node.
   * @param node The node to compare with.
   * @return True if this node is greater than the given node, false otherwise.
   */
  bool operator>(const Node &node) const;

  /**
   * @brief Checks if this node is less than or equal to the given node.
   * @param node The node to compare with.
   * @return True if this node is less than or equal to the given node, false
   * otherwise.
   */
  bool operator<=(const Node &node) const;

  /**
   * @brief Checks if this node is greater than or equal to the given node.
   * @param node The node to compare with.
   * @return True if this node is greater than or equal to the given node, false
   * otherwise.
   */
  bool operator>=(const Node &node) const;

  // --------------------------------------------------
  // Printers
  // --------------------------------------------------

  /**
   * @brief Prints the (sub)tree starting with this node.
   */
  void printTree();

  /**
   * @brief Returns the (sub)tree as a string starting with this node.
   * @return The (sub)tree as a string.
   */
  string toString();
};

// ---------------------------------------------------------------------------------------------
// Struct NodeCompare used to sort a list of Node pointers based on the gain of
// the nodes itself
// ---------------------------------------------------------------------------------------------

/**
 * @struct NodeCompare
 * @brief Compares two nodes based on their gain.
 */
struct NodeCompare
{
  /**
   * @brief Compares two nodes.
   * @param node1 The first node to compare.
   * @param node2 The second node to compare.
   * @return True if node1's gain is less than node2's gain, false otherwise.
   */
  bool operator()(const Node *node1, const Node *node2) const;
};

#endif