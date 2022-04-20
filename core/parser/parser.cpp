#include <iostream>
#include <sstream>

#include <core/parser/parser.h>
#include <gumbo-query/src/Node.h>

HTML::HTML(const std::string &document)
{
  document_.parse(document);
}

HTML::~HTML()
{
  // std::cout << "HTML::~HTML()" << std::endl;
}

std::vector<std::shared_ptr<Element>> HTML::select(const std::string &selector)
{
  CSelection nodes = document_.find(selector);
  if (nodes.nodeNum() == 0)
    return {};

  std::vector<std::shared_ptr<Element>> elements;
  for (size_t i = 0; i < nodes.nodeNum(); i++)
    elements.push_back(std::make_shared<Element>(nodes.nodeAt(i)));
  return elements;
}

std::shared_ptr<Element> HTML::selectFirst(const std::string &selector)
{
  CSelection nodes = document_.find(selector);
  if (nodes.nodeNum() == 0)
    return nullptr;
  return std::make_shared<Element>(nodes.nodeAt(0));
}

std::shared_ptr<Element> HTML::selectLast(const std::string &selector)
{
  CSelection nodes = document_.find(selector);
  if (nodes.nodeNum() == 0)
    return nullptr;
  return std::make_shared<Element>(nodes.nodeAt(nodes.nodeNum() - 1));
}

Element::Element(const CNode &node) : node_(std::make_shared<CNode>(node)) {}
Element::Element(std::shared_ptr<CNode> node) : node_(node) {}
Element::Element(const Element &element) : node_(element.node_) {}

Element::~Element()
{
  // std::cout << "Element::~Element()" << std::endl;
}

std::vector<std::shared_ptr<Element>> Element::select(const std::string &selector)
{
  CSelection nodes = node_->find(selector);
  if (nodes.nodeNum() == 0)
    return {};

  std::vector<std::shared_ptr<Element>> elements;
  for (size_t i = 0; i < nodes.nodeNum(); i++)
    elements.push_back(std::make_shared<Element>(nodes.nodeAt(i)));
  return elements;
}

std::shared_ptr<Element> Element::selectFirst(const std::string &selector)
{
  CSelection nodes = node_->find(selector);
  if (nodes.nodeNum() == 0)
    return nullptr;
  return std::make_shared<Element>(nodes.nodeAt(0));
}

std::shared_ptr<Element> Element::selectLast(const std::string &selector)
{
  CSelection nodes = node_->find(selector);
  if (nodes.nodeNum() == 0)
    return nullptr;

  return std::make_shared<Element>(nodes.nodeAt(nodes.nodeNum() - 1));
}

std::shared_ptr<Element> Element::parent()
{
  CNode parent = node_->parent();
  if (!parent.valid())
    return nullptr;
  return std::make_shared<Element>(parent);
}

std::shared_ptr<Element> Element::previous()
{
  CNode prev = node_->prevSibling();
  if (!prev.valid())
    return nullptr;
  return std::make_shared<Element>(prev);
}

std::shared_ptr<Element> Element::next()
{
  CNode next = node_->nextSibling();
  if (!next.valid())
    return nullptr;
  return std::make_shared<Element>(next);
}

std::string Element::attr(const std::string &name)
{
  return node_->attribute(name);
}

std::vector<std::string> Element::classes()
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

std::string Element::text()
{
  return node_->text();
}

bool Element::isValid()
{
  return node_->valid();
}