#ifndef _TRANSFORM_NODE_H
#define _TRANSFORM_NODE_H

#include <vector>
using std::vector;

#include <SFML/Graphics.hpp>

class TransformNode
{
public:
  TransformNode();
  TransformNode(sf::Shape* shapeToUse);

  void AddChild(TransformNode *child);
  TransformNode* RemoveChild(TransformNode *child);

  void Draw(sf::RenderTarget *target, const sf::Transform &parentTransform = sf::Transform::Identity);

  sf::Shape* m_CurrentShape;

protected:
  TransformNode *m_Parent;
  std::vector<TransformNode *> m_Children;

  void DrawSelf(sf::RenderTarget *target, const sf::Transform &parentTransform = sf::Transform::Identity);
};

#endif