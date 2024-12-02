﻿#pragma once

#include "library.h"

#include <codecvt>
#include <locale>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "helpers.h"
#include "Shared/inicpp.h"

// the conversion functor must live in the "ini" namespace
namespace ini
{
    /** Conversion functor to parse std::vectors from an ini field-
     * The generic template can be passed down to the vector. */
    template<typename T>
    struct Convert<std::vector<T>>
    {
        /** Decodes a std::vector from a string. */
        void decode(const std::string &value, std::vector<T> &result)
        {
            result.clear();

            // variable to store the decoded value of each element
            T decoded;
            // maintain a start and end pos within the string
            size_t startPos = 0;
            size_t endPos = 0;
            size_t cnt;

            while(endPos != std::string::npos)
            {
                if(endPos != 0)
                    startPos = endPos + 1;
                // search for the next comma as separator
                endPos = value.find(',', startPos);

                // if no comma was found use the rest of the string
                // as input
                if(endPos == std::string::npos)
                    cnt = value.size() - startPos;
                else
                    cnt = endPos - startPos;

                std::string tmp = value.substr(startPos, cnt);
            	if(tmp.empty()){continue;}

                // use the conversion functor for the type contained in
                // the vector, so the vector can use any type that
                // is compatible with inifile-cpp
                Convert<T> conv;
                conv.decode(tmp, decoded);
            	result.push_back(decoded);
            }
        }

        /** Encodes a std::vector to a string. */
        void encode(const std::vector<T> &value, std::string &result)
        {
            // variable to store the encoded element value
            std::string encoded;
            // string stream to build the result stream
            std::stringstream ss;
            for(size_t i = 0; i < value.size(); ++i)
            {
                // use the conversion functor for the type contained in
                // the vector, so the vector can use any type that
                // is compatible with inifile-cp
                Convert<T> conv;
                conv.encode(value[i], encoded);
                ss << encoded;

                // if this is not the last element add a comma as separator
                if(i != value.size() - 1)
                    ss << ',';
            }
            // store the created string in the result
            result = ss.str();
        }
    };

	template<>
	struct Convert<std::wstring> {
		void decode(const std::string& value, std::wstring& result) {
			const char* test = value.c_str();
			result = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(test);
		}

		void encode(const std::wstring& value, std::string& result) {
			result = std::string(value.begin(), value.end());
		}
	};
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class SHARED_API IniManager : public Singleton<IniManager>{
public:
	static ini::IniFile& LoadFile(const std::string& fileName) { return Get().LoadFile_Impl(fileName); }
	static ini::IniFile& SaveFile(const std::string& fileName) { return Get().SafeFile_Impl(fileName); }
	static ini::IniFile& GetFile(const std::string& fileName)  { return Get().GetFile_Impl(fileName); }


protected:
	ini::IniFile& LoadFile_Impl(const std::string& fileName);
	ini::IniFile& SafeFile_Impl(const std::string& fileName);
	ini::IniFile& GetFile_Impl(const std::string& fileName);


private:
	std::unordered_map<std::string, ini::IniFile> files;
};
