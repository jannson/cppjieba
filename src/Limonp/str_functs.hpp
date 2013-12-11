/************************************
 * file enc : ascii
 * author   : wuyanyi09@gmail.com
 ************************************/
#ifndef LIMONP_STR_FUNCTS_H
#define LIMONP_STR_FUNCTS_H
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <map>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <memory.h>
#include <functional> 
#include <locale>
#include <sstream>
#include <sys/types.h>
#include <iterator>
#include <algorithm>
#include "std_outbound.hpp"
#include "map_functs.hpp"

#define print(x) cout<<(x)<<endl

namespace Limonp
{
    using namespace std;
    inline string string_format(const char* fmt, ...) 
    {
        int size = 256;
        std::string str;
        va_list ap;
        while (1) {
            str.resize(size);
            va_start(ap, fmt);
            int n = vsnprintf((char *)str.c_str(), size, fmt, ap);
            va_end(ap);
            if (n > -1 && n < size) {
                str.resize(n);
                return str;
            }
            if (n > -1)
              size = n + 1;
            else
              size *= 2;
        }
        return str;
    }

    inline void string_format(string& res, const char* fmt, ...)
    {
        int size = 256;
        va_list ap;
        res.clear();
        while (1) {
            res.resize(size);
            va_start(ap, fmt);
            int n = vsnprintf((char *)res.c_str(), size, fmt, ap);
            va_end(ap);
            if (n > -1 && n < size) {
                res.resize(n);
                return;
            }
            if (n > -1)
              size = n + 1;
            else
              size *= 2;
        }
    }

    //inline bool joinStr(const vector<string>& src, string& dest, const string& connectorStr)
    //{
    //    if(src.empty())
    //    {
    //        return false;
    //    }
    //    for(uint i = 0; i < src.size() - 1; i++)
    //    {
    //        dest += src[i];
    //        dest += connectorStr;
    //    }
    //    dest += src[src.size() - 1];
    //    return true;
    //}

    //inline string joinStr(const vector<string>& source, const string& connector)
    //{
    //    string res;
    //    joinStr(source, res, connector);
    //    return res;
    //}

    template<class T>
        void join(T begin, T end, string& res, const string& connector)
        {
            if(begin == end)
            {
                return;
            }
            stringstream ss;
            ss<<*begin;
            begin++;
            while(begin != end)
            {
                ss << connector << *begin;
                begin ++;
            }
            res = ss.str();
        }

    template<class T>
        string join(T begin, T end, const string& connector)
        {
            string res;
            join(begin ,end, res, connector);
            return res;
        }



    inline bool splitStr(const string& src, vector<string>& res, const string& pattern)
    {
        if(src.empty())
        {
            return false;
        }
        res.clear();

        size_t start = 0;
        size_t end = 0;
        while(start < src.size())
        {
            end = src.find_first_of(pattern, start);
            if(string::npos == end)
            {
                res.push_back(src.substr(start));
                return true;
            }
            res.push_back(src.substr(start, end - start));
            if(end == src.size() - 1)
            {
                res.push_back("");
                break;
            }
            start = end + 1;
        }
        return true;
    }

    inline string& upper(string& str)
    {
        transform(str.begin(), str.end(), str.begin(), (int (*)(int))toupper);
        return str;
    }

    inline string& lower(string& str)
    {
        transform(str.begin(), str.end(), str.begin(), (int (*)(int))tolower);
        return str;
    }

