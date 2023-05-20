//2205112E39李萍

#include<iostream>
#include<dirent.h>
#include<sys/stat.h>
#include<string.h>//memcmp()
#include<grp.h>//getgrgid
#include<unistd.h>//getcwd
#include<pwd.h>//getpwuid
#include<cstdio>
using namespace std;

int ls(char *dirpath);//ls命令
int ls_l(char *dirpath);//ls -l命令

char filename[100][255];
int filenum = 0;

int main(int args,char* argv[]) {
	char dir[255];
	getcwd(dir,255);//获取当前工作环境绝对路径,存入dir中
	if( args > 1){//ls -l命令
		ls_l(dir);
		//cout<<"000";
	}else{//ls命令
		ls(dir);
	}
	return 0;
}
int ls(char *dirpath){
	DIR *dir;//用于存放opendir()成功时的返回值
	struct dirent *file;//用于存放readir()的返回值
	/*函数DIR *opendir(const char *pathname)，用于打开pathname，
	若成功将返回值DIR类型的值，若失败，返回空指针*/
	struct stat info;//用于存放文件信息
	if(!(dir = opendir(dirpath))){
		//打开目录失败
		//printf("error opendir %s\n",dirpath);
		cout<<"fail to read"<<dirpath;
		return -1;
	}
	//读取opendir的返回值，其中d_name是文件名
	while((file = readdir(dir))!= NULL){
		//文件无文件名只有后缀
		if(strncmp(file->d_name, ".", 1) == 0)
			continue;
		//获取文件信息，存储在info中
		stat(file->d_name,&info);
		//打印文件名
		cout<<file->d_name<<endl;
	}
	cout<<endl;
	closedir(dir);
	return 1;
}
void power(int mode, char str[]){//权限解析
    strcpy(str, "----------");
    if (S_ISDIR(mode)) str[0] = 'd';
    if (S_ISCHR(mode)) str[0] = 'c';
    if (S_ISBLK(mode)) str[0] = 'b';
    if ((mode & S_IRUSR)) str[1] = 'r';
    if ((mode & S_IWUSR)) str[2] = 'w';
    if ((mode & S_IXUSR)) str[3] = 'x';
    if ((mode & S_IRGRP)) str[4] = 'r';
    if ((mode & S_IWGRP)) str[5] = 'w';
    if ((mode & S_IXGRP)) str[6] = 'x';
    if ((mode & S_IROTH)) str[7] = 'r';
    if ((mode & S_IWOTH)) str[8] = 'w';
    if ((mode & S_IXOTH)) str[9] = 'x';
}
int ls_l(char *dirpath){
	DIR *dir;//用于存放opendir()成功时的返回值
	struct dirent *file;//用于存放readir()的返回值
	struct stat info;//目录项描述结构体
	if(!(dir = opendir(dirpath))){
		//打开目录失败
		cout<<"fail to read"<<dirpath;
		return -1;
	}
	struct passwd *userinfo;
	struct group *groupinfo;
	while((file = readdir(dir))!= NULL){
		//文件无文件名只有后缀
		if(strncmp(file->d_name, ".", 1) == 0)
			continue;
		strcpy(filename[filenum++],file->d_name);
		//获取文件信息，存储在info中
		stat(file->d_name,&info); 
		userinfo = getpwuid(info.st_uid);//所有者用户名 
		groupinfo = getgrgid(info.st_gid);//所有者所在组用户名
		char pw[11];
		power(info.st_mode,pw);//文件权限信息
		cout<<pw<<" " <<info.st_nlink<<" "<< userinfo->pw_name<<" "<<groupinfo->gr_name<<" "<<info.st_size<<" ";
		printf(" %.12s", 4 + ctime(&info.st_mtime));
		cout<<" "<<file->d_name;
		cout<<endl;
	}
	closedir(dir);
	return 1;
}
