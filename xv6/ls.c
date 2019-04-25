#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

int remove_cont_id (char *str) {
  // Go to end
  int i = 0;
  for (i = 0; !(str[i] == '#' || str[i] == '\0'); i++);
  // printf(1, "%s %d", str, i);// str[i]);
  if (str[i] == '#') {
    int cont_id = str[i+1] - '0';
    str[i] = ' ';
    str[i+1] = ' ';
    return cont_id;
  }

  return -1;
  // while (str[i] != '\0') i++;
  // i -= 2;
  // if (str[i] == '#') {
    
  // } else {
  //   return -1;
  // }
}

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
ls(char *path)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    printf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    printf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  char *fname;

  switch(st.type){
  case T_FILE:
    
    fname = fmtname(path);
    int rmv = remove_cont_id (fname);
    if (rmv < 0 || rmv == get_container_id()) {
      printf(1, "%d %s %d %d %d\n", rmv, fname, st.type, st.ino, st.size);
    }
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, "ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf(1, "ls: cannot stat %s\n", buf);
        continue;
      }

      fname = fmtname(buf);
      int rmv = remove_cont_id (fname);
      if (rmv < 0 || rmv == get_container_id()) {
        printf(1, "%s %d %d %d\n", fname, st.type, st.ino, st.size);
      }
      // printf(1, "DD %s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
    }
    break;
  }
  close(fd);
}

// int
// main(int argc, char *argv[])
// {
//   int i;

//   if(argc < 2){
//     ls(".");
//     exit();
//   }
//   for(i=1; i<argc; i++)
//     ls(argv[i]);
//   exit();
// }
