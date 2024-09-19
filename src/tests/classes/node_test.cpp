// Library includes
using namespace std;
#include <algorithm>
#include <iostream>
#include <list>
#include <string>

// Doctest include
#include "../doctest/doctest.h"

// Include the files to test
#include "../../classes/node.h"

// ---------------------------------------------------------------------------------------------
// Class-level tests
// ---------------------------------------------------------------------------------------------
// Tests are focussed on testing the functionality of the class as a whole

TEST_CASE("Check if a tree can be build by creating a root node and adding "
          "substrings to it")
{
  SUBCASE("TEST")
  {
    // Create the root node
    Node *root = new Node('T', 0, 0, NULL);

    // Add a string to the tree
    root->addSubstring("TEST");

    // Get the different nodes
    Node *e_node = root->getChild('E');
    Node *s_node = e_node->getChild('S');
    Node *t_node = s_node->getChild('T');

    // Check if the nodes correctly return the chars they represent
    CHECK('T' == root->getSymbol());
    CHECK('E' == e_node->getSymbol());
    CHECK('S' == s_node->getSymbol());
    CHECK('T' == t_node->getSymbol());

    // Check if the nodes correctly return the frequency they represent
    CHECK(1 == root->getFrequency());
    CHECK(1 == e_node->getFrequency());
    CHECK(1 == s_node->getFrequency());
    CHECK(1 == t_node->getFrequency());

    // Clean up
    delete root;
  }

  SUBCASE("TEST and TARGET")
  {
    // Create the root node
    Node *root = new Node('T', 0, 0, NULL);

    // Add a string to the tree
    root->addSubstring("TEST");
    root->addSubstring("TARGET");

    // Get the different nodes for the first string
    Node *s1_e_node = root->getChild('E');
    Node *s1_s_node = s1_e_node->getChild('S');
    Node *s1_t_node = s1_s_node->getChild('T');

    // Get the different nodes for the second string
    Node *s2_a_node = root->getChild('A');
    Node *s2_r_node = s2_a_node->getChild('R');
    Node *s2_g_node = s2_r_node->getChild('G');
    Node *s2_e_node = s2_g_node->getChild('E');
    Node *s2_t_node = s2_e_node->getChild('T');

    // Check if the nodes correctly return the chars they represent
    CHECK('T' == root->getSymbol());
    CHECK('E' == s1_e_node->getSymbol());
    CHECK('S' == s1_s_node->getSymbol());
    CHECK('T' == s1_t_node->getSymbol());
    CHECK('A' == s2_a_node->getSymbol());
    CHECK('R' == s2_r_node->getSymbol());
    CHECK('G' == s2_g_node->getSymbol());
    CHECK('E' == s2_e_node->getSymbol());
    CHECK('T' == s2_t_node->getSymbol());

    // Check if the nodes correctly return the frequency they represent
    CHECK(2 == root->getFrequency());
    CHECK(1 == s1_e_node->getFrequency());
    CHECK(1 == s1_s_node->getFrequency());
    CHECK(1 == s1_t_node->getFrequency());
    CHECK(1 == s2_a_node->getFrequency());
    CHECK(1 == s2_r_node->getFrequency());
    CHECK(1 == s2_g_node->getFrequency());
    CHECK(1 == s2_e_node->getFrequency());
    CHECK(1 == s2_t_node->getFrequency());

    // Clean up
    delete root;
  }
}

TEST_CASE("Check if a node returns the substrings it represents correctly")
{
  SUBCASE("TEST")
  {
    // Create the root node
    Node *root = new Node('T', 0, 0, NULL);

    // Add a string to the tree
    root->addSubstring("TEST");

    // Get the different nodes
    Node *e_node = root->getChild('E');
    Node *s_node = e_node->getChild('S');
    Node *t_node = s_node->getChild('T');

    // Check if the nodes correctly return the substring they represent
    CHECK(root->getSubstring() == "T");
    CHECK(e_node->getSubstring() == "TE");
    CHECK(s_node->getSubstring() == "TES");
    CHECK(t_node->getSubstring() == "TEST");

    // Clean up
    delete root;
  }
}

TEST_CASE("Check if a node returns its root node correctly")
{
  SUBCASE("TEST")
  {
    // Create the root node
    Node *root = new Node('T', 0, 0, NULL);

    // Add a string to the tree
    root->addSubstring("TEST");

    // Get the different nodes
    Node *e_node = root->getChild('E');
    Node *s_node = e_node->getChild('S');
    Node *t_node = s_node->getChild('T');

    // Check if the nodes correctly return the root node correctly
    CHECK(root->getRoot() == root);
    CHECK(e_node->getRoot() == root);
    CHECK(s_node->getRoot() == root);
    CHECK(t_node->getRoot() == root);

    // Clean up
    delete root;
  }
}

