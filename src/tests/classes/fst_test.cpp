// Library includes
using namespace std;
#include <algorithm>
#include <iostream>
#include <list>
#include <string>

// Doctest include
#include "../doctest/doctest.h"

// Include the files to test
#include "../../classes/fst.h"

// ---------------------------------------------------------------------------------------------
// Class-level tests
// ---------------------------------------------------------------------------------------------
// Tests are focussed on testing the functionality of the class as a whole

TEST_CASE("Check if a node representing a substring is correctly identified")
{
  SUBCASE("TEST")
  {
    // Create the FST
    FST *fst = new FST();

    // Add the string to the FST
    fst->addString("TEST");

    // Get the different nodes
    Node *root = fst->getPath('T');
    Node *e_node = root->getChild('E');
    Node *s_node = e_node->getChild('S');
    Node *t_node = s_node->getChild('T');

    // Check if the nodes are returned correctly
    CHECK(fst->getNodeRepresentingSubstring("T") == root);
    CHECK(fst->getNodeRepresentingSubstring("TE") == e_node);
    CHECK(fst->getNodeRepresentingSubstring("TES") == s_node);
    CHECK(fst->getNodeRepresentingSubstring("TEST") == t_node);
    CHECK(fst->getNodeRepresentingSubstring("TESTE") == NULL);
    CHECK(fst->getNodeRepresentingSubstring("DOMI") == NULL);

    // Clean up
    delete fst;
  }
}

TEST_CASE("Check if equal FSTs are correctly identified")
{
  SUBCASE("Two equal FSTs with strings TEST, TARGET and DOMINIK")
  {
    // Create the first FST
    FST *fst1 = new FST();

    // Add the strings themselves to the FST
    fst1->addString("TEST");
    fst1->addString("TARGET");
    fst1->addString("DOMINIK");

    // Create the second FST
    FST *fst2 = new FST();

    // Add the strings themselves to the FST
    fst2->addString("TEST");
    fst2->addString("TARGET");
    fst2->addString("DOMINIK");

    // Check if the FSTs are equal
    CHECK(*fst1 == *fst2);

    // Clean up
    delete fst1;
    delete fst2;
  }

  SUBCASE("Two unequal FSTs with strings TEST, TARGET and DOMINIK "
          "resp. TEST, TARGET and PROBST")
  {
    // Create the first FST
    FST *fst1 = new FST();

    // Add the strings themselves to the FST
    fst1->addString("TEST");
    fst1->addString("TARGET");
    fst1->addString("DOMINIK");

    // Create the second FST
    FST *fst2 = new FST();

    // Add the strings themselves to the FST
    fst2->addString("TEST");
    fst2->addString("TARGET");
    fst2->addString("PROBST");

    // Check if the FSTs are equal
    CHECK(!(*fst1 == *fst2));

    // Clean up
    delete fst1;
    delete fst2;
  }

  SUBCASE("Two unequal FSTs with strings TEST, TARGET and DOMINIK "
          "resp. no strings")
  {
    // Create the first FST
    FST *fst1 = new FST();

    // Add the strings themselves to the FST
    fst1->addString("TEST");
    fst1->addString("TARGET");
    fst1->addString("DOMINIK");

    // Create the second FST
    FST *fst2 = new FST();

    // Check if the FSTs are equal
    CHECK(!(*fst1 == *fst2));

    // Clean up
    delete fst1;
    delete fst2;
  }

  SUBCASE("Two unequal FSTs with strings no strings resp. TEST, "
          "TARGET and PROBST")
  {
    // Create the first FST
    FST *fst1 = new FST();

    // Create the second FST
    FST *fst2 = new FST();

    // Add the strings themselves to the FST
    fst2->addString("TEST");
    fst2->addString("TARGET");
    fst2->addString("PROBST");

    // Check if the FSTs are equal
    CHECK(!(*fst1 == *fst2));

    // Clean up
    delete fst1;
    delete fst2;
  }
}

TEST_CASE("Check if unequal FSTs are correctly identified")
{
  SUBCASE("Two unequal FSTs with strings TEST, TARGET and DOMINIK "
          "resp. TEST, TARGET and PROBST")
  {
    // Create the first FST
    FST *fst1 = new FST();

    // Add the strings themselves to the FST
    fst1->addString("TEST");
    fst1->addString("TARGET");
    fst1->addString("DOMINIK");

    // Create the second FST
    FST *fst2 = new FST();

    // Add the strings themselves to the FST
    fst2->addString("TEST");
    fst2->addString("TARGET");
    fst2->addString("PROBST");

    // Check if the FSTs are unequal
    CHECK(*fst1 != *fst2);

    // Clean up
    delete fst1;
    delete fst2;
  }

  SUBCASE("Two unequal FSTs with strings TEST, TARGET and DOMINIK "
          "resp. no strings")
  {
    // Create the first FST
    FST *fst1 = new FST();

    // Add the strings themselves to the FST
    fst1->addString("TEST");
    fst1->addString("TARGET");
    fst1->addString("DOMINIK");

    // Create the second FST
    FST *fst2 = new FST();

    // Check if the FSTs are unequal
    CHECK(*fst1 != *fst2);

    // Clean up
    delete fst1;
    delete fst2;
  }

  SUBCASE("Two unequal FSTs with strings no strings resp. TEST, "
          "TARGET and PROBST")
  {
    // Create the first FST
    FST *fst1 = new FST();

    // Create the second FST
    FST *fst2 = new FST();

    // Add the strings themselves to the FST
    fst2->addString("TEST");
    fst2->addString("TARGET");
    fst2->addString("PROBST");

    // Check if the FSTs are unequal
    CHECK(*fst1 != *fst2);

    // Clean up
    delete fst1;
    delete fst2;
  }

  SUBCASE("Two equal FSTs with strings TEST, TARGET and DOMINIK")
  {
    // Create the first FST
    FST *fst1 = new FST();

    // Add the strings themselves to the FST
    fst1->addString("TEST");
    fst1->addString("TARGET");
    fst1->addString("DOMINIK");

    // Create the second FST
    FST *fst2 = new FST();

    // Add the strings themselves to the FST
    fst2->addString("TEST");
    fst2->addString("TARGET");
    fst2->addString("DOMINIK");

    // Check if the FSTs are unequal
    CHECK(!(*fst1 != *fst2));

    // Clean up
    delete fst1;
    delete fst2;
  }
}

