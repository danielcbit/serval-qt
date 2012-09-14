#include <dlfcn.h>
#include <stdio.h>

#include <serval.h>

int main(int argc, char *argv[])
{

//    int (*servalmain)(const char *, int, const char *const *);
//    void *h = dlopen("/opt/serval/lib/libserval.so",RTLD_LAZY);
//    *(void **) (&servalmain) = dlsym(h,"parseCommandLine");
//    if (!servalmain) {
//        qDebug() << "Could not load libserval.so\n";
//        return 0;
//    } else {
//        (*servalmain)(argv[0], argc - 1, (const char*const*)&argv[1]);
//    }

   return parseCommandLine(argv[0], argc - 1, (const char*const*)&argv[1]);
}
