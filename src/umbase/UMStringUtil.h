/**
 * @file UMStringUtil.h
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <codecvt>
#include <locale>
#include <algorithm>
#include "UMMacro.h"
#include <wchar.h>

namespace umbase
{

/**
 * string utility
 */
class UMStringUtil
{
	DISALLOW_COPY_AND_ASSIGN(UMStringUtil);

public:
	
	template <typename T>
	static std::wstring number_to_wstring(T value)
	{
		std::wstringstream converter;
		std::wstring  wstr;
		converter << value;
		converter >> wstr;
		return wstr;
	}
	
	template <typename T>
	static std::string number_to_string(T value)
	{
		std::stringstream converter;
		std::string  str;
		converter << value;
		converter >> str;
		return str;
	}
	
	template <typename T>
	static std::string number_to_sequence_string(T value, int n)
	{
		std::stringstream converter;
		std::string  str;
		converter << std::setw(n) << std::setfill('0') << value;
		converter >> str;
		return str;
	}
	
	/**
	 * convert wstring to utf16
	 */
	static umstring wstring_to_utf16(const std::wstring& str)
	{
#if defined _WIN32 && !defined (WITH_EMSCRIPTEN)
		const char16_t* p = reinterpret_cast<const char16_t*>(str.c_str());
		umstring u16str(p);
#else
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>,wchar_t> convert;
        umstring u16str = convert.to_bytes(str);
#endif
		return u16str;
	}
	
	/**
	 * convert utf16 to wstring
	 */
	static std::wstring utf16_to_wstring(const umstring& utf16str)
	{
#if defined _WIN32 && !defined (WITH_EMSCRIPTEN)
		const wchar_t* p = reinterpret_cast<const wchar_t*>(utf16str.c_str());
		std::wstring wstr(p);
#else
		// not implemented
		std::wstring wstr;
#endif
		return wstr;
	}
	
	/**
	 * convert wstring to utf8
	 */
	static std::string wstring_to_utf8(const std::wstring& str)
	{
#if defined _WIN32 && !defined (WITH_EMSCRIPTEN)
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
        std::string utf8str = convert.to_bytes(str);
#else
		// not implemented
		umstring utf8str;
#endif
		return utf8str;
	}

	/**
	 * convert utf8 string to utf16
	 */
	static umstring utf8_to_utf16(const std::string& utf8str)
	{
#if defined _WIN32 && !defined (WITH_EMSCRIPTEN)
		std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
		umstring utf16str = convert.from_bytes(utf8str);
#else
		// not implemented
		umstring utf16str = utf8str;
#endif
		return utf16str;
	}

	/**
	 * convert utf16 to utf8 string
	 */
	static std::string utf16_to_utf8(const umstring& str)
	{
#if defined _WIN32 && !defined (WITH_EMSCRIPTEN)
		std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
		std::string stdstr = convert.to_bytes(str);
#else
		// not implemented
		std::string stdstr = str;
#endif
		return stdstr;
	}

#if !defined (WITH_EMSCRIPTEN)
		/**
		 * convert utf8 to utf32 string
		 */
		static std::u32string utf8_to_utf32(const std::string& str)
		{
			std::wstring_convert<std::codecvt_utf8<char32_t>,char32_t> convert;
			std::u32string u32str = convert.from_bytes(str);
			return u32str;
		}
#endif //  !defined (WITH_EMSCRIPTEN)

	static bool insensitive_compare(const std::string& a, const std::string& b) {
		std::string str1(a);
		std::string str2(b);
		std::transform(str1.begin(), str1.end(), str1.begin(), ::tolower);
		std::transform(str2.begin(), str2.end(), str2.begin(), ::tolower);
		return (str1 == str2);
	}

};

} // umbase
