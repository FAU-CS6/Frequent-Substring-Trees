using namespace std;

#include <cstring>
#include <iostream>
#include <stdexcept>

#include "string_helpers.h"

// ---------------------------------------------------------------------------------------------
// Namespace string_helpers
// ---------------------------------------------------------------------------------------------

// --------------------------------------------------
// Overlap functions
// --------------------------------------------------

int string_helpers::number_of_occurrences_with_overlap(string str,
                                                       string substr)
{
  // If the substring is empty, return 0
  if (substr.length() == 0)
    return 0;

  // Count the number of occurrences
  int count = 0;
  size_t pos = 0;
  while ((pos = str.find(substr, pos)) != string::npos)
  {
    count++;
    pos++;
  }
  return count;
}

int string_helpers::number_of_occurrences_without_overlap(string str,
                                                          string substr)
{
  // If the substring is empty, return 0
  if (substr.length() == 0)
    return 0;

  // Count the number of occurrences
  int count = 0;
  size_t pos = 0;
  while ((pos = str.find(substr, pos)) != string::npos)
  {
    count++;
    pos += substr.length();
  }
  return count;
}

// --------------------------------------------------
// Contained char functions
// --------------------------------------------------

set<char> string_helpers::get_contained_chars(string str)
{
  set<char> contained_chars;
  for (char c : str)
    contained_chars.insert(c);
  return contained_chars;
}

// --------------------------------------------------
// Compare two lists of strings
// --------------------------------------------------

bool string_helpers::compare_lists_of_strings(list<string> list1,
                                              list<string> list2)
{
  // If the lists are not the same size, return false
  if (list1.size() != list2.size())
    return false;

  // Sort the lists
  list1.sort();
  list2.sort();

  // Compare the lists
  return equal(list1.begin(), list1.end(), list2.begin());
}

// --------------------------------------------------
// String modification functions
// --------------------------------------------------

string string_helpers::delete_everything_starting_with_the_second_occurrence(
    const string str, const string substr)
{
  // If the substring is empty, return the string
  if (substr.length() == 0)
  {
    return str;
  }

  // Find the first occurrence of the substring
  size_t pos = str.find(substr);

  // If the substring is not found, return the string
  if (pos == string::npos)
  {
    return str;
  }

  // Find the second occurrence of the substring
  pos = str.find(substr, pos + 1);

  // If the substring is not found, return the string
  if (pos == string::npos)
  {
    return str;
  }

  // Make a copy of the string
  string str_copy = str;

  // Delete everything starting with the second occurrence of the substring
  str_copy.erase(pos, str_copy.length() - pos);

  // Return the string
  return str_copy;
}

list<string>
string_helpers::delete_contained_substrings_helper(string str,
                                                   list<string> strings)
{
  // Make a copy of the list
  list<string> strings_copy = strings;

  // Iterate over the list
  for (list<string>::iterator it = strings_copy.begin();
       it != strings_copy.end(); ++it)
  {
    // Get the string
    string str2 = *it;

    // Find str2 in str (not considiring the first character)
    size_t pos = str.find(str2, 1);

    // Check if the string is contained in the string
    if (pos != string::npos)
    {
      // Check if the string
      // Remove the string from the list
      strings_copy.erase(it);

      // Search for substrings contained in str2
      strings_copy = delete_contained_substrings_helper(str2, strings_copy);

      // Break the loop
      break;
    }
  }

  // Return the list
  return strings_copy;
}

list<string> string_helpers::delete_contained_substrings(list<string> strings)
{
  // Make a copy of the list
  list<string> strings_copy = strings;

  // Sort the list by length (longest first)
  strings_copy.sort(
      [](const string &a, const string &b)
      { return a.length() > b.length(); });

  // Make an empty return list
  list<string> strings_return;

  // As long as the list is not empty
  while (!strings_copy.empty())
  {
    // Get the first string
    string str = strings_copy.front();

    // Remove the first string from the list
    strings_copy.pop_front();

    // Remove all substrings contained in the string
    strings_copy = delete_contained_substrings_helper(str, strings_copy);

    // Add the string to the return list
    strings_return.push_back(str);
  }

  // Return the list
  return strings_return;
}

list<string> string_helpers::get_substrings_to_delete(string substr,
                                                      list<string> strings)
{
  // Make a copy of the list
  list<string> strings_copy = strings;

  // Prepare the return list
  list<string> strings_return;

  // For each string in the list
  for (list<string>::iterator it = strings_copy.begin();
       it != strings_copy.end(); ++it)
  {
    // Get the string
    string current_string = *it;

    // Find the first occurrence of the substring in the current_string
    size_t pos = current_string.find(substr);

    // While the substring is found
    while (pos != string::npos)
    {
      // Get the substring
      string current_substring =
          current_string.substr(pos, current_string.length() - pos);

      // Add the substring to the return list
      strings_return.push_back(current_substring);

      // Find the next occurrence of the substring in the current_string
      pos = current_string.find(substr, pos + substr.length());
    }
  }

  // Return the list
  return strings_return;
}
