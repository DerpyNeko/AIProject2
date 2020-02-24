#ifndef _HG_PathFollowBehaviour_
#define _HG_PathFollowBehaviour_

#include "Behaviour.h"
#include "Entity.h"

#include <glm/glm.hpp>
#include <vector>

struct PathNode
{
	PathNode(glm::vec3 position) : position(position) {}
	glm::vec3 position;
};

struct Path
{
	std::vector<PathNode> pathNodes;
};

class PathFollowingBehaviour : public Behaviour
{
public:
	PathFollowingBehaviour(Entity* entity, Path* path);
	virtual ~PathFollowingBehaviour(void);

	virtual void Update(float dt);

private:
	Entity* mEntity;			// The agent following the path
	Path* mPath;				// The path to follow
	unsigned int mCurrentGoal;	// The index of the node to move to
	float mPathRadius;
};

#endif
