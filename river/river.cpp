#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <cassert>

using namespace std;

#include "river.h"

/* You are pre-supplied with the functions below. Add your own
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new (nothrow) char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new (nothrow) char[columns];
    assert(m[r]);
  }
  return m;
}

/* internal helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* pre-supplied function which creates an empty ASCII-art scene */
char **create_scene() {
  char **scene = allocate_2D_array(SCENE_HEIGHT, SCENE_WIDTH);

  for (int i=0; i<SCENE_HEIGHT; i++)
    for (int j=0; j<SCENE_WIDTH; j++)
      scene[i][j] = ' ';

  return scene;
}

/* pre-supplied function which frees up memory allocated for an ASCII-art scene */
void destroy_scene(char **scene) {
  deallocate_2D_array(scene, SCENE_HEIGHT);
}

/* pre-supplied function which displays an ASCII-art scene */
void print_scene(char **scene) {
  for (int i=0; i<SCENE_HEIGHT; i++) {
    for (int j=0; j<SCENE_WIDTH; j++)
      cout << scene[i][j];
    cout << endl;
  }
}

/* helper function which removes carriage returns and newlines from strings */
void filter(char *line) {
  while (*line) {
    if (*line >= ' ')
      line++;
    else
      *line = '\0';
  }
}

/* pre-supplied function which inserts an ASCII-art drawing stored in a file
   into a given ASCII-art scene starting at coordinates (row,col)  */
bool add_to_scene(char **scene, int row, int col, const char *filename) {

  ifstream in(filename);
  if (!in)
    return false;

  int start_col = col;
  char line[512];
  in.getline(line,512);
  filter(line);
  while (!in.fail()) {
    for (int n=0; n<strlen(line); n++) {
      if (row >= SCENE_HEIGHT)
	return false;
      if (col >= SCENE_WIDTH)
	break;
      scene[row][col++] = line[n];
    }
    row++;
    col = start_col;
    in.getline(line,512);
    filter(line);
  }
  return true;
}

/* pre-supplied helper function to report the status codes encountered in Question 3 */
const char *status_description(int code) {
  switch(code) {
  case ERROR_INVALID_CANNIBAL_COUNT:
    return "Invalid cannibal count";
  case ERROR_INVALID_MISSIONARY_COUNT:
    return "Invalid missionary count";
  case ERROR_INVALID_MOVE:
    return "Invalid move";
  case ERROR_MISSIONARIES_EATEN:
    return "The missionaries have been eaten! Oh dear!";
  case ERROR_INPUT_STREAM_ERROR:
    return "Unexpected input stream error";
  case ERROR_BONUS_NO_SOLUTION:
    return "No solution";
  case VALID_GOAL_STATE:
    return "Problem solved!";
  case VALID_NONGOAL_STATE:
    return "Valid state, everything is OK!";
  }
  return "Unknown error";
}

void add_boat(char** &scene, string boat, int position){
  add_to_scene(scene, 17, position, "boat.txt");
  int b_count = 0;

  for (int i = 0; i < boat.size(); i++){
    if(boat[i] == 'M'){
      add_to_scene(scene, 11, position + 3 + b_count*5 +b_count, "missionary.txt");
    }

    if(boat[i] == 'C'){
      add_to_scene(scene, 11, position + 3 + b_count*5 +b_count, "cannibal.txt");
    }
    b_count++;
  }
}