TEST_CASE("Check if a node representing a substring is correctly identified")
{
  SUBCASE("TEST")
  {
    // Create the root node
    Node *root = new Node('T', 0, 0, NULL);

    // Add a string to the tree
    root->addSubstring("TEST");

    // Get the different nodes
    Node *e_node = root->getChild('E');
    Node *s_node = e_node->getChild('S');
    Node *t_node = s_node->getChild('T');

    // Check if the nodes are returned correctly
    CHECK(root->getNodeRepresentingSubstring("T") == root);
    CHECK(root->getNodeRepresentingSubstring("TE") == e_node);
    CHECK(root->getNodeRepresentingSubstring("TES") == s_node);
    CHECK(root->getNodeRepresentingSubstring("TEST") == t_node);
    CHECK(root->getNodeRepresentingSubstring("TESTE") == NULL);
    CHECK(root->getNodeRepresentingSubstring("DOMI") == NULL);

    // Clean up
    delete root;
  }
}

TEST_CASE("Check if overlaps are added correctly")
{
  SUBCASE("TESTESTE")
  {
    // Create the root node
    Node *root = new Node('T', 0, 0, NULL);

    // Add a string to the tree
    root->addSubstring("TESTESTE");

    // Add the overlaps
    root->addOverlaps("TESTESTE");

    // Get the different nodes
    Node *e_node = root->getChild('E');
    Node *s_node = e_node->getChild('S');
    Node *t_node = s_node->getChild('T');
    Node *e2_node = t_node->getChild('E');
    Node *s2_node = e2_node->getChild('S');
    Node *t2_node = s2_node->getChild('T');
    Node *e3_node = t2_node->getChild('E');

    // Check if the nodes correctly return the substring they represent
    CHECK(root->getOverlaps() == 0);
    CHECK(e_node->getOverlaps() == 0);
    CHECK(s_node->getOverlaps() == 0);
    CHECK(t_node->getOverlaps() == 1);
    CHECK(e2_node->getOverlaps() == 1);
    CHECK(s2_node->getOverlaps() == 0);
    CHECK(t2_node->getOverlaps() == 0);
    CHECK(e3_node->getOverlaps() == 0);

    // Clean up
    delete root;
  }
}

