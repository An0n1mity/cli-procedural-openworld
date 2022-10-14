#include <ncurses.h>
#include <locale.h>

int main(int argc, char const *argv[])
{
    setlocale(LC_ALL, "");
    initscr();
    printw("🌲");
    refresh();
    return 0;
}
