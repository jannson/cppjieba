#ifndef CPPJIEBA_MIXSEGMENT_H
#define CPPJIEBA_MIXSEGMENT_H

#include <assert.h>
#include "MPSegment.hpp"
#include "HMMSegment.hpp"
#include "Limonp/str_functs.hpp"

namespace CppJieba
{
    class MixSegment: public SegmentBase
    {
        private:
            MPSegment _mpSeg;
            HMMSegment _hmmSeg;
        public:
            MixSegment(const char * const mpSegDict, const char * const hmmSegDict): _mpSeg(mpSegDict), _hmmSeg(hmmSegDict)
            {
            }
            virtual ~MixSegment()
            {
                dispose();
            }
        public:
            virtual bool init()
            {
                if(_getInitFlag())
                {
                    LogError("inited.");
                    return false;
                }
                if(!_mpSeg.init())
                {
                    LogError("_mpSeg init");
                    return false;
                }
                if(!_hmmSeg.init())
                {
                    LogError("_hmmSeg init");
                    return false;
                }
                return _setInitFlag(true);
            }
            virtual bool dispose()
            {
                if(!_getInitFlag())
                {
                    return true;
                }
                _mpSeg.dispose();
                _hmmSeg.dispose();
                _setInitFlag(false);
                return true;
            }
        public:
            using SegmentBase::cut;
        public:
            virtual bool cut(Unicode::const_iterator begin, Unicode::const_iterator end, vector<string>& res)const
            {
				assert(_getInitFlag());
                if(begin == end)
                {
                    return false;
                }
                vector<TrieNodeInfo> infos;
                if(!_mpSeg.cut(begin, end, infos))
                {
                    LogError("mpSeg cutDAG failed.");
                    return false;
                }
                Unicode unico;
                vector<Unicode> hmmRes;
                string tmp;
                for(uint i= 0; i < infos.size(); i++)
                {
                    TransCode::encode(infos[i].word,tmp);
                    if(1 == infos[i].word.size())
                    {
                        unico.push_back(infos[i].word[0]);
                    }
                    else
                    {
                        if(!unico.empty())
                        {
                            hmmRes.clear();
                            if(!_hmmSeg.cut(unico.begin(), unico.end(), hmmRes))
                            {
                                LogError("_hmmSeg cut failed.");
                                return false;
                            }
                            for(uint j = 0; j < hmmRes.size(); j++)
                            {
                                TransCode::encode(hmmRes[j], tmp);
                                res.push_back(tmp);
                            }
                        }
                        unico.clear();
                        TransCode::encode(infos[i].word, tmp);
                        res.push_back(tmp);
                    }
                }
                if(!unico.empty())
                {
                    hmmRes.clear();
                    if(!_hmmSeg.cut(unico.begin(), unico.end(), hmmRes))
                    {
                        LogError("_hmmSeg cut failed.");
                        return false;
                    }
                    for(uint j = 0; j < hmmRes.size(); j++)
                    {
                        TransCode::encode(hmmRes[j], tmp);
                        res.push_back(tmp);
                    }
                }

                return true;
            }

			/* TODO do better for this function, now just copy from SegmentBase.cpp 18/11/13 09:54:02 */
            virtual bool cut_type(const string& str, vector<string>& res)const
            {
				assert(_getInitFlag());
                Unicode unico;
#ifdef NO_FILTER
                unico.clear();
                if(!TransCode::decode(str, unico))
                {
                    LogFatal("str[%s] decode failed.", str.c_str());
                    return false;
                }
                return cut_type(unico.begin(), unico.end(), res);
#else
                const char * const cstr = str.c_str();
                uint size = str.size();
                uint offset = 0;
                string subs;
                int ret;
                uint len;
                while(offset < size)
                {
                    const char * const nstr = cstr + offset;
                    uint nsize = size - offset;
                    if(-1 == (ret = filterAscii(nstr, nsize, len)) || 0 == len || len > nsize)
                    {
                        LogFatal("str[%s] illegal.", cstr);
                        return false;
                    }
                    subs.assign(nstr, len);
                    if(!ret)
                    {
                        res.push_back(subs);
                    }
                    else
                    {
                        unico.clear();
                        if(!TransCode::decode(subs, unico))
                        {
                            LogFatal("str[%s] decode failed.", subs.c_str());
                            return false;
                        }
                        cut_type(unico.begin(), unico.end(), res);
                    }
                    offset += len;
                }
                return true;
#endif
            }

			/* Cut for 'n' or 'v' words only */
            virtual bool cut_type(Unicode::const_iterator begin, Unicode::const_iterator end, vector<string>& res)const
            {
				assert(_getInitFlag());
                if(begin == end)
                {
                    return false;
                }
                vector<TrieNodeInfo> infos;
                if(!_mpSeg.cut(begin, end, infos))
                {
                    LogError("mpSeg cutDAG failed.");
                    return false;
                }
                Unicode unico;
                vector<Unicode> hmmRes;
                string tmp;
                for(uint i= 0; i < infos.size(); i++)
                {
                    TransCode::encode(infos[i].word,tmp);
                    if(1 == infos[i].word.size())
                    {
                        unico.push_back(infos[i].word[0]);
                    }
                    else
                    {
                        if(!unico.empty())
                        {
                            hmmRes.clear();
                            if(!_hmmSeg.cut(unico.begin(), unico.end(), hmmRes))
                            {
                                LogError("_hmmSeg cut failed.");
                                return false;
                            }
                            for(uint j = 0; j < hmmRes.size(); j++)
                            {
                                TransCode::encode(hmmRes[j], tmp);
                                res.push_back(tmp);
                            }
                        }
                        unico.clear();

						/* Ignore "Not n,v words" */
						//TransCode::encode(infos[i].word, tmp); //Just test
						//cout<<tmp<<infos[i].tag<<" / ";
						if( (infos[i].tag.empty())
								|| (string::npos != infos[i].tag.find("n"))
								|| (string::npos != infos[i].tag.find("v"))
							)
						{
							TransCode::encode(infos[i].word, tmp);
							res.push_back(tmp);
						}
                    }
                }
                if(!unico.empty())
                {
                    hmmRes.clear();
                    if(!_hmmSeg.cut(unico.begin(), unico.end(), hmmRes))
                    {
                        LogError("_hmmSeg cut failed.");
                        return false;
                    }
                    for(uint j = 0; j < hmmRes.size(); j++)
                    {
                        TransCode::encode(hmmRes[j], tmp);
                        res.push_back(tmp);
                    }
                }

                return true;
            }
    };
}

#endif