TEST_CASE("Check if full occurrences of a string within the FST are "
          "deleted correctly")
{
  SUBCASE("FST with strings ATTEST and ACADEMIC where TEST should "
          "be deleted")
  {
    // Create the FST
    FST *fst = new FST();

    // Add the strings themselves to the FST
    fst->addString("ATTEST");
    fst->addString("ACADEMIC");

    // Delete the all occurrences of TEST
    fst->deleteFullStringOccurrences("TEST");

    // Create the validation FST
    FST *validation_fst = new FST();

    // Add the strings themselves to the FST
    validation_fst->addString("ACADEMIC");

    // Only the full string "TEST" should be deleted => "AT[T]EST" should still
    // be in the FST
    validation_fst->addString("AT");
    validation_fst->addString("EST");

    // Check if the FSTs are equal
    CHECK(*fst == *validation_fst);

    // Clean up
    delete fst;
    delete validation_fst;
  }
}

TEST_CASE(
    "Check if substrings are correctly subtracted from the FST")
{
  SUBCASE("FST with strings TEST and TEMPO where TEST should be "
          "deleted")
  {
    // Create the FST
    FST *fst = new FST();

    // Add the strings themselves to the FST
    fst->addString("TEST");
    fst->addString("TEMPO");

    // Subtract substring TEST
    fst->subtractSubstring("TEST");

    // Create the validation FST
    FST *validation_fst = new FST();

    // Add the strings themselves to the FST
    validation_fst->addString("TEMPO");

    // Only the substring "TEST" in the "T" path should be deleted => "[T]EST"
    // should still be in the FST
    validation_fst->addString("EST");

    // Check if the FSTs are equal
    CHECK(*fst == *validation_fst);

    // Clean up
    delete fst;
    delete validation_fst;
  }

  SUBCASE("FST with strings TEST, TEST and TEMPO where TEST should "
          "be deleted")
  {
    // Create the FST
    FST *fst = new FST();

    // Add the strings themselves to the FST
    fst->addString("TEST");
    fst->addString("TEST");
    fst->addString("TEMPO");

    // Subtract substring TEST
    fst->subtractSubstring("TEST");

    // Create the validation FST
    FST *validation_fst = new FST();

    // Add the strings themselves to the FST
    validation_fst->addString("TEST");
    validation_fst->addString("TEMPO");

    // Only the substring "TEST" in the "T" path should be deleted => "[T]EST"
    // should still be in the FST
    validation_fst->addString("EST");

    // Check if the FSTs are equal
    CHECK(*fst == *validation_fst);

    // Clean up
    delete fst;
    delete validation_fst;
  }

  SUBCASE("FST with strings TEST and TEMPO where DOMINIK should be "
          "deleted (exception expected)")
  {
    // Create the FST
    FST *fst = new FST();

    // Add the strings themselves to the FST
    fst->addString("TEST");
    fst->addString("TEMPO");

    // Subtract substring TEST
    CHECK_THROWS(fst->subtractSubstring("DOMINIK"));

    // Create the validation FST
    FST *validation_fst = new FST();

    // Add the strings themselves to the FST
    validation_fst->addString("TEST");
    validation_fst->addString("TEMPO");

    // Check if the FSTs are equal
    CHECK(*fst == *validation_fst);

    // Clean up
    delete fst;
    delete validation_fst;
  }

  SUBCASE("FST with strings TEST and TEMPO where TESTER should be "
          "deleted (exception expected)")
  {
    // Create the FST
    FST *fst = new FST();

    // Add the strings themselves to the FST
    fst->addString("TEST");
    fst->addString("TEMPO");

    // Subtract substring TEST
    CHECK_THROWS(fst->subtractSubstring("TESTER"));

    // Create the validation FST
    FST *validation_fst = new FST();

    // Add the strings themselves to the FST
    validation_fst->addString("TEST");
    validation_fst->addString("TEMPO");

    // Check if the FSTs are equal
    CHECK(*fst == *validation_fst);

    // Clean up
    delete fst;
    delete validation_fst;
  }
}

