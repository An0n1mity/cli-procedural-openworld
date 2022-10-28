#ifndef CONSTANT_H
#define CONSTANT_H

//for unicode compatibility
#define _XOPEN_SOURCE_EXTENDED 1
#define NCURSES_WIDECHAR 1

//To define color atributes
#define COLOR_WATER 16
#define COLOR_GRASS 17
#define COLOR_SAND 18
#define COLOR_STONE 19
#define WHITE 20

//Length and height of a chunk, cant be under 3
#define CHUNK_SIZE 10

//square of chunk loaded with the player in the middle
//can't be a multiple of two
#define MAX_CHUNK_DISTANCE 9

//macro definition
#define MIN(a, b) ((a) < (b)) ? (a) : (b)

//default settings for the player
#define PLAYER_DEFAULT_HEALT 3
#define PLAYER_DEFAULT_WATER 100
#define PLAYER_DEFAULT_FOOD 100

//default settings for the chicken
#define CHICKEN_DEFAULT_HEALT 2

#define PLAYER_MV_SPEED 100
#define CHICKEN_MV_SPEED 150



#endif // CONSTANT_H