    inline std::string &ltrim(std::string &s) 
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
    }

    inline std::string &rtrim(std::string &s) 
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
    }

    inline std::string &trim(std::string &s) 
    {
        return ltrim(rtrim(s));
    }


    inline uint16_t twocharToUint16(char high, char low)
    {
        return (((uint16_t(high) & 0x00ff ) << 8) | (uint16_t(low) & 0x00ff));
    }

    inline pair<char, char> uint16ToChar2(uint16_t in)
    {
        pair<char, char> res;
        res.first = (in>>8) & 0x00ff; //high
        res.second = (in) & 0x00ff; //low
        return res;
    }

    inline bool strStartsWith(const string& str, const string& prefix)
    {
        //return str.substr(0, prefix.size()) == prefix;
        if(prefix.length() > str.length())
        {
            return false;
        }
        return 0 == str.compare(0, prefix.length(), prefix);
    }

    inline bool strEndsWith(const string& str, const string& suffix)
    {
        if(suffix.length() > str.length())
        {
            return false;
        }
        return 0 == str.compare(str.length() -  suffix.length(), suffix.length(), suffix);
    }

    inline bool isInStr(const string& str, char ch)
    {
        return str.find(ch) != string::npos;
    }

    inline bool utf8ToUnicode(const char * const str, uint len, vector<uint16_t>& vec)
    {
        char ch1, ch2;
        if(!str)
        {
            return false;
        }
        vec.clear();
        for(uint i = 0;i < len;)
        {
            if(!(str[i] & 0x80)) // 0xxxxxxx
            {
                vec.push_back(str[i]);
                i++;
            }
            else if ((unsigned char)str[i] <= 0xdf && i + 1 < len) // 110xxxxxx
            {
                ch1 = (str[i] >> 2) & 0x07;
                ch2 = (str[i+1] & 0x3f) | ((str[i] & 0x03) << 6 );
                vec.push_back(twocharToUint16(ch1, ch2));
                i += 2;
            }
            else if((unsigned char)str[i] <= 0xef && i + 2 < len)
            {
                ch1 = (str[i] << 4) | ((str[i+1] >> 2) & 0x0f );
                ch2 = ((str[i+1]<<6) & 0xc0) | (str[i+2] & 0x3f); 
                vec.push_back(twocharToUint16(ch1, ch2));
                i += 3;
            }
            else
            {
                return false;
            }
        }
        return true;
    }
    inline bool utf8ToUnicode(const string& str, vector<uint16_t>& vec)
    {
        return utf8ToUnicode(str.c_str(), str.size(), vec);
    }

    inline bool unicodeToUtf8(vector<uint16_t>::const_iterator begin, vector<uint16_t>::const_iterator end, string& res)
    {
        if(begin >= end)
        {
            return false;
        }
        res.clear();
        uint16_t ui;
        while(begin != end)
        {
            ui = *begin;
            if(ui <= 0x7f)
            {
                res += char(ui);
            }
            else if(ui <= 0x7ff)
            {
                res += char(((ui>>6) & 0x1f) | 0xc0);
                res += char((ui & 0x3f) | 0x80);
            }
            else
            {
                res += char(((ui >> 12) & 0x0f )| 0xe0);
                res += char(((ui>>6) & 0x3f )| 0x80 );
                res += char((ui & 0x3f) | 0x80);
            }
            begin ++;
        }
        return true;
    }

    
    inline bool gbkTrans(const char* const str, uint len, vector<uint16_t>& vec)
    {
        vec.clear();
        if(!str)
        {
            return false;
        }
        uint i = 0;
        while(i < len)
        {
            if(0 == (str[i] & 0x80))
            {
                vec.push_back(uint16_t(str[i]));
                i++;
            }
            else
            {
                if(i + 1 < len) //&& (str[i+1] & 0x80))
                {
                    vec.push_back(twocharToUint16(str[i], str[i + 1]));
                    i += 2;
                }
                else
                {
                    return false;
                }
            }
        }
        return true;
    }
    inline bool gbkTrans(const string& str, vector<uint16_t>& vec)
    {
        return gbkTrans(str.c_str(), str.size(), vec);
    }

    inline bool gbkTrans(vector<uint16_t>::const_iterator begin, vector<uint16_t>::const_iterator end, string& res)
    {
        if(begin >= end)
        {
            return false;
        }
        res.clear();
        pair<char, char> pa;
        while(begin != end)
        {
            pa = uint16ToChar2(*begin);
            if(pa.first & 0x80)
            {
                res += pa.first;
                res += pa.second;
            }
            else
            {
                res += pa.second;
            }
            begin++;
        }
        return true;
    }

}
#endif
