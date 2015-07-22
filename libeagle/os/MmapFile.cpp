#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include "MmapFile.h"
#include "StrUtil.h"

namespace eagle
{

namespace
{
const int F_MODE[] = {O_RDONLY, O_WRONLY | O_CREAT, O_RDWR | O_CREAT};
const int M_MODE[] = {PROT_READ, PROT_WRITE, PROT_READ | PROT_WRITE};
}

MmapFile::MmapFile(const char *fileName, const int isShm, 
        const Mode mode, const int pageSize)
    : m_startOffset(0), m_endOffset(0), m_isShm(isShm), 
    m_ptr(NULL), m_fileName(NULL)
{
    int sysPageSize = getpagesize();

    m_pageSize = pageSize > 0 ? pageSize : sysPageSize;
    m_fd = m_isShm ? shm_open(fileName, F_MODE[mode], S_IRUSR | S_IWUSR)
        : open(fileName, F_MODE[mode], S_IRUSR | S_IWUSR);
    if (-1 == m_fd)
    {
        ERRORLOG2("open file %s failed, err %s", 
                fileName, strerror(errno));

        return;
    }

    StrUtil::copy(m_fileName, fileName);
    m_prot = M_MODE[mode];
    m_fileSize = File::getSize(m_fd);
    mapByOffset(0);
}

MmapFile::~MmapFile()
{
    if (-1 == m_fd) return;

    if (NULL != m_ptr) unmap();
    closeFile();
    if (NULL != m_fileName) delete []m_fileName;
}

int MmapFile::flush()
{
    if (msync(m_ptr, m_pageSize, MS_ASYNC) != 0) 
    {
        ERRORLOG2("msync file %s failed, err %s", 
                m_fileName, strerror(errno));

        return -1;
    }

    return 0;
}

int MmapFile::unlink()
{
    if (-1 == m_fd && !m_isShm) return -1;

    if (shm_unlink(m_fileName) != 0)
    {
        ERRORLOG2("shm_unlink file %s failed, err %s", 
                m_fileName, strerror(errno));

        return -1;
    }

    return 0;
}

void MmapFile::closeFile()
{
    if (close(m_fd) != 0)
    {
        ERRORLOG2("close file %s failed, err %s", 
                m_fileName, strerror(errno));
    }
    m_fd = -1;
}

int MmapFile::unmap()
{
    int ret = munmap(m_ptr, m_pageSize);
    if (ret != 0)
    {
        ERRORLOG2("munmap file %s failed, err %s", 
                m_fileName, strerror(errno));
    }
    m_ptr = NULL;
    
    return ret ? -1 : 0;
}

int MmapFile::setSize(const int size)
{
    if ((PROT_WRITE & m_prot) == 0) return -1;

    return File::setSize(m_fd, size);
}

int MmapFile::mapByOffset(const int startOffset)
{
    if (-1 == m_fd) return -1;
    
    if (startOffset != m_startOffset && m_ptr && unmap() != 0) return -1;

    int ret = -1;
    m_startOffset = startOffset;
    m_endOffset = startOffset + m_pageSize;
    if (m_fileSize < m_endOffset && (ret = setSize(m_endOffset)))
    {
        ERRORLOG1("file %s setSize failed", m_fileName);

        return -1;
    }
    if (0 == ret) m_fileSize = m_endOffset;

    m_ptr = (uint8_t *)mmap(NULL, m_pageSize, m_prot, 
            MAP_SHARED, m_fd, m_startOffset);
    if (MAP_FAILED != m_ptr) return 0;

    m_ptr = NULL;
    ERRORLOG2("mmap file %s failed, err %s", 
            m_fileName, strerror(errno));

    return -1;
}

int MmapFile::read(char *buf, const int len, const int offset)
{
    if (read((uint8_t *)buf, len, offset) != 0) return -1;

    buf[len] = 0;

    return 0;
}

int MmapFile::write(const char *buf, const int offset)
{
    int len = strlen(buf);

    return write((const uint8_t *)buf, len, offset);
}

int MmapFile::read(uint8_t *buf, const int len, const int offset)
{
    if (NULL == m_ptr) return -1;
    if ((m_prot & PROT_READ) == 0) return -1;
    
    int tmp;
    int readLen = 0;
    int readOffset = offset;

    do
    {
        if (readOffset < m_startOffset || readOffset > m_endOffset)
        {
            if (mapByOffset((readOffset / m_pageSize) * m_pageSize) != 0)
                return -1;
        }

        tmp = m_endOffset - readOffset;
        if (tmp > len) tmp = len;
        memcpy(buf + readLen, m_ptr + readOffset - m_startOffset, tmp);
        readLen += tmp;
        readOffset += tmp;
    } while (len != readLen);

    return 0;
}

int MmapFile::write(const uint8_t *buf, const int len, const int offset)
{
    if (NULL == m_ptr) return -1;
    if ((m_prot & PROT_WRITE) == 0) return -1;

    int tmp;
    int writeLen = 0;
    int writeOffset = offset;

    do
    {
        if (writeOffset < m_startOffset || writeOffset >= m_endOffset)
        {
            if (mapByOffset((writeOffset / m_pageSize) * m_pageSize) != 0)
                return -1;
        }

        tmp = m_endOffset - writeOffset;
        if (tmp > len) tmp = len;
        memcpy(m_ptr + writeOffset - m_startOffset, buf + writeLen, tmp);
        writeLen += tmp;
        writeOffset += tmp;
    } while (len != writeLen);

    return 0;
}

}
