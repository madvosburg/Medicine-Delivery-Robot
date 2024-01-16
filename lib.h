/*
 * lib.h
 *
 *  Created on: Dec 4, 2023
 *      Author: mvosburg
 */

void putty_string_print(char inputString[]);

int objectCount();

int quick_sweep(Scan_t *scan, int sweepWidth);

//int pathTo(oi_t *sensor, Scan_t *scan);

void findCenterDistance(float sonicBuffer[]);

void populate_array(Scan_t *scan, int infraBuffer[], float sonicBuffer[], int sweepWidth);

void object_detection(Scan_t *scan, int infraBuffer[], float sonicBuffer[], int sweepWidth);

void info_dump();

int clearStruct();

void patient_ID();

