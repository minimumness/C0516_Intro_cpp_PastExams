/* you may assume a maximum word length of 512 characters*/
#define MAX_LENGTH 512

/* insert your function prototypes here */
int frequency(std::string word);

int edit_distance(std::string const a, std::string const b);

bool spell_correct(std::string word, char fixed[MAX_LENGTH]);
