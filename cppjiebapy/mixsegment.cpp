/*
 * =====================================================================================
 *
 *       Filename:  mixsegment.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年11月07日 16时17分14秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *   Organization:  
 *
 * g++ -c -fPIC  mixsegment.cpp -L/usr/lib/CppJieba/ -lcppjieba -o mixsegment.o
 * g++ -shared -Wl,-soname,libmixsegment.so -o libmixsegment.so mixsegment.o
 *
 * swig -python -c++  mixsegment.i
 * g++ -c -fPIC  mixsegment.cpp mixsegment_wrap.cxx  -I/usr/local/bin/python/include/python2.7/
 * g++ -shared mixsegment.o mixsegment_wrap.o -L/usr/lib/CppJieba/ -lcppjieba  -o _mixsegment.so
 * =====================================================================================
 */
#include <iostream>
#include <fstream>
#include <CppJieba/Limonp/ArgvContext.hpp>
#include <CppJieba/MPSegment.h>
#include <CppJieba/HMMSegment.h>
#include <CppJieba/MixSegment.h>
#include "mixsegment.h"

using namespace CppJieba;

//extern "C" {
    MixSegment GMIXSEG;

	int mix_segment_init(char* jieba_dic, char* hmm_model);

	bool mix_segment_cut(const std::string& str, StringVector& res);
	bool mix_segment_cut_type(const std::string& str, StringVector& res);

	void mix_segment_dispose();
//}

int mix_segment_init(char* jieba_dic, char* hmm_model)
{
	MixSegment &seg = GMIXSEG;
	if(!seg.init(jieba_dic, hmm_model))
	{
		cout<<"seg init failed."<<endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

bool mix_segment_cut(const std::string& str, vector<string>& res)
{
	MixSegment &seg = GMIXSEG;
	return seg.cut(str, res);
}

bool mix_segment_cut_type(const std::string& str, vector<string>& res)
{
	MixSegment &seg = GMIXSEG;
	return seg.cut_type(str, res);
}

void mix_segment_dispose()
{
	MixSegment &seg = GMIXSEG;
	seg.dispose();
}
