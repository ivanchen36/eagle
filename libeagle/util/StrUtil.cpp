#include <string.h>

#include "StrUtil.h"

namespace eagle
{

void StrUtil::copy(char *&dest, const char *src)
{
    int len = strlen(src) + 1;

    dest = new char[len];
    if (dest == NULL) return;

    strcpy(dest, src);
}

void StrUtil::copy(char *&dest, const char *src, const int len)
{
    dest = new char[len + 1];
    if (dest == NULL) return;

    strcpy(dest, src);
}

void StrUtil::split(const char *sep, const char *str, 
        std::vector<std::string> &vec)
{
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

void StrUtil::split(const char *sep1, const char *sep2, 
        const char *str, std::map<std::string, std::string> &map)
{
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
            continue;
        }

        key.assign(s1, s3 - s1);
        map[key].assign(s3 + len2, s2 - s3 - len2);
        s1 = s2 + len1;
    }
    s3 = strstr(s1, sep2);
    if (NULL == s3) return;

    key.assign(s1, s3 - s1);
    map[key].assign(s3 + len2, s2 - s3 - len2);
}

void StrUtil::split(const char *sep1, const char *sep2, 
        char *str, std::map<char *, char *> &map)
{
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
            continue;
        }

        *s3 = 0;
        *s2 = 0;
        map[s1] = s3 + len2;
        s1 = s2 + len1;
    }
    s3 = strstr(s1, sep2);
    if (NULL == s3) return;

    *s3 = 0;
    map[s1] = s3 + len2;
}

}
