#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <pwd.h>
#include <grp.h>
/* -------------------------------------------------------------------------------- */

static void print_type(struct stat *sb);
static void print_ino(const struct stat *sb);
static void print_perms(const struct stat *sb);
static void print_linkc(const struct stat *sb);
static void print_owner(const struct stat *sb);
static void print_size(const struct stat *sb);
static void print_laststch(const struct stat *sb);
static void print_lastacc(const struct stat *sb);
static void print_lastmod(const struct stat *sb);
static void print_name(const struct stat *sb, char *name);
/* -------------------------------------------------------------------------------- */

int main(int argc, char *argv[])
{
  struct stat sb;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  if (stat(argv[1], &sb) == -1) {
    perror("stat");
    exit(EXIT_SUCCESS);
  }

  print_type(&sb);
  print_name(&sb, argv[1]);
  print_ino(&sb);
  print_perms(&sb);
  print_linkc(&sb);
  print_owner(&sb);
  print_size(&sb);
  print_laststch(&sb);
  print_lastacc(&sb);
  print_lastmod(&sb);

  exit(EXIT_SUCCESS);
}
/* -------------------------------------------------------------------------------- */

static void print_type(struct stat *sb){
  printf("File type:                ");
  switch (sb->st_mode & S_IFMT) {
    case S_IFBLK:  printf("block device\n");            break;
    case S_IFCHR:  printf("character device\n");        break;
    case S_IFDIR:  printf("directory\n");               break;
    case S_IFIFO:  printf("FIFO/pipe\n");               break;
    case S_IFLNK:  printf("symlink\n");                 break;
    case S_IFREG:  printf("regular file\n");            break;
    case S_IFSOCK: printf("socket\n");                  break;
    default:       printf("unknown?\n");                break;
  }
}
/* -------------------------------------------------------------------------------- */

static void print_ino(const struct stat *sb){
  printf("I-node number:            %ld\n", (long) sb->st_ino);
}
/* -------------------------------------------------------------------------------- */

static void print_perms(const struct stat *sb){
  printf("Mode:                     %o (octal)\n", sb->st_mode & ( S_IRWXU | S_IRWXG | S_IRWXO));
}
/* -------------------------------------------------------------------------------- */

static void print_linkc(const struct stat *sb){
  printf("Link count:               %ld\n", (long) sb->st_nlink);
}
/* -------------------------------------------------------------------------------- */

static void print_owner(const struct stat *sb){
  register struct passwd *pw;
  register struct group *grp;

  pw = getpwuid(sb->st_uid);
  grp = getgrgid(sb->st_gid);

  printf("Ownership:                user: %s(%ld)   group: %s(%ld)\n", pw->pw_name,(long)sb->st_uid, grp->gr_name,(long)sb->st_gid);
}
/* -------------------------------------------------------------------------------- */

static void print_size(const struct stat *sb){
  printf("Preferred I/O block size: %ld kB\n", (long) sb->st_blksize/1024);
  printf("File size:                %lld kB\n",(long long) sb->st_size/1024);
  printf("Blocks allocated:         %lld\n",(long long) sb->st_blocks);
}
/* -------------------------------------------------------------------------------- */

static void print_laststch(const struct stat *sb){
  printf("Last status change:       %s", ctime(&sb->st_ctime));
}
/* -------------------------------------------------------------------------------- */

static void print_lastacc(const struct stat *sb){
  printf("Last file access:         %s", ctime(&sb->st_atime));
}
/* -------------------------------------------------------------------------------- */

static void print_lastmod(const struct stat *sb){
  printf("Last file modification:   %s", ctime(&sb->st_mtime));
}
/* -------------------------------------------------------------------------------- */

static void print_name(const struct stat *sb, char *name){
  char* bname = basename(name);
  printf("Name of the file:         %s\n", bname);
}
/* -------------------------------------------------------------------------------- */
