#ifndef _HG_FormationBehaviour_
#define _HG_FormationBehaviour_

#include "Behaviour.h"
#include "Entity.h"
#include <vector>
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
	FormationBehaviour(std::vector<glm::vec3> formation);
	virtual ~FormationBehaviour(void);

	virtual void Update(float dt);

	virtual std::string GetName();
private:
	std::vector<glm::vec3> mCurrentFormation;
};

#endif
