/*
 * MultiMail offline mail reader
 * mmail class

 Copyright 1996 Toth Istvan <stoty@vma.bme.hu>
 Copyright 1998-2021 William McBrine <wmcbrine@gmail.com>,
                     Robert Vukovic <vrobert@uns.ns.ac.yu>
 Distributed under the GNU General Public License, version 3 or later. */

#include "mmail.h"
#include "compress.h"
#include "../interfac/error.h"

net_address::net_address()
{
    isSet = isInternet = false;
    zone = 0;
    inetAddr = 0;
}

net_address::net_address(net_address &x)
{
    isSet = isInternet = false;
    zone = 0;
    inetAddr = 0;
    copy(x);
}

net_address::~net_address()
{
    if (isSet && isInternet)
        delete[] inetAddr;
}

bool net_address::operator==(net_address &x)
{
    if (isInternet != x.isInternet)
        return false;

    if (isInternet)
        return !strcmp(inetAddr, x.inetAddr);
    else
        return (zone == x.zone) && (net == x.net) &&
               (node == x.node) && (point == x.point);
}

net_address &net_address::operator=(const char *source)
{
    isInternet = source ? !(!strchr(source, '@')) : false;

    if (isInternet) {
        delete[] inetAddr;
        inetAddr = strdupplus(source);
        isSet = true;
    } else {
        if (sscanf(source ? source : "", "%u:%u/%u.%u",
                   &zone, &net, &node, &point) == 3)
            point = 0;

        isSet = !(!zone);
    }

    return *this;
}

void net_address::copy(net_address &x)
{
    isSet = x.isSet;
    if (isSet) {
        isInternet = x.isInternet;
        if (isInternet)
            inetAddr = strdupplus(x.inetAddr);
        else {
            zone = x.zone;
            net = x.net;
            node = x.node;
            point = x.point;
        }
    }
}

net_address &net_address::operator=(net_address &x)
{
    copy(x);
    return *this;
}

net_address::operator const char *()
{
    static char netText[25];

    if (isSet)
        if (isInternet)
            return inetAddr;
        else
            if (point)
                sprintf(netText, "%u:%u/%u.%u", zone, net, node, point);
            else
                sprintf(netText, "%u:%u/%u", zone, net, node);
    else
        netText[0] = '\0';

    return netText;
}

void mmail::Delete()
{
    delete areaList;
    delete driverList;
    delete workList;
}

// Open a packet
pktstatus mmail::selectPacket(const char *packetName)
{
    pktstatus result;

    const char *x = strrchr(packetName, '/');
    if (!x)
        x = strrchr(packetName, '\\');
    if (x) {
        size_t len = x - packetName;
        char *fname = new char[len + 1];
        strnzcpy(fname, packetName, len);

        mychdir(error.getOrigDir());
        mychdir(fname);
        delete[] fname;

        fname = mygetcwd();
        res.set_noalloc(PacketDir, fname);
        packetName = x + 1;
    }
    res.set(PacketName, packetName);

    // Uncompression is done here
    char *fpath = fullpath(res.get(PacketDir), packetName);

    if (!res.get(oldPacketName) ||
        strcmp(packetName, res.get(oldPacketName))) {

        res.set(oldPacketName, packetName);
        result = uncompressFile(fpath, res.get(WorkDir), true);
        if (result != PKT_OK)
            return result;
    }

    delete[] fpath;

    workList = new file_list(res.get(WorkDir));

    if (!workList->getNoOfFiles()) {
        delete workList;
        return PKT_NOFILES;
    }

    driverList = new driver_list();

    packet = driverList->getDriver(REPLY_AREA + 1);
    reply = driverList->getReplyDriver();

    if (!driverList->getNoOfDrivers()) {
        delete driverList;
        delete workList;
        return PTYPE_UNK;
    }
    return PKT_OK;
}

// Save last read pointers
bool mmail::saveRead()
{
    return driverList->getReadObject(packet)->saveAll();
}

// Is there a reply packet?
bool mmail::checkForReplies()
{
    return reply->checkForReplies();
}

// Create a reply packet
bool mmail::makeReply()
{
    return reply->makeReply();
}

void mmail::deleteReplies()
{
    reply->deleteReplies();

    // to reset the "replyExists" flag (inelegant, I know):
    checkForReplies();
}

void mmail::openReply()
{
    reply->init();
}

bool mmail::getOffConfig()
{
    return reply->getOffConfig();
}

