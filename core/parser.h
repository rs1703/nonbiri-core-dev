#ifndef NONBIRI_CORE_PARSER_H_
#define NONBIRI_CORE_PARSER_H_

#include <memory>
#include <string>
#include <vector>

#include <gumbo-query/src/Document.h>

class CNode;

class Element
{
  std::shared_ptr<CNode> node_;

public:
  Element(const CNode &node);
  Element(std::shared_ptr<CNode> node);
  Element(const Element &element);
  ~Element();

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

class HTML
{
  CDocument document_;

public:
  HTML(const std::string &document);
  ~HTML();

  std::vector<std::shared_ptr<Element>> select(const std::string &selector);
  std::shared_ptr<Element> selectFirst(const std::string &selector);
  std::shared_ptr<Element> selectLast(const std::string &selector);
};

#endif  // NONBIRI_CORE_PARSER_H_