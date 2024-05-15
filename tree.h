#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> 
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#define _GNU_SOURCE

#define SetBit(A,k)     ( A[(k/32)] |= (1 << (k%32)) )
#define ClearBit(A,k)   ( A[(k/32)] &= ~(1 << (k%32)) )
#define TestBit(A,k)    ( A[(k/32)] & (1 << (k%32)) )
