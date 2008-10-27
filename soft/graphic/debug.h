/*
 *      Debug configurations
 *
 *            Sungjun Kim
 *
 * This file has configurations for debugging options.
 *
 *
 * 2008.03.28 - The file is created.
 *
 */

#ifndef DEBUG_H
#define DEBUG_H

//#define DBG

#ifdef DBG
#define PRINT printf
#else
#define PRINT
#endif

#endif //DEBUG_H
