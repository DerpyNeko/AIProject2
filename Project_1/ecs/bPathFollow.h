#ifndef _HG_PathFollowBehaviour_
#define _HG_PathFollowBehaviour_

#include "Behaviour.h"

#include <glm/glm.hpp>

struct PathNode
{
	PathNode(glm::vec3 position) : position(position) {}
	glm::vec3 position;
};

struct Path
{
	std::vector<PathNode> pathNodes;
};

class PathFollowBehaviour : public Behaviour
{
public:
	PathFollowBehaviour(Entity* entity, Path* path);

	virtual ~PathFollowBehaviour(void);
	virtual void Update(float dt);
	virtual std::string GetName(void);

private:
	Entity* mEntity;			// The agent following the path
	Path* mPath;				// The path to follow
	unsigned int mCurrentGoal;	// The index of the node to move to
	float mPathRadius;
};

#endif
