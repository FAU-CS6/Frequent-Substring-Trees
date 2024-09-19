#include "../../lib/fsst/libfsst.hpp"
#include "../classes/node.h"
#include "../classes/fst.h"

#include <cctype>
#include <iomanip>
#include <iostream>
#include <list>
#include <random>
#include <sstream>
#include <string>

std::list<std::string> split_string_by_newline(const std::string &str)
{
  std::list<std::string> result;
  std::stringstream ss(str);

  std::string line;
  while (std::getline(ss, line, '\n'))
  {
    result.push_back(line);
  }

  return result;
}

std::list<std::string> sample_strings(const std::list<std::string> &input_list,
                                      size_t x)
{
  std::list<std::string> sampled_list;
  size_t list_size = input_list.size();

  // Create a random number generator with a seed
  std::random_device rd;
  std::mt19937 gen(rd());

  // Create a uniform distribution for indices in the range [0, list_size)
  std::uniform_int_distribution<> dist(0, list_size - 1);

  // Iterate x times
  for (size_t i = 0; i < x; ++i)
  {
    // Generate a random index from the uniform distribution
    size_t random_index = dist(gen);

    // Access the corresponding string in the list
    std::list<std::string>::const_iterator it = input_list.begin();
    std::advance(it, random_index);

    // Add the string to the sampled_list
    sampled_list.push_back(*it);
  }

  return sampled_list;
}

std::list<std::string>
get_first_n_items(const std::list<std::string> &input_list, size_t n)
{
  std::list<std::string> output_list;

  std::list<std::string>::const_iterator it = input_list.begin();
  for (size_t i = 0; i < n && it != input_list.end(); ++i)
  {
    output_list.push_back(*it);
    ++it;
  }

  return output_list;
}

fsst_encoder_t *fsst_create_with_fst(size_t n_samples, unsigned char *strIn[])
{
  // Split the input string by newline
  string str = string((char *)strIn[0]);
  std::list<std::string> strs = split_string_by_newline(str);

  // Make a FST
  FST *fstc = new FST();

  // Sample strings from the input list
  std::list<std::string> sample_strs = sample_strings(strs, n_samples);

  // Add a sample of strings to the FST
  fstc->addStrings(sample_strs);

  // Find 255 dictionary entries
  list<string> dict_entries = fstc->getDictionaryEntries(255, 7);

  // Create a new SymbolTable
  SymbolTable *symbol_table = new SymbolTable();

  // Add the dictionary entries to the SymbolTable
  for (list<string>::iterator it = dict_entries.begin();
       it != dict_entries.end(); it++)
  {
    const char *begin = it->data();
    const char *end = it->data() + it->size();
    symbol_table->add(Symbol(begin, end));
  }

  // Create a encoder based on our SymbolTable
  Encoder *encoder = new Encoder();
  encoder->symbolTable = shared_ptr<SymbolTable>(symbol_table);
  return (fsst_encoder_t *)encoder;
}

fsst_encoder_t *fsst_create_with_fst_w_fsst_sampling(size_t n, size_t lenIn[], u8 *strIn[], int zeroTerminated)
{
  // Call makeSample
  u8 *sampleBuf = new u8[FSST_SAMPLEMAXSZ];
  size_t *sampleLen = lenIn;
  std::vector<u8 *> sample = makeSample(sampleBuf, strIn, &sampleLen, n ? n : 1);

  // Transform sample into a list of strings
  std::list<std::string> sample_strs;
  for (size_t i = 0; i < sample.size(); ++i)
  {
    std::string str(sample[i], sample[i] + sampleLen[i]);
    sample_strs.push_back(str);
  }

  // Make a FST
  FST *fstc = new FST();

  // Add a sample of strings to the FST
  fstc->addStrings(sample_strs);

  // Find 255 dictionary entries
  list<string> dict_entries = fstc->getDictionaryEntries(255, 7);

  // Create a new SymbolTable
  SymbolTable *symbol_table = new SymbolTable();

  // Add the dictionary entries to the SymbolTable
  for (list<string>::iterator it = dict_entries.begin();
       it != dict_entries.end(); it++)
  {
    const char *begin = it->data();
    const char *end = it->data() + it->size();
    symbol_table->add(Symbol(begin, end));
  }

  // Create a encoder based on our SymbolTable
  Encoder *encoder = new Encoder();
  encoder->symbolTable = shared_ptr<SymbolTable>(symbol_table);
  return (fsst_encoder_t *)encoder;
}