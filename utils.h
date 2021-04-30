// Author Young-hoon Ji

#include <iostream>
#include <string>
#include <vector>
#include <io.h>

bool IsPathExist(const std::string& s)
{
	struct stat buffer;
	return (stat(s.c_str(), &buffer) == 0);
}

std::vector<std::string> get_files_inDirectory(const std::string& _path, const std::string& _filter) {

	std::string searching_dir = _path + "*." + _filter;

	std::vector<std::string> return_;
	int cnt = 0;

	struct _finddata_t fd;
	intptr_t handle;
	if ((handle = _findfirst(searching_dir.c_str(), &fd)) == -1L)

		std::cout << "No file in directory!" << std::endl;

	do {
		//std::cout << fd.name << std::endl;
		return_.push_back(fd.name);
		cnt += 1;
	} while (_findnext(handle, &fd) == 0);

	_findclose(handle);

	return return_;
}