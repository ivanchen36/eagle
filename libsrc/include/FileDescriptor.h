/**
 *       Filename:  FileDescriptor.h
 *
 *    Description:  file descriptor
 *
 *        Version:  1.0
 *        Created:  05/03/2015 08:53:31 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */


#ifndef  _FILEDESCRIPTOR_H_
#define  _FILEDESCRIPTOR_H_
class FileDescriptor
{
public:
	FileDescriptor(const int fd) : m_fd(fd){};
	virtual ~FileDescriptor();

	int setAsync();
	int setPid(pid_t pid);
    int setCloseOnexe();
	int setNonBlocking();
    int read(const char *str, const int len);
    int read(const uint8_t *str, const int len);
    int write(const char *str);
    int write(const uint8_t *str, const int len);

private:
	const int m_fd;
};

#endif   /* ----- #ifndef _FILEDESCRIPTOR_H_  ----- */
