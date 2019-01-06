#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>

using namespace std;

/* You are pre-supplied with the functions below. Add your own
   function definitions to the end of this file. */

/* helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* helper function for internal use only which gets the dimensions of a maze */
bool get_maze_dimensions(const char *filename, int &height, int &width) {
  char line[512];

  ifstream input(filename);

  height = width = 0;

  input.getline(line,512);
  while (input) {
    if ( (int) strlen(line) > width)
      width = strlen(line);
    height++;
    input.getline(line,512);
  }

  if (height > 0)
    return true;
  return false;
}

/* pre-supplied function to load a maze from a file*/
char **load_maze(const char *filename, int &height, int &width) {

  bool success = get_maze_dimensions(filename, height, width);

  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width);

  ifstream input(filename);

  char line[512];

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
  }

  return m;
}

/* pre-supplied function to print a maze */
void print_maze(char **m, int height, int width) {
  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0)
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(4) << r << " ";
    for (int c=0; c<width; c++)
      cout << m[r][c];
    cout << endl;
  }
}

void find_marker(char ch, char** maze, int height, int width, int& row, int& column){
  for (int r = 0; r<height; r++){
    for (int c = 0; c<width; c++){
      if(maze[r][c] == ch){
        row = r;
        column = c;
      }
    }
  }
}


bool valid_solution(std::string path, char** maze, int height, int width){
  int beginning_row;
  int beginning_col;

  find_marker('>', maze, height, width, beginning_row, beginning_col);

  int current_row = beginning_row;
  int current_col = beginning_col;

  for(size_t i = 0; i<path.size(); i++){
    if(path[i] == 'E'){
      current_col++;
    }
    if(path[i] == 'S'){
      current_row++;
    }
    if(path[i] == 'W'){
      current_col--;
    }
    if(path[i] == 'N'){
      current_row--;
    }
    if(maze[current_row][current_col] == '|' || maze[current_row][current_col] == '+' || maze[current_row][current_col] == '-'){
      return false;
    }
  }

  if(maze[current_row][current_col] == 'X'){
    return true;
  }else{
    return false;
  }
}

bool valid_path(std::string path, char** maze, int height, int width, char start, char end){
  int beginning_row;
  int beginning_col;

  find_marker(start, maze, height, width, beginning_row, beginning_col);

  int current_row = beginning_row;
  int current_col = beginning_col;

  for(size_t i = 0; i<path.size(); i++){
    if(path[i] == 'E'){
      current_col++;
    }
    if(path[i] == 'S'){
      current_row++;
    }
    if(path[i] == 'W'){
      current_col--;
    }
    if(path[i] == 'N'){
      current_row--;
    }
    if(maze[current_row][current_col] == '|' || maze[current_row][current_col] == '+' || maze[current_row][current_col] == '-'){
      return false;
    }
  }

  if(maze[current_row][current_col] == end){
    return true;
  }else{
    return false;
  }
}

bool potential_solution(std::string path, char** maze, int height, int width, char start){
  int current_row;
  int current_col;
  find_marker(start, maze, height, width, current_row, current_col);

  for(size_t i = 0; i<path.size(); i++){
    if(path[i] == 'E'){
      current_col++;
    }
    if(path[i] == 'S'){
      current_row++;
    }
    if(path[i] == 'W'){
      current_col--;
    }
    if(path[i] == 'N'){
      current_row--;
    }
  }

  if(current_col < 0 || current_row < 0 || current_col > width || current_row > height){
    return false;
  }

  if(maze[current_row][current_col] == ' '){
    return true;
  }else{
    return false;
  }
}

void mark_path(char** maze, int height, int width, string path){
  int current_row;
  int current_col;
  find_marker('>', maze, height, width, current_row, current_col);

  for(size_t i = 0; i<path.size(); i++){
    if(path[i] == 'E'){
      current_col++;
    }
    if(path[i] == 'S'){
      current_row++;
    }
    if(path[i] == 'W'){
      current_col--;
    }
    if(path[i] == 'N'){
      current_row--;
    }
    maze[current_row][current_col] = '#';
  }
}


bool try_direction(char** maze, int height, int width, string& path, char start, char end){

  char directions[4] = {'N', 'E', 'S', 'W'};

  for(int i = 0; i<4; i++){
    cout << path <<endl;
    mark_path(maze, height, width, path);
    print_maze(maze, height, width);
    string lol;
    getline(cin, lol);
    if(path[path.size()-1] == 'N' && directions[i] == 'S'){
      continue;
    }

    if(path[path.size()-1] == 'S' && directions[i] == 'N'){
      continue;
    }

    if(path[path.size()-1] == 'E' && directions[i] == 'W'){
      continue;
    }

    if(path[path.size()-1] == 'W' && directions[i] == 'E'){
      continue;
    }

    path += directions[i];
    if(valid_path(path, maze, height, width, start, end)){
      return true;
    }

    if(potential_solution(path, maze, height, width, start)){
      if(try_direction(maze, height, width, path, start, end)){
        return true;
      }
    }
    path = path.substr(0, path.size()-1);
  }
  return false;
}


string find_path(char** maze, int height, int width, char start, char end){
  // int beginning_row;
  // int beginning_col;
  string path;

  // find_marker(start, maze, height, width, beginning_row, beginning_col);
  if(try_direction(maze, height, width, path, start, end)){
    mark_path(maze, height, width, path);
    return path;
  }else{
    return " no path available";
  }
}
