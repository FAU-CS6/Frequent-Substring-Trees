#ifndef FST_H
#define FST_H

using namespace std;

#include <list>
#include <string>
#include <vector>

#include "node.h"

// ---------------------------------------------------------------------------------------------
// Class FST
// ---------------------------------------------------------------------------------------------

/**
 * @class FST
 * @brief Class representing an FST (Frequent Substring Tree).
 */
class FST
{
private:
  /**
   * @brief A vector containing the root nodes of all paths in the FST.
   */
  std::vector<Node *> paths;

  // --------------------------------------------------
  // Simple setters
  // --------------------------------------------------

  /**
   * @brief Adds a new path to the FST.
   * @param node The path to add.
   */
  void addPath(Node *node);

  /**
   * @brief Removes a path from the FST.
   * @param node The path to remove.
   */
  void removePath(Node *node);

  // --------------------------------------------------
  // Adders
  // --------------------------------------------------

  /**
   * @brief Adds a substring to the FST.
   * @param substring The substring to add.
   * @param length The length of the substring.
   */
  void addSubstring(char *substring, unsigned int length);

  /**
   * @brief Adds overlaps to the FST.
   * @param string The string to process for overlaps.
   */
  void addOverlaps(string string);

  // --------------------------------------------------
  // Subtractors
  // --------------------------------------------------

  /**
   * @brief Subtracts overlaps from the FST.
   * @param string The string to process for overlaps.
   */
  void subtractOverlaps(string string);

public:
  // --------------------------------------------------
  // Constructor and destructor
  // --------------------------------------------------

  /**
   * @brief Construct a new FST object.
   */
  FST(void);

  /**
   * @brief Destroy the FST object.
   */
  virtual ~FST(void);

  // --------------------------------------------------
  // Getters
  // --------------------------------------------------

  /**
   * @brief Get the number of paths in the FST.
   * @return The number of paths.
   */
  unsigned int getNrPaths() const;

  /**
   * @brief Get a specific path from the FST by ID.
   * @param i The ID of the path.
   * @return The root node of the path.
   */
  Node *getPath(unsigned int i) const;

  /**
   * @brief Get a specific path from the FST by symbol.
   * @param symbol The symbol of the path's root node.
   * @return The root node of the path.
   */
  Node *getPath(const char &symbol) const;

  /**
   * @brief Get the node representing the given substring.
   * @param substring The substring to search for.
   * @return The node representing the substring.
   */
  Node *getNodeRepresentingSubstring(string substring) const;

  /**
   * @brief Get a list of all non-root nodes, sorted by gain.
   * @param max_level The maximum level of nodes to include in the list.
   * @return A list of nodes sorted by gain.
   */
  list<Node *> getSortedGainList(unsigned int max_level = 7) const;

  // --------------------------------------------------
  // Get dictionary entries (and remove them from the FST)
  // --------------------------------------------------

  /**
   * @brief Get dictionary entries and remove them from the FST.
   * @param x The number of dictionary entries to get.
   * @param max_level The maximum level of nodes to include in the dictionary.
   * @return A list of dictionary entries.
   */
  list<string> getDictionaryEntries(unsigned int x = 255,
                                    unsigned int max_level = 7);

  // --------------------------------------------------
  // Remove substring from the FST
  // --------------------------------------------------

  /**
   * @brief Subtract a substring from the FST.
   * @param substring The substring to subtract.
   */
  void subtractSubstring(string substring);

  // --------------------------------------------------
  // Add string to the FST
  // --------------------------------------------------

  /**
   * @brief Add a string to the FST.
   * @param string The string to add.
   * @param length The length of the string.
   */
  void addString(char *string, unsigned int length);

  /**
   * @brief Add a string to the FST.
   * @param str The string to add.
   */
  void addString(string str);

  // --------------------------------------------------
  // Add a list of strings to the FST
  // --------------------------------------------------

  /**
   * @brief Add a list of strings to the FST.
   * @param strings The list of strings to add.
   */
  void addStrings(list<string> strings);

  // --------------------------------------------------
  // Add a substring to the dictionary
  // (remove the corresponding occurrences from the FST)
  // --------------------------------------------------

  /**
   * @brief Remove all occurrences of a given substring from the FST.
   * @param substring The substring to remove.
   */
  void deleteFullStringOccurrences(string substring);

  /**
   * @brief Handle the case when a node is used to add a new substring to the
   * dictionary.
   * @param node The node used to add the new substring.
   */
  void handleSubstringAddedToDict(Node *node);

  // --------------------------------------------------
  // Printers
  // --------------------------------------------------

  /**
   * @brief Print the paths in the FST.
   */
  void printPaths();

  /**
   * @brief Returns the FST as a string
   * @return The FST as a string.
   */
  string toString();

  // --------------------------------------------------
  // Comparison operators
  // --------------------------------------------------

  /**
   * @brief Checks if this FST is equal to another FST.
   * @param tc The other FST.
   * @return True if the FST are equal, false otherwise.
   */
  bool operator==(const FST &tc) const;

  /**
   * @brief Checks if this FST is not equal to another FST.
   * @param tc The other FST.
   * @return True if the FSTs are not equal, false otherwise.
   */
  bool operator!=(const FST &tc) const;
};

#endif
