#include <ncurses.h>
#include <form.h>

int main()
{
    WINDOW *pad;
    FIELD *field[3];
    FORM *my_form;
    FIELD *field_two[3];
    FORM *form_two;
    int ch;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    pad = newpad(LINES, COLS);
    box(pad, 0, 0);
    keypad(pad, TRUE);

    field[0] = new_field(1, 10, 4, 18, 0, 0);
    field[1] = new_field(1, 10, 6, 18, 0, 0);
    field[2] = NULL;

    field_two[0] = new_field(1, 10, 8, 18, 0, 0);
    field_two[1] = new_field(1, 10, 10, 18, 0, 0);
    field_two[2] = NULL;

    set_field_back(field[0], A_UNDERLINE);
    set_field_back(field[1], A_UNDERLINE);

    set_field_back(field_two[0], A_UNDERLINE);
    set_field_back(field_two[1], A_UNDERLINE);

    wclear(pad);
    my_form = new_form(field);
    set_form_sub(my_form, pad);
    post_form(my_form);

    form_two = new_form(field_two);
    set_form_sub(form_two, pad);
    post_form(form_two);

    mvwprintw(pad, 1, 1, "Some text");
    mvwprintw(pad, 4, 10, "Value 1:");
    mvwprintw(pad, 6, 10, "Value 2:");
    mvwprintw(pad, 8, 10, "Value 3:");
    mvwprintw(pad, 10, 10, "Value 4:");
    prefresh(pad, 0, 0, 0, 0, LINES, COLS);

    bool form1 = true;
    while ((ch = getch()) != KEY_F(1))
    {
        switch (ch)
        {
        case KEY_DOWN:
            if (form1)
            {
                form_driver(my_form, REQ_NEXT_FIELD);
                form_driver(my_form, REQ_END_LINE);
            }
            else
            {
                form_driver(form_two, REQ_NEXT_FIELD);
                form_driver(form_two, REQ_END_LINE);
            }
            break;
        case KEY_UP:
            if (form1)
            {
                form_driver(my_form, REQ_PREV_FIELD);
                form_driver(my_form, REQ_END_LINE);
            }
            else
            {
                form_driver(form_two, REQ_PREV_FIELD);
                form_driver(form_two, REQ_END_LINE);
            }

            break;
        case KEY_RIGHT:
            form1 = false;
            break;
        case KEY_LEFT:
            form1 = true;
            break;
        default:
            if (form1)
            {
                form_driver(my_form, ch);
            }
            else
            {
                form_driver(form_two, ch);
            }
            break;
        }
        prefresh(pad, 0, 0, 0, 0, LINES, COLS);
    }

    unpost_form(my_form);
    free_form(my_form);
    free_field(field[0]);
    free_field(field[1]);
    unpost_form(form_two);
    free_form(form_two);
    free_field(field_two[0]);
    free_field(field_two[1]);
    endwin();
    return 0;
}