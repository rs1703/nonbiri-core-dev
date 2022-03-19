#include <iostream>
#include <sstream>

#include <core/parser/parser.h>

CHtml::CHtml(const char *document)
{
  document_.parse(document);
}

CHtml::CHtml(const std::string &document) : CHtml(document.c_str()) {}

CHtml::~CHtml()
{
  // std::cout << "CHtml::~CHtml()" << std::endl;
}

std::vector<std::shared_ptr<CElement>> CHtml::select(const char *selector)
{
  CSelection nodes = document_.find(selector);
  if (nodes.nodeNum() == 0)
    return {};

  std::vector<std::shared_ptr<CElement>> elements;
  for (int i = 0; i < nodes.nodeNum(); i++)
    elements.push_back(std::make_shared<CElement>(nodes.nodeAt(i)));
  return elements;
}

std::vector<std::shared_ptr<CElement>> CHtml::select(const std::string &selector)
{
  return select(selector.c_str());
}

std::shared_ptr<CElement> CHtml::selectFirst(const char *selector)
{
  CSelection nodes = document_.find(selector);
  if (nodes.nodeNum() == 0)
    return nullptr;
  return std::make_shared<CElement>(nodes.nodeAt(0));
}

std::shared_ptr<CElement> CHtml::selectFirst(const std::string &selector)
{
  return selectFirst(selector.c_str());
}

std::shared_ptr<CElement> CHtml::selectLast(const char *selector)
{
  CSelection nodes = document_.find(selector);
  if (nodes.nodeNum() == 0)
    return nullptr;
  return std::make_shared<CElement>(nodes.nodeAt(nodes.nodeNum() - 1));
}

std::shared_ptr<CElement> CHtml::selectLast(const std::string &selector)
{
  return selectLast(selector.c_str());
}

CElement::CElement(const CNode &node) : node_(std::make_shared<CNode>(node)) {}

CElement::CElement(std::shared_ptr<CNode> node) : node_(node) {}
CElement::CElement(const CElement &element) : node_(element.node_) {}

CElement::~CElement()
{
  // std::cout << "CElement::~CElement()" << std::endl;
}

std::vector<std::shared_ptr<CElement>> CElement::select(const char *selector)
{
  CSelection nodes = node_->find(selector);
  if (nodes.nodeNum() == 0)
    return {};

  std::vector<std::shared_ptr<CElement>> elements;
  for (int i = 0; i < nodes.nodeNum(); i++)
    elements.push_back(std::make_shared<CElement>(nodes.nodeAt(i)));
  return elements;
}

std::vector<std::shared_ptr<CElement>> CElement::select(const std::string &selector)
{
  return select(selector.c_str());
}

std::shared_ptr<CElement> CElement::selectFirst(const char *selector)
{
  CSelection nodes = node_->find(selector);
  if (nodes.nodeNum() == 0)
    return nullptr;
  return std::make_shared<CElement>(nodes.nodeAt(0));
}

std::shared_ptr<CElement> CElement::selectFirst(const std::string &selector)
{
  return selectFirst(selector.c_str());
}

std::shared_ptr<CElement> CElement::selectLast(const char *selector)
{
  CSelection nodes = node_->find(selector);
  if (nodes.nodeNum() == 0)
    return nullptr;

  return std::make_shared<CElement>(nodes.nodeAt(nodes.nodeNum() - 1));
}

std::shared_ptr<CElement> CElement::selectLast(const std::string &selector)
{
  return selectLast(selector.c_str());
}

std::shared_ptr<CElement> CElement::parent()
{
  CNode parent = node_->parent();
  if (!parent.valid())
    return nullptr;
  return std::make_shared<CElement>(parent);
}

std::shared_ptr<CElement> CElement::previous()
{
  CNode prev = node_->prevSibling();
  if (!prev.valid())
    return nullptr;
  return std::make_shared<CElement>(prev);
}

std::shared_ptr<CElement> CElement::next()
{
  CNode next = node_->nextSibling();
  if (!next.valid())
    return nullptr;
  return std::make_shared<CElement>(next);
}

std::string CElement::attr(const char *name)
{
  return node_->attribute(name);
}

std::string CElement::attr(const std::string &name)
{
  return attr(name.c_str());
}

std::vector<std::string> CElement::classes()
{
  std::string class_attr = attr("class");
  if (class_attr.empty())
    return {};

  std::stringstream ss(class_attr);
  std::string item;

  std::vector<std::string> classes;
  while (std::getline(ss, item, ' '))
    classes.push_back(item);
  return classes;
}

std::string CElement::text()
{
  return node_->text();
}

bool CElement::isValid()
{
  return node_->valid();
}