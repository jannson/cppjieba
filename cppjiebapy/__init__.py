# -*- coding=utf-8 -*-
import sys, os.path, codecs, re, math
import collections
import threading

import mixsegment

DICT_LOCK = threading.RLock()
DICT_INIT = False
segment_wrapper = None

class segment(object):
    def __init__(self):
        mixsegment.mix_segment_init('','')

    def cut(self, str):
        segs = mixsegment.StringVector()
        mixsegment.mix_segment_cut(str, segs)
        for s in segs:
            yield s

    def __del__(self):
        mixsegment.mix_segment_dispose()

def __do_stage1(self, sentence):
    start = 0
    for m in self.stage1_regex.finditer(sentence):
        yield (sentence[start:m.start(0)], True)
        yield (sentence[m.start(0):m.end(0)], False)
        start = m.end(0)
    yield (sentence[start:], True)

def cut_to_sentence(self, line):
    if not isinstance(line, unicode):
        try:
            line = line.decode('utf-8')
        except UnicodeDecodeError:
            line = line.decode('gbk','ignore')
    for s,need_cut in self.fn_stage1(line):
        if need_cut:
            if s != '':
                str = ''
                for c in s:
                    if self.sentence_dict.has_key(c):
                        if str != '':
                            yield (str, True)
                        str = ''
                        yield (c, False)
                    else:
                        str += c
                if str != '':
                    yield (str, True)
        else:
            yield (s, False)
'''