TEST_CASE("Check if represented substrings are returned correctly")
{
  SUBCASE("TEST and TESTESTE")
  {
    // Create the root node
    Node *root = new Node('T', 0, 0, NULL);

    // Add the strings themselves to the tree
    root->addSubstring("TEST");
    root->addSubstring("TESTESTE");

    // As we want to simulate the bevaviour of the tree when both strings are
    // regulary (by using FST::addString) added to the tree we need
    // to add the substrings begining with "T" to the tree as well

    // Substrings "TEST"
    root->addSubstring("T");

    // Substrings "TESTESTE"
    root->addSubstring("TESTE");
    root->addSubstring("TE");

    // Get the different nodes
    Node *e_node = root->getChild('E');
    Node *s_node = e_node->getChild('S');
    Node *t_node = s_node->getChild('T');
    Node *e2_node = t_node->getChild('E');
    Node *s2_node = e2_node->getChild('S');
    Node *t2_node = s2_node->getChild('T');
    Node *e3_node = t2_node->getChild('E');

    // Check if the nodes correctly return the substring they represent
    SUBCASE("Root")
    {
      list<string> represented_strings = root->getRepresentedSubstrings();
      CHECK(represented_strings.size() == 5);
      CHECK(find(represented_strings.begin(), represented_strings.end(),
                 "TESTESTE") != represented_strings.end());
      CHECK(find(represented_strings.begin(), represented_strings.end(),
                 "TESTE") != represented_strings.end());
      CHECK(find(represented_strings.begin(), represented_strings.end(),
                 "TEST") != represented_strings.end());
      CHECK(find(represented_strings.begin(), represented_strings.end(),
                 "TE") != represented_strings.end());
      CHECK(find(represented_strings.begin(), represented_strings.end(), "T") !=
            represented_strings.end());
    }

    SUBCASE("E")
    {
      list<string> represented_strings = e_node->getRepresentedSubstrings();
      CHECK(represented_strings.size() == 4);
      CHECK(find(represented_strings.begin(), represented_strings.end(),
                 "TESTESTE") != represented_strings.end());
      CHECK(find(represented_strings.begin(), represented_strings.end(),
                 "TESTE") != represented_strings.end());
      CHECK(find(represented_strings.begin(), represented_strings.end(),
                 "TEST") != represented_strings.end());
      CHECK(find(represented_strings.begin(), represented_strings.end(),
                 "TE") != represented_strings.end());
    }

    SUBCASE("S")
    {
      list<string> represented_strings = s_node->getRepresentedSubstrings();
      CHECK(represented_strings.size() == 3);
      CHECK(find(represented_strings.begin(), represented_strings.end(),
                 "TESTESTE") != represented_strings.end());
      CHECK(find(represented_strings.begin(), represented_strings.end(),
                 "TESTE") != represented_strings.end());
      CHECK(find(represented_strings.begin(), represented_strings.end(),
                 "TEST") != represented_strings.end());
    }

    SUBCASE("T")
    {
      list<string> represented_strings = t_node->getRepresentedSubstrings();
      CHECK(represented_strings.size() == 3);
      CHECK(find(represented_strings.begin(), represented_strings.end(),
                 "TESTESTE") != represented_strings.end());
      CHECK(find(represented_strings.begin(), represented_strings.end(),
                 "TESTE") != represented_strings.end());
      CHECK(find(represented_strings.begin(), represented_strings.end(),
                 "TEST") != represented_strings.end());
    }

    SUBCASE("E2")
    {
      list<string> represented_strings = e2_node->getRepresentedSubstrings();
      CHECK(represented_strings.size() == 2);
      CHECK(find(represented_strings.begin(), represented_strings.end(),
                 "TESTESTE") != represented_strings.end());
      CHECK(find(represented_strings.begin(), represented_strings.end(),
                 "TESTE") != represented_strings.end());
    }

    SUBCASE("S2")
    {
      list<string> represented_strings = s2_node->getRepresentedSubstrings();
      CHECK(represented_strings.size() == 1);
      CHECK(find(represented_strings.begin(), represented_strings.end(),
                 "TESTESTE") != represented_strings.end());
    }

    SUBCASE("T2")
    {
      list<string> represented_strings = t2_node->getRepresentedSubstrings();
      CHECK(represented_strings.size() == 1);
      CHECK(find(represented_strings.begin(), represented_strings.end(),
                 "TESTESTE") != represented_strings.end());
    }

    SUBCASE("E3")
    {
      list<string> represented_strings = e3_node->getRepresentedSubstrings();
      CHECK(represented_strings.size() == 1);
      CHECK(find(represented_strings.begin(), represented_strings.end(),
                 "TESTESTE") != represented_strings.end());
    }

    // Clean up
    delete root;
  }

  SUBCASE("TEST and TARGET")
  {
    // Create the root node
    Node *root = new Node('T', 0, 0, NULL);

    // Add the strings themselves to the tree
    root->addSubstring("TEST");
    root->addSubstring("TARGET");

    // As we want to simulate the bevaviour of the tree when both strings are
    // regulary (by using FST::addString) added to the tree we need
    // to add the substrings begining with "T" to the tree as well

    // Substrings "TEST"
    root->addSubstring("T");

    // Substrings "TARGET"
    root->addSubstring("T");

    // Get the "TEST" nodes
    Node *e_node = root->getChild('E');
    Node *s_node = e_node->getChild('S');
    Node *t_node = s_node->getChild('T');

    // Get the "TARGET" nodes
    Node *a_node = root->getChild('A');
    Node *r_node = a_node->getChild('R');
    Node *g_node = r_node->getChild('G');
    Node *e2_node = g_node->getChild('E');
    Node *t2_node = e2_node->getChild('T');

    // Check if the nodes correctly return the substring they represent
    SUBCASE("Root")
    {
      list<string> represented_strings = root->getRepresentedSubstrings();
      CHECK(represented_strings.size() == 4);
      CHECK(find(represented_strings.begin(), represented_strings.end(),
                 "TARGET") != represented_strings.end());
      CHECK(find(represented_strings.begin(), represented_strings.end(),
                 "TEST") != represented_strings.end());
      CHECK(find(represented_strings.begin(), represented_strings.end(), "T") !=
            represented_strings.end());
      CHECK(find(represented_strings.begin(), represented_strings.end(), "T") !=
            represented_strings.end());
    }

    SUBCASE("E")
    {
      list<string> represented_strings = e_node->getRepresentedSubstrings();
      CHECK(represented_strings.size() == 1);
      CHECK(find(represented_strings.begin(), represented_strings.end(),
                 "TEST") != represented_strings.end());
    }

    SUBCASE("S")
    {
      list<string> represented_strings = s_node->getRepresentedSubstrings();
      CHECK(represented_strings.size() == 1);
      CHECK(find(represented_strings.begin(), represented_strings.end(),
                 "TEST") != represented_strings.end());
    }

    SUBCASE("T")
    {
      list<string> represented_strings = t_node->getRepresentedSubstrings();
      CHECK(represented_strings.size() == 1);
      CHECK(find(represented_strings.begin(), represented_strings.end(),
                 "TEST") != represented_strings.end());
    }

    SUBCASE("A")
    {
      list<string> represented_strings = a_node->getRepresentedSubstrings();
      CHECK(represented_strings.size() == 1);
      CHECK(find(represented_strings.begin(), represented_strings.end(),
                 "TARGET") != represented_strings.end());
    }

    SUBCASE("R")
    {
      list<string> represented_strings = r_node->getRepresentedSubstrings();
      CHECK(represented_strings.size() == 1);
      CHECK(find(represented_strings.begin(), represented_strings.end(),
                 "TARGET") != represented_strings.end());
    }

    SUBCASE("G")
    {
      list<string> represented_strings = g_node->getRepresentedSubstrings();
      CHECK(represented_strings.size() == 1);
      CHECK(find(represented_strings.begin(), represented_strings.end(),
                 "TARGET") != represented_strings.end());
    }

    SUBCASE("E2")
    {
      list<string> represented_strings = e2_node->getRepresentedSubstrings();
      CHECK(represented_strings.size() == 1);
      CHECK(find(represented_strings.begin(), represented_strings.end(),
                 "TARGET") != represented_strings.end());
    }

    SUBCASE("T2")
    {
      list<string> represented_strings = t2_node->getRepresentedSubstrings();
      CHECK(represented_strings.size() == 1);
      CHECK(find(represented_strings.begin(), represented_strings.end(),
                 "TARGET") != represented_strings.end());
    }

    // Clean up
    delete root;
  }
}

