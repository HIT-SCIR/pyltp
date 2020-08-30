#!/usr/bin/env python
# -*- coding: utf-8 -*-
import sys, os

ROOTDIR = os.path.join(os.path.dirname(__file__), os.pardir)
sys.path = [os.path.join(ROOTDIR, "lib")] + sys.path

# Set your own model path
MODELDIR = os.path.join(ROOTDIR, "./ltp_data")

from pyltp import SentenceSplitter, Segmentor, Postagger, Parser, NamedEntityRecognizer, SementicRoleLabeller

if __name__ == '__main__':
    paragraph = '他叫汤姆去拿外衣。'

    # --------------------- 断句 ------------------------
    sentence = SentenceSplitter.split(paragraph)[0]

    # -------------------- Context Manager -------------
    with Segmentor(os.path.join(MODELDIR, "cws.model")) as s:
        words = s.segment(sentence)
        print("\t".join(words))

    # --------------------- 分词 ------------------------
    segmentor = Segmentor(os.path.join(MODELDIR, "cws.model"))

    segmentor_with_vocab = Segmentor(
        os.path.join(MODELDIR, "cws.model"),
        lexicon_path='lexicon.txt',  # 分开的会合并在一起
    )

    segmentor_with_force_vocab = Segmentor(
        os.path.join(MODELDIR, "cws.model"),
        force_lexicon_path='lexicon.txt'  # 除上述功能外，原本合并在一起的亦会拆分
    )

    words = segmentor.segment(sentence)
    print("\t".join(words))

    words_with_vocab = segmentor_with_vocab.segment(sentence)
    print("\t".join(words_with_vocab), "\t\t| With Vocab")

    words_with_force_vocab = segmentor_with_force_vocab.segment(sentence)
    print("\t".join(words_with_force_vocab), "\t| Force Vocab")

    # --------------------- 词性标注 ------------------------
    postagger = Postagger(os.path.join(MODELDIR, "pos.model"))
    postags = postagger.postag(words)
    # list-of-string parameter is support in 0.1.5
    # postags = postagger.postag(["中国","进出口","银行","与","中国银行","加强","合作"])
    print("\t".join(postags))

    # --------------------- 语义依存分析 ------------------------
    parser = Parser(os.path.join(MODELDIR, "parser.model"))
    arcs = parser.parse(words, postags)

    print("\t".join("%d:%s" % (head, relation) for (head, relation) in arcs))

    # --------------------- 命名实体识别 ------------------------
    recognizer = NamedEntityRecognizer(os.path.join(MODELDIR, "ner.model"))
    netags = recognizer.recognize(words, postags)
    print("\t".join(netags))

    # --------------------- 语义角色标注 ------------------------
    labeller = SementicRoleLabeller(os.path.join(MODELDIR, "pisrl.model"))
    roles = labeller.label(words, postags, arcs)

    for index, arguments in roles:
        print(index, " ".join(["%s: (%d,%d)" % (name, start, end) for (name, (start, end)) in arguments]))

    segmentor.release()
    segmentor_with_vocab.release()
    segmentor_with_force_vocab.release()
    segmentor.release()
    postagger.release()
    parser.release()
    recognizer.release()
    labeller.release()
