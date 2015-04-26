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

#ifndef _FILE_EX_H_
#define _FILE_EX_H_

#include <stdint.h>
#include <stdio.h>

#include "AutoPtr.h"

#define RDONLY 0
#define WRONLY_TRUNC 1
#define RDWR 2
#define RDWR_CREATE 3
#define RDWR_TRUNC 4
#define RDWR_APPEND 5

const static char *MODE[] = {"r", "w", "r+", "a", "w+", "a+"};

class FileEx
{
public:
    FileEx(const char *fileName, int mode);
    ~FileEx();

	void flush();
    int getFd();
    int seek(long offset, int pos);
    int read(uint8_t *buf, int len);
    int write(const uint8_t *buf, int len);
    int writeStr(const char *str);
    int readByLen(uint8_t *buf, int len);
    int writeByLen(const uint8_t *buf, int len);
    int readByOffset(uint8_t *buf, int len, int offset);
    int writeByOffset(uint8_t *buf, int len, int offset);

private:
    FileEx():m_file(NULL), m_fileName(NULL){};
    int m_mode;
    FILE *m_file;
    char *m_fileName;

    friend class AutoPtr<FileEx>;
};

typedef AutoPtr<FileEx> FileExPtr;
#endif