TEST_CASE("Test whether the substring replacement works correctly")
{
  SUBCASE("FST with the strings TEST, ATTEST, TESTER and ESTI "
          "where TEST should be replaced")
  {
    // Create the FST
    FST *fst = new FST();

    // Add the strings themselves to the FST
    fst->addString("TEST");
    fst->addString("ATTEST");
    fst->addString("TESTER");
    fst->addString("ESTI");

    // Get the node representing "TEST"
    Node *test_node = fst->getNodeRepresentingSubstring("TEST");

    // Handle the replacement of the substring "TEST"
    fst->handleSubstringAddedToDict(test_node);

    // Create the validation FST
    FST *validation_fst = new FST();

    // Add the strings themselves to the FST
    validation_fst->addString("AT");
    validation_fst->addString("ESTI");
    validation_fst->addString("ER");

    // Check if the FSTs are equal
    CHECK(*fst == *validation_fst);

    // Clean up
    delete fst;
    delete validation_fst;
  }

  SUBCASE("FST with the strings TESTESTESTEST, ATTEST and ESTI "
          "where TEST should be replaced")
  {
    // Create the FST
    FST *fst = new FST();

    // Add the strings themselves to the FST
    fst->addString("TESTESTESTEST");
    fst->addString("ATTEST");
    fst->addString("ESTI");

    // Get the node representing "TEST"
    Node *test_node = fst->getNodeRepresentingSubstring("TEST");

    // Handle the replacement of the substring "TEST"
    fst->handleSubstringAddedToDict(test_node);

    // Create the validation FST
    FST *validation_fst = new FST();

    // Add the strings themselves to the FST
    validation_fst->addString("AT");
    validation_fst->addString("ESTI");
    validation_fst->addString("ES");
    validation_fst->addString("EST");

    // Check if the FSTs are equal
    CHECK(*fst == *validation_fst);

    // Clean up
    delete fst;
    delete validation_fst;
  }

  SUBCASE("FST with the strings TESTESTEST and TESTER where TEST "
          "should be replaced")
  {
    // Create the FST
    FST *fst = new FST();

    // Add the strings themselves to the FST
    fst->addString("TESTESTEST");
    fst->addString("TESTER");

    // Get the node representing "TEST"
    Node *test_node = fst->getNodeRepresentingSubstring("TEST");

    // Handle the replacement of the substring "TEST"
    fst->handleSubstringAddedToDict(test_node);

    // Create the validation FST
    FST *validation_fst = new FST();

    // Add the strings themselves to the FST
    validation_fst->addString("ES");
    validation_fst->addString("ER");

    // Check if the FSTs are equal
    CHECK(*fst == *validation_fst);

    // Clean up
    delete fst;
    delete validation_fst;
  }

  SUBCASE("FST with the strings TESTESTE, TEST where TEST should "
          "be replaced")
  {
    // Create the FST
    FST *fst = new FST();

    // Add the strings themselves to the FST
    fst->addString("TESTESTE");
    fst->addString("TEST");

    // Get the node representing "TEST"
    Node *test_node = fst->getNodeRepresentingSubstring("TEST");

    // Handle the replacement of the substring "TEST"
    fst->handleSubstringAddedToDict(test_node);

    // Create the validation FST
    FST *validation_fst = new FST();

    // Add the strings themselves to the FST
    validation_fst->addString("ESTE");

    // Check if the FSTs are equal
    CHECK(*fst == *validation_fst);

    // Clean up
    delete fst;
    delete validation_fst;
  }

  SUBCASE("FST with the strings TESTESTI, ESTIMATE, WRONGESTIMATE, "
          "TESTIESTI where ESTI should be replaced")
  {
    // Create the FST
    FST *fst = new FST();

    // Add the strings themselves to the FST
    fst->addString("TESTESTI");
    fst->addString("ESTIMATE");
    fst->addString("WRONGESTIMATE");
    fst->addString("TESTIESTI");

    // Get the node representing "TEST"
    Node *test_node = fst->getNodeRepresentingSubstring("ESTI");

    // Handle the replacement of the substring "ESTI"
    fst->handleSubstringAddedToDict(test_node);

    // Create the validation FST
    FST *validation_fst = new FST();

    // Add the strings themselves to the FST
    validation_fst->addString("TEST");
    validation_fst->addString("MATE");
    validation_fst->addString("WRONG");
    validation_fst->addString("MATE");
    validation_fst->addString("T");

    // Check if the FSTs are equal
    CHECK(*fst == *validation_fst);

    // Clean up
    delete fst;
    delete validation_fst;
  }

  SUBCASE("FST with the strings TESTESTESTESTEST, TEST where TEST "
          "should be replaced")
  {
    // Create the FST
    FST *fst = new FST();

    // Add the strings themselves to the FST
    fst->addString("TESTESTESTESTEST");
    fst->addString("TEST");

    // Get the node representing "TEST"
    Node *test_node = fst->getNodeRepresentingSubstring("TEST");

    // Handle the replacement of the substring "TEST"
    fst->handleSubstringAddedToDict(test_node);

    // Create the validation FST
    FST *validation_fst = new FST();

    // Add the strings themselves to the FST
    validation_fst->addString("ES");
    validation_fst->addString("ES");

    // Check if the FSTs are equal
    CHECK(*fst == *validation_fst);

    // Clean up
    delete fst;
    delete validation_fst;
  }

  SUBCASE("FST with the strings TESTESTESTESTESTEST, TEST where "
          "TEST should be replaced")
  {
    // Create the FST
    FST *fst = new FST();

    // Add the strings themselves to the FST
    fst->addString("TESTESTESTESTESTEST");
    fst->addString("TEST");

    // Get the node representing "TEST"
    Node *test_node = fst->getNodeRepresentingSubstring("TEST");

    // Handle the replacement of the substring "TEST"
    fst->handleSubstringAddedToDict(test_node);

    // Create the validation FST
    FST *validation_fst = new FST();

    // Add the strings themselves to the FST
    validation_fst->addString("ES");
    validation_fst->addString("ES");
    validation_fst->addString("EST");

    // Check if the FSTs are equal
    CHECK(*fst == *validation_fst);

    // Clean up
    delete fst;
    delete validation_fst;
  }

  SUBCASE("FST with the strings TESTESTDOMINIKTEST, TESTER where "
          "TEST should be replaced")
  {
    // Create the FST
    FST *fst = new FST();

    // Add the strings themselves to the FST
    fst->addString("TESTESTDOMINIKTEST");
    fst->addString("TESTER");

    // Get the node representing "TEST"
    Node *test_node = fst->getNodeRepresentingSubstring("TEST");

    // Handle the replacement of the substring "TEST"
    fst->handleSubstringAddedToDict(test_node);

    // Create the validation FST
    FST *validation_fst = new FST();

    // Add the strings themselves to the FST
    validation_fst->addString("ESTDOMINIK");
    validation_fst->addString("ER");

    // Check if the FSTs are equal
    CHECK(*fst == *validation_fst);

    // Clean up
    delete fst;
    delete validation_fst;
  }

  SUBCASE("FST with the strings TESTESTDOMINIKTESTEST, TESTER "
          "where TEST should be replaced")
  {
    // Create the FST
    FST *fst = new FST();

    // Add the strings themselves to the FST
    fst->addString("TESTESTDOMINIKTESTEST");
    fst->addString("TESTER");

    // Get the node representing "TEST"
    Node *test_node = fst->getNodeRepresentingSubstring("TEST");

    // Handle the replacement of the substring "TEST"
    fst->handleSubstringAddedToDict(test_node);

    // Create the validation FST
    FST *validation_fst = new FST();

    // Add the strings themselves to the FST
    validation_fst->addString("ESTDOMINIK");
    validation_fst->addString("EST");
    validation_fst->addString("ER");

    // Check if the FSTs are equal
    CHECK(*fst == *validation_fst);

    // Clean up
    delete fst;
    delete validation_fst;
  }
}

