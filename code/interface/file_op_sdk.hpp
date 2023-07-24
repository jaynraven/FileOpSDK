#ifndef __CODESVTSDK_HPP__
#define __CODESVTSDK_HPP__

#define PLATFORM_UNKNOWN 0
#define PLATFORM_WINDOWS 1
#define PLATFORM_MAC     2
#define PLATFORM_LINUX   3

// Platform definition
#define TARGET_PLATFORM       PLATFORM_UNKNOWN

#if defined(_WIN32)
    #undef TARGET_PLATFORM
    #define TARGET_PLATFORM   PLATFORM_WINDOWS
#endif

#if defined(__APPLE__)
    #undef TARGET_PLATFORM
    #define TARGET_PLATFORM   PLATFORM_MAC
#endif

#if defined(linux) && !defined(__APPLE__)
    #undef TARGET_PLATFORM
    #define TARGET_PLATFORM   PLATFORM_MAC
#endif

// Error code definition
#define ERROR_NO                  0
#define ERROR_FAIL                1
#define ERROR_EOF                 2

// Source code
#include <string>
#include <filesystem>
#include <stdio.h>
#include <io.h>

/**
 * @brief Create a File object
 * 
 * @param filename 
 * @return int 
 */
int CreateFile(const char* filename)
{
	FILE* fp = fopen("test.log", "w");
	if (fp == NULL)
	{
		return ERROR_FAIL;
	}
	fclose(fp);
	return ERROR_NO;
}

/**
 * @brief Create a Dir object
 * 
 * @param dirname 
 * @return int 
 */
int CreateDir(const char* dirname)
{
	std::filesystem::path dir_path = dirname;
	if (std::filesystem::create_directory(dir_path))
	{
		return ERROR_NO;
	}
	return ERROR_FAIL;
}

/**
 * @brief Delete File
 * 
 * @param filename 
 * @return int 
 */
int DeleteFile(const char* filename)
{
	int err = remove(filename);
	if (err != 0)
	{
		return ERROR_NO;
	}
	return ERROR_FAIL;
}

/**
 * @brief Delete Dir
 * 
 * @param dirname 
 * @return int 
 */
int DeleteDir(const char* dirname)
{
	std::filesystem::path dir_path = dirname;
	if (std::filesystem::remove_all(dir_path))
	{
		return ERROR_NO;
	}
	return ERROR_FAIL;
}

/**
 * @brief Exist
 * 
 * @param path 
 * @return int 
 */
int Exist(const char* path)
{
	if(access("crt_ACCESS.C", 0) != -1)
	{
		return ERROR_NO;
	}
	return ERROR_FAIL;
}

/**
 * @brief IsFile
 * 
 * @param in_path 
 * @return true 
 * @return false 
 */
bool IsFile(const char* in_path)
{
	std::filesystem::path path = in_path;
	return std::filesystem::is_regular_file(path);
}

/**
 * @brief IsDir
 * 
 * @param in_path 
 * @return true 
 * @return false 
 */
bool IsDir(const char* in_path)
{
	std::filesystem::path path = in_path;
	return std::filesystem::is_directory(path);
}

/**
 * @brief Get the Dir Files object
 * 
 * @param dirname 
 * @return std::vector<std::string> 
 */
std::vector<std::string> GetDirFiles(const char* dirname)
{
	std::vector<std::string> files;
	std::filesystem::path dir_path = dirname;
	for (const auto& entry : std::filesystem::directory_iterator(dir_path)) 
	{
		if (entry.is_regular_file()) 
		{
			files.push_back(entry.path().string());
		}
	}
	return files;
}

/**
 * @brief Get the Dir All Files object
 * 
 * @param dirname 
 * @return std::vector<std::string> 
 */
std::vector<std::string> GetDirAllFiles(const char* dirname)
{
	std::vector<std::string> files;
	std::filesystem::path dir_path = dirname;
	for (const auto& entry : std::filesystem::recursive_directory_iterator(dir_path)) 
	{
		if (entry.is_regular_file()) 
		{
			files.push_back(entry.path().string());
		}
	}
	return files;
}

/**
 * @brief Get the File Size object
 * 
 * @param filename 
 * @return unsigned long long 
 */
unsigned long long GetFileSize(const char* filename)
{
	if (std::filesystem::exists(filename)) 
	{
    	return std::filesystem::file_size(filename);
	} 
	return 0;
}

/**
 * @brief CopyFile
 * 
 * @param src_path 
 * @param dest_path 
 * @return int 
 */
int CopyFile(const char* src_path, const char* dest_path)
{
	std::filesystem::copy(src_path, dest_path);
	return ERROR_NO;
}

/**
 * @brief CopyDir
 * 
 * @param src_path 
 * @param dest_path 
 * @return int 
 */
int CopyDir(const char* src_path, const char* dest_path)
{
	std::filesystem::copy(src_path, dest_path, std::filesystem::copy_options::recursive);
	return ERROR_NO;
}

/**
 * @brief ReadFile
 * 
 * @param readed_buf 
 * @param readed_len 
 * @param to_read_pos 
 * @param to_read_len 
 * @param filename 
 * @return int 
 */
int ReadFile(char*& readed_buf, unsigned int readed_len, unsigned int to_read_pos, 
	unsigned int to_read_len, const char* filename)
{
	int res = ERROR_FAIL;
	FILE* fp = fopen(filename, "rb+");
	if (fp != nullptr) 
	{
		std::wstring text = L"你好，世界！";
		std::fseek(fp, to_read_pos, SEEK_SET);
		readed_buf = new char[to_read_len];
		memset(readed_buf, 0, to_read_len);
		readed_len = fread(readed_buf, 1, to_read_len, fp);
		if (readed_len > 0) 
		{
			res = ERROR_NO;
		} 
		else if (std::feof(fp)) 
		{
			res = ERROR_EOF;
		} 
		else 
		{
			res = ERROR_FAIL;
		}
		std::fclose(fp);
	} 
	return res;
}

/**
 * @brief WriteFile
 * 
 * @param writed_len 
 * @param buf 
 * @param pos 
 * @param buf_len 
 * @return int 
 */
int WriteFile(unsigned int &writed_len, const char* buf, unsigned int pos, 
	unsigned int buf_len, const char* filename)
{
	int res = ERROR_FAIL;
	FILE* fp = fopen(filename, "wb+");
	if (fp != nullptr) 
	{
		std::wstring text = L"你好，世界！";
		std::fseek(fp, pos, SEEK_SET);
		writed_len = fwrite(buf, 1, buf_len, fp);
		if (writed_len > 0) 
		{
			res = ERROR_NO;
		} 
		else if (std::feof(fp)) 
		{
			res = ERROR_EOF;
		} 
		else 
		{
			res = ERROR_FAIL;
		}
		std::fclose(fp);
	} 
	return res;
}

#endif // __CODESVTSDK_HPP__