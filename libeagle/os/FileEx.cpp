#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/statvfs.h>
#include <sys/stat.h>

#include "FileEx.h"
#include "Define.h"
#include "StrUtil.h"

namespace
{
const char *MODE[] = {"r", "r+", "w", "w+", "a", "a+"};
}

int FileEx::isExist(const char *fileName)
{
    if (access(fileName, F_OK) == 0) return 1;

    return 0;
}

int FileEx::del(const char *fileName)
{
    if (remove(fileName) == 0) return EG_SUCCESS;

    return EG_FAILED;
}

int FileEx::getSize(const int fd)
{
    struct stat st;

    if (fstat(fd, &st) != 0)
    {
        printf("[%s:%d] fstat error: %s!\n", 
                __FILE__, __LINE__, strerror(errno));

        return EG_FAILED;
    }

    return st.st_size;
}

int FileEx::setSize(const int fd, const unsigned int size)
{
    struct statvfs vfs;

    if (fstatvfs(fd, &vfs) != 0)
    {
        printf("[%s:%d] fstatvfs error: %s!\n", 
                __FILE__, __LINE__, strerror(errno));

        return EG_FAILED;
    }

    if (vfs.f_bfree * vfs.f_bsize < size) return EG_FAILED;

    if (ftruncate(fd, size) != 0)
    {
        printf("[%s:%d] ftruncate error: %s!\n", 
                __FILE__, __LINE__, strerror(errno));

        return EG_FAILED;
    }

    return EG_SUCCESS;
}

FileEx::FileEx(const char *fileName, const Mode mode) 
    : m_file(NULL), m_fileName(NULL)
{
    m_mode = mode;
    m_file = fopen(fileName, MODE[m_mode]);

    if (NULL == m_file)
    {
        printf("[%s:%d] fopen error: %s!\n", 
                __FILE__, __LINE__, strerror(errno));

        return;
    }

    char *tmp = rindex((char *)fileName,'/');

    if (NULL != tmp)
    {
        StrUtil::copy(m_fileName, tmp + 1);
    }else
    {
        StrUtil::copy(m_fileName, fileName);
    }
}

FileEx::~FileEx()
{
    if (NULL != m_file)
    {
        fflush(m_file);
        fclose(m_file);
    }
    if (NULL != m_fileName) delete []m_fileName;
}

int FileEx::getSize()
{
    return getSize(fileno(m_file));
}

int FileEx::setSize(const int size)
{
    if (RDONLY == m_mode) return EG_FAILED;

    return setSize(fileno(m_file), size);
}

int FileEx::getFd()
{
    return fileno(m_file);
}

void FileEx::flush()
{
    if (NULL == m_file) return;

    fflush(m_file);
}

int FileEx::seek(const int offset, const int pos)
{
    if (NULL == m_file) return EG_FAILED;

    return fseek(m_file, offset, pos);
}

int FileEx::read(uint8_t *buf, const int len)
{
    if (NULL == m_file) return EG_FAILED;

    int rs;
    FILE *file = m_file;

    rs = fread(buf, 1, len, file);

    if (0 == rs && ferror(file) != 0)
    {
        clearerr(file);
        printf("[%s:%d] an error occurred when read file %s!\n", 
                __FILE__, __LINE__, m_fileName);
    }

    return rs;
}

int FileEx::write(const uint8_t *buf, const int len)
{
    if (NULL == m_file) return EG_FAILED;

    int rs;
    FILE *file = m_file;

    rs = fwrite(buf, 1, len, m_file);

    if (0 == rs && ferror(file) != 0)
    {
        clearerr(file);
        printf("[%s:%d] an error occurred when write file %s!\n", 
                __FILE__, __LINE__, m_fileName);
    }

    return rs;
}

int FileEx::writeStr(const char *str)
{
    if (NULL == m_file) return EG_FAILED;

    int len = strlen(str);
    
    return writeByLen((const uint8_t *)str, len);
}

int FileEx::readByLen(uint8_t *buf, const int len)
{
    if (NULL == m_file) return EG_FAILED;

    int rs = 0; 
    int readLen = 0;
    do  
    {   
        rs = fread(buf + readLen, 1, len - readLen, m_file);
        if (rs > 0) readLen += rs;
    }while (rs > 0 && len != readLen);

    if (len == readLen) return EG_SUCCESS;

    if (ferror(m_file) != 0)
    {
        clearerr(m_file);
        printf("[%s:%d] an error occurred when read file %s!\n", 
                __FILE__, __LINE__, m_fileName);
    }

    return EG_FAILED;
}

int FileEx::writeByLen(const uint8_t *buf, const int len)
{
    if (NULL == m_file) return EG_FAILED;

    int rs = 0; 
    int writeLen = 0;
    do  
    {   
        rs = fwrite(buf + writeLen, 1, len - writeLen, m_file);
        if (rs > 0) writeLen += rs;
    }while (rs > 0 && len != writeLen);

    if (len == writeLen) return EG_SUCCESS;

    if (ferror(m_file) != 0)
    {
        clearerr(m_file);
        printf("[%s:%d] an error occurred when write file %s!\n", 
                __FILE__, __LINE__, m_fileName);
    }

    return EG_FAILED;
}

int FileEx::readByOffset(uint8_t *buf, const int len, const int offset)
{
    if (NULL == m_file) return EG_FAILED;

    if (fseek(m_file, offset, SEEK_SET) != 0) return EG_FAILED;

    return readByLen(buf, len);

}

int FileEx::writeByOffset(const uint8_t *buf, const int len, const int offset)
{
    if (NULL == m_file) return EG_FAILED;

    if (fseek(m_file, offset, SEEK_SET) != 0) return EG_FAILED;

    return writeByLen(buf, len);
}
