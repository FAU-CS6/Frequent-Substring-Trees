#ifndef STRINGHELPERS_H
#define STRINGHELPERS_H

using namespace std;

#include <list>
#include <set>
#include <string>
#include <tuple>

// ---------------------------------------------------------------------------------------------
// Namespace string_helpers
// ---------------------------------------------------------------------------------------------
namespace string_helpers
{
    /**
     * @brief Calculate the number of occurrences of a substring in a string,
     * considering overlaps.
     * @param str The string to search in.
     * @param substr The substring to search for.
     * @return The number of occurrences of the substring.
     */
    int number_of_occurrences_with_overlap(string str, string substr);

    /**
     * @brief Calculate the number of occurrences of a substring in a string, not
     * considering overlaps.
     * @param str The string to search in.
     * @param substr The substring to search for.
     * @return The number of occurrences of the substring.
     */
    int number_of_occurrences_without_overlap(string str, string substr);

    /**
     * @brief Get a set of all characters contained in a string.
     * @param str The string to analyze.
     * @return A set of all characters contained in the string.
     */
    set<char> get_contained_chars(string str);

    /**
     * @brief Compare two lists of strings.
     * @param list1 The first list of strings.
     * @param list2 The second list of strings.
     * @return True if the lists are equal, false otherwise.
     */
    bool compare_lists_of_strings(list<string> list1, list<string> list2);

    /**
     * @brief Delete everything in a string starting with the second occurrence of a
     * substring.
     * @param str The string to modify.
     * @param substr The substring to search for.
     * @return The modified string.
     */
    string
    delete_everything_starting_with_the_second_occurrence(const string str,
                                                          const string substr);

    /**
     * @brief Helper function for delete_contained_substrings() for a single string.
     * @param str The string to modify.
     * @param strings The list of substrings to delete.
     * @return The modified string.
     */
    list<string> delete_contained_substrings_helper(string str,
                                                    list<string> strings);

    /**
     * @brief Delete all substrings that are contained in other strings (e.g. "TEST"
     * and "TESTEST" are contained in "TESTESTEST")
     * @param strings The list of strings to modify.
     * @return The list of modified strings.
     */
    list<string> delete_contained_substrings(list<string> strings);

    /**
     * @brief Get the correct substrings to delete from the fstc (e.g. if
     * "TESTESTEST" and "TESTER" are given in the list of strings and the str(ing to
     * replace) is "TEST", then "TESTESTEST", "TEST" and "TESTER" should be returned
     * @param str The string to analyze.
     * @param strings The list of potential substrings to delete.
     * @return A list of substrings to delete.
     */
    list<string> get_substrings_to_delete(string str, list<string> strings);

} // namespace string_helpers

#endif
