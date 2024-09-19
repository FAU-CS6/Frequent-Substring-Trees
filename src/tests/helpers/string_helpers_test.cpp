// Library includes
using namespace std;
#include <iostream>
#include <set>
#include <string>
#include <tuple>

// Doctest include
#include "../doctest/doctest.h"

// Include the files to test
#include "../../helpers/string_helpers.h"

// ---------------------------------------------------------------------------------------------
// Function-level tests
// ---------------------------------------------------------------------------------------------
// Tests are foccused on testing the functionality of a single function

TEST_CASE("string_helpers::number_of_occurrences_with_overlap")
{
  SUBCASE("No overlaps of the substring in the string")
  {
    SUBCASE("Substring shorter than the string")
    {
      SUBCASE("Substring with length 0")
      {
        CHECK(string_helpers::number_of_occurrences_with_overlap("TEST", "") ==
              0);
      }

      SUBCASE("Substring with length 1")
      {
        CHECK(string_helpers::number_of_occurrences_with_overlap("DOMINIK",
                                                                 "D") == 1);
        CHECK(string_helpers::number_of_occurrences_with_overlap("DOMINIK",
                                                                 "O") == 1);
        CHECK(string_helpers::number_of_occurrences_with_overlap("DOMINIK",
                                                                 "I") == 2);
        CHECK(string_helpers::number_of_occurrences_with_overlap("DOMINIK",
                                                                 "K") == 1);
      }

      SUBCASE("Substring with length 4")
      {
        CHECK(string_helpers::number_of_occurrences_with_overlap("DOMINIK",
                                                                 "TEST") == 0);
        CHECK(string_helpers::number_of_occurrences_with_overlap("TESTER",
                                                                 "TEST") == 1);
        CHECK(string_helpers::number_of_occurrences_with_overlap("ATTEST",
                                                                 "TEST") == 1);
        CHECK(string_helpers::number_of_occurrences_with_overlap("ATTESTER",
                                                                 "TEST") == 1);
        CHECK(string_helpers::number_of_occurrences_with_overlap("ATTESTTEST",
                                                                 "TEST") == 2);
      }
    }

    SUBCASE("Substring with same lenght as the string")
    {
      SUBCASE("Substring with length 0")
      {
        CHECK(string_helpers::number_of_occurrences_with_overlap("", "") == 0);
      }

      SUBCASE("Substring with length 1")
      {
        CHECK(string_helpers::number_of_occurrences_with_overlap("D", "D") == 1);
        CHECK(string_helpers::number_of_occurrences_with_overlap("P", "D") == 0);
      }

      SUBCASE("Substring with length 4")
      {
        CHECK(string_helpers::number_of_occurrences_with_overlap("TEST",
                                                                 "TEST") == 1);
        CHECK(string_helpers::number_of_occurrences_with_overlap("ESTE",
                                                                 "TEST") == 0);
      }
    }

    SUBCASE("Substring longer than the string")
    {
      SUBCASE("Substring with length 1")
      {
        CHECK(string_helpers::number_of_occurrences_with_overlap("", "D") == 0);
      }

      SUBCASE("Substring with length 4")
      {
        CHECK(string_helpers::number_of_occurrences_with_overlap("", "TEST") ==
              0);
        CHECK(string_helpers::number_of_occurrences_with_overlap("TE", "TEST") ==
              0);
        CHECK(string_helpers::number_of_occurrences_with_overlap("DP", "TEST") ==
              0);
      }
    }
  }

  SUBCASE("Overlaps of the substring in the string")
  {
    SUBCASE("Substring shorter than the string")
    {
      SUBCASE("Substring with length 1")
      {
        CHECK(string_helpers::number_of_occurrences_with_overlap("DDDD", "D") ==
              4);
      }

      SUBCASE("Substring with length 4")
      {
        CHECK(string_helpers::number_of_occurrences_with_overlap("TESTEST",
                                                                 "TEST") == 2);
        CHECK(string_helpers::number_of_occurrences_with_overlap(
                  "ATTESTESTESTER", "TEST") == 3);
        CHECK(string_helpers::number_of_occurrences_with_overlap(
                  "ATTESTESTERTEST", "TEST") == 3);
      }
    }
  }
}