TEST_CASE("Check if equal trees are correctly identified")
{
  SUBCASE("Two equal trees with strings TEST and TARGET")
  {
    // Create the first tree
    Node *root1 = new Node('T', 0, 0, NULL);

    // Add the strings themselves to the tree
    root1->addSubstring("TEST");
    root1->addSubstring("TARGET");

    // Create the second tree
    Node *root2 = new Node('T', 0, 0, NULL);

    // Add the strings themselves to the tree
    root2->addSubstring("TEST");
    root2->addSubstring("TARGET");

    // Check if the trees are equal
    CHECK(*root1 == *root2);

    // Clean up
    delete root1;
    delete root2;
  }

  SUBCASE(
      "Two unequal trees with strings TEST and TARGET resp. TEST and TARG")
  {
    // Create the first tree
    Node *root1 = new Node('T', 0, 0, NULL);

    // Add the strings themselves to the tree
    root1->addSubstring("TEST");
    root1->addSubstring("TARGET");

    // Create the second tree
    Node *root2 = new Node('T', 0, 0, NULL);

    // Add the strings themselves to the tree
    root2->addSubstring("TEST");
    root2->addSubstring("TARG");

    // Check if the trees are equal
    CHECK(!(*root1 == *root2));

    // Clean up
    delete root1;
    delete root2;
  }
}

TEST_CASE("Check if unequal trees are correctly identified")
{
  SUBCASE(
      "Two unequal trees with strings TEST and TARGET resp. TEST and TARG")
  {
    // Create the first tree
    Node *root1 = new Node('T', 0, 0, NULL);

    // Add the strings themselves to the tree
    root1->addSubstring("TEST");
    root1->addSubstring("TARGET");

    // Create the second tree
    Node *root2 = new Node('T', 0, 0, NULL);

    // Add the strings themselves to the tree
    root2->addSubstring("TEST");
    root2->addSubstring("TARG");

    // Check if the trees are unequal
    CHECK(*root1 != *root2);

    // Clean up
    delete root1;
    delete root2;
  }

  SUBCASE("Two equal trees with strings TEST and TARGET")
  {
    // Create the first tree
    Node *root1 = new Node('T', 0, 0, NULL);

    // Add the strings themselves to the tree
    root1->addSubstring("TEST");
    root1->addSubstring("TARGET");

    // Create the second tree
    Node *root2 = new Node('T', 0, 0, NULL);

    // Add the strings themselves to the tree
    root2->addSubstring("TEST");
    root2->addSubstring("TARGET");

    // Check if the trees are unequal
    CHECK(!(*root1 != *root2));

    // Clean up
    delete root1;
    delete root2;
  }
}

