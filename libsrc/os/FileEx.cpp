#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/statvfs.h>
#include <sys/stat.h>

#include "FileEx.h"

namespace
{
    const char *MODE[] = {"r", "r+", "w", "w+", "a", "a+"};
}

int FileEx::getSize(const int fd)
{
    struct stat st;

    if (fstat(fd, &st))
    {
        printf("fstat error: %s!", strerror(errno));

        return -1;
    }

    return st.st_size;
}

int FileEx::setSize(const int fd, const int size)
{
    struct statvfs vfs;

    if (fstatvfs(fd, &vfs))
    {
        printf("fstatvfs error: %s!", strerror(errno));

        return -1;
    }

    if (vfs.f_bfree * vfs.f_bsize < size) return -1;

    if (ftruncate(fd, size))
    {
        printf("ftruncate error: %s!", strerror(errno));

        return -1;
    }

    return 0;
}

FileEx::FileEx(const char *fileName, const Mode mode) 
    : m_file(NULL), m_fileName(NULL)
{
    m_mode = mode;
    m_file = fopen(fileName, MODE[m_mode]);

    if (!m_file)
    {
        printf("fopen error: %s!", strerror(errno));

        return;
    }

    int len = 0;
    char *tmp = rindex((char *)fileName,'/');

    if (!tmp) tmp = rindex((char *)fileName,'\\');
    len = tmp ? strlen(tmp) : strlen(fileName);
    m_fileName = new char[len + 1];    
    if (tmp)
    {
        strcpy(m_fileName, tmp);
    }else
    {
        strcpy(m_fileName, fileName);
    }
}

FileEx::~FileEx()
{
    if (m_file)
    {
        fflush(m_file);
        fclose(m_file);
    }
    if (m_fileName) delete []m_fileName;
}

int FileEx::getSize()
{
    return getSize(fileno(m_file));
}

int FileEx::setSize(const int size)
{
    if (RDONLY == m_mode) return -1;

    return setSize(fileno(m_file), size);
}

int FileEx::getFd()
{
    return fileno(m_file);
}

void FileEx::flush()
{
    if (!m_file) return;

    fflush(m_file);
}

int FileEx::seek(const int offset, const int pos)
{
    if (!m_file) return -1;

    return fseek(m_file, offset, pos);
}

int FileEx::read(uint8_t *buf, const int len)
{
    if (!m_file) return -1;

    int rs;
    FILE *file = m_file;

    rs = fread(buf, 1, len, file);

    if (!rs && ferror(file))
    {
        clearerr(file);
        printf("an error occurred when read file %s!", m_fileName);
    }

    return rs;
}

int FileEx::write(const uint8_t *buf, const int len)
{
    if (!m_file) return -1;

    int rs;
    FILE *file = m_file;

    rs = fwrite(buf, 1, len, m_file);

    if (!rs && ferror(file))
    {
        clearerr(file);
        printf("an error occurred when write file %s!", m_fileName);
    }

    return rs;
}

int FileEx::writeStr(const char *str)
{
    if (!m_file) return -1;

    int len = strlen(str);
    
    return writeByLen((const uint8_t *)str, len);
}

int FileEx::readByLen(uint8_t *buf, const int len)
{
    if (!m_file) return -1;

    int rs = 0; 
    int readLen = 0;
    do  
    {   
        rs = fread(buf + readLen, 1, len - readLen, m_file);
        if (rs > 0) readLen += rs;
    }while (rs > 0 && len != readLen);

    if (len == readLen) return 0;

    if (ferror(m_file))
    {
        clearerr(m_file);
        printf("an error occurred when read file %s!", m_fileName);
    }

    return -1;
}

int FileEx::writeByLen(const uint8_t *buf, const int len)
{
    if (!m_file) return -1;

    int rs = 0; 
    int writeLen = 0;
    do  
    {   
        rs = fwrite(buf + writeLen, 1, len - writeLen, m_file);
        if (rs > 0) writeLen += rs;
    }while (rs > 0 && len != writeLen);

    if (len == writeLen) return 0;

    if (ferror(m_file))
    {
        clearerr(m_file);
        printf("an error occurred when write file %s!", m_fileName);
    }

    return -1;
}

int FileEx::readByOffset(uint8_t *buf, const int len, const int offset)
{
    if (!m_file) return -1;

    if(fseek(m_file, offset, SEEK_SET) == -1) return -1;

    return readByLen(buf, len);

}

int FileEx::writeByOffset(uint8_t *buf, const int len, const int offset)
{
    if (!m_file) return -1;

    if(fseek(m_file, offset, SEEK_SET) == -1) return -1;

    return writeByLen(buf, len);
}
