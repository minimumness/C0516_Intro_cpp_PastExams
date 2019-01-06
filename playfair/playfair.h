void prepare(std::string const input, char output[]);

void grid(std::string const input, char playfair[6][6]);

void bigram(char const playfair[6][6], char in1, char in2, char& out1, char& out2);

void encode(char const playfair[6][6], char prepared[], char encoded[]);