TEST_CASE("Check if full occurrences of a string within a tree are deleted "
          "correctly")
{
  SUBCASE(
      "Tree with strings ATTEST and ACADEMIC where TEST should be deleted")
  {
    // Create the tree
    Node *root = new Node('A', 0, 0, NULL);

    // Add the strings themselves to the tree
    root->addSubstring("ATTEST");
    root->addSubstring("ACADEMIC");

    // Delete the all occurrences of TEST
    root->deleteFullStringOccurrences("TEST");

    // Create the validation tree
    Node *validation_root = new Node('A', 0, 0, NULL);

    // Add the strings themselves to the tree
    validation_root->addSubstring("AT");
    validation_root->addSubstring("ACADEMIC");

    // Check if the trees are equal
    CHECK(*root == *validation_root);

    // Clean up
    delete root;
    delete validation_root;
  }

  SUBCASE(
      "Tree with strings TEST, TESTER and TEMPO where TEST should be deleted")
  {
    // Create the tree
    Node *root = new Node('T', 0, 0, NULL);

    // Add the strings themselves to the tree
    root->addSubstring("TEST");
    root->addSubstring("TESTER");
    root->addSubstring("TEMPO");

    // Delete the all occurrences of TEST
    root->deleteFullStringOccurrences("TEST");

    // Create the validation tree
    Node *validation_root = new Node('T', 0, 0, NULL);

    // Add the strings themselves to the tree
    validation_root->addSubstring("TEMPO");

    // Check if the trees are equal
    CHECK(*root == *validation_root);

    // Clean up
    delete root;
    delete validation_root;
  }

  SUBCASE("Tree with strings T, TEST and TTEST where TEST should be deleted")
  {
    // Create the tree
    Node *root = new Node('T', 0, 0, NULL);

    // Add the strings themselves to the tree
    root->addSubstring("T");
    root->addSubstring("TTEST");
    root->addSubstring("TEST");

    // Delete the all occurrences of TEST
    root->deleteFullStringOccurrences("TEST");

    // Create the validation tree
    Node *validation_root = new Node('T', 0, 0, NULL);

    // Add the strings themselves to the tree
    validation_root->addSubstring("T");
    validation_root->addSubstring("T");

    // Check if the trees are equal
    CHECK(*root == *validation_root);

    // Clean up
    delete root;
    delete validation_root;
  }

  SUBCASE("Tree with strings TE, TE, TESTESTI, TESTIESTI, TESTI where ESTI "
          "should be deleted")
  {
    // Create the tree
    Node *root = new Node('T', 0, 0, NULL);

    // Add the strings themselves to the tree
    root->addSubstring("TE");
    root->addSubstring("TE");
    root->addSubstring("TESTESTI");
    root->addSubstring("TESTIESTI");
    root->addSubstring("TESTI");

    // Delete the all occurrences of ESTI
    root->deleteFullStringOccurrences("ESTI");

    // Create the validation tree
    Node *validation_root = new Node('T', 0, 0, NULL);

    // Add the strings themselves to the tree
    validation_root->addSubstring("TE");
    validation_root->addSubstring("TE");
    validation_root->addSubstring("TEST");
    validation_root->addSubstring("T");
    validation_root->addSubstring("T");

    // Check if the trees are equal
    CHECK(*root == *validation_root);

    // Clean up
    delete root;
    delete validation_root;
  }

  SUBCASE("Tree based on dbtext/city (see integration test in "
          "tree_collection_test with 100 samples that needs to be fixed) there "
          "TON should be deleteted")
  {
    // Create the tree
    Node *root = new Node('A', 0, 0, NULL);

    // Add the strings themselves to the tree
    root->addSubstring("A DUNES");
    root->addSubstring("ACAU");
    root->addSubstring("ACA");
    root->addSubstring("ACENTIA");
    root->addSubstring("ACH");
    root->addSubstring("ACKLAND AFB");
    root->addSubstring("ACON");
    root->addSubstring("AD AXE");
    root->addSubstring("ADFORD");
    root->addSubstring("ADOWVIEW");
    root->addSubstring("ADS");
    root->addSubstring("AD");
    root->addSubstring("AFB");
    root->addSubstring("AFFREY");
    root->addSubstring("AFFTON");
    root->addSubstring("AGO");
    root->addSubstring("AH");
    root->addSubstring("AH");
    root->addSubstring("AINT STEPHENS CHURCH");
    root->addSubstring("AIN");
    root->addSubstring("ALE");
    root->addSubstring("ALL");
    root->addSubstring("ALM BEACH");
    root->addSubstring("AL");
    root->addSubstring("AMBERG");
    root->addSubstring("AMONDHEAD");
    root->addSubstring("AM");
    root->addSubstring("AM");
    root->addSubstring("AND AFB");
    root->addSubstring("ANDLER");
    root->addSubstring("ANDYWINE");
    root->addSubstring("AND");
    root->addSubstring("ANKSTON");
    root->addSubstring("ANOGA");
    root->addSubstring("ANTON");
    root->addSubstring("AN");
    root->addSubstring("AN");
    root->addSubstring("AONIA");
    root->addSubstring("APOLEON");
    root->addSubstring("ARBLE");
    root->addSubstring("ARBOR");
    root->addSubstring("ARIEN");
    root->addSubstring("ARL HARBOR");
    root->addSubstring("ARLEVOIX");
    root->addSubstring("ARSHALL");
    root->addSubstring("ARWICH");
    root->addSubstring("AR");
    root->addSubstring("AST BERNE");
    root->addSubstring("AST CHICAGO");
    root->addSubstring("ASTVALE");
    root->addSubstring("ATIONAL");
    root->addSubstring("ATTANOGA");
    root->addSubstring("ATTE");
    root->addSubstring("AUDER");
    root->addSubstring("AUQUA");
    root->addSubstring("AU");
    root->addSubstring("AVACA");
    root->addSubstring("AVE");
    root->addSubstring("AVIA");
    root->addSubstring("AVRE");
    root->addSubstring("AW");
    root->addSubstring("AXE");
    root->addSubstring("AYPOOL");
    root->addSubstring("AY");
    root->addSubstring("A");
    root->addSubstring("A");
    root->addSubstring("A");
    root->addSubstring("A");
    root->addSubstring("A");
    root->addSubstring("A");
    root->addSubstring("A");
    root->addSubstring("A");

    // Delete the all occurrences of TON
    root->deleteFullStringOccurrences("TON");

    // Create the validation tree
    Node *validation_root = new Node('A', 0, 0, NULL);

    // Add the strings themselves to the tree
    validation_root->addSubstring("A DUNES");
    validation_root->addSubstring("ACAU");
    validation_root->addSubstring("ACA");
    validation_root->addSubstring("ACENTIA");
    validation_root->addSubstring("ACH");
    validation_root->addSubstring("ACKLAND AFB");
    validation_root->addSubstring("ACON");
    validation_root->addSubstring("AD AXE");
    validation_root->addSubstring("ADFORD");
    validation_root->addSubstring("ADOWVIEW");
    validation_root->addSubstring("ADS");
    validation_root->addSubstring("AD");
    validation_root->addSubstring("AFB");
    validation_root->addSubstring("AFFREY");
    validation_root->addSubstring("AFF");
    validation_root->addSubstring("AGO");
    validation_root->addSubstring("AH");
    validation_root->addSubstring("AH");
    validation_root->addSubstring("AINT STEPHENS CHURCH");
    validation_root->addSubstring("AIN");
    validation_root->addSubstring("ALE");
    validation_root->addSubstring("ALL");
    validation_root->addSubstring("ALM BEACH");
    validation_root->addSubstring("AL");
    validation_root->addSubstring("AMBERG");
    validation_root->addSubstring("AMONDHEAD");
    validation_root->addSubstring("AM");
    validation_root->addSubstring("AM");
    validation_root->addSubstring("AND AFB");
    validation_root->addSubstring("ANDLER");
    validation_root->addSubstring("ANDYWINE");
    validation_root->addSubstring("AND");
    validation_root->addSubstring("ANKS");
    validation_root->addSubstring("ANOGA");
    validation_root->addSubstring("AN");
    validation_root->addSubstring("AN");
    validation_root->addSubstring("AN");
    validation_root->addSubstring("AONIA");
    validation_root->addSubstring("APOLEON");
    validation_root->addSubstring("ARBLE");
    validation_root->addSubstring("ARBOR");
    validation_root->addSubstring("ARIEN");
    validation_root->addSubstring("ARL HARBOR");
    validation_root->addSubstring("ARLEVOIX");
    validation_root->addSubstring("ARSHALL");
    validation_root->addSubstring("ARWICH");
    validation_root->addSubstring("AR");
    validation_root->addSubstring("AST BERNE");
    validation_root->addSubstring("AST CHICAGO");
    validation_root->addSubstring("ASTVALE");
    validation_root->addSubstring("ATIONAL");
    validation_root->addSubstring("ATTANOGA");
    validation_root->addSubstring("ATTE");
    validation_root->addSubstring("AUDER");
    validation_root->addSubstring("AUQUA");
    validation_root->addSubstring("AU");
    validation_root->addSubstring("AVACA");
    validation_root->addSubstring("AVE");
    validation_root->addSubstring("AVIA");
    validation_root->addSubstring("AVRE");
    validation_root->addSubstring("AW");
    validation_root->addSubstring("AXE");
    validation_root->addSubstring("AYPOOL");
    validation_root->addSubstring("AY");
    validation_root->addSubstring("A");
    validation_root->addSubstring("A");
    validation_root->addSubstring("A");
    validation_root->addSubstring("A");
    validation_root->addSubstring("A");
    validation_root->addSubstring("A");
    validation_root->addSubstring("A");
    validation_root->addSubstring("A");

    // root->printTree();
    // list<string> substrings = root->getRepresentedSubstrings();

    // Print the substrings
    // for (auto it = substrings.begin(); it != substrings.end(); it++) {
    //    cout << *it << endl;
    //}

    // validation_root->printTree();
    // list<string> validation_substrings =
    // validation_root->getRepresentedSubstrings();

    // Print the validation_substrings
    // for (auto it = validation_substrings.begin(); it !=
    // validation_substrings.end(); it++) {
    //    cout << *it << endl;
    //}

    // Check if the trees are equal
    CHECK(*root == *validation_root);

    // Clean up
    delete root;
    delete validation_root;
  }

  SUBCASE("Tree based on dbtext/city (see integration test in "
          "tree_collection_test with 100 samples that needs to be fixed) there "
          " PARK should be deleteted")
  {
    // Create the tree
    Node *root = new Node('N', 0, 0, NULL);

    // Add the strings themselves to the tree
    root->addSubstring("NT WASHINGTON");
    root->addSubstring("NT FURNACE");
    root->addSubstring("NT ORAB");
    root->addSubstring("NT LOUIS");
    root->addSubstring("NTERHAVEN");
    root->addSubstring("NTEREY");
    root->addSubstring("NTA GORDA");
    root->addSubstring("NTANBURG");
    root->addSubstring("NTO DOMINGO PUEBLO");
    root->addSubstring("NTON");
    root->addSubstring("NT");
    root->addSubstring("NGTON");
    root->addSubstring("NGSTOWN");
    root->addSubstring("NGS");
    root->addSubstring("NGS");
    root->addSubstring("NGS");
    root->addSubstring("NGER");
    root->addSubstring("NGO PUEBLO");
    root->addSubstring("NGLE SPRINGS");
    root->addSubstring("ND PARK");
    root->addSubstring("NDALE");
    root->addSubstring("NDALL PARK");
    root->addSubstring("NDA HTS");
    root->addSubstring("NY");
    root->addSubstring("NY");
    root->addSubstring("NACE");
    root->addSubstring("NAVASOTA");
    root->addSubstring("NAHAWKIN");
    root->addSubstring("NA");
    root->addSubstring("NELLIS AIR FORCE BASE");
    root->addSubstring("NEOYE");
    root->addSubstring("NEW KINGSTOWN");
    root->addSubstring("NEWTON SQ");
    root->addSubstring("NEWPORT RICHEY");
    root->addSubstring("NE BRIDGE");
    root->addSubstring("NE BEACH");
    root->addSubstring("NE PLAINS");
    root->addSubstring("NESBORO");
    root->addSubstring("NE");
    root->addSubstring("N SQ");
    root->addSubstring("N SPA");
    root->addSubstring("N ON GAULEY");
    root->addSubstring("N GAULEY");
    root->addSubstring("NMOUTH");
    root->addSubstring("NORTHGLENN");
    root->addSubstring("NORTH PALM BEACH");
    root->addSubstring("NOROFLK");
    root->addSubstring("NNY");
    root->addSubstring("NNS GROVE");
    root->addSubstring("NN");
    root->addSubstring("NS GROVE");
    root->addSubstring("NSHIP");
    root->addSubstring("NS");
    root->addSubstring("NKSTER");
    root->addSubstring("NITE CITY");
    root->addSubstring("NBURG");
    root->addSubstring("NBURY");
    root->addSubstring("N");
    root->addSubstring("N");
    root->addSubstring("N");
    root->addSubstring("N");
    root->addSubstring("N");
    root->addSubstring("N");
    root->addSubstring("N");
    root->addSubstring("N");
    root->addSubstring("N");
    root->addSubstring("N");
    root->addSubstring("N");
    root->addSubstring("N");
    root->addSubstring("N");
    root->addSubstring("N");

    // Delete the all occurrences of " PARK"
    root->deleteFullStringOccurrences(" PARK");

    // Create the validation tree
    Node *validation_root = new Node('N', 0, 0, NULL);

    // Add the strings themselves to the tree
    validation_root->addSubstring("NT WASHINGTON");
    validation_root->addSubstring("NT FURNACE");
    validation_root->addSubstring("NT ORAB");
    validation_root->addSubstring("NT LOUIS");
    validation_root->addSubstring("NTERHAVEN");
    validation_root->addSubstring("NTEREY");
    validation_root->addSubstring("NTA GORDA");
    validation_root->addSubstring("NTANBURG");
    validation_root->addSubstring("NTO DOMINGO PUEBLO");
    validation_root->addSubstring("NTON");
    validation_root->addSubstring("NT");
    validation_root->addSubstring("NGTON");
    validation_root->addSubstring("NGSTOWN");
    validation_root->addSubstring("NGS");
    validation_root->addSubstring("NGS");
    validation_root->addSubstring("NGS");
    validation_root->addSubstring("NGER");
    validation_root->addSubstring("NGO PUEBLO");
    validation_root->addSubstring("NGLE SPRINGS");
    validation_root->addSubstring("ND");
    validation_root->addSubstring("NDALE");
    validation_root->addSubstring("NDALL");
    validation_root->addSubstring("NDA HTS");
    validation_root->addSubstring("NY");
    validation_root->addSubstring("NY");
    validation_root->addSubstring("NACE");
    validation_root->addSubstring("NAVASOTA");
    validation_root->addSubstring("NAHAWKIN");
    validation_root->addSubstring("NA");
    validation_root->addSubstring("NELLIS AIR FORCE BASE");
    validation_root->addSubstring("NEOYE");
    validation_root->addSubstring("NEW KINGSTOWN");
    validation_root->addSubstring("NEWTON SQ");
    validation_root->addSubstring("NEWPORT RICHEY");
    validation_root->addSubstring("NE BRIDGE");
    validation_root->addSubstring("NE BEACH");
    validation_root->addSubstring("NE PLAINS");
    validation_root->addSubstring("NESBORO");
    validation_root->addSubstring("NE");
    validation_root->addSubstring("N SQ");
    validation_root->addSubstring("N SPA");
    validation_root->addSubstring("N ON GAULEY");
    validation_root->addSubstring("N GAULEY");
    validation_root->addSubstring("NMOUTH");
    validation_root->addSubstring("NORTHGLENN");
    validation_root->addSubstring("NORTH PALM BEACH");
    validation_root->addSubstring("NOROFLK");
    validation_root->addSubstring("NNY");
    validation_root->addSubstring("NNS GROVE");
    validation_root->addSubstring("NN");
    validation_root->addSubstring("NS GROVE");
    validation_root->addSubstring("NSHIP");
    validation_root->addSubstring("NS");
    validation_root->addSubstring("NKSTER");
    validation_root->addSubstring("NITE CITY");
    validation_root->addSubstring("NBURG");
    validation_root->addSubstring("NBURY");
    validation_root->addSubstring("N");
    validation_root->addSubstring("N");
    validation_root->addSubstring("N");
    validation_root->addSubstring("N");
    validation_root->addSubstring("N");
    validation_root->addSubstring("N");
    validation_root->addSubstring("N");
    validation_root->addSubstring("N");
    validation_root->addSubstring("N");
    validation_root->addSubstring("N");
    validation_root->addSubstring("N");
    validation_root->addSubstring("N");
    validation_root->addSubstring("N");
    validation_root->addSubstring("N");

    // Check if the trees are equal
    CHECK(*root == *validation_root);

    // Clean up
    delete root;
    delete validation_root;
  }
}