/* insert your functions here */
char** make_river_scene(string left, string boat){
  char **scene = create_scene();
  add_to_scene(scene, 2, 31, "sun.txt");
  add_to_scene(scene, 0, 0, "bank.txt");
  add_to_scene(scene, 0, 53, "bank.txt");
  add_to_scene(scene, 19, 19, "river.txt");

  int m_count = 0;
  int c_count = 0;
  bool boat_right = true;

  //add left bank and boat if its on the left
  for(int i = 0; i < left.size(); i++){
    if(left[i] == 'M'){
      add_to_scene(scene, 2, m_count*5+ 1 +m_count, "missionary.txt");
      m_count++;
    }

    if(left[i] == 'C'){
      add_to_scene(scene, 11, c_count*5+ 1 +c_count, "cannibal.txt");
      c_count++;
    }

    if(left[i] == 'B'){
      boat_right = false;
      add_boat(scene, boat, 19);
    }
  }

  for (int i = 0; i < boat.size(); i++){
    if(boat[i] == 'M'){
      m_count++;
    }
    if(boat[i] == 'C'){
      c_count++;
    }
  }

  //add boat to the right if its on the right
  if(boat_right){
    add_boat(scene, boat, 36);
  }

  int m_right_spacing = 0;
  int c_right_spacing = 0;

  for(; m_count <3; m_count++){
    add_to_scene(scene, 2, m_right_spacing*5+ 54 +m_right_spacing, "missionary.txt");
    m_right_spacing++;
  }

  for(; c_count <3; c_count++){
    add_to_scene(scene, 11, c_right_spacing*5+ 54 +c_right_spacing, "cannibal.txt");
    c_right_spacing++;
  }

  return scene;
}

int perform_crossing(char left_bank[10], string targets){
  char **scene = create_scene();
  string left;
  string new_left;
  left = left_bank;
  bool target1_passed = false;
  bool target2_passed = false;

  if(!(targets.size() == 1 || targets.size() == 2)){
    return ERROR_INVALID_MOVE;
  }

  //First scene
  cout << endl;
  cout << "Loading the boat..... " << endl << endl;
  for(int i = 0; i < left.size(); i++){
    if(left[i] == targets[0] && !target1_passed){
      target1_passed = true;
      continue;
    }
    if(left[i] == targets[1] && !target2_passed){
      target2_passed = true;
      continue;
    }
    new_left +=left[i];
  }
  left = new_left;
  cout << left << endl;
  cout << targets << endl;
  scene = make_river_scene(left, targets);
  print_scene(scene);

  //Second scene
  cout << endl;
  cout << "Crossing the river..... " << endl << endl;
  new_left = "";
  for(int i = 0; i < left.size(); i++){
    if(left[i] == 'B'){
      continue;
    }
    new_left +=left[i];
  }
  left = new_left;
  cout << left << endl;
  cout << targets << endl;
  scene = make_river_scene(left, targets);
  print_scene(scene);

  //Third Scene
  cout << endl;
  cout << "Unloading the boat..... " << endl << endl;
  targets = "";
  cout << left << endl;
  cout << targets << endl;

  scene = make_river_scene(left, targets);
  print_scene(scene);

  // left_bank = left.c_string(); //WTF
  // strncpy(left_bank, left, 10);

  for(int i = 0; i < 10; i++){
    left_bank[i] = left[i];
  }

  int right_m_count = 0;
  int right_c_count = 0;
  int left_m_count = 0;
  int left_c_count = 0;
  for(int i = 0; i < left.size(); i++){
    if(left[i] == 'M'){
      left_m_count ++;
    }
    if(left[i] == 'C'){
      left_c_count ++;
    }
  }
  right_m_count = 3 - left_m_count;
  right_c_count = 3 - left_c_count;

  if(left_c_count > left_m_count){
    return ERROR_MISSIONARIES_EATEN;
  }
  if(right_c_count > right_m_count){
    return ERROR_MISSIONARIES_EATEN;
  }
  return VALID_NONGOAL_STATE;
}

int play_game(){
  char left_bank[10] = "BCCCMMM";
  char** scene = make_river_scene(left_bank, "");
  print_scene(scene);
  int code = 0;
  bool game_over = false;
  bool missionaries_eaten = false;

  while(!game_over || !missionaries_eaten){
    string boat;
    cout << "Which characters would you like to take across? M or C"<<endl;
    cin >> boat;
    code = perform_crossing(left_bank, boat);
  }

  destroy_scene(scene);
  return code;
}
