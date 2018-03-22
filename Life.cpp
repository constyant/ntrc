////////////////
//BY CONSTYANT//
////////////////

#include <ncurses.h>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <vector>
#include <ctime>
#include <unistd.h>

//ИТОГОВЫЙ ВАРИАНТ
//РАБОТАЙ
//цвет, псевдографика

using namespace std::chrono;
using namespace std::this_thread;

std::vector < std::vector <char> > original_field;
std::vector < std::vector <char> > zero_field;

//создание игрового и нулевого полей
int createFields(unsigned int n) {
  unsigned rand_value = 1;
  srand(rand_value);

  for (int i = 0; i < n; i++) {
    original_field.insert(original_field.end(), std::vector <char> (n));
    zero_field.insert(zero_field.end(), std::vector <char> (n));
    for (int j = 0; j < n; j++) {
      if (rand() % 2 == 0) {
        original_field[i][j] = '.';
      }
      else {
        original_field[i][j] = '@';
      }
      zero_field[i][j] = '.';
    }
  }

  return 0;
}


int getNeighbours(int x, int y) {
  int c = 0;
  int ii;
  int jj;
  unsigned int size = original_field.size();

  for (int i = x - 1; i <= x + 1; i++) {
    ii = (i + size) % size;
    for (int j = y - 1; j <= y + 1; j++) {
      jj = (j + size) % size;
      if ((i != x) || (j != y)) {
        if (original_field[ii][jj] == '@') {
          c++;
        }
      }
    }
  }
  return c;
}


int mainLoop() {
  std::vector < std::vector <char> > local_field (original_field.size(), std::vector <char> (original_field.size()));
  unsigned long count = 0;

  while (1) {
	count++;

    for (int i = 0; i < original_field.size(); i++) {
      for (int j = 0; j < original_field.size(); j++) {
        unsigned int neigh = getNeighbours(i, j);
        if (original_field[i][j] == '@') {
          if ((neigh < 2) || (neigh > 3)) {
            attroff (COLOR_PAIR(2));
            local_field[i][j] = '.';
          }
        }
        else {
          if (neigh == 3) {
            attron (COLOR_PAIR(1));
            local_field[i][j] = '@';
          }
        }
      }
	}


  attron(COLOR_PAIR(2));
  mvwprintw(stdscr, 2, 0, "%d", count);
  attroff (COLOR_PAIR(2));

	for (int i = 0; i < original_field.size(); i++) {
    	for (int j = 0; j < original_field.size(); j++) {
        if (original_field[i][j] == '@') {
          attron(COLOR_PAIR(1));
          mvwprintw(stdscr, i + 4, j * 2, "%c", original_field[i][j]);
          attroff (COLOR_PAIR(1));
        }
        else {
          attron(COLOR_PAIR(2));
          mvwprintw(stdscr, i + 4, j * 2, "%c", original_field[i][j]);
          attroff (COLOR_PAIR(2));
        }
    	}
    	printw("\n");
  	}


  if ((original_field == local_field) || (original_field == zero_field))	 {
    printw("Ends");
    return 0;
  }
  else {
	  original_field = local_field;
	  refresh();
	  fflush(stdout);
	  usleep(100000);
	}
}

  return 0;
}


int main() {
  unsigned int size;

  using namespace std::this_thread;
  using std::chrono::system_clock;

  initscr();
  echo();
  start_color();

  init_pair (1, COLOR_GREEN, COLOR_BLACK);
  init_pair (2, COLOR_WHITE, COLOR_BLACK);

  wattron(stdscr, COLOR_PAIR(2));

  scanw("%u", &size);
  createFields(size);

  mainLoop();

  refresh();
  getch();
  endwin();

  return 0;
}
