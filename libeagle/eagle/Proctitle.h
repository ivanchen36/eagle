/**
 *       Filename:  Proctitle.h
 *
 *    Description:  Proctitle
 *
 *        Version:  1.0
 *        Created:  06/20/2015 04:02:18 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef  _PROCTITLE_H_
#define  _PROCTITLE_H_
class Proctitle 
{
public:
    Proctitle();
    ~Proctitle();

    void init(const int argc, char *const *argv);
    void setMaster(const int argc, char *const *argv, const char *format, ...);
    void setWorker(const int argc, char *const *argv, const char *format, ...);

private:
    char *m_environ;
    char *m_last;
};
#endif   /* ----- #ifndef _PROCTITLE_H_  ----- */
