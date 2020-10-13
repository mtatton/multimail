/*
 * MultiMail offline mail reader
 * miscellaneous routines (global)

 Copyright 1996-1997 Toth Istvan <stoty@vma.bme.hu>
 Copyright 1997-2020 William McBrine <wmcbrine@gmail.com>
 Distributed under the GNU General Public License, version 3 or later. */

#ifndef MISC_H
#define MISC_H

extern "C" {
#include <stdio.h>
#include <time.h>
}

unsigned getshort(const unsigned char *);
unsigned long getlong(const unsigned char *);
unsigned long getblong(const unsigned char *);
void putshort(unsigned char *, unsigned);
void putlong(unsigned char *, unsigned long);
void putblong(unsigned char *, unsigned long);

struct tm *getdostime(unsigned long);
unsigned long mkdostime(struct tm *);

char *strnzcpy(char *, const char *, size_t);
void cropesp(char *);
void unspace(char *);
char *strdupplus(const char *);
char *strdupblank(const char *);
char *fullpath(const char *, const char *);
char *quotespace(const char *);
const char *findBaseName(const char *);
char *fixPath(const char *);
int getNumExt(const char *);
const char *stripre(const char *);
const char *searchstr(const char *, const char *, int = -1);
const char *fromAddr(const char *);
const char *fromName(const char *);
bool quoteIt(const char *);

void headdec(const char *, const char *, char *);
void headenc(const unsigned char *, const char *, FILE *);
unsigned char *qpdecode(unsigned char *);
long qpdecode(FILE *, FILE *);
void qpencode(FILE *, FILE *);

void fatalError(const char *);  // actually in ../interfac/main.cc!
void pauseError(const char *);  // actually in ../interfac/main.cc!

#endif
