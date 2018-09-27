/**
 * pyltp - A python extension for Language Technology Platform
 *
 *  @author: HuangFJ <biohfj@gmail.com>
 *
 * This project forks from https://github.com/HuangFJ/pyltp. The basic structure of
 * the project is perserved. But interface is adopted from XML level to library level
 * to allow more flexible usage.
 *
 *  @author: Yijia Liu <yjliu@ir.hit.edu.cn>
 *  @author: Zixiang Xu <zxxu@ir.hit.edu.cn>
 */
#include <iostream>
#include <vector>
#include <boost/python.hpp>
#include <boost/python/scope.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include "SplitSentence.h"
#include "segment_dll.h"
#include "postag_dll.h"
#include "parser_dll.h"
#include "ner_dll.h"
#include "SRL_DLL.h"

const std::string PYLTP_VERSION = "0.2.1";

template <class T>
std::vector<T> py_list_to_std_vector(const boost::python::list& l){
  std::vector<T> v;
  for(int i = 0; i < len(l); ++i){
    v.push_back(boost::python::extract<T>(l[i]));
  }
  return v;
}

struct SentenceSplitter {
  SentenceSplitter() {}

  static std::vector<std::string> split(const std::string& paragraph) {
    std::vector<std::string> ret;
    SplitSentence(paragraph, ret);
    return ret;
  }
};

struct Segmentor {
  Segmentor()
    : model(NULL) {}

  void load(const std::string& model_path) {
    if (model == NULL) {
      model = segmentor_create_segmentor(model_path.c_str());
    } else {
      std::cerr << "Segmentor: Model reloaded!" << std::endl;
    }
  }

  void load_with_lexicon(const std::string& model_path, const std::string& lexicon_path) {
    if (model == NULL) {
      model = segmentor_create_segmentor(model_path.c_str(), lexicon_path.c_str());
    } else {
      std::cerr << "Segmentor: Model reloaded!" << std::endl;
    }
  }

  std::vector<std::string> segment(const std::string& sentence) {
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

  void * model;
};

struct CustomizedSegmentor {
  CustomizedSegmentor()
    : model(NULL) {}

  void load(
      const std::string& base_model_path,
      const std::string& customized_model_path) {
    if (model == NULL) {
      model = customized_segmentor_create_segmentor(
          base_model_path.c_str(),
          customized_model_path.c_str());
    } else {
      std::cerr << "CustomizedSegmentor: Model reloaded!" << std::endl;
    }
  }

  void load_with_lexicon(
      const std::string& base_model_path,
      const std::string& customized_model_path,
      const std::string& lexicon_path) {
    if (model == NULL) {
      model = customized_segmentor_create_segmentor(
          base_model_path.c_str(),
          customized_model_path.c_str(),
          lexicon_path.c_str());
    } else {
      std::cerr << "CustomizedSegmentor: Model reloaded!" << std::endl;
    }
  }

  std::vector<std::string> segment(const std::string& sentence) {
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

  void * model;
};


struct Postagger {
  Postagger()
    : model(NULL) {}

  void load(const std::string& model_path) {
    if (model == NULL) {
      model = postagger_create_postagger(model_path.c_str());
    } else {
      std::cerr << "Postagger: Model reloaded!" << std::endl;
    }
  }

  void load_with_lexicon(const std::string& model_path, const std::string& lexicon_path) {
    if (model == NULL) {
      model = postagger_create_postagger(model_path.c_str(), lexicon_path.c_str());
    } else {
      std::cerr << "Postagger: Model reloaded!" << std::endl;
    }
  }

  std::vector<std::string> postag(const std::vector<std::string>& words) {
    std::vector<std::string> ret;
    if (model == NULL) {
      std::cerr << "Postagger: Model not loaded!" << std::endl;
    } else {
      postagger_postag(model, words, ret);
    }
    return ret;
  }

  std::vector<std::string> postag(const boost::python::list& words) {
    return postag(py_list_to_std_vector<std::string>(words));
  }

  void release() {
    if (model != NULL) {
      postagger_release_postagger(model);
      model = NULL;
    }
  }

  void * model;
};


typedef std::pair<int, std::string> ParseResult;

struct Parser {
  Parser()
    : model(NULL) {}

  void load(const std::string& model_path) {
    if (model == NULL) {
      model = parser_create_parser(model_path.c_str());
    } else {
      std::cerr << "Parser: Model reloaded!" << std::endl;
    }
  }

  std::vector<ParseResult> parse(const std::vector<std::string>& words,
      const std::vector<std::string>& postags) {
    std::vector<ParseResult> ret;
    std::vector<int>          heads;
    std::vector<std::string>  relations;

    if (model == NULL) {
      std::cerr << "Parser: Model not loaded!" << std::endl;
    } else {
      parser_parse(model, words, postags, heads, relations);
    }

    for (std::size_t i = 0; i < heads.size(); ++ i) {
      ret.push_back(ParseResult(heads[i], relations[i]));
    }
    return ret;
  }

