#include "IniManager.h"

//////////////////////////////////////////////////////////////////////////////////////////

ini::IniFile& IniManager::LoadFile_Impl(const std::string& file_name){
	ini::IniFile& file = GetFile(file_name);
	return file;
}

ini::IniFile& IniManager::SafeFile_Impl(const std::string& file_name) {
	bool was_saved_already = false;
	ini::IniFile& file = GetFile(file_name, was_saved_already);
	if (!was_saved_already) {
		file.save(file_name);
	}
	return file;
}

ini::IniFile& IniManager::GetFile_Impl(const std::string& file_name, bool& file_got_created) {
	if(!m_files.contains(file_name)) {
		m_files[file_name] = ini::IniFile();
		if(!m_files[file_name].load(file_name)) {
			m_files[file_name].save(file_name);
			file_got_created = true;
		}
	}

	return m_files[file_name];
}
