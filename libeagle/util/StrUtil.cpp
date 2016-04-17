#include <string.h>

#include "StrUtil.h"
#include "Define.h"

namespace eagle
{

void StrUtil::copy(char *&dest, const char *src)
{
    int len = NULL == src ? 0 : strlen(src);

    dest = new char[len + 1];
    if (dest == NULL) return;

    if (0 == len)
    {
        *dest = 0;

        return;
    }

    strcpy(dest, src);
}

void StrUtil::copy(char *&dest, const char *src, const int len)
{
    int l = NULL == src ? 0 : len;

    dest = new char[l + 1];
    if (dest == NULL) return;

    if (0 == l)
    {
        *dest = 0;

        return;
    }

    strncpy(dest, src, l);
}

void StrUtil::split(const char *sep, const char *str, 
        std::vector<std::string> &vec)
{
    if (NULL == sep || NULL == str) return;

    const char *s1 = str;
    const char *s2;
    int len = strlen(sep);
    int pos = vec.size() - 1;
    std::string nullString;

    vec.reserve(10);
    while ((s2 = strstr(s1, sep)))
    {
        vec.push_back(nullString);
        vec[++pos].assign(s1, s2 - s1);
        s1 = s2 + len;
    }
    vec.push_back(s1);
}

void StrUtil::split(const char *sep, char *str, 
        std::vector<char *> &vec)
{
    if (NULL == sep || NULL == str) return;

    char *s1 = str;
    char *s2;
    int len = strlen(sep);

    vec.reserve(10);
    while ((s2 = strstr(s1, sep)))
    {
        *s2 = 0;
        vec.push_back(s1);
        s1 = s2 + len;
    }
    vec.push_back(s1);
}

int StrUtil::split(const char *sep1, const char *sep2, 
        const char *str, std::map<std::string, std::string> &map)
{
    if (NULL == sep1 || NULL == sep2 || NULL == str) return EG_FAILED;

    const char *s1 = str;
    const char *s2;
    const char *s3;
    int len1 = strlen(sep1);
    int len2 = strlen(sep2);
    std::string key;

    while ((s2 = strstr(s1, sep1)))
    {
        s3 = strstr(s1, sep2);
        if (NULL == s3 || s2 < s3)
        {
            s1 = s2 + len1;

            return EG_FAILED;
        }

        key.assign(s1, s3 - s1);
        map[key].assign(s3 + len2, s2 - s3 - len2);
        s1 = s2 + len1;
    }
    s3 = strstr(s1, sep2);
    if (NULL == s3) return EG_FAILED;

    key.assign(s1, s3 - s1);
    map[key].assign(s3 + len2, strlen(s3) - len2);

    return EG_SUCCESS;
}

int StrUtil::split(const char *sep1, const char *sep2, 
        char *str, std::map<char *, char *> &map)
{
    if (NULL == sep1 || NULL == sep2 || NULL == str) return EG_FAILED;

    char *s1 = str;
    char *s2;
    char *s3;
    int len1 = strlen(sep1);
    int len2 = strlen(sep2);
    std::string key;

    while ((s2 = strstr(s1, sep1)))
    {
        s3 = strstr(s1, sep2);
        if (NULL == s3 || s2 < s3) 
        {
            s1 = s2 + len1;

            return EG_FAILED;
        }

        *s3 = 0;
        *s2 = 0;
        map[s1] = s3 + len2;
        s1 = s2 + len1;
    }
    s3 = strstr(s1, sep2);
    if (NULL == s3) return EG_FAILED;

    *s3 = 0;
    map[s1] = s3 + len2;

    return EG_SUCCESS;
}

#if 0
int gbkToUtf8(const std::string &src, std::string &dest)
{
    int len = src.length();
    char *tmp = new char[len];
    char *buf = new char[len * 2];
    memcpy(tmp, src.c_str(), len);
    if (charsetConvert("gbk","utf-8", tmp, len, buf, len * 2))
    {
        delete[] tmp;
        delete[] buf;

        return -1;
    }

    dest = buf;
    delete[] tmp;
    delete[] buf;

    return 0;
}
#endif

}
