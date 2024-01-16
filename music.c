/*
 * music.c
 *
 *  Created on: Dec 4, 2023
 *      Author: mvosburg
 */

#include "open_interface.h"
#include "music.h"

void load_song(){
   // int notes = 11;
    int notes = 3;
   // unsigned char song[11] = {53, 55, 48, 55, 57, 60, 58, 57, 53, 55, 48};
    unsigned char song[3] = {60, 55, 65};
   // unsigned char duration[11] = {48, 64, 16, 48, 48, 8, 8, 8, 48, 64, 64};
    unsigned char duration[3] = {48, 32, 40};
    oi_loadSong(0, notes, song, duration);

}
