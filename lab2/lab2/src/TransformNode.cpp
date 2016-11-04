#include "TransformNode.h"

TransformNode::TransformNode() : m_CurrentShape(nullptr) { }

TransformNode::TransformNode(sf::Shape* shapeToUse)
{
  m_CurrentShape = shapeToUse;
}

void TransformNode::AddChild(TransformNode *child) {
  std::vector<TransformNode *>::iterator itr = std::find(m_Children.begin(), m_Children.end(), child);
  if (itr == m_Children.end()) {
    child->m_Parent = this;
    m_Children.push_back(child);
  }
}
TransformNode* TransformNode::RemoveChild(TransformNode *child) {
  TransformNode *toReturn = nullptr;
  std::vector<TransformNode *>::iterator itr = std::find(m_Children.begin(), m_Children.end(), child);
  if (itr != m_Children.end()) {
    toReturn = child;

    child->m_Parent = nullptr;
    m_Children.erase(itr);
  }

  return toReturn;
}

void TransformNode::DrawSelf(sf::RenderTarget *target, const sf::Transform &parentTransform) {
  target->draw(*m_CurrentShape, parentTransform);
}

void TransformNode::Draw(sf::RenderTarget *target, const sf::Transform &parentTransform) {
  sf::Transform origin;
  origin.translate(m_CurrentShape->getOrigin());

  sf::Transform currentTransform = parentTransform * m_CurrentShape->getTransform() * origin;

  DrawSelf(target, parentTransform);

  for (int childIndex = 0; childIndex < m_Children.size(); childIndex++)
  {
    m_Children[childIndex]->Draw(target, currentTransform);
  }
}