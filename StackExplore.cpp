#if defined _WIN32
// Disable warnings because strncpy can be unsafe
#define _CRT_SECURE_NO_WARNINGS 1
#endif

// If compiling on Windows, you will need to change this setting:
// Disable Security Check (/GS-)
// If using clang, then use -fno-stack-protector
// You may also need to disable Control Flow Guard, if that setting is available
// gcc and clang will have similar settings, to know which these are, please RTFM

// Note - please compile with -std=c++11 when using clang or g++
// If you want to debug with gcc, compile it like so:
// g++ -std=c++11 -fno-stack-protector -g StackExplore.cpp


#include <stdio.h>
#include <string.h>

void Foo(const char* sz)
{
    char buf[24];
    //printf("Address of buf= %p\n", &buf);
    printf("%016lx %016lx %016lx %016lx %016lx %016lx\n",
        *(__uint64_t*)(buf+24),
        *(__uint64_t*)(buf+32),
        *(__uint64_t*)(buf+40),
        *(__uint64_t*)(buf+48),
        *(__uint64_t*)(buf+56),
        *(__uint64_t*)(buf+64)
        );
    // This is an overflow bug, don't do this, either
    memcpy(buf, sz, strlen(sz)+2);
    printf("%016lx %016lx %016lx %016lx %016lx %016lx\n",
        *(__uint64_t*)(buf+24),
        *(__uint64_t*)(buf+32),
        *(__uint64_t*)(buf+40),
        *(__uint64_t*)(buf+48),
        *(__uint64_t*)(buf+56),
        *(__uint64_t*)(buf+64)
        );
    printf("Buf= %s", buf);
}

void Bar(const char* sz)
{
    printf("\n\n!!!We've been hacked!!!, sz=%s\n", sz);
}

int main(int argc, char* argv[])
{
    printf("Address of main= %p\n", main);
    printf("Address of Bar=  %p\n", Bar);
    printf("Address of Foo=  %p\n", Foo);

    printf("Hit enter when the hacked.txt file is ready\n");
    (void)getchar();

    FILE* pf = fopen("hacked.txt", "r");

    if (pf == nullptr)
    {
        printf("Couldn't open file\n");
        return -1;
    }

    char fileBuf[128];
    memset(fileBuf, 0, sizeof(fileBuf));
    size_t cch = fread(fileBuf, sizeof(char), sizeof(fileBuf), pf);

    // Terminate string
    if (cch >= sizeof(fileBuf) - 1)
        cch = sizeof(fileBuf) - 1;

    fileBuf[cch] = '\0';

    Foo(fileBuf);
    fclose(pf);
    printf("\nExiting\n");
    return 0;
}
