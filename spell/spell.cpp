#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "spell.h"

using namespace std;

/* insert your function definitions here */
int frequency(string word){
  ifstream in_stream;
  in_stream.open("words.dat");

  string line;
  int frequency_tmp = 0;
  int frequency = 0;
  string word_tmp;

  while(getline(in_stream, line)){
    istringstream tmp(line);
    tmp >> frequency_tmp;
    tmp >> word_tmp;
    if(word_tmp == word){
      frequency = frequency_tmp;
      break;
    }
  }

  return frequency;
}

bool transposition(char a1, char a2, char b1, char b2){
  if(a1 == b2 && b1 == a2){
    return true;
  }
  return false;
}

void insert(string& word, int pos){
  word.insert(pos, "_");
  // cout << word;
}

int edit_distance(string const a, string const b){
  int edit_distance1 = 0;

  string s1;
  string s2;
  //Compare bigger to smaller
  if(a.size() > b.size()){
    s1 = a;
    s2 = b;
  }else{
    s1 = b;
    s2 = a;
  }

  for(size_t i = 0; i < s1.size(); i++){
    if(s1[i] && s2[i]){
      if(s1[i] == s2[i]){
        continue;
      }
    }
    //transposition
    if(s1[i+1] && s2[i+1]){
      if(transposition(s1[i], s1[i+1], s2[i], s2[i+1])){
        i++;
        edit_distance1++;
        continue;
      }
    }

    if(s2[i]){
      if(s1[i]!= s2[i]){
        //determine if insertion tbd or replacement
        if(s1.size() > s2.size()){ //insert
          insert(s2, i);
        }
        edit_distance1++;
        continue;
      }
    }else{ //insertion
      edit_distance1++;
    }
  }


  return edit_distance1;
}

bool spell_correct(string word, char fixed[MAX_LENGTH]){
  if(frequency(word)){
    return false;
  }

  ifstream in_stream;
  in_stream.open("words.dat");

  string line;
  string word_tmp;
  int frequency_tmp;
  int ed;
  int lowest_ed = 10000;
  int highest_freq = -1;

  //loop thru dict
  while(getline(in_stream, line)){
    istringstream tmp(line);
    tmp >> frequency_tmp;
    tmp >> word_tmp;

    ed = edit_distance(word, word_tmp);
    if(ed < lowest_ed){
      lowest_ed = ed;
      highest_freq = frequency_tmp;
      strcpy(fixed, word_tmp.c_str());
      if(highest_freq < frequency_tmp){
        highest_freq = frequency_tmp;
        strcpy(fixed, word_tmp.c_str());
      }
    }
  }

  return true;
}
