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

  std::vector<std::shared_ptr<Element>> children() const;
  std::shared_ptr<Element> firstChild() const;
  std::shared_ptr<Element> lastChild() const;

  std::vector<std::shared_ptr<Element>> select(const std::string &selector) const;
  std::shared_ptr<Element> selectFirst(const std::string &selector) const;
  std::shared_ptr<Element> selectLast(const std::string &selector) const;

  std::shared_ptr<Element> parent() const;
  std::shared_ptr<Element> previous() const;
  std::shared_ptr<Element> next() const;

  std::string attr(const std::string &name) const;
  std::vector<std::string> classes() const;
  std::string text() const;

  bool isValid() const;
};

#endif  // NONBIRI_CORE_PARSER_H_