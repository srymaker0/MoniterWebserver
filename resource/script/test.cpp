/*************************************************************************
	> File Name: text.cpp
	> Author: 
	> Mail: 
	> Created Time: Sun 03 Apr 2022 06:00:18 PM CST
 ************************************************************************/

#include <iostream>
#include <stdlib.h>
using namespace std;

int main() {
    FILE *fp;
    char buffer[1024];
    fp=popen("./test_all.sh","r");
    fgets(buffer,sizeof(buffer),fp);
    printf("%s",buffer);
    pclose(fp);
    return 0;
}
