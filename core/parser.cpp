#include <sstream>

#include <core/parser.h>

HTML::HTML(const std::string &document)
{
  this->document.parse(document);
}

std::vector<std::shared_ptr<Element>> HTML::select(const std::string &selector)
{
  CSelection nodes = document.find(selector);
  std::vector<std::shared_ptr<Element>> elements {};
  for (size_t i = 0; i < nodes.nodeNum(); i++)
    elements.push_back(std::make_shared<Element>(nodes.nodeAt(i)));
  return elements;
}

std::shared_ptr<Element> HTML::selectFirst(const std::string &selector)
{
  CSelection nodes = document.find(selector);
  return std::make_shared<Element>(nodes.nodeAt(0));
}

std::shared_ptr<Element> HTML::selectLast(const std::string &selector)
{
  CSelection nodes = document.find(selector);
  return std::make_shared<Element>(nodes.nodeAt(nodes.nodeNum() - 1));
}

Element::Element(const CNode &node) : node {std::make_shared<CNode>(node)} {}

Element::Element(std::shared_ptr<CNode> node) : node {node}
{
  if (this->node == nullptr) {
    this->node = std::make_shared<CNode>();
  }
}

Element::Element(const Element &element) : Element(element.node) {}

std::vector<std::shared_ptr<Element>> Element::select(const std::string &selector)
{
  CSelection nodes = node->find(selector);
  std::vector<std::shared_ptr<Element>> elements {};
  for (size_t i = 0; i < nodes.nodeNum(); i++)
    elements.push_back(std::make_shared<Element>(nodes.nodeAt(i)));
  return elements;
}

std::shared_ptr<Element> Element::selectFirst(const std::string &selector)
{
  CSelection nodes = node->find(selector);
  return std::make_shared<Element>(nodes.nodeAt(0));
}

std::shared_ptr<Element> Element::selectLast(const std::string &selector)
{
  CSelection nodes = node->find(selector);
  return std::make_shared<Element>(nodes.nodeAt(nodes.nodeNum() - 1));
}

std::shared_ptr<Element> Element::parent()
{
  return std::make_shared<Element>(node->parent());
}

std::shared_ptr<Element> Element::previous()
{
  return std::make_shared<Element>(node->prevSibling());
}

std::shared_ptr<Element> Element::next()
{
  return std::make_shared<Element>(node->nextSibling());
}

std::string Element::attr(const std::string &name)
{
  return node->attribute(name);
}

std::vector<std::string> Element::classes()
{
  const std::string class_attr = attr("class");
  if (class_attr.empty())
    return {};

  std::stringstream ss {class_attr};
  std::string item {};

  std::vector<std::string> classes {};
  while (std::getline(ss, item, ' '))
    classes.push_back(item);
  return classes;
}

std::string Element::text()
{
  return node->text();
}

bool Element::isValid()
{
  return node->valid();
}