  std::vector<ParseResult> parse(const std::vector<std::string>& words, const boost::python::list& postags){
    return parse(words, py_list_to_std_vector<std::string>(postags));
  }

  std::vector<ParseResult> parse(const boost::python::list& words, const std::vector<std::string>& postags){
    return parse(py_list_to_std_vector<std::string>(words), postags);
  }

  std::vector<ParseResult> parse(const boost::python::list& words, const boost::python::list& postags){
    return parse(py_list_to_std_vector<std::string>(words), py_list_to_std_vector<std::string>(postags));
  }


  void release() {
    if (model != NULL) {
      parser_release_parser(model);
      model = NULL;
    }
  }

  void * model;
};


struct NamedEntityRecognizer {
  NamedEntityRecognizer()
    : model(NULL) {}

  void load(const std::string& model_path) {
    if (model == NULL) {
      model = ner_create_recognizer(model_path.c_str());
    } else {
      std::cerr << "NER: Model reloaded!" << std::endl;
    }
  }

  std::vector<std::string> recognize(const std::vector<std::string>& words,
      const std::vector<std::string>& postags) {
    std::vector<std::string> netags;
    if (model == NULL) {
      std::cerr << "NER: Model not loaded!" << std::endl;
    } else {
      ner_recognize(model, words, postags, netags);
    }
    return netags;
  }

  std::vector<std::string> recognize(const std::vector<std::string>& words, const boost::python::list& postags){
    return recognize(words, py_list_to_std_vector<std::string>(postags));
  }

  std::vector<std::string> recognize(const boost::python::list& words, const std::vector<std::string>& postags){
    return recognize(py_list_to_std_vector<std::string>(words), postags);
  }

  std::vector<std::string> recognize(const boost::python::list& words, const boost::python::list& postags){
    return recognize(py_list_to_std_vector<std::string>(words), py_list_to_std_vector<std::string>(postags));
  }

  void release() {
    if (model != NULL) {
      ner_release_recognizer(model);
      model = NULL;
    }
  }

  void * model;
};

typedef std::pair<int, int>                 ArgRange;
typedef std::pair<std::string, ArgRange>    Arg;
typedef std::pair<int, std::vector<Arg> >   SementicRole;

struct SementicRoleLabeller {
  SementicRoleLabeller()
    : loaded(false) {}

  void load(const std::string& model_path) {
    loaded = (srl_load_resource(model_path) == 0);
  }

  std::vector<SementicRole> label(
      const std::vector<std::string>& words,
      const std::vector<std::string>& postags,
      const std::vector<ParseResult>& parse
      ) {
    std::vector<SementicRole> ret;

    // Some trick
    std::vector<ParseResult> tmp_parse(parse);
    for (std::size_t i = 0; i < tmp_parse.size(); ++ i) {
      tmp_parse[i].first --;
    }
    if (!loaded) {
      std::cerr << "SRL: Model not loaded!" << std::endl;
    } else {
      srl_dosrl(words, postags, tmp_parse, ret);
    }
    return ret;
  }

  std::vector<SementicRole> label(
      const std::vector<std::string>& words,
      const boost::python::list& postags,
      const std::vector<ParseResult>& parse
      ) {
    return label(words, py_list_to_std_vector<std::string>(postags), parse);
  }

  std::vector<SementicRole> label(
      const boost::python::list& words,
      const std::vector<std::string>& postags,
      const std::vector<ParseResult>& parse
      ) {
    return label(py_list_to_std_vector<std::string>(words), postags, parse);
  }

  std::vector<SementicRole> label(
      const boost::python::list& words,
      const boost::python::list& postags,
      const std::vector<ParseResult>& parse
      ) {
    return label(py_list_to_std_vector<std::string>(words), py_list_to_std_vector<std::string>(postags), parse);
  }

  void release() {
    if (loaded) {
      srl_release_resource();
    }
  }