TEST_CASE("string_helpers::number_of_occurrences_without_overlap")
{
  SUBCASE("No overlaps of the substring in the string")
  {
    SUBCASE("Substring shorter than the string")
    {
      SUBCASE("Substring with length 0")
      {
        CHECK(string_helpers::number_of_occurrences_without_overlap("TEST",
                                                                    "") == 0);
      }

      SUBCASE("Substring with length 1")
      {
        CHECK(string_helpers::number_of_occurrences_without_overlap("DOMINIK",
                                                                    "D") == 1);
        CHECK(string_helpers::number_of_occurrences_without_overlap("DOMINIK",
                                                                    "O") == 1);
        CHECK(string_helpers::number_of_occurrences_without_overlap("DOMINIK",
                                                                    "I") == 2);
        CHECK(string_helpers::number_of_occurrences_without_overlap("DOMINIK",
                                                                    "K") == 1);
      }

      SUBCASE("Substring with length 4")
      {
        CHECK(string_helpers::number_of_occurrences_without_overlap(
                  "DOMINIK", "TEST") == 0);
        CHECK(string_helpers::number_of_occurrences_without_overlap(
                  "TESTER", "TEST") == 1);
        CHECK(string_helpers::number_of_occurrences_without_overlap(
                  "ATTEST", "TEST") == 1);
        CHECK(string_helpers::number_of_occurrences_without_overlap(
                  "ATTESTER", "TEST") == 1);
        CHECK(string_helpers::number_of_occurrences_without_overlap(
                  "ATTESTTEST", "TEST") == 2);
      }
    }

    SUBCASE("Substring with same lenght as the string")
    {
      SUBCASE("Substring with length 0")
      {
        CHECK(string_helpers::number_of_occurrences_without_overlap("", "") ==
              0);
      }

      SUBCASE("Substring with length 1")
      {
        CHECK(string_helpers::number_of_occurrences_without_overlap("D", "D") ==
              1);
        CHECK(string_helpers::number_of_occurrences_without_overlap("P", "D") ==
              0);
      }

      SUBCASE("Substring with length 4")
      {
        CHECK(string_helpers::number_of_occurrences_without_overlap(
                  "TEST", "TEST") == 1);
        CHECK(string_helpers::number_of_occurrences_without_overlap(
                  "ESTE", "TEST") == 0);
      }
    }

    SUBCASE("Substring longer than the string")
    {
      SUBCASE("Substring with length 1")
      {
        CHECK(string_helpers::number_of_occurrences_without_overlap("", "D") ==
              0);
      }

      SUBCASE("Substring with length 4")
      {
        CHECK(string_helpers::number_of_occurrences_without_overlap(
                  "", "TEST") == 0);
        CHECK(string_helpers::number_of_occurrences_without_overlap(
                  "TE", "TEST") == 0);
        CHECK(string_helpers::number_of_occurrences_without_overlap(
                  "DP", "TEST") == 0);
      }
    }
  }

  SUBCASE("Overlaps of the substring in the string")
  {
    SUBCASE("Substring shorter than the string")
    {
      SUBCASE("Substring with length 1")
      {
        CHECK(string_helpers::number_of_occurrences_without_overlap("DDDD",
                                                                    "D") == 4);
      }

      SUBCASE("Substring with length 4")
      {
        CHECK(string_helpers::number_of_occurrences_without_overlap(
                  "TESTEST", "TEST") == 1);
        CHECK(string_helpers::number_of_occurrences_without_overlap(
                  "ATTESTESTESTER", "TEST") == 2);
        CHECK(string_helpers::number_of_occurrences_without_overlap(
                  "ATTESTESTERTEST", "TEST") == 2);
      }
    }
  }
}

TEST_CASE("string_helpers::compare_lists_of_strings")
{
  SUBCASE("Equal lists (TESTESTEST and TESTER)")
  {
    // Create the lists
    list<string> list1 = {"TESTESTEST", "TESTER"};
    list<string> list2 = {"TESTER", "TESTESTEST"};

    // Compare the lists
    CHECK(string_helpers::compare_lists_of_strings(list1, list2) == true);
  }

  SUBCASE("Inequal lists (TESTESTEST and TESTER vs TESTESTEST and TEST)")
  {
    // Create the lists
    list<string> list1 = {"TESTESTEST", "TESTER"};
    list<string> list2 = {"TEST", "TESTESTEST"};

    // Compare the lists
    CHECK(string_helpers::compare_lists_of_strings(list1, list2) == false);
  }
}

TEST_CASE("string_helpers::get_contained_chars")
{
  SUBCASE("Empty string")
  {
    CHECK(string_helpers::get_contained_chars("") == set<char>());
  }

  SUBCASE("String with length 1")
  {
    CHECK(string_helpers::get_contained_chars("D") == set<char>({'D'}));
  }

  SUBCASE("String with length 4")
  {
    CHECK(string_helpers::get_contained_chars("TEST") ==
          set<char>({'E', 'S', 'T'}));
  }
}

