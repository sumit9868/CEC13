#include <iostream>
#include <dirent.h>
#include <sys/types.h>
using namespace std;
void list_dir(const char *path) {
   struct dirent *entry;
   DIR *dir = opendir(path);
   if (dir == NULL) {
      return;
   }
   while ((entry = readdir(dir)) != NULL) {
   cout << entry->d_name << endl;
   }
   closedir(dir);
}
int main(int argc,char *args[]) {
   list_dir(args[1]);
}