  bool loaded;
};


BOOST_PYTHON_MODULE(pyltp)
{
  using namespace boost::python;

  scope().attr("__version__") = PYLTP_VERSION;
  
  class_<ParseResult>("ParseResult")
    .def_readwrite("head",     &ParseResult::first)
    .def_readwrite("relation", &ParseResult::second);

  class_<ArgRange>("ArgRange")
    .def_readwrite("start",    &ArgRange::first)
    .def_readwrite("end",      &ArgRange::second);

  class_<Arg>("Arg")
    .def_readwrite("name",     &Arg::first)
    .def_readwrite("range",    &Arg::second);

  class_<std::vector<Arg> >("Args")
    .def(vector_indexing_suite<std::vector<Arg> >() );

  class_<SementicRole >("SementicRole")
    .def_readwrite("index",     &SementicRole::first)
    .def_readwrite("arguments", &SementicRole::second);

  class_<std::vector<SementicRole> >("SementicRoles")
    .def(vector_indexing_suite<std::vector<SementicRole> >() );

  class_<std::vector<std::string> >("VectorOfString")
    .def(vector_indexing_suite<std::vector<std::string> >() );

  class_<std::vector<ParseResult > >("VectorOfParseResult")
    .def(vector_indexing_suite<std::vector<ParseResult> >() );

  class_<SentenceSplitter>("SentenceSplitter")
    .def("split", &SentenceSplitter::split)
    .staticmethod("split")
    ;

  class_<Segmentor>("Segmentor")
    .def("load", &Segmentor::load)
    .def("load_with_lexicon", &Segmentor::load_with_lexicon)
    .def("segment", &Segmentor::segment)
    .def("release", &Segmentor::release)
    ;

  class_<CustomizedSegmentor>("CustomizedSegmentor")
    .def("load", &CustomizedSegmentor::load)
    .def("load_with_lexicon", &CustomizedSegmentor::load_with_lexicon)
    .def("segment", &CustomizedSegmentor::segment)
    .def("release", &CustomizedSegmentor::release)
    ;

  class_<Postagger>("Postagger")
    .def("load", &Postagger::load)
    .def("load_with_lexicon", &Postagger::load_with_lexicon)
    .def("postag", static_cast<std::vector<std::string> (Postagger::*)(const std::vector<std::string>&)>(&Postagger::postag))
    .def("postag", static_cast<std::vector<std::string> (Postagger::*)(const boost::python::list&)>(&Postagger::postag))
    .def("release", &Postagger::release)
    ;

  class_<Parser>("Parser")
    .def("load", &Parser::load)
    .def("parse", static_cast<std::vector<ParseResult> (Parser::*)(const std::vector<std::string>&, const std::vector<std::string>&)>(&Parser::parse))
    .def("parse", static_cast<std::vector<ParseResult> (Parser::*)(const std::vector<std::string>&, const boost::python::list&)>(&Parser::parse))
    .def("parse", static_cast<std::vector<ParseResult> (Parser::*)(const boost::python::list&, const std::vector<std::string>&)>(&Parser::parse))
    .def("parse", static_cast<std::vector<ParseResult> (Parser::*)(const boost::python::list&, const boost::python::list&)>(&Parser::parse))
    .def("release", &Parser::release)
    ;

  class_<NamedEntityRecognizer>("NamedEntityRecognizer")
    .def("load", &NamedEntityRecognizer::load)
    .def("recognize", static_cast<std::vector<std::string> (NamedEntityRecognizer::*)(const std::vector<std::string>&, const std::vector<std::string>&)>(&NamedEntityRecognizer::recognize))
    .def("recognize", static_cast<std::vector<std::string> (NamedEntityRecognizer::*)(const std::vector<std::string>&, const boost::python::list&)>(&NamedEntityRecognizer::recognize))
    .def("recognize", static_cast<std::vector<std::string> (NamedEntityRecognizer::*)(const boost::python::list&, const std::vector<std::string>&)>(&NamedEntityRecognizer::recognize))
    .def("recognize", static_cast<std::vector<std::string> (NamedEntityRecognizer::*)(const boost::python::list&, const boost::python::list&)>(&NamedEntityRecognizer::recognize))
    .def("release", &NamedEntityRecognizer::release)
    ;

  class_<SementicRoleLabeller>("SementicRoleLabeller")
    .def("load", &SementicRoleLabeller::load)
    .def("label", static_cast<std::vector<SementicRole> (SementicRoleLabeller::*)(const std::vector<std::string>&, const std::vector<std::string>&, const std::vector<ParseResult>&)>(&SementicRoleLabeller::label))
    .def("label", static_cast<std::vector<SementicRole> (SementicRoleLabeller::*)(const std::vector<std::string>&, const boost::python::list&, const std::vector<ParseResult>&)>(&SementicRoleLabeller::label))
    .def("label", static_cast<std::vector<SementicRole> (SementicRoleLabeller::*)(const boost::python::list&, const std::vector<std::string>&, const std::vector<ParseResult>&)>(&SementicRoleLabeller::label))
    .def("label", static_cast<std::vector<SementicRole> (SementicRoleLabeller::*)(const boost::python::list&, const boost::python::list&, const std::vector<ParseResult>&)>(&SementicRoleLabeller::label))
    .def("release", &SementicRoleLabeller::release)
    ;
}
