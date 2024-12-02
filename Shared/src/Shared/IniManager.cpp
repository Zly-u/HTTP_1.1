#include "IniManager.h"

//////////////////////////////////////////////////////////////////////////////////////////

ini::IniFile& IniManager::LoadFile_Impl(const std::string& fileName) {
	ini::IniFile& file = GetFile(fileName);
	file.load(fileName);
	return file;
}

ini::IniFile& IniManager::SafeFile_Impl(const std::string& fileName) {
	ini::IniFile& file = GetFile(fileName);
	file.save(fileName);
	return file;
}

ini::IniFile& IniManager::GetFile_Impl(const std::string& fileName) {
	if(!files.contains(fileName)) {
		files[fileName] = ini::IniFile(fileName);
	}

	return files[fileName];
}
