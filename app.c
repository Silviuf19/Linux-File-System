#include <stdio.h>

#include <stdlib.h>

#include <string.h>

typedef struct Dir {
  char * name; // numele directorului
  struct Dir * parent; // pointer catre parintele directorului(null pentru radacina)
  struct File * head_children_files; // pointer catre primul element de tip File din interiorul directorului
  struct Dir * head_children_dirs; // pointer catre primul element de tip Dir din interiorul directorului
  struct Dir * next; // pointer catre urmatorul element din lista in care se afla directorul
}
Dir; // structura de tip director

typedef struct File {
  char * name; // numele fisierului
  struct Dir * parent; // pointer catre directorul pe
  struct File * next; // pointer catre urmatorul element din lista de fisiere
}
File; // structura de tip fisier

void recursiveDelete(Dir * parent) {

  if (parent -> head_children_dirs == NULL && parent -> head_children_files == NULL) {
    return;
  }

  if (parent -> head_children_dirs == NULL && parent -> head_children_files != NULL) {
    File * temp;
    while (parent -> head_children_files -> next != NULL) {
      temp = parent -> head_children_files -> next;
      free(parent -> head_children_files);
      parent -> head_children_files = temp;
    }

    free(parent -> head_children_files);
    parent -> head_children_files = NULL;
    temp = NULL;
    return;
  }

  while (parent -> head_children_dirs != NULL) {
    if (parent -> head_children_files != NULL) {
      File * temp;
      while (parent -> head_children_files -> next != NULL) {
        temp = parent -> head_children_files -> next;
        free(parent -> head_children_files);
        parent -> head_children_files = temp;
      }

      free(parent -> head_children_files);
      parent -> head_children_files = NULL;
      temp = NULL;
    }
    recursiveDelete(parent -> head_children_dirs);
    Dir * tempd;
    tempd = parent -> head_children_dirs -> next;
    free(parent -> head_children_dirs);
    parent -> head_children_dirs = tempd;
  }
  return;

}

void touch(Dir * parent, char * name) {
  File * current; //avea spatiu alocat
  current = parent -> head_children_files;
  while (current != NULL) {
    if (!strcmp(current -> name, name)) {
      current = NULL;
      printf("File already exists\n");
      return;
    }
    current = current -> next;
  }
  current = NULL;
  Dir * current2 = parent -> head_children_dirs;
  while (current2 != NULL) {
    if (!strcmp(current2 -> name, name)) {
      current2 = NULL;
      printf("File already exists\n");
      return;
    }
    current2 = current2 -> next;
  }
  current2 = NULL;
  File * newFile = (File * ) malloc(sizeof(File));
  if (parent -> head_children_files == NULL) {
    newFile -> name = strdup(name);
    newFile -> parent = parent;
    newFile -> next = NULL;
    parent -> head_children_files = newFile;
    newFile = NULL;
    return;
  }
  File * cap; //avea alocata memorie
  cap = parent -> head_children_files; //retine capul filelor
  while (cap -> next != NULL) {
    cap = cap -> next;
  }
  newFile -> name = strdup(name);
  newFile -> parent = parent;
  newFile -> next = NULL;
  cap -> next = newFile;
  newFile = NULL;
  cap = NULL;
  name = NULL;
}

void mkdir(Dir * parent, char * name) {
  Dir * current;
  current = parent -> head_children_dirs;
  while (current != NULL) {

    if (!strcmp(current -> name, name)) {
      current = NULL;
      printf("Directory already exists\n");
      return;
    }
    current = current -> next;
  }
  current = NULL;
  File * current2 = parent -> head_children_files;
  while (current2 != NULL) {

    if (!strcmp(current2 -> name, name)) {
      current2 = NULL;
      printf("Directory already exists\n");
      return;
    }
    current2 = current2 -> next;
  }
  current2 = NULL;
  Dir * newDir = (Dir * ) malloc(sizeof(Dir));
  if (parent -> head_children_dirs == NULL) {
    newDir -> name = strdup(name);
    newDir -> parent = parent;
    newDir -> next = NULL;
    newDir -> head_children_dirs = NULL;
    newDir -> head_children_files = NULL;
    parent -> head_children_dirs = newDir;
    newDir = NULL;
    return;
  }
  Dir * cap;
  cap = parent -> head_children_dirs; //retine capul directoarelor
  while (cap -> next != NULL) {
    cap = cap -> next;
  }
  newDir -> name = strdup(name);;
  newDir -> parent = parent;
  newDir -> next = NULL;
  newDir -> head_children_dirs = NULL;
  newDir -> head_children_files = NULL;
  cap -> next = newDir;
  newDir = NULL;
  cap = NULL;
  name = NULL;
}

