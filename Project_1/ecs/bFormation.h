#ifndef _HG_FormationBehaviour_
#define _HG_FormationBehaviour_

#include "Behaviour.h"

#include <glm/vec3.hpp>

// Vectors of formation offsets based on the black player position
extern std::vector<glm::vec3> circleFormation;
extern std::vector<glm::vec3> vFormation;
extern std::vector<glm::vec3> squareFormation;
extern std::vector<glm::vec3> lineFormation;
extern std::vector<glm::vec3> rowsFormation;

class FormationBehaviour : public Behaviour
{
public:
	FormationBehaviour(Entity* entity, std::vector<glm::vec3> formation);

	virtual ~FormationBehaviour(void);
	virtual void Update(float dt);
	virtual std::string GetName(void);

private:
	Entity* mAnchor;
	std::vector<glm::vec3> mCurrentFormation;
};

#endif
