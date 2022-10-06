#include <curses.h>
#include <menu.h>
#include <stdlib.h>
void main()
{
    char *options[] = {
        "Red",
        "Blue",
        "Green",
        "Orange",
        "Brown",
        "White",
        "Black",
        "Yellow",
        "Violet",
        "Purple",
        "Aqua",
        "Slate"};
    int i;
    int ch;
    MENU *menu;
    ITEM **items;
    WINDOW *w;
    initscr();   /* initialize curses */
    curs_set(0); /* hide the blinking cursor */
    noecho();    /* hide the keyboard input */
    /* Create Items */
    items = (ITEM **)calloc(12, sizeof(ITEM *));
    for (i = 0; i < 12; i++)
        items[i] = new_item(options[i], "");
    /* Create Menu */
    menu = new_menu(items);
    /* Create Window for the menu */
    w = newwin(7, 15, 5, 5);
    box(w, 0, 0);
    keypad(w, TRUE);
    /* set menu window */
    set_menu_win(menu, w);
    /*set menu sub window */
    set_menu_sub(menu, derwin(w, 6, 14, 1, 1));
    set_menu_mark(menu, " "); /* string used as menu marker */
    /* set menu format - no of items to be displayed */
    set_menu_format(menu, 5, 1);
    /*post the menu */
    post_menu(menu);
    wrefresh(w);
    while ((ch = wgetch(w)) != 27) /* 27 is the key code for ESC */
    {
        switch (ch)
        {
        case KEY_DOWN:
            menu_driver(menu, REQ_DOWN_ITEM);
            break;
        case KEY_UP:
            menu_driver(menu, REQ_UP_ITEM);
            break;
        case KEY_NPAGE:
            menu_driver(menu, REQ_SCR_DPAGE);
            break;
        case KEY_PPAGE:
            menu_driver(menu, REQ_SCR_UPAGE);
            break;
        }
        wrefresh(w);
    }
unpost_menu(menu);
free_menu(menu);
for (i = 0; i < 12; i++)
    free_item(items[i]);
endwin();
}