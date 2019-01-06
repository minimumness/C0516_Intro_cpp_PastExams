#include <iostream>
#include <cstring>
#include <cctype>
#include <cassert>
#include <map>
#include <fstream>
#include <string>

using namespace std;

#include "sonnet.h"

/* PRE-SUPPLIED HELPER FUNCTIONS START HERE */

/* NOTE: It is much more important to understand the interface to and
   the "black-box" operation of these functions (in terms of inputs and
   outputs) than it is to understand the details of their inner working. */

/* get_word(...) retrieves a word from the input string input_line
   based on its word number. If the word number is valid, the function
   places an uppercase version of the word in the output parameter
   output_word, and the function returns true. Otherwise the function
   returns false. */

bool get_word(const char *input_line, int word_number, char *output_word) {
  char *output_start = output_word;
  int words = 0;

  if (word_number < 1) {
    *output_word = '\0';
    return false;
  }

  do {
    while (*input_line && !isalnum(*input_line))
      input_line++;

    if (*input_line == '\0')
      break;

    output_word = output_start;
    while (*input_line && (isalnum(*input_line) || *input_line=='\'')) {
      *output_word = toupper(*input_line);
      output_word++;
      input_line++;
    }
    *output_word = '\0';

    if (++words == word_number)
      return true;

  } while (*input_line);

  *output_start = '\0';
  return false;
}

/* char rhyming_letter(const char *ending) generates the rhyme scheme
   letter (starting with 'a') that corresponds to a given line ending
   (specified as the parameter). The function remembers its state
   between calls using an internal lookup table, such that subsequents
   calls with different endings will generate new letters.  The state
   can be reset (e.g. to start issuing rhyme scheme letters for a new
   poem) by calling rhyming_letter(RESET). */

char rhyming_letter(const char *ending) {

  // the next rhyming letter to be issued (persists between calls)
  static char next = 'a';
  // the table which maps endings to letters (persists between calls)
  static map<string, char> lookup;

  // providing a way to reset the table between poems
  if (ending == RESET) {
    lookup.clear();
    next = 'a';
    return '\0';
  }

  string end(ending);

  // if the ending doesn't exist, add it, and issue a new letter
  if (lookup.count(end) == 0) {
    lookup[end]=next;
    assert(next <= 'z');
    return next++;
  }

  // otherwise return the letter corresponding to the existing ending
  return lookup[end];
}

/* START WRITING YOUR FUNCTION BODIES HERE */

int count_words(string line){
  int count = 1;
  char tmp[512];
  while (get_word(line.c_str(), count, tmp)){
    count++;
  }
  count = count-1;
  return count;
}

bool contains_vowel(char tmp[512]){
  for(int i = 0; i < 512; i++){
    if(tmp[i]){
      if(tmp[i] == 'A' || tmp[i] == 'E' || tmp[i] == 'I' || tmp[i] == 'O' || tmp[i] == 'U'){
        return true;
      }
    }else{
      break;
    }
  }
  return false;
}

bool find_phonetic_ending(string word, char find_phonetic_ending[512]){
  string line;
  char tmp[512];
  bool found = false;
  ifstream in_stream;
  in_stream.open("dictionary.txt");

  //find word in dictionary
  while(getline(in_stream, line)){
    get_word(line.c_str(), 1, tmp);
    if(strcmp(word.c_str(), tmp) == 0){
      found = true;
      break;
    }
  }

  //find where there is a vowel
  int words_in_line = count_words(line);
  int word_from = 0;
  for(; words_in_line > 0; words_in_line--){
    get_word(line.c_str(), words_in_line, tmp);
    if(contains_vowel(tmp)){
      word_from = words_in_line;
      break;
    }
  }

  //Create the phonetic ending
  string phonetic_ending;
  for(; word_from<=count_words(line); word_from++){
    get_word(line.c_str(), word_from, tmp);
    phonetic_ending += tmp;
  }
  strcpy(find_phonetic_ending, phonetic_ending.c_str());

  return found;
}

bool find_rhyme_scheme(string filename, char scheme[512]){
  ifstream in_stream;
  in_stream.open(filename);
  if(!in_stream){
    return false;
  }
  string line;
  string rhyming_scheme;
  char last_word[512];
  char phonetic_ending[512];
  int word_count;

  while(getline(in_stream, line)){
    word_count = count_words(line);
    get_word(line.c_str(), word_count, last_word);
    find_phonetic_ending(last_word, phonetic_ending);
    char a = rhyming_letter(phonetic_ending);
    rhyming_scheme+= a;
  }
  strcpy(scheme, rhyming_scheme.c_str());
  cout << endl;
  return true;
}

string identify_sonnet(string filename){
  char scheme[512];
  if(!find_rhyme_scheme(filename, scheme)){
    return "Unkown";
  }
  string Shakespearean = "ababcdcdefefgg";
  string Petrarchan = "abbaabbacdcdcd";
  string Spenserian = "ababbcbccdcdee";

  string compare_scheme;
  string seen_letters;
  char a = 'a';
  int different_chars = 0;
  for(int i = 0; i < 512; i++){
    if(scheme[i]){
      bool seen = false;
      for(int n = 0; n < seen_letters.size(); n++){
        if(scheme[i] == seen_letters[n]){
          seen = true;
          compare_scheme += a +n;
          break;
        }
      }
      if(!seen){
        compare_scheme += a + different_chars;
        seen_letters += scheme[i];
        different_chars++;
      }

    }else{
      break;
    }
  }

  if(compare_scheme == Shakespearean){
    return "Shakespearean";
  }

  if(compare_scheme == Petrarchan){
    return "Petrarchan";
  }

  if(compare_scheme == Spenserian){
    return "Spenserian";
  }

  return "Unkown";
}
