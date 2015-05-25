/**
 *       fileName:  MmapFile.h
 *
 *    Description:  memory map file
 *
 *        Version:  1.0
 *        Created:  04/30/2015 10:53:10 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef  _MMAPFILE_H_
#define  _MMAPFILE_H_
#include <stdint.h>

#include "Log.h"
#include "AutoPtr.h"

class MmapFile
{
public:
    enum Mode
    {
        RDONLY = 0,
        WRONLY,
        RDWR
    };

    MmapFile(const char *fileName, const int isShm = 0,
            const Mode mode = RDWR, const int pageSize = 0);
    ~MmapFile();

    int flush();
    int unlink();
    int setSize(const int size);
    int read(char *buf, const int len, const int offset);
    int write(const char *buf, const int offset);
    int read(uint8_t *buf, const int len, const int offset);
    int write(const uint8_t *buf, const int len, const int offset);

private:
    int mapByOffset(const int startOffset);
    int unmap();
    void closeFile();

    int m_fd;
    int m_prot;
    int m_startOffset;
    int m_endOffset;
    int m_pageSize;
    int m_fileSize;
    int m_isShm;
    uint8_t *m_ptr;
    char *m_fileName;
};

typedef AutoPtr<MmapFile> MmapFilePtr;
#endif   /* ----- #ifndef _MMAPFILE_H_  ----- */
