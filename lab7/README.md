Process Control

Write a program that combines the functionalities of the file copy program (`mycp`) from Experiment 5 and the directory traversal program (`myls`) from Experiment 6. The program should achieve the following:

1. Modify the `myls` program to act as the parent process. While traversing the directory, for each non-directory file encountered, the parent process should create a child process to execute the `mycp` program.

2. The source file path for each child process (`mycp`) should be the file path obtained by the parent process during the directory traversal (passed as a command-line argument to the child process). The child process should then copy the source file to a specified directory (create a directory under `/home` with your name written in Chinese Pinyin).

3. The parent process should wait for each child process (`mycp`) to finish its execution, reclaiming its kernel space resources, before continuing with the directory traversal until completion.