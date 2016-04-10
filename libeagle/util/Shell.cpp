#include "Shell.h"
#include <stdio.h>

namespace ealge
{

int Shell::execute(const std::string &cmd, std::string &rs)
{
    int len = 0;
    char buf[1024];
    FILE *pf = popen(cmd.c_str(), "r");

    if (NULL == pf) return -1; 

    rs.clear();
    do  
    {   
        len = fread(buf, 1, 1024, pf);
        if (len > 0) rs.append(buf, 0, len);
    }while (len > 0); 

    pclose(pf);

    return 0;
}

}
