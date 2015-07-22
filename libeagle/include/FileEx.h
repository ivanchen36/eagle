/**
 *       fileName:  FileEx.h
 *
 *    Description:  File
 *
 *        Version:  1.0
 *        Created:  04/26/2015 11:20:47 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */

#ifndef _EAGLE_FILE_H_
#define _EAGLE_FILE_H_

#include <stdint.h>
#include <stdio.h>

#include "AutoPtr.h"

namespace eagle
{

class File
{
public:
    enum Mode
    {
        RDONLY = 0,
        RDWR,
        WRONLY_CREATE_RESET,
        RDWR_CREATE_RESET,
        WRONLY_CREATE_APPEND,
        RDWR_CREATE_APPEND
    };
    File(const char *fileName, const Mode mode);
    ~File();

	void flush();
    int getFd();
    int getSize();
    int setSize(const int size);
    int seek(const int offset, const int pos);
    int read(uint8_t *buf, const int len);
    int write(const uint8_t *buf, const int len);
    int writeStr(const char *str);
    int readByLen(uint8_t *buf, const int len);
    int writeByLen(const uint8_t *buf, const int len);
    int readByOffset(uint8_t *buf, const int len, const int offset);
    int writeByOffset(const uint8_t *buf, const int len, const int offset);

    static int isExist(const char *fileName);
    static int del(const char *fileName);
    static int getSize(int fd);
    static int setSize(const int fd, const unsigned int size);

private:
    int m_mode;
    FILE *m_file;
    char *m_fileName;
};

typedef AutoPtr<File> FilePtr;

}
#endif