void ls(Dir * parent) {
  if (parent == NULL) {
    return;
  }
  File * tempFile;
  tempFile = parent -> head_children_files;
  Dir * tempDir;
  tempDir = parent -> head_children_dirs;
  if (tempDir == NULL) {
    printf("");
  }
  if (tempFile == NULL) {
    printf("");
  }
  while (tempDir != NULL) {
    printf("%s\n", tempDir -> name);
    tempDir = tempDir -> next;
  }
  while (tempFile != NULL) {
    printf("%s\n", tempFile -> name);
    tempFile = tempFile -> next;
  }
}

void rm(Dir * parent, char * name) {
  File * temp, * current;
  current = parent -> head_children_files;
  int ok = 0;
  while (current != NULL) {
    if (!strcmp(current -> name, name)) {
      ok = 1;
      break;
    } else
      current = current -> next;
  }
  if (!ok) {
    temp = NULL;
    current = NULL;
    printf("Could not find the file\n");
    return;
  }
  if (parent -> head_children_files -> next == NULL) {
    temp = NULL;
    current = NULL;
    free(parent -> head_children_files);
    parent -> head_children_files = NULL;
    return;
  }
  if (!strcmp(parent -> head_children_files -> name, name)) {
    temp = NULL;
    current = NULL;
    free(parent -> head_children_files);
    parent -> head_children_files = parent -> head_children_files -> next;
    return;
  }
  int poz = 0;
  current = parent -> head_children_files;
  while (current -> next != NULL) {
    current = current -> next;
    poz++;
  }
  int i; //cauta penultimul element
  if (!strcmp(current -> name, name)) {
    free(current);
    current = parent -> head_children_files;
    for (i = 0; i < poz - 1; i++) {
      current = current -> next;
    }
    current -> next = NULL;
    temp = NULL;
    current = NULL;
    return;
  }
  current = parent -> head_children_files;
  while (strcmp(current -> next -> name, name)) {
    current = current -> next;
    printf("next\n");
  }

  temp = current -> next -> next;
  free(current -> next);
  current -> next = temp;
  current = NULL;
  temp = NULL;
}

void rmdir(Dir * parent, char * name) {
  Dir * current, * temp;
  current = parent -> head_children_dirs;
  int poz = 0;
  int ok = 0;

  while (current != NULL) {
    if (!strcmp(current -> name, name)) {
      ok = 1;
      break;
    } else
      current = current -> next;
  }

  if (!ok) {
    temp = NULL;
    current = NULL;
    printf("Could not find the dir\n");
    return;
  }

  if (parent -> head_children_dirs -> next == NULL) {
    temp = NULL;
    current = NULL;
    recursiveDelete(parent -> head_children_dirs);
    free(parent -> head_children_dirs);
    parent -> head_children_dirs = NULL;
    return;
  }

  if (!strcmp(parent -> head_children_dirs -> name, name)) {
    temp = NULL;
    current = NULL;
    recursiveDelete(parent -> head_children_dirs);
    free(parent -> head_children_dirs);
    parent -> head_children_files = parent -> head_children_files -> next;
    return;
  }

  current = parent -> head_children_dirs;
  while (current -> next != NULL) {
    current = current -> next;
    poz++;
  }

  int i; //cauta penultimul element
  if (!strcmp(current -> name, name)) {
    recursiveDelete(current);
    free(current);
    current = parent -> head_children_dirs;
    for (i = 0; i < poz - 1; i++) {
      current = current -> next;
    }
    current -> next = NULL;
    temp = NULL;
    current = NULL;
    return;
  }

  current = parent -> head_children_dirs;
  while (strcmp(current -> next -> name, name)) {
    current = current -> next;
  }

  temp = current -> next -> next;
  recursiveDelete(current);
  free(current -> next);
  current -> next = temp;
  current = NULL;
  temp = NULL;

}

void cd(Dir ** target, char * name) {
  Dir * current, * temp;
  current = ( * target);

  temp = current -> head_children_dirs;

  if (!strcmp(name, "..")) {
    if (!strcmp(current -> name, "home")) {
      return;
    } else {

      ( * target) = current -> parent;
      return;
    }
  }

  int ok = 0;
  while (temp != NULL) {
    if (!strcmp(temp -> name, name)) {
      ok = 1;
    }
    temp = temp -> next;
  }

  if (!ok) {
    printf("No directories found!\n");
    temp = NULL;
    return;
  }

  temp = current -> head_children_dirs;

  while (temp != NULL) {
    if (!strcmp(temp -> name, name)) {

      ( * target) = temp;
      temp = NULL;
      return;
    }
    temp = temp -> next;
  }
}

void tree(Dir * target, int level) {

  Dir * current = target -> head_children_dirs;
  File * currentFile = target -> head_children_files;
  int i;

  if (current == NULL) {
    while (currentFile != NULL) {

      for (i = 0; i < level * 4; i++) {
        printf(" ");
      }

      printf("%s\n", currentFile -> name);
      currentFile = currentFile -> next;
    }
    current = NULL;
    currentFile = NULL;
    return;
  }

  while (current != NULL) {

    for (i = 0; i < level * 4; i++) {
      printf(" ");
    }
    printf("%s\n", current -> name);
    tree(current, level + 1);
    current = current -> next;

    if (current == NULL) {
      while (currentFile != NULL) {
        for (i = 0; i < level * 4; i++) {
          printf(" ");
        }
        printf("%s\n", currentFile -> name);
        currentFile = currentFile -> next;
      }
    }
  }
}

char * pwd(Dir * target) {
  Dir * current = target;
  char * temp = (char * ) malloc(300 * sizeof(char));
  char * path = (char * ) malloc(300 * sizeof(char));
  char * slash = (char * ) malloc(sizeof(char));
  char * slash2 = (char * ) malloc(300 * sizeof(char));
  path = "";
  temp = "";
  slash = "/";

  if (!strcmp(current -> name, "home")) {
    path = "/home";
    return path;
  }

  while (strcmp(current -> name, "home")) {
    temp = strdup(current -> name);
    slash2 = strdup(slash);
    strcat(temp, path);
    strcat(slash2, temp);
    path = strdup(slash2);
    current = current -> parent;
  }

  temp = strdup(current -> name);
  slash2 = strdup(slash);
  strcat(temp, path);
  strcat(slash2, temp);
  path = strdup(slash2);
  free(temp);
  temp = NULL;
  current = NULL;
  free(slash2);
  return path;
}

