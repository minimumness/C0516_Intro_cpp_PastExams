#include <iostream>
#include <openssl/sha.h>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <fstream>
#include "stamp.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own
   function definitions to the end of this file. */

// helper function for internal use only
// transforms raw binary hash value into human-friendly hexademical form
void convert_hash(const unsigned char *str, char *output, int hash_length) {
  char append[16];
  strcpy (output, "");
  for (int n=0; n<hash_length; n++) {
    sprintf(append,"%02x",str[n]);
    strcat(output, append);
  }
}

// pre-supplied helper function
// generates the SHA1 hash of input string text into output parameter digest
// ********************** IMPORTANT **************************
// ---> remember to include -lcrypto in your linking step <---
// ---> so that the definition of the function SHA1 is    <---
// ---> included in your program                          <---
// ***********************************************************
void text_to_SHA1_digest(const char *text, char *digest) {
  unsigned char hash[SHA_DIGEST_LENGTH];
  SHA1( (const unsigned char *) text, strlen(text), hash);
  convert_hash(hash, digest, SHA_DIGEST_LENGTH);
}

/* add your function definitions here */
int leading_zeros(std::string digest){
  for(size_t i = 0; i<digest.size(); i++){
    if((digest[i]>='0' && digest[i] <='9') || (digest[i]>='a' && digest[i]<='f')){
      continue;
    }else{
      return -1;
    }
  }

  int leading_zeros = 0;
  for(size_t i = 0; i<digest.size(); i++){
    if(digest[i] == '0'){
      leading_zeros++;
    }else{
      break;
    }
  }
  return leading_zeros;
}

bool file_to_SHA1_digest(string filename, char* digest){
  ifstream in_stream;
  in_stream.open(filename);
  if(!in_stream){
    return false;
  }

  string all_digest;
  char a;
  while(in_stream.get(a), !in_stream.eof()){
    all_digest += a;
  }
  text_to_SHA1_digest(all_digest.c_str(), digest);
  return true;
}

bool add_counter(string &tmp){
  int counter = 0;
  string original = tmp + ":";
  char hashed[41];

  while(counter < 10000000){
    tmp += to_string(counter);
    text_to_SHA1_digest(tmp.c_str(), hashed);
    if(leading_zeros(hashed) <5){
      counter++;
      tmp = original;
    }else{
      break;
    }
  }
  if(counter >= 10000000){
    return false;
  }

  return true;
}

bool make_header(string recipient, string filename, char header[512]){
  string tmp = "";

  tmp += recipient;
  char digest[41];
  file_to_SHA1_digest(filename, digest);
  tmp += ":";
  tmp += digest;

  if(!add_counter(tmp)){
    return false;
  }

  strcpy(header, tmp.c_str());
  return true;
}

bool wrong_recipient(string email_address, string header){
  for(size_t i = 0; i< email_address.size(); i++){
    if(email_address[i] != header[i]){
      return true;
    }
  }
  return false;
}

bool invalid_message_digest(string filename, string header){
  char digest[41];
  file_to_SHA1_digest(filename, digest);
  string tmp = digest;
  bool match = true;

  for(size_t i = 0; i < header.size(); i++){
    match = true;
    for(size_t n = 0; n < tmp.size(); n++){
      if(header[i+n] != tmp[n]){
        match = false;
      }
    }
    if(match){
      return false;
    }
  }
  return true;
}

bool invalid_header(string header){
  return false;
}
MessageStatus check_header(std::string email_address, std::string header, std::string filename){

  if(wrong_recipient(email_address, header)){
    return WRONG_RECIPIENT;
  }
  if(invalid_message_digest(filename, header)){
    return INVALID_MESSAGE_DIGEST;
  }
  char digest[41];
  text_to_SHA1_digest(header.c_str(), digest);
  if(leading_zeros(digest)<5){
    return INVALID_HEADER_DIGEST;
  }
  if (invalid_header(header)) {
    return INVALID_HEADER;
  }
  // VALID_EMAIL=1
  return VALID_EMAIL;
}
