#include <iostream>

void prepare(std::string input, char output[]){
  int output_index = 0;
  size_t i = 0;
  for (; i < input.size(); i++){
    if(isalnum(input[i])){
      if(isalpha(input[i])){
        if(islower(input[i])){
          output[output_index] = (char)toupper(input[i]);
          output_index++;
          continue;
        }
      }
      output[output_index] = input[i];
      output_index++;
    }
  }
  if(i%2){
    output[output_index] = 'X';
  }
}

bool occurs_before(char c, char playfair[6][6]){
  for(int i = 0; i<6; i++){
    for(int n = 0; n<6; n++){
      if(playfair[i][n] == c){
        return true;
      }
    }
  }
  return false;
}

void grid(std::string const input, char playfair[6][6]){
  for(int i = 0; i<6; i++){
    for(int n = 0; n<6; n++){
      playfair[i][n] = '?';
    }
  }

  for(size_t r = 0; r<input.size(); r++){
    for(int i = 0; i<6; i++){
      for(int n = 0; n<6; n++){
        if(!isalnum(playfair[i][n]) && !occurs_before(input[r], playfair)){
          playfair[i][n] = input[r];
        }
      }
    }
  }

  for(char c = 'A'; c <= 'Z'; c++){
    for(int i = 0; i<6; i++){
      for(int n = 0; n<6; n++){
        if(!isalnum(playfair[i][n]) && !occurs_before(c, playfair)){
          playfair[i][n] = c;
        }
      }
    }
  }

  for(char c = '0'; c <= '9'; c++){
    for(int i = 0; i<6; i++){
      for(int n = 0; n<6; n++){
        if(!isalnum(playfair[i][n]) && !occurs_before(c, playfair)){
          playfair[i][n] = c;
        }
      }
    }
  }
}

void bigram(char const playfair[6][6], char in1, char in2, char &out1, char &out2){
  int in1_row;
  int in1_col;
  int in2_row;
  int in2_col;

  for(int r = 0; r < 6; r++){
    for(int c = 0; c < 6; c++){
      if(playfair[r][c] == in1){
        in1_row = r;
        in1_col = c;
      }
      if(playfair[r][c] == in2){
        in2_row = r;
        in2_col = c;
      }
    }
  }

  out1 = playfair[in1_row][in2_col];
  out2 = playfair[in2_row][in1_col];
}

void encode(char const playfair[6][6], char prepared[], char encoded[]){
  bigram(playfair, prepared[0], prepared[1], encoded[0], encoded[1]);
  char* new_prepared = prepared+2;
  char* new_encoded = encoded+2;

  if(new_prepared[0]){
    encode(playfair, new_prepared, new_encoded);
  }
}
