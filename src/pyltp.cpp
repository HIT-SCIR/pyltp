/**
 * pyltp - A python extension for Language Technology Platform
 *
 *  @author: HuangFJ <biohfj@gmail.com>
 *
 * This project forks from https://github.com/HuangFJ/pyltp. The basic structure
 * of the project is perserved. But interface is adopted from XML level to
 * library level to allow more flexible usage.
 *
 *  @author: Yijia Liu <yjliu@ir.hit.edu.cn>
 *  @author: Zixiang Xu <zxxu@ir.hit.edu.cn>
 *  @author: Yang Liu <yliu@ir.hit.edu.cn>
 *  @author: YunLong Feng <ylfeng@ir.hit.edu.cn>
 */
#include "ltp/ner_dll.h"
#include "ltp/parser_dll.h"
#include "ltp/postag_dll.h"
#include "ltp/segment_dll.h"
#include "ltp/SplitSentence.h"
#include "ltp/srl_dll.h"
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>

namespace py = pybind11;

struct SentenceSplitter {
  SentenceSplitter() {}

  static std::vector<std::string> split(const std::string &paragraph) {
    std::vector<std::string> ret;
    SplitSentence(paragraph, ret);
    return ret;
  }
};

struct Segmentor {
  Segmentor() : model(NULL) {}

  void load(const std::string &model_path) {
    if (model == NULL) {
      model = segmentor_create_segmentor(model_path.c_str());
    } else {
      std::cerr << "Segmentor: Model reloaded!" << std::endl;
    }
  }

  void load_with_lexicon(const std::string &model_path,
                         const std::string &lexicon_path,
                         const std::string &force_lexicon_file) {
    if (model == NULL) {
      model = segmentor_create_segmentor(
          model_path.c_str(), lexicon_path.c_str(), force_lexicon_file.c_str());
    } else {
      std::cerr << "Segmentor: Model reloaded!" << std::endl;
    }
  }

  std::vector<std::string> segment(const std::string &sentence) {
    std::vector<std::string> ret;
    if (model == NULL) {
      std::cerr << "Segmentor: Model not loaded!" << std::endl;
    } else {
      segmentor_segment(model, sentence.c_str(), ret);
    }
    return ret;
  }

  void release() {
    if (model != NULL) {
      segmentor_release_segmentor(model);
      model = NULL;
    }
  }

  void *model;
};

struct CustomizedSegmentor {
  CustomizedSegmentor() : model(NULL) {}

  void load(const std::string &base_model_path,
            const std::string &customized_model_path) {
    if (model == NULL) {
      model = customized_segmentor_create_segmentor(
          base_model_path.c_str(), customized_model_path.c_str());
    } else {
      std::cerr << "CustomizedSegmentor: Model reloaded!" << std::endl;
    }
  }

  void load_with_lexicon(const std::string &base_model_path,
                         const std::string &customized_model_path,
                         const std::string &lexicon_path) {
    if (model == NULL) {
      model = customized_segmentor_create_segmentor(
          base_model_path.c_str(), customized_model_path.c_str(),
          lexicon_path.c_str());
    } else {
      std::cerr << "CustomizedSegmentor: Model reloaded!" << std::endl;
    }
  }

  std::vector<std::string> segment(const std::string &sentence) {
    std::vector<std::string> ret;
    if (model == NULL) {
      std::cerr << "CustomizedSegmentor: Model not loaded!" << std::endl;
    } else {
      customized_segmentor_segment(model, sentence.c_str(), ret);
    }
    return ret;
  }

  void release() {
    if (model != NULL) {
      customized_segmentor_release_segmentor(model);
      model = NULL;
    }
  }

  void *model;
};

struct Postagger {
  Postagger() : model(NULL) {}

  void load(const std::string &model_path) {
    if (model == NULL) {
      model = postagger_create_postagger(model_path.c_str());
    } else {
      std::cerr << "Postagger: Model reloaded!" << std::endl;
    }
  }

  void load_with_lexicon(const std::string &model_path,
                         const std::string &lexicon_path) {
    if (model == NULL) {
      model =
          postagger_create_postagger(model_path.c_str(), lexicon_path.c_str());
    } else {
      std::cerr << "Postagger: Model reloaded!" << std::endl;
    }
  }

  std::vector<std::string> postag(const std::vector<std::string> &words) {
    std::vector<std::string> ret;
    if (model == NULL) {
      std::cerr << "Postagger: Model not loaded!" << std::endl;
    } else {
      postagger_postag(model, words, ret);
    }
    return ret;
  }

  void release() {
    if (model != NULL) {
      postagger_release_postagger(model);
      model = NULL;
    }
  }

  void *model;
};

typedef std::pair<int, std::string> ParseResult;

struct Parser {
  Parser() : model(NULL) {}

  void load(const std::string &model_path) {
    if (model == NULL) {
      model = parser_create_parser(model_path.c_str());
    } else {
      std::cerr << "Parser: Model reloaded!" << std::endl;
    }
  }

  std::vector<ParseResult> parse(const std::vector<std::string> &words,
                                 const std::vector<std::string> &postags) {
    std::vector<ParseResult> ret;
    std::vector<int> heads;
    std::vector<std::string> relations;

    if (model == NULL) {
      std::cerr << "Parser: Model not loaded!" << std::endl;
    } else {
      parser_parse(model, words, postags, heads, relations);
    }

    for (std::size_t i = 0; i < heads.size(); ++i) {
      ret.push_back(ParseResult(heads[i], relations[i]));
    }
    return ret;
  }

