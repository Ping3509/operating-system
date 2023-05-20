//2205112E39李萍
#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <cstring>

void mycp(const std::string& srcPath, const std::string& destDir) {
    pid_t pid = fork();

    if (pid == -1) {
        std::cerr << "无法创建子进程。" << std::endl;
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // 子进程
        execlp("./mycp", "mycp", srcPath.c_str(), destDir.c_str(), NULL);
        std::cerr << "无法执行 mycp 程序。" << std::endl;
        exit(EXIT_FAILURE);
    } else {
        // 父进程
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS) {
            std::cout << "成功拷贝文件：" << srcPath << std::endl;
        } else {
            std::cerr << "无法拷贝文件：" << srcPath << std::endl;
        }
    }
}

void myls(const std::string& dirPath, const std::string& destDir) {
    DIR* dir = opendir(dirPath.c_str());
    if (!dir) {
        std::cerr << "无法打开目录：" << dirPath << std::endl;
        exit(EXIT_FAILURE);
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        std::string filename = entry->d_name;
        if (filename == "." || filename == "..") {
            continue;
        }

        std::string fullPath = dirPath + "/" + filename;
        struct stat fileStat;
        if (stat(fullPath.c_str(), &fileStat) == -1) {
            std::cerr << "无法获取文件信息：" << fullPath << std::endl;
            continue;
        }

        if (S_ISDIR(fileStat.st_mode)) {
            // 目录
            std::cout << "目录：" << fullPath << std::endl;
            myls(fullPath, destDir);
        } else {
            // 文件
            std::cout << "文件：" << fullPath << std::endl;
            mycp(fullPath, destDir);
        }
    }

    closedir(dir);
}

int main() {
    std::string sourceDir =  "/home/lpxx/实验/src/testfile";
    std::string destDir = "/home/lpxx/实验/liping“;

    pid_t pid = fork();

    if (pid == -1) {
        std::cerr << "无法创建子进程。" << std::endl;
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // 子进程
        myls(sourceDir, destDir);
        exit(EXIT_SUCCESS);
    } else {
        // 父进程
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS) {
           std::cout << "所有文件拷贝完成。" << std::endl;
    } else {
        std::cerr << "无法完成文件拷贝。" << std::endl;
    }

    return 0;
    }
}
