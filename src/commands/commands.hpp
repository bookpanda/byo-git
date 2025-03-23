#ifndef COMMANDS_H
#define COMMANDS_H

void handleInitGit();
void handleCatFile(int argc, char *argv[]);
void handleHashObject(int argc, char *argv[]);
void handleLsTree(int argc, char *argv[]);
void handleWriteTree();

#endif // COMMANDS_H