# -*- coding=utf-8 -*-

import os, codecs
import mixsegment

class segment(object):
    def __init__(self):
        mixsegment.mix_segment_init("../dicts/jieba.dict.utf8", "../dicts/hmm_model.utf8")

    def cut(self, str):
        segs = mixsegment.StringVector()
        mixsegment.mix_segment_cut(str, segs)
        for s in segs:
            yield s

    def __del__(self):
        mixsegment.mix_segment_dispose()

seg = segment()
for s in seg.cut(u'韦晟敢是爱你的'.encode('utf-8')):
    print s.decode('utf-8'),