  void release() {
    if (model != NULL) {
      parser_release_parser(model);
      model = NULL;
    }
  }

  void *model;
};

struct NamedEntityRecognizer {
  NamedEntityRecognizer() : model(NULL) {}

  void load(const std::string &model_path) {
    if (model == NULL) {
      model = ner_create_recognizer(model_path.c_str());
    } else {
      std::cerr << "NER: Model reloaded!" << std::endl;
    }
  }

  std::vector<std::string> recognize(const std::vector<std::string> &words,
                                     const std::vector<std::string> &postags) {
    std::vector<std::string> netags;
    if (model == NULL) {
      std::cerr << "NER: Model not loaded!" << std::endl;
    } else {
      ner_recognize(model, words, postags, netags);
    }
    return netags;
  }

  void release() {
    if (model != NULL) {
      ner_release_recognizer(model);
      model = NULL;
    }
  }

  void *model;
};

typedef std::pair<int, int> ArgRange;
typedef std::pair<std::string, ArgRange> Arg;
typedef std::pair<int, std::vector<Arg>> SementicRole;

struct SementicRoleLabeller {
  SementicRoleLabeller() : loaded(false) {}

  void load(const std::string &model_path) {
    loaded = (srl_load_resource(model_path) == 0);
  }

  std::vector<SementicRole> label(const std::vector<std::string> &words,
                                  const std::vector<std::string> &postags,
                                  const std::vector<ParseResult> &parse) {
    std::vector<SementicRole> ret;

    // Some trick
    std::vector<ParseResult> tmp_parse(parse);
    for (std::size_t i = 0; i < tmp_parse.size(); ++i) {
      tmp_parse[i].first--;
    }
    if (!loaded) {
      std::cerr << "SRL: Model not loaded!" << std::endl;
    } else {
      srl_dosrl(words, postags, tmp_parse, ret);
    }
    return ret;
  }

  void release() {
    if (loaded) {
      srl_release_resource();
    }
  }

  bool loaded;
};

#ifdef SDPG
#include "ltp/lstm_sdparser_dll.h"

typedef std::pair<std::string, int> SemanticArc;
typedef std::vector<SemanticArc> SemanticNode;

struct SDGraphParser {

  void load(const std::string &model_path) {
    if (model == NULL) {
      model = lstmsdparser_create_parser(model_path.c_str());
    } else {
      std::cerr << "SDGraphParser: Model reloaded!" << std::endl;
    }
  }

  std::vector<SemanticNode> parse(const std::vector<std::string> &words,
                                  const std::vector<std::string> &postags) {
    std::vector<std::vector<std::string>> vecSemResult;
    std::vector<SemanticNode> ret;
    if (model == NULL) {
      std::cerr << "SDGraphParser: Model not loaded!" << std::endl;
    } else {
      lstmsdparser_parse(model, words, postags, vecSemResult);
    }
    for (int i = 0; i < vecSemResult.size(); i++) {
      SemanticNode node;
      for (int j = 0; j < vecSemResult[i].size(); j++) {
        if (vecSemResult[i][j] != "-NULL-") {
          node.push_back(SemanticArc(
              vecSemResult[i][j], j < vecSemResult[i].size() - 1 ? j + 1 : -1));
        }
      }
      ret.push_back(node);
    }
    return ret;
  }

  void release() {
    if (model != NULL) {
      lstmsdparser_release_parser(model);
      model = NULL;
    }
  }

  void *model;
};
#endif

PYBIND11_MODULE(pyltp, m) {
  py::class_<SentenceSplitter>(m, "SentenceSplitter")
      .def(py::init<>())
      .def_static("split", &SentenceSplitter::split);

  py::class_<Segmentor>(m, "Segmentor")
      .def(py::init<>())
      .def("load", &Segmentor::load)
      .def("load_with_lexicon", &Segmentor::load_with_lexicon)
      .def("segment", &Segmentor::segment)
      .def("release", &Segmentor::release);

  py::class_<CustomizedSegmentor>(m, "CustomizedSegmentor")
      .def(py::init<>())
      .def("load", &CustomizedSegmentor::load)
      .def("load_with_lexicon", &CustomizedSegmentor::load_with_lexicon)
      .def("segment", &CustomizedSegmentor::segment)
      .def("release", &CustomizedSegmentor::release);

  py::class_<Postagger>(m, "Postagger")
      .def(py::init<>())
      .def("load", &Postagger::load)
      .def("load_with_lexicon", &Postagger::load_with_lexicon)
      .def("postag", &Postagger::postag)
      .def("release", &Postagger::release);

  py::class_<Parser>(m, "Parser")
      .def(py::init<>())
      .def("load", &Parser::load)
      .def("parse", &Parser::parse)
      .def("release", &Parser::release);

  py::class_<NamedEntityRecognizer>(m, "NamedEntityRecognizer")
      .def(py::init<>())
      .def("load", &NamedEntityRecognizer::load)
      .def("recognize",&NamedEntityRecognizer::recognize)
      .def("release", &NamedEntityRecognizer::release);

  py::class_<SementicRoleLabeller>(m, "SementicRoleLabeller")
      .def(py::init<>())
      .def("load", &SementicRoleLabeller::load)
      .def("label",&SementicRoleLabeller::label)
//      .def("pi",&SementicRoleLabeller::pi)
      .def("release", &SementicRoleLabeller::release);
#ifdef SDPG
  py::class_<SDGraphParser>(m, "SDGraphParser")
      .def(py::init<>())
      .def("load", &SDGraphParser::load)
      .def("parse",&SDGraphParser::parse)
      .def("release", &SDGraphParser::release);
#endif
}
