#ifndef NONBIRI_CORE_PARSER_H_
#define NONBIRI_CORE_PARSER_H_

#include <memory>
#include <string>
#include <vector>

#include <gumbo-query/src/Document.h>
#include <gumbo-query/src/Node.h>

class Element;

class HTML
{
  CDocument document {};

public:
  HTML(const std::string &document);

  std::vector<std::shared_ptr<Element>> select(const std::string &selector);
  std::shared_ptr<Element> selectFirst(const std::string &selector);
  std::shared_ptr<Element> selectLast(const std::string &selector);
};

class Element
{
  std::shared_ptr<CNode> node {nullptr};

public:
  Element(const CNode &node);
  Element(std::shared_ptr<CNode> node);
  Element(const Element &element);

  std::vector<std::shared_ptr<Element>> select(const std::string &selector);
  std::shared_ptr<Element> selectFirst(const std::string &selector);
  std::shared_ptr<Element> selectLast(const std::string &selector);

  std::shared_ptr<Element> parent();
  std::shared_ptr<Element> previous();
  std::shared_ptr<Element> next();

  std::string attr(const std::string &name);
  std::vector<std::string> classes();
  std::string text();

  bool isValid();
};

#endif  // NONBIRI_CORE_PARSER_H_