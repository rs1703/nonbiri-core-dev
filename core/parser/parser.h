#ifndef NONBIRI_CORE_PARSER_PARSER_H_
#define NONBIRI_CORE_PARSER_PARSER_H_

#include <memory>
#include <string>
#include <vector>

#include <gumbo-query/src/Document.h>
#include <gumbo-query/src/Node.h>

class CElement
{
  std::shared_ptr<CNode> node_;

public:
  CElement(const CNode &node);
  CElement(std::shared_ptr<CNode> node);
  CElement(const CElement &element);
  ~CElement();

  std::vector<std::shared_ptr<CElement>> select(const std::string &selector);
  std::shared_ptr<CElement> selectFirst(const std::string &selector);
  std::shared_ptr<CElement> selectLast(const std::string &selector);

  std::shared_ptr<CElement> parent();
  std::shared_ptr<CElement> previous();
  std::shared_ptr<CElement> next();

  std::string attr(const std::string &name);

  std::vector<std::string> classes();
  std::string text();

  bool isValid();
};

class CHtml
{
  CDocument document_;

public:
  CHtml(const std::string &document);
  ~CHtml();

  std::vector<std::shared_ptr<CElement>> select(const std::string &selector);
  std::shared_ptr<CElement> selectFirst(const std::string &selector);
  std::shared_ptr<CElement> selectLast(const std::string &selector);
};

#endif  // NONBIRI_CORE_PARSER_PARSER_H_