#include <string.h>
#include <errno.h>

#include "FileEx.h"

FileEx::FileEx(const char *fileName, int mode) 
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

    if (RDWR_CREATE == m_mode)
    {
        if (fseek(m_file, 0L, SEEK_SET) == -1)
        {
            fclose(m_file);
            m_file = NULL;
            printf("fseek error: %s!", strerror(errno));

            return;
        }
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


int FileEx::getFd()
{
    return fileno(m_file);
}

void FileEx::flush()
{
    if (!m_file) return;

    fflush(m_file);
}

int FileEx::seek(long offset, int pos)
{
    if (!m_file) return -1;

    return fseek(m_file, offset, pos);
}

int FileEx::read(uint8_t *buf, int len)
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

int FileEx::write(const uint8_t *buf, int len)
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

int FileEx::readByLen(uint8_t *buf, int len)
{
    if (!m_file) return -1;

    int rs = 0; 
    do  
    {   
        rs = fread(buf, 1, len, m_file);
        len = len - rs; 
    }while (len && rs);

    if (!len) return 0;

    if (ferror(m_file))
    {
        clearerr(m_file);
        printf("an error occurred when read file %s!", m_fileName);
    }

    return -1;
}

int FileEx::writeByLen(const uint8_t *buf, int len)
{
    if (!m_file) return -1;

    int rs = 0; 
    do  
    {   
        rs = fwrite(buf, 1, len, m_file);
        len = len - rs; 
    }while (len && rs);

    if (!len) return 0;

    if (ferror(m_file))
    {
        clearerr(m_file);
        printf("an error occurred when write file %s!", m_fileName);
    }

    return -1;
}

int FileEx::readByOffset(uint8_t *buf, int len, int offset)
{
    if (!m_file) return -1;

    if(fseek(m_file, offset, SEEK_SET) == -1) return -1;

    return readByLen(buf, len);

}

int FileEx::writeByOffset(uint8_t *buf, int len, int offset)
{
    if (!m_file) return -1;

    if(fseek(m_file, offset, SEEK_SET) == -1) return -1;

    return writeByLen(buf, len);
}
