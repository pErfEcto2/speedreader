#include <iostream>
#include <unistd.h>

#define WORDS_PER_MINUTE 250.0
#define OFFSET_X 30
#define OFFSET_Y 5

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0]
              << " [-w | --words-per-minute WORDS_PER_MINUTE] FILE"
              << std::endl;
    return 1;
  }

  int wpm = WORDS_PER_MINUTE;
  FILE *file = NULL;

  for (int i = 1; i < argc; i++) {
    if (std::string(argv[i]) == "-w" ||
        std::string(argv[i]) == "--words-per-minute") {
      if (argc < 4) {
        std::cerr << "WORDS_PER_MINUTE or FILE is not provided" << std::endl;
        return 2;
      }
      wpm = 0;
      for (int j = 0; argv[i + 1][j]; j++) {
        if (argv[i + 1][j] > '9' || argv[i + 1][j] < '0') {
          std::cerr << "Error: " << argv[i + 1] << " is not an integer value"
                    << std::endl;
          return 3;
        }
        wpm = wpm * 10 + argv[i + 1][j] - '0';
      }
      i++;
    } else if ((file = fopen(argv[i], "r")) == NULL) {
      std::cerr << "Error: '" << argv[1] << "' no such file" << std::endl;
      std::cerr << "Usage: " << argv[0]
                << " [-w | --words-per-minute WORDS_PER_MINUTE] FILE"
                << std::endl;
      return 4;
    }
  }

  system("clear");
  for (int i = 0; i < OFFSET_Y; i++)
    std::cout << std::endl;
  for (int i = 0; i < OFFSET_X; i++)
    std::cout << " ";

  char c;
  int cnt = 0;
  bool last_not_prnt = false;

  while ((c = fgetc(file)) != EOF) {
    if (c == ' ' || c == '\n') {
      if (last_not_prnt)
        continue;

      for (int i = 0; i < OFFSET_X * 2 - cnt; i++)
        std::cout << " ";
      std::cout << "wpm: " << wpm << std::endl;

      cnt = 0;
      usleep(60.0 / wpm * 1e6);
      system("clear");

      for (int i = 0; i < OFFSET_Y; i++)
        std::cout << std::endl;
      for (int i = 0; i < OFFSET_X; i++)
        std::cout << " ";

      last_not_prnt = true;

      continue;
    }
    std::cout << c;
    last_not_prnt = false;
    cnt++;
  }
  fclose(file);
  return 0;
}