void mv(Dir * parent, char * oldname, char * newname) {

  File * current = parent -> head_children_files;
  int ok1 = 0;
  Dir * tempd = (Dir * ) malloc(sizeof(Dir));
  File * tempf = (File * ) malloc(sizeof(File));
  while (current != NULL) {
    if (!strcmp(current -> name, oldname)) {
      ok1 = 1;

    }
    current = current -> next;
  }

  current = NULL;
  Dir * current2 = parent -> head_children_dirs;
  while (current2 != NULL) {
    if (!strcmp(current2 -> name, oldname)) {

      ok1 = 1;
    }
    current2 = current2 -> next;
  }
  current2 = NULL;

  if (!ok1) {
    printf("File/Director not found\n");
    return;
  }

  int ok2 = 0;
  current = parent -> head_children_files;
  while (current != NULL) {
    if (!strcmp(current -> name, newname)) {

      ok2 = 1;
    }
    current = current -> next;
  }

  current2 = parent -> head_children_dirs;
  while (current2 != NULL) {
    if (!strcmp(current2 -> name, newname)) {
      current2 = NULL;
      ok2 = 1;
    }
    current2 = current2 -> next;
  }

  if (ok2) {
    printf("File/Director already exists\n");
    return;
  }

  current = parent -> head_children_files;
  if ((current -> next == NULL) && (!strcmp(current -> name, oldname))) {
    current -> name = strdup(newname);
    return;
  }

  current2 = parent -> head_children_dirs;
  if ((current2 -> next == NULL) && (!strcmp(current2 -> name, oldname))) {
    current2 -> name = strdup(newname);
    return;
  }

  current = parent -> head_children_files;
  while (current != NULL) {
    if ((current -> next == NULL) && (!strcmp(current -> name, oldname))) {
      current -> name = strdup(newname);
      return;
    }
    current = current -> next;
  }

  current2 = parent -> head_children_dirs;
  while (current2 != NULL) {
    if ((current2 -> next == NULL) && (!strcmp(current2 -> name, oldname))) {
      current2 -> name = strdup(newname);
      return;
    }
    current2 = current2 -> next;
  }

  current = parent -> head_children_files;
  while (current != NULL) {
    if (!strcmp(current -> name, oldname)) {
      current -> name = strdup(newname);
      * tempf = * current;
      rm(parent, current -> name);
    }
    current = current -> next;
  }

  current = parent -> head_children_files;
  while (current -> next != NULL) {
    current = current -> next;
  }
  current -> next = tempf;
  current -> next -> parent = current -> parent;
  current -> next -> next = NULL;

  current2 = parent -> head_children_dirs;
  while (current2 != NULL) {
    if (!strcmp(current2 -> name, oldname)) {
      current2 -> name = strdup(newname);
      * tempd = * current2;
      rmdir(parent, current2 -> name);
    }
    current2 = current2 -> next;
  }

  current2 = parent -> head_children_dirs;
  while (current2 -> next != NULL) {
    current2 = current2 -> next;
  }
  current2 -> next = tempd;
  current2 -> next -> parent = current2 -> parent;
  current -> next -> next = NULL;
}

void stop(Dir * target) {
  recursiveDelete(target);
  free(target);
}

int main() {
  Dir * current;
  Dir * home = (Dir * ) malloc(sizeof(Dir));
  home -> name = "home";
  home -> head_children_files = NULL;
  home -> head_children_dirs = NULL;
  current = home;
  char * line = (char * ) malloc(300 * sizeof(char));
  char * token = (char * ) malloc(300 * sizeof(char));
  int k = 20;
  int ok = 0;

  while (!ok) {
    gets(line);
    token = strtok(line, " ");

    if (!strcmp(token, "touch")) {
      token = strtok(NULL, " ");
      touch(current, token);
    }

    if (!strcmp(token, "ls")) {
      ls(current);
    }

    if (!strcmp(token, "stop")) {
      stop(home);
      ok = 1;
    }

    if (!strcmp(token, "mkdir")) {
      token = strtok(NULL, " ");
      mkdir(current, token);
    }

    if (!strcmp(token, "rm")) {
      token = strtok(NULL, " ");
      rm(current, token);
    }

    if (!strcmp(token, "rmdir")) {
      token = strtok(NULL, " ");
      rmdir(current, token);
    }

    if (!strcmp(token, "cd")) {
      token = strtok(NULL, " ");
      Dir ** pointer;
      pointer = & current;
      cd(pointer, token);
    }

    if (!strcmp(token, "pwd")) {
      printf("%s\n", pwd(current));
    }

    if (!strcmp(token, "tree")) {
      tree(current, 0);
    }

    if (!strcmp(token, "mv")) {
      token = strtok(NULL, " ");
      char * arg1 = token;
      token = strtok(NULL, " ");
      char * arg2 = token;
      mv(current, arg1, arg2);
    }
  }
  return 0;
}