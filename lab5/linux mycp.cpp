//2205112E39李萍
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>

void copyFile(const char* sourceFile, const char* destFile) {
    int sourceFd = open(sourceFile, O_RDONLY);
    if (sourceFd == -1) {
        std::cerr << "无法打开源文件：" << sourceFile << std::endl;
        return;
    }

    int destFd = open(destFile, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
    if (destFd == -1) {
        std::cerr << "无法创建目标文件：" << destFile << std::endl;
        close(sourceFd);
        return;
    }

    const int bufferSize = 4096;
    char buffer[bufferSize];
    ssize_t bytesRead, bytesWritten;

    while ((bytesRead = read(sourceFd, buffer, bufferSize)) > 0) {
        bytesWritten = write(destFd, buffer, bytesRead);
        if (bytesWritten == -1) {
            std::cerr << "写入目标文件出错：" << destFile << std::endl;
            break;
        }
    }

    if (bytesRead == -1) {
        std::cerr << "读取源文件出错：" << sourceFile << std::endl;
    }

    close(sourceFd);
    close(destFd);

    std::cout << "文件复制完成：" << sourceFile << " -> " << destFile << std::endl;
}

void mergeFile(const char* sourceFile, const char* destFile) {
    int sourceFd = open(sourceFile, O_RDONLY);
    if (sourceFd == -1) {
        std::cerr << "无法打开源文件：" << sourceFile << std::endl;
        return;
    }

    int destFd = open(destFile, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
    if (destFd == -1) {
        std::cerr << "无法打开目标文件：" << destFile << std::endl;
        close(sourceFd);
        return;
    }

    const int bufferSize = 4096;
    char buffer[bufferSize];
    ssize_t bytesRead, bytesWritten;

    while ((bytesRead = read(sourceFd, buffer, bufferSize)) > 0) {
        bytesWritten = write(destFd, buffer, bytesRead);
        if (bytesWritten == -1) {
            std::cerr << "写入目标文件出错：" << destFile << std::endl;
            break;
        }
    }

    if (bytesRead == -1) {
        std::cerr << "读取源文件出错：" << sourceFile << std::endl;
    }

    close(sourceFd);
    close(destFd);

    std::cout << "文件合并完成：" << sourceFile << " -> " << destFile << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "用法: " << argv[0] << " <源文件> <目标文件/目录>" << std::endl;
        return 1;
    }

    const char* sourceFile = argv[1];
    const char* destFile = argv[2];

    struct stat sourceStat, destStat;
    

    // 获取源文件信息
    if (stat(sourceFile, &sourceStat) == -1) {
        std::cerr << "源文件不存在，无法获取源文件信息：" << sourceFile << std::endl;
        return 1;
    }

    // 获取目标文件/目录信息
    if (stat(destFile, &destStat) == 0) {
        // 目标文件存在
        if (S_ISDIR(destStat.st_mode)) {
            // 目标是一个目录
            std::string destFilePath(destFile);
            std::string destFileName = destFilePath + "/" + std::string(sourceFile);
            if (S_ISDIR(sourceStat.st_mode)) {
                // 源文件是目录
                std::cerr << "源文件是目录，无法复制目录到目录，请选择正确的操作方式。" << std::endl;
                return 1;
            } else {
                // 源文件是文件
                mergeFile(sourceFile, destFileName.c_str());
            }
        } else {
            // 目标是一个文件
            std::string prompt;
            if (S_ISDIR(sourceStat.st_mode)) {
                // 源文件是目录
                std::cerr << "源文件是目录，无法复制目录到文件，请选择正确的操作方式。" << std::endl;
                return 1;
            } else {
                // 源文件是文件
                std::cout << "目标文件已存在，是否覆盖？(y/n): ";
                std::cin >> prompt;
                if (prompt == "y" || prompt == "Y") {
                    copyFile(sourceFile, destFile);
                } else if (prompt == "n" || prompt == "N") {
                    mergeFile(sourceFile, destFile);
                } else {
                    std::cerr << "无效的选择。" << std::endl;
                    return 1;
                }
            }
        }
    } else {
        // 目标文件/目录不存在
        if (S_ISDIR(sourceStat.st_mode)) {
            // 源文件是目录
            std::cerr << "目录不存在，无法复制目录到不存在的目录。" << std::endl;
            return 1;
        } else {
            // 源文件是文件
            copyFile(sourceFile, destFile);
        }
    }

    return 0;
}



/*g++ -o mycp mycp.cpp 
./mycp source.txt destination.txt 
./mycp source.txt destination_directory/ 

*/