TEST_CASE("Big integration TEST with addStrings and getDictionaryEntries")
{
  SUBCASE("Sample of 100 Strings from the dbtext/city database (1)")
  {
    // Create the FST
    FST *fst = new FST();

    // Add the strings themselves to the FST
    fst->addStrings({"AGAWAM",
                     "MOSS POINT",
                     "MURRAY",
                     "JEDDAH",
                     "CLEARWATER",
                     "EAST CARONDELET",
                     "CORSICA",
                     "S.I.",
                     "LAMIRADA",
                     "ALBIA",
                     "BOKCHITO",
                     "NASSAU BAY",
                     "SAMMAMISH",
                     "SAIT LOUIS",
                     "CICRA",
                     "NORTHFORD",
                     "FRAMINGHAM",
                     "JOBSTOWN",
                     "ATHENA",
                     "CASPER",
                     "ELSBERRY",
                     "PERCY",
                     "PITTSTON TWP",
                     "HAZLETON",
                     "BETHAL",
                     "CHINO",
                     "APPLETON",
                     "ANACORTES",
                     "LACONIA",
                     "SOUTHGATE",
                     "BLAIR",
                     "CROCKETT",
                     "CLAREMOUNT",
                     "HUNTINGTON MILLS",
                     "FARMERSBURG",
                     "HEIDELBERG",
                     "YOUNGSTOWN",
                     "BEECHHURST",
                     "STILWELL",
                     "GOLD BAR",
                     "RIVER VALE",
                     "BECKET",
                     "TEATICKET",
                     "GLENDALE HEIGHTS",
                     "MOUNTAIN HOME AFB",
                     "NICHOLS",
                     "LACONIA",
                     "MAYAJUEZ",
                     "NARANJA",
                     "SYLACAUGA",
                     "BOOKER",
                     "SUPPLY",
                     "DES PLAINES",
                     "ST CLAIREVILLE",
                     "EMMETT",
                     "PORT NECHES",
                     "ROBBINS",
                     "FULTON",
                     "LEVINTON",
                     "HIGHSPIRE",
                     "MILL CITY",
                     "APALACHICOLA",
                     "HALLANDALE",
                     "SOUTH HEIGHTS",
                     "AUBURN HILLS",
                     "LADUE",
                     "PROSPECT PARK",
                     "SLATON",
                     "FT. SAM HOUSTON",
                     "WILLIAMSTON",
                     "HARRISVILLE",
                     "FREDERICKBURG",
                     "BLUFFTON",
                     "AMENIA",
                     "ROSLYN HEIGHTS",
                     "NARANIJITO",
                     "WASHINGTONVILLE",
                     "NORTH ST PAUL",
                     "PINE RIVER",
                     "VAN HORNE",
                     "MARIONVILLE",
                     "GILMAN",
                     "ARROYO GRANDE",
                     "ORRINGTON",
                     "SCANDIA",
                     "GETZVILLE",
                     "NAUVOO",
                     "BALDWINVILLE",
                     "WESTBORO",
                     "MILWAUKEE",
                     "OAK GLEN",
                     "SAINT STEPHENS CHURCH",
                     "VIBURNUM",
                     "MT. WASHINGTON",
                     "ST. PETER",
                     "FT WASHAKIE",
                     "PALM HARBOR",
                     "SEWICKLEY",
                     "WINTERPORT",
                     "N CARROLLTON"});

    // Get the dictionary entries
    list<string> dict_entries;
    CHECK_NOTHROW(dict_entries = fst->getDictionaryEntries(255));

    // For now we only check the size of the dictionary entries
    CHECK(dict_entries.size() > 0);
    CHECK(dict_entries.size() <= 255);

    // Clean up
    delete fst;
  }

  SUBCASE("Sample of 100 Strings from the dbtext/city database (2)")
  {
    // Create the FST
    FST *fst = new FST();

    // Add the strings themselves to the FST
    fst->addStrings(
        {"BROOKFIELD", "AFFTON", "POPLARVILLE",
         "PLUMVILLE", "MENDHAM", "GLENS FALLS",
         "MARBLE", "LYNNVILLE", "KIRTLAND",
         "OLIVETTE", "PLACENTIA", "CHATTANOGA",
         "HAVRE", "ROLFE", "BONHAM",
         "BURKBURNETT", "KEYTESVILLE", "FT LEE",
         "RUSH SPRINGS", "CLAYPOOL", "RUSH CITY",
         "BAD AXE", "TOWNER", "TRUMBOLL",
         "EAST CHICAGO", "BERMUDA DUNES", "BRADFORD",
         "BROHEADSVILLE", "KRUGERVILLE", "N KINGSVILLE",
         "LAUDERDALE", "BUSHNELL", "NORDMAN",
         "STANTON", "MOUNTAIN", "CANDLER",
         "MEADOWVIEW", "ELMER", "MUSKOGEE",
         "CENTERLINE", "BONIFAY", "SAINT STEPHENS CHURCH",
         "KING GEORGE", "WILLITS", "DIAMONDHEAD",
         "RICH HILL", "EASTVALE", "NEENAH",
         "HENRYETTA", "PAONIA", "WEST TRENTON",
         "KEOSAUQUA", "MARSHALL", "ENOSBURG FALLS",
         "FORT MOHAVE", "SHERMAN", "DARIEN",
         "ST HENRY", "EAST BERNE", "LAVACA",
         "VESTAVIA", "SOUTHSIDE", "NESPELEM",
         "CHEWELAH", "BROWNSVILLE", "CHOCTAW",
         "GRUNDY CENTER", "ROSEDALE", "TYLER",
         "RUFFS DALE", "THREE FORKS", "KEMBLESVILLLE",
         "NAPOLEON", "ENDLEWOOD", "NORTH PLATTE",
         "BRANDYWINE", "HYMACAU", "ZIEGLERSVILLE",
         "HUMESTON", "CHARLEVOIX", "NIPOMO",
         "NORRIDGE", "FORT TOTTEN", "LACKLAND AFB",
         "WEST MILTON", "N PALM BEACH", "BAMBERG",
         "BRUNDIDGE", "FRANKSTON", "INTERNATIONAL FALLS",
         "BEACON FALLS", "PITTSBURGH", "SCOBEY",
         "JAFFREY", "CROZET", "RIVERWOODS",
         "COWETA", "PEARL HARBOR", "BLUFF DALE",
         "E HARWICH"});

    // Get the dictionary entries
    list<string> dict_entries;
    CHECK_NOTHROW(dict_entries = fst->getDictionaryEntries(255));

    // For now we only check the size of the dictionary entries
    CHECK(dict_entries.size() > 0);
    CHECK(dict_entries.size() <= 255);

    // Clean up
    delete fst;
  }

  SUBCASE("Sample of 100 Strings from the dbtext/city database (3 - doesn't "
          "work before the fix)")
  {
    // Create the FST
    FST *fst = new FST();

    // Add the strings themselves to the FST
    fst->addStrings({"S. WEYMOUTH",
                     "MOUNT WASHINGTON",
                     "OAKLAND PARK",
                     "KEARNY",
                     "VALATIE",
                     "ASHEVILLE",
                     "MAYWOOD",
                     "GALWAY",
                     "LEMONT FURNACE",
                     "NELLIS AIR FORCE BASE",
                     "FERGUS FALLS",
                     "BOERNE",
                     "HONEOYE",
                     "CAPE ELIZABETH",
                     "HOWARDS GROVE",
                     "CORONA",
                     "APISON",
                     "EARLVILLE",
                     "ARMOUR",
                     "NEW KINGSTOWN",
                     "DIETERICH",
                     "NEWTON SQ",
                     "FT MONMOUTH",
                     "GRANGER",
                     "PEMBROKE PARK",
                     "DORMONT",
                     "SULPHUR",
                     "PEKIN",
                     "MIDDLEVILLE",
                     "RATLIFF CITY",
                     "GRASSE",
                     "MARICOPA",
                     "BEDFORD PARK",
                     "LAJOLLA",
                     "NORTHGLENN",
                     "NORTH PALM BEACH",
                     "STRAFORD",
                     "BROADUS",
                     "CALLAHAN",
                     "EAST HARDWICK",
                     "PALATINE BRIDGE",
                     "COLO SPRINGS",
                     "DRAVOSBURG",
                     "OMRO",
                     "BRIDGEPORT",
                     "CLEARFIELD",
                     "MACCLENNY",
                     "WINTERHAVEN",
                     "PUNTA GORDA",
                     "WEBB CITY",
                     "PINE PLAINS",
                     "MELBOURNE BEACH",
                     "INKSTER",
                     "GRANITE CITY",
                     "FRACKVILLE",
                     "PATCHOGUE",
                     "LANESBORO",
                     "SPANTANBURG",
                     "CAMDEN ON GAULEY",
                     "FORT GIBSON",
                     "BALLSTON SPA",
                     "COLORADO CITY",
                     "SOUTH GLASTONBURY",
                     "ROSLINDALE",
                     "MAYAGUEZ",
                     "SANTO DOMINGO PUEBLO",
                     "NEWPORT RICHEY",
                     "EMPORIUM",
                     "OLD LYME",
                     "BILLINGS",
                     "THIELLS",
                     "BOULDER",
                     "ELBERT",
                     "DALZELL",
                     "NAVASOTA",
                     "MANAHAWKIN",
                     "KENDALL PARK",
                     "CLAUDE",
                     "VA. BEACH",
                     "PENNS GROVE",
                     "FAITH",
                     "SHINGLE SPRINGS",
                     "LIBERTY TOWNSHIP",
                     "ILION",
                     "TREMONTON",
                     "FT. GIBSON",
                     "RUPERT",
                     "GALLOWAY",
                     "GALES FERRY",
                     "MONTEREY",
                     "NOROFLK",
                     "SUMMIT",
                     "HACIENDA HTS",
                     "CLAY",
                     "MOUNT ORAB",
                     "HIGH RIDGE",
                     "WORDEN",
                     "LEECHBURG",
                     "OLD TOWN",
                     "LAKE SAINT LOUIS"});

    // Get the dictionary entries
    list<string> dict_entries;

    // cout << fst->toString() << endl;

    CHECK_NOTHROW(dict_entries = fst->getDictionaryEntries(255));

    // For now we only check the size of the dictionary entries
    CHECK(dict_entries.size() > 0);
    CHECK(dict_entries.size() <= 255);

    // Clean up
    delete fst;
  }

  SUBCASE("Sample of 100 Strings from the dbtext/city database (4 - doesn't "
          "work before the fix)")
  {
    // Create the FST
    FST *fst = new FST();

    // Add the strings themselves to the FST
    fst->addStrings({"JERSEY VILLAGE",
                     "BLUE RAPIDS",
                     "SADSBURYVILLE",
                     "FOWLERVILLE",
                     "SOUTH RIDING",
                     "HAHIRA",
                     "DEWEY",
                     "WOODRUFF",
                     "LOMPOC",
                     "KREMMLING",
                     "SISSETON",
                     "WORCETER",
                     "VALPARAISO",
                     "KINGS PARK",
                     "TONTO BASIN",
                     "LONGBOAT KEY",
                     "VERDIGRE",
                     "ST HELENS",
                     "CABIN JOHN",
                     "WATCHUNG",
                     "CHINCOTEAGUE ISLAND",
                     "LAKE CRYSTAL",
                     "VOLUNTOWN",
                     "ALBQ.",
                     "WARE",
                     "NORTH HIGHLANDS",
                     "MAUPIN",
                     "OSBURN",
                     "FISHERSVILLE",
                     "POLLOCKSVILLE",
                     "LOS ALAMOS",
                     "MOUNDS",
                     "MUSCATINE",
                     "SOUTH RIVER",
                     "COMMACK",
                     "CARMICHAELS",
                     "STORRS",
                     "WALNUT PARK",
                     "GARNETT",
                     "LUXEMBURG",
                     "ZEPHRYHILLS",
                     "ADAMS",
                     "SOLVAY",
                     "OZARK",
                     "ALAMOGORDO",
                     "KIRTLAND",
                     "OCRACOKE",
                     "EAST MEADOW",
                     "WEIMAR",
                     "OKA, TAMUNING",
                     "BEECH ISLAND",
                     "BARING",
                     "ROCHDALE",
                     "LAKE PLACID",
                     "GILMER",
                     "AMORY",
                     "RAY",
                     "ANMOORE",
                     "LAVA HOT SPRINGS",
                     "LAGUNA BCH",
                     "TRINITY",
                     "IRWINTON",
                     "CHERITON",
                     "HORACE",
                     "GRAND ISLE",
                     "BLAKESLEE",
                     "RONDOLPH",
                     "WINTERHAVEN",
                     "MILTON",
                     "BOYDS",
                     "VEGA ALTA",
                     "EDGERTON",
                     "BRACKENRIDGE",
                     "RECTOR",
                     "S BURLINGTON",
                     "SANDSTON",
                     "NORTH COLLINS",
                     "POTTSBORO",
                     "THOMSON",
                     "DURYEA",
                     "STEAMBOAT SPRINGS",
                     "OVERGAARD",
                     "KINGSLEY",
                     "SCOTTSVILLE",
                     "WESTPORT",
                     "GUILFORD",
                     "SUISUN",
                     "PAGOSA SPRINGS",
                     "CAPON BRIDGE",
                     "HONEY BROOK",
                     "WEST",
                     "BLADES",
                     "KRUM",
                     "OAKTON",
                     "TONALEA",
                     "COLUMBIAVILLE",
                     "COPPER HILL",
                     "DAVISVILLE",
                     "GLOVERSVILLE, NY 12078",
                     "BOLINGBROOK"});

    // Get the dictionary entries
    list<string> dict_entries;
    CHECK_NOTHROW(dict_entries = fst->getDictionaryEntries(255));

    // For now we only check the size of the dictionary entries
    CHECK(dict_entries.size() > 0);
    CHECK(dict_entries.size() <= 255);

    // Clean up
    delete fst;
  }

  SUBCASE("Sample of 255 Strings from the dbtext/city database (1)")
  {
    // Create the FST
    FST *fst = new FST();

    // Add the strings themselves to the FST
    fst->addStrings({"TALLAPOOSA",
                     "VALLEY COTTAGE",
                     "MIRAMAR",
                     "WEST HENRIETTA",
                     "ANDREWS",
                     "S SALT LAKE",
                     "BRADY",
                     "MINERAL WELLS",
                     "BELCHERTOWN",
                     "MARYLAND HEIGHTS",
                     "SALISBURY",
                     "OLLA",
                     "BETHPAGE",
                     "WEST GREENWICH",
                     "MENOMONIE",
                     "SAN JUAN",
                     "HINSDALE",
                     "HILL CITY",
                     "SOUTH BELOIT",
                     "DAYTONA BEACH",
                     "HAWLEY",
                     "MORRISON",
                     "MANCHETER",
                     "ROSEDALE",
                     "APLINGTON",
                     "LANESBOROUGH",
                     "WAUKON",
                     "LANSDOWNE",
                     "WILLIAMS BAY",
                     "TAOS",
                     "S.I.",
                     "DILLSBURG",
                     "BELMOND",
                     "INGRAM",
                     "HARVARD",
                     "POINT ROBERTS",
                     "NORTH POTOMAC",
                     "CORALVILLE",
                     "NESPELEM",
                     "MARBLE FALLS",
                     "LAKE TAPPS",
                     "EASTLAKE",
                     "SAINTE GENEVIEVE",
                     "CAPE CHARLES",
                     "TEUTOPOLIS",
                     "LA VALLE",
                     "TEMPLETON",
                     "COPLEY",
                     "HYGIENE",
                     "PORT GIBSON",
                     "RANCHO PALOS VERDES",
                     "GLADSTONE",
                     "EVEGREEN",
                     "MANHESSET",
                     "DELL RAPIDS",
                     "KEWASKUM",
                     "VOORHEESVILLE",
                     "NO DARTMOUTH",
                     "COLONIAL HEIGHTS,",
                     "CICRA",
                     "ASHEVILE",
                     "ALMO",
                     "STROUDSBURG",
                     "BINGHAM",
                     "ROCK VALLEY",
                     "DES ARC",
                     "LAKELAND",
                     "BASALT",
                     "RIVER HILLS",
                     "GALESVILLE",
                     "CRAFTON",
                     "N. SCITUATE",
                     "MIAMI",
                     "LINDEN",
                     "METLAKATLA",
                     "WELCOME",
                     "BLUE MOUNDS",
                     "COLUMBIA FALLS",
                     "IRON RIVER",
                     "PREMONT",
                     "PARKSLEY",
                     "IOTA",
                     "INDIAN RIVER SHORES",
                     "PIPERSVILLE",
                     "SANIBEL",
                     "PRUNEDALE",
                     "EARLYSVILLE",
                     "FINDLAY",
                     "WENTZVILLE",
                     "CANADIAN LAKES",
                     "BRAHAM",
                     "AVALON",
                     "HOLLY RIDGE",
                     "PUTTSVILLE",
                     "ORACLE",
                     "MANDERVILLE",
                     "N. CHALRESTON",
                     "MAGEE",
                     "ADA",
                     "CHERY HILL",
                     "JAL",
                     "ELBURN",
                     "OJAI",
                     "EAST GRANDY",
                     "SCOTTS MILLS",
                     "PIONEER",
                     "WEST HOLLYWOOD",
                     "DELCO",
                     "PRAIRIE CITY",
                     "COLO SPGS",
                     "PLYMOUTH MEETING",
                     "WICHESTER",
                     "NAGS HEAD",
                     "FORT COLLINS,",
                     "SAN FELIPE PUEBLO",
                     "LA JOLLAA",
                     "HARBOR",
                     "MAUSTON",
                     "MOUNTAIN LAKES",
                     "FT. OGLETHORPE",
                     "NORTH MYRTLE BEACH",
                     "LIMA",
                     "PIEDMONT",
                     "NORTH TUSTIN",
                     "VERMILION",
                     "FORT TOTTEN",
                     "MIDDLE ISLAND",
                     "NEW ROADS",
                     "KUTTAWA",
                     "WYANDANCH",
                     "GRAINTE",
                     "PAWLEYS ISLAND",
                     "MC DONALD",
                     "LAKE POWELL",
                     "TERLINGUA",
                     "FAIRMONT",
                     "BUFFALO",
                     "POLKTON",
                     "PETERSHAM",
                     "CHAMPLAIN",
                     "SAN SABA",
                     "ONANCOCK",
                     "FT GORDON",
                     "GREEN LANE",
                     "RANCHO CUCAMONGA",
                     "WAIKOLOA",
                     "WAKEMAN",
                     "SIGNAL HILL",
                     "BONNEY LAKE",
                     "BEAVERTOWN",
                     "QUITMAN",
                     "BIRDSBORO",
                     "OTTUMWA",
                     "PUYALLUP",
                     "ALCOA",
                     "BAUDETTE",
                     "ARCHBALD",
                     "LA PORTE CITY",
                     "WINTER PARK",
                     "NEEDMORE",
                     "N TONAWANDA",
                     "CHASSELL",
                     "KEYSVILLE",
                     "PETERSON AFB",
                     "SAINTE GENEVIEVE",
                     "LANSE",
                     "EAST ROCHESTER",
                     "BAXTER SPRINGS",
                     "VALRICO",
                     "REDWAY",
                     "EUGENE",
                     "BRAZIL",
                     "JUSTIN",
                     "SIOUX- CITY",
                     "SAINT GABRIEL",
                     "CENTRAL POINT",
                     "ETON",
                     "PLEASANT HILLS",
                     "CONGERS",
                     "HARWOOD HEIGHTS",
                     "SUMNER",
                     "NUTLEY",
                     "PIGEON FALLS",
                     "BREMEN",
                     "CAPE COD",
                     "SCHUYLKILL HAVEN",
                     "SUFFERN",
                     "HILLSGROVE",
                     "TITUSVILLE",
                     "BELL BUCKLE",
                     "ARCHBALD",
                     "NEWYORK",
                     "LINCOLN,",
                     "MC LEAN",
                     "PLACEVILLE",
                     "SEAMAN",
                     "BLASDELL",
                     "TITUSVILLE",
                     "SAINT ANN",
                     "CHATTONOOGA",
                     "WOODBRIDGE",
                     "GLEN CARBON",
                     "HIALEAH GARDENS",
                     "POLK CITY",
                     "MINOTOLA",
                     "SEBREE",
                     "GIBSONIA",
                     "KINGSFORD",
                     "NORTH SMITHFIELD",
                     "ELKHART",
                     "CHARLOTTE C H",
                     "LEMONT FURNACE",
                     "OROSI",
                     "S. ELGIN",
                     "FOX ISLAND",
                     "FAIRMONT",
                     "CALDWELL",
                     "PINEAPPLE",
                     "IMMOKALEE",
                     "ALLENDALE",
                     "CURTIS",
                     "ORACLE",
                     "AINSWORTH",
                     "LINCOLN PARK",
                     "MORRILL",
                     "WILLACOOCHEE",
                     "BIG BEAR CITY",
                     "CANBY",
                     "PARLIN",
                     "HAZEL PARK",
                     "DESHLER",
                     "CAMP HLL",
                     "PARLIN",
                     "WINNER",
                     "HOLLANDALE",
                     "WINSTON-SALEM",
                     "DERWOOD",
                     "BLOUNTSVILLE",
                     "ANNAPOLIS",
                     "MILPITAS",
                     "ST. LOUIS",
                     "ALTURAS",
                     "EDWARDS AFB",
                     "SPRINGFIELD",
                     "COUDERSPORT",
                     "OGEMA",
                     "DORRIS",
                     "WOODBRIDGE",
                     "W SPRINGFIELD",
                     "WINSTON",
                     "COLLEGE CORNER",
                     "LIVERMORE FALLS",
                     "AURORA",
                     "YACHATS",
                     "TUBA CITY"});

    // Get the dictionary entries
    list<string> dict_entries;
    CHECK_NOTHROW(dict_entries = fst->getDictionaryEntries(255));

    // For now we only check the size of the dictionary entries
    CHECK(dict_entries.size() > 0);
    CHECK(dict_entries.size() <= 255);

    // Clean up
    delete fst;
  }

  SUBCASE("Sample of 255 Strings from the dbtext/city database (2 - does not "
          "work before fix)")
  {
    // Create the FST
    FST *fst = new FST();

    // Add the strings themselves to the FST
    fst->addStrings({"COLUMBIAVILLE",
                     "BAYBORO",
                     "ANDREWS AIR FORCE BASE",
                     "MARGARETVILLE",
                     "POSEN",
                     "DEER PARK",
                     "PILOT MOUNTAIN",
                     "YOUNGSVILLE",
                     "HANCOCK",
                     "SCHENECTADY",
                     "DALHART",
                     "SAN ANSELMO",
                     "JACINTO CITY",
                     "WINTERSVILLE",
                     "GAINESVILLE",
                     "NORTOLK",
                     "MAMARONECK",
                     "WEIR",
                     "BOISSEVAIN",
                     "ACKLEY",
                     "CHICOPEE",
                     "STEELTON",
                     "HERON LAKE",
                     "NEAH BAY",
                     "MERILLVILLE",
                     "AMBRIDGE",
                     "STRASBURG",
                     "WRIGHT CITY",
                     "BERTHA",
                     "DUNN",
                     "HAINESVILLE",
                     "CONNELLSVILLE",
                     "STRONGSVILLE",
                     "YELLOWSTONE NATIONAL PARK",
                     "N MYRTLE BEACH",
                     "DOTHAN",
                     "JACKSONVILLE BEACH",
                     "ISANTI",
                     "BENTON CITY",
                     "LEMMON",
                     "BROADVIEW",
                     "BRAZIL",
                     "COOPERSTOWN",
                     "300 NORTH AVE.",
                     "PORT GIBSON",
                     "WILLIAMSTOWN",
                     "HEBRON",
                     "VINTON",
                     "NORTH ROYALTON",
                     "TAYLOR",
                     "BEATRICE",
                     "CROOKSVILLE",
                     "INDIO",
                     "ATTAMONTE SPRINGS",
                     "APACHE JUNCTION",
                     "DENISON",
                     "YORBA LINDA",
                     "CORBIN",
                     "SHOSHONE",
                     "FINLEY",
                     "NEW ORLEANS",
                     "ANGOLA",
                     "DEER LODGE",
                     "NAPAVINE",
                     "CROSS CITY",
                     "YORKTOWN HTS",
                     "ROYAL OAK",
                     "LYNN HAVEN",
                     "ALPHORETTA",
                     "BAGDAD",
                     "AZUSA",
                     "NEAPOLIS",
                     "ESTHERVILLE",
                     "GASSAWAY",
                     "BERWYN",
                     "NEBO",
                     "FAIRMOUNT",
                     "BRECKSVILLE",
                     "WINSTED",
                     "ARENA",
                     "N. MANCHESTER",
                     "CONEWANGO VALLEY",
                     "ST. LOUIS",
                     "EAST PALESTINE",
                     "DANRIDGE",
                     "GEROGETOWN",
                     "TERLINGUA",
                     "KOSCIUSKO",
                     "MENASHA",
                     "PILOT POINT",
                     "LONG ISLAND CITY",
                     "MUSKEGON",
                     "IMBODEN",
                     "ALLIANCE",
                     "NEW SMYRNA BEACH",
                     "ASHEVILLE",
                     "MOOSIC",
                     "ROXBORO",
                     "GIBBON",
                     "CHICOPEE",
                     "GREENPORT",
                     "DOVE",
                     "PICKERINGTON",
                     "EDMESTON",
                     "PEWEE VALLEY",
                     "OSSINING",
                     "LUZERNE",
                     "DUNELLEN",
                     "SOUTHINGTON",
                     "ROLLA",
                     "U SADDLE RIV",
                     "LUMBER CITY",
                     "QUANTICO",
                     "LUBEC",
                     "T OR C",
                     "NORA SPRINGS",
                     "WAUTOMA",
                     "MAYNARD",
                     "VERGENNES",
                     "DODGE CENTER",
                     "MINE HILL",
                     "MANGUM",
                     "GREEN LANE",
                     "RAINBOW CITY",
                     "LACKEY",
                     "DEVILS LAKE",
                     "MIO",
                     "EAST NORRITON",
                     "SANDIA PARK",
                     "FAIRVIEW",
                     "NORTH EASTHAM",
                     "AURORA",
                     "COLERAINE",
                     "MARINGOUIN",
                     "HAVERSTRAW",
                     "NORTH PORT",
                     "NEW PORT RICHEY",
                     "SAC CITY",
                     "SWIFTON",
                     "YADKINVILLE",
                     "WATERLOO",
                     "HARROGATE",
                     "SABINE",
                     "ROYSE CITY",
                     "PANACEA",
                     "POLACCA",
                     "MARBLETON",
                     "NEW VIENNA",
                     "ISTANBUL",
                     "JACKSONS GAP",
                     "GREENHURST",
                     "SCRUNTON",
                     "CANON",
                     "PRESCOTT",
                     "MERCEDITA",
                     "NEON",
                     "GRAND FROKS",
                     "CLARKS SUMMIT",
                     "HANAHAN",
                     "TEWKSBURY",
                     "ARLEE",
                     "POPE AFB",
                     "REWOOD CITY",
                     "SULTAN",
                     "RATCLIFF",
                     "MINERAL WELLS",
                     "DELAND",
                     "E LONGMEADOW",
                     "UKIAH",
                     "DAWES",
                     "BRYSON CITY",
                     "PACOLET",
                     "LA MESA",
                     "FAIRLEA",
                     "OCCOQUAN",
                     "FORT RUCKER",
                     "CABOT",
                     "WATSONVILLE",
                     "FLINTSTONE",
                     "SUCCASUNNA",
                     "PRAIRIE DU SAC",
                     "HUFFMAN",
                     "TREVORTON",
                     "W. HARTFORD",
                     "PENROSE",
                     "W COXSACKIE",
                     "EAST LYME",
                     "THORNHURST",
                     "BELLE FOURCHE",
                     "HILLSIDE",
                     "SHAMOKIN DAM",
                     "SACKETS HARBOR",
                     "SNOHOMISH",
                     "EASTERN",
                     "SHARPTOWN",
                     "IRWINDALE",
                     "INDIANAPOLIST",
                     "CLIFTON FORGE",
                     "WESTBY",
                     "AUXIER",
                     "ALBERTVILLE",
                     "VERNON HILLS",
                     "WESTMINSTER",
                     "PRESQUE ISLE",
                     "ADDY",
                     "PENNDEL",
                     "BAXTER",
                     "WINSTED",
                     "ANNVILLE",
                     "CANON",
                     "VIAN",
                     "GROVETON",
                     "MARSEILLES",
                     "GOLD HILL",
                     "PENDLETON",
                     "SWEETWATER",
                     "ROOSEVELT",
                     "NUTLEY",
                     "GYPSUM",
                     "KALISPEL",
                     "GREEN FOREST",
                     "CHATFIELD",
                     "DODGE CITY",
                     "RAMONA",
                     "NEW MIDDLETOWN",
                     "MT LAUREL",
                     "PULASKI",
                     "CHEROKEE",
                     "MT. STERLING",
                     "ROCKY MOUNT",
                     "ROSE HILL",
                     "BENWOOD",
                     "MACOMB",
                     "AUBREY",
                     "ATOKA",
                     "MISION VIEJO",
                     "MARGATE",
                     "FULDA",
                     "RAEFORD",
                     "YORK",
                     "NEW",
                     "TULLAHOMA",
                     "ARCARDIA",
                     "MOUNT CLEMES",
                     "E SMITHFIELD",
                     "SHAWNEETOWN",
                     "COEURD' ALENE",
                     "HUSTONVILLE",
                     "EL DARADO",
                     "SILT",
                     "CLINTON",
                     "DONORA",
                     "PENINSULA",
                     "WALNUT RIDGE",
                     "VIVIAN"});

    // Get the dictionary entries
    list<string> dict_entries;
    CHECK_NOTHROW(dict_entries = fst->getDictionaryEntries(255));

    // For now we only check the size of the dictionary entries
    CHECK(dict_entries.size() > 0);
    CHECK(dict_entries.size() <= 255);

    // Clean up
    delete fst;
  }
}