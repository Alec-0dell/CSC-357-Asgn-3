#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> 
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#define _GNU_SOURCE

#define SetBit(B,k)     ( B[(k/32)] |= (1 << (k%32)) )
#define ClearBit(B,k)   ( B[(k/32)] &= ~(1 << (k%32)) )
#define TestBit(B,k)    ( B[(k/32)] & (1 << (k%32)) )
