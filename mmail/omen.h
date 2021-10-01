/*
 * MultiMail offline mail reader
 * OMEN

 Copyright 1999-2021 William McBrine <wmcbrine@gmail.com>
 Distributed under the GNU General Public License, version 3 or later. */

#ifndef OMEN_H
#define OMEN_H

#include "pktbase.h"

// HEADERxy.BBS records:
struct omenReplyRec {
    unsigned char command;
    unsigned char curboard, moveboard;
    unsigned char msgnumber[2];

    unsigned char tolen;
    char to[35];

    unsigned char sublen;
    char subject[72];

    unsigned char destzone[2], destnet[2], destnode[2];

    unsigned char netattrib;

    unsigned char aliaslen;
    char alias[20];

    unsigned char curhighboard;
    unsigned char movehighboard;
    unsigned char msghighnumber[2];
    char extraspace[4];
};

class omen : public pktbase
{
    char extent[4];
    unsigned useLatin;

    void readSystemBBS();
    void buildIndices();

    void prefirstblk();
 public:
    omen();
    ~omen();
    area_header *getNextArea();
    letter_header *getNextLetter();
    const char *getExtent();
    bool isLatin();
};

class omenrep : public pktreply
{
    class upl_omen : public upl_base
    {
        omenReplyRec omen_rec;
     public:
        char subject[73], to[36];
        net_address na;
        long refnum;
        int origArea;
        bool privat;

        upl_omen(const char * = 0);

        bool init(FILE *);
        void output(FILE *);
    };
    bool getRep1(FILE *, upl_omen *, int);
    void getReplies(FILE *);
    void addRep1(FILE *, upl_base *, int);
    void addHeader(FILE *);
    void repFileName();
    const char *repTemplate(bool);
 public:
    omenrep();
    ~omenrep();
    area_header *getNextArea();
    letter_header *getNextLetter();
    void enterLetter(letter_header &, const char *, long);
    bool getOffConfig();
    bool makeOffConfig();
};

#endif
