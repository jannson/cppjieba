#ifndef CPPJIEBA_MIXSEGMENT_H
#define CPPJIEBA_MIXSEGMENT_H

#include "MPSegment.h"
#include "HMMSegment.h"
#include "Limonp/str_functs.hpp"

namespace CppJieba
{
    class MixSegment: public SegmentBase
    {
        private:
            MPSegment _mpSeg;
            HMMSegment _hmmSeg;
        public:
            MixSegment();
            virtual ~MixSegment();
        public:
            bool init(const char* const _mpSegDict, const char* const _hmmSegDict);
            bool dispose();
        public:
            //virtual bool cut(const string& str, vector<string>& res) const;
            bool cut(const string& str, vector<string>& res)const;
            bool cut(Unicode::const_iterator begin, Unicode::const_iterator end, vector<string>& res)const;

			/* TODO do better for this two functions 18/11/13 10:11:13 */
			bool cut_type(const string& str, vector<string>& res) const;
			bool cut_type(Unicode::const_iterator begin, Unicode::const_iterator end, vector<string>& res) const;
    };
}

#endif