TEST_CASE("Check if substrings are correctly subtracted from the tree")
{
  SUBCASE("Tree with strings TEST and TEMPO where TEST should be deleted")
  {
    // Create the tree
    Node *root = new Node('T', 0, 0, NULL);

    // Add the strings themselves to the tree
    root->addSubstring("TEST");
    root->addSubstring("TEMPO");

    // Subtract substring TEST
    root->subtractSubstring("TEST");

    // Create the validation tree
    Node *validation_root = new Node('T', 0, 0, NULL);

    // Add the strings themselves to the tree
    validation_root->addSubstring("TEMPO");

    // Check if the trees are equal (there should not be any deletion)
    CHECK(*root == *validation_root);

    // Clean up
    delete root;
    delete validation_root;
  }

  SUBCASE(
      "Tree with strings TEST, TESTER and TEMPO where TEST should be deleted")
  {
    // Create the tree
    Node *root = new Node('T', 0, 0, NULL);

    // Add the strings themselves to the tree
    root->addSubstring("TEST");
    root->addSubstring("TEMPO");
    root->addSubstring("TESTER");

    // Subtract substring TEST
    root->subtractSubstring("TEST");

    // Create the validation tree
    Node *validation_root = new Node('T', 0, 0, NULL);

    // Add the strings themselves to the tree
    validation_root->addSubstring("TEMPO");
    validation_root->addSubstring("TESTER");

    // Check if the trees are equal (there should not be any deletion)
    CHECK(*root == *validation_root);

    // Clean up
    delete root;
    delete validation_root;
  }

  SUBCASE("Tree with strings TEST and TEMPO where DOMINIK should be deleted "
          "(exception expected)")
  {
    // Create the tree
    Node *root = new Node('T', 0, 0, NULL);

    // Add the strings themselves to the tree
    root->addSubstring("TEST");
    root->addSubstring("TEMPO");

    // Subtract substring TEST
    CHECK_THROWS(root->subtractSubstring("DOMINIK"));

    // Create the validation tree
    Node *validation_root = new Node('T', 0, 0, NULL);

    // Add the strings themselves to the tree
    validation_root->addSubstring("TEMPO");
    validation_root->addSubstring("TEST");

    // Check if the trees are equal (there should not be any deletion)
    CHECK(*root == *validation_root);

    // Clean up
    delete root;
    delete validation_root;
  }

  SUBCASE("Tree with strings TEST and TEMPO where TESTER should be deleted "
          "(exception expected)")
  {
    // Create the tree
    Node *root = new Node('T', 0, 0, NULL);

    // Add the strings themselves to the tree
    root->addSubstring("TEST");
    root->addSubstring("TEMPO");

    // Subtract substring TEST
    CHECK_THROWS(root->subtractSubstring("TESTER"));

    // Create the validation tree
    Node *validation_root = new Node('T', 0, 0, NULL);

    // Add the strings themselves to the tree
    validation_root->addSubstring("TEMPO");
    validation_root->addSubstring("TEST");

    // Check if the trees are equal (there should not be any deletion)
    CHECK(*root == *validation_root);

    // Clean up
    delete root;
    delete validation_root;
  }
}