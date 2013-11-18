#include <assert.h>
#include "MixSegment.h"

namespace CppJieba
{
    MixSegment::MixSegment()
    {
    }
    
    MixSegment::~MixSegment()
    {
        dispose();
    }

    bool MixSegment::init(const char* const mpSegDict, const char* const hmmSegDict)
    {
        if(_getInitFlag())
        {
            LogError("inited.");
            return false;
        }
        if(!_mpSeg.init(mpSegDict))
        {
            LogError("_mpSeg init");
            return false;
        }
        if(!_hmmSeg.init(hmmSegDict))
        {
            LogError("_hmmSeg init");
            return false;
        }
        return _setInitFlag(true);
    }
    
    bool MixSegment::dispose()
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
    bool MixSegment::cut(const string& str, vector<string>& res)const
    {
        return SegmentBase::cut(str, res);
    }

    bool MixSegment::cut(Unicode::const_iterator begin, Unicode::const_iterator end, vector<string>& res)const
    {
        /*if(!_getInitFlag())
        {
            LogError("not inited.");
            return false;
        }*/
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
	bool MixSegment::cut_type(const string& str, vector<string>& res) const
	{
		assert(_getInitFlag());
		ChineseFilter filter;
		filter.feed(str);
		for(ChineseFilter::iterator it = filter.begin(); it != filter.end(); it++)
		{
			if(it.charType == CHWORD)
			{
				cut_type(it.begin, it.end, res);
			}
			else
			{
				string tmp;
				if(TransCode::encode(it.begin, it.end, tmp))
				{
					res.push_back(tmp);
				}
			}
		}
		return true;
	}

	/* Cut for 'n' or 'v' words only */
    bool MixSegment::cut_type(Unicode::const_iterator begin, Unicode::const_iterator end, vector<string>& res) const
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
						if(hmmRes[j].size() > 1)
						{
							TransCode::encode(hmmRes[j], tmp);
							res.push_back(tmp);
						}
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
				if(hmmRes[j].size() > 1)
				{
					TransCode::encode(hmmRes[j], tmp);
					res.push_back(tmp);
				}
            }
        }
        
        return true;
	}
}

#ifdef MIXSEGMENT_UT
using namespace CppJieba;

int main()
{
    return 0;
}

#endif
