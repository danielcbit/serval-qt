#include <dlfcn.h>
#include <stdio.h>

#include <serval.h>

int main(int argc, char *argv[])
{
   return parseCommandLine(argv[0], argc - 1, (const char*const*)&argv[1]);
}