TEST_CASE(
    "string_helpers::delete_everything_starting_with_the_second_occurrence")
{
  SUBCASE("Substring shorter than the string")
  {
    SUBCASE("Empty substring")
    {
      CHECK(
          string_helpers::delete_everything_starting_with_the_second_occurrence(
              "TEST", "") == "TEST");
    }

    SUBCASE("Substring with length 1")
    {
      CHECK(
          string_helpers::delete_everything_starting_with_the_second_occurrence(
              "TEST", "T") == "TES");
      CHECK(
          string_helpers::delete_everything_starting_with_the_second_occurrence(
              "TTTT", "T") == "T");
    }

    SUBCASE("Substring with length 4")
    {
      CHECK(
          string_helpers::delete_everything_starting_with_the_second_occurrence(
              "TESTEST", "TEST") == "TES");
      CHECK(
          string_helpers::delete_everything_starting_with_the_second_occurrence(
              "TESTESTER", "TEST") == "TES");
      CHECK(
          string_helpers::delete_everything_starting_with_the_second_occurrence(
              "TESTESTEST", "TEST") == "TES");
    }
  }
}

TEST_CASE("string_helpers::delete_contained_substrings")
{
  SUBCASE("TESTESTEST and TESTER")
  {
    // Create the inital list of strings as the tree_collection would output
    list<string> strings = {"TESTER", "TESTESTEST", "TESTEST", "TEST"};

    // Create the expected list of strings
    list<string> expected_strings = {"TESTER", "TESTESTEST"};

    // Delete the contained strings
    list<string> returned_strings =
        string_helpers::delete_contained_substrings(strings);

    // Check if the contained strings are the same in both lists
    CHECK(string_helpers::compare_lists_of_strings(returned_strings,
                                                   expected_strings));
  }

  SUBCASE("TESTESTEST, TESTEST and TEST")
  {
    // Create the inital list of strings as the tree_collection would output
    list<string> strings = {"TESTESTEST", "TESTEST", "TEST",
                            "TESTEST", "TEST", "TEST"};

    // Create the expected list of strings
    list<string> expected_strings = {"TESTESTEST", "TESTEST", "TEST"};

    // Delete the contained strings
    list<string> returned_strings =
        string_helpers::delete_contained_substrings(strings);

    // Check if the contained strings are the same in both lists
    CHECK(string_helpers::compare_lists_of_strings(returned_strings,
                                                   expected_strings));
  }

  SUBCASE("TESTER, TEST and TEST")
  {
    // Create the inital list of strings as the tree_collection would output
    list<string> strings = {"TESTER", "TEST", "TEST"};

    // Create the expected list of strings
    list<string> expected_strings = {"TESTER", "TEST", "TEST"};

    // Delete the contained strings
    list<string> returned_strings =
        string_helpers::delete_contained_substrings(strings);

    // Check if the contained strings are the same in both lists
    CHECK(string_helpers::compare_lists_of_strings(returned_strings,
                                                   expected_strings));
  }
}

TEST_CASE("string_helpers::get_substrings_to_delete")
{
  SUBCASE("TESTESTEST and TESTER with string to replace TEST")
  {
    // Create the inital list of strings as the tree_collection would output
    list<string> strings = {"TESTER", "TESTESTEST"};

    // Create the expected list of strings
    list<string> expected_strings = {"TESTER", "TESTESTEST", "TEST"};

    // Get the substrings to delete
    list<string> returned_strings =
        string_helpers::get_substrings_to_delete("TEST", strings);

    // Check if the contained strings are the same in both lists
    CHECK(string_helpers::compare_lists_of_strings(returned_strings,
                                                   expected_strings));
  }

  SUBCASE("TESTER, TEST and TEST with string to replace TEST")
  {
    // Create the inital list of strings as the tree_collection would output
    list<string> strings = {"TESTER", "TEST", "TEST"};

    // Create the expected list of strings
    list<string> expected_strings = {"TESTER", "TEST", "TEST"};

    // Get the substrings to delete
    list<string> returned_strings =
        string_helpers::get_substrings_to_delete("TEST", strings);

    // Check if the contained strings are the same in both lists
    CHECK(string_helpers::compare_lists_of_strings(returned_strings,
                                                   expected_strings));
  }
}