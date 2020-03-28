#ifndef TNT_RIGID_BODY_HPP
#define TNT_RIGID_BODY_HPP

#include "ecs/Object.hpp"
#include "ecs/Component.hpp"

// TODO: find a way to apply forces to the RigidBody.
// TODO: handle collision in Update.
// TODO: move Joint on another file.

// TODO(maybe):
// Update() from outside RigidBody ??
// use a force accumulator for adding forces ??

namespace tnt
{
class RigidBody : public Object
{
public:
    RigidBody();

protected:
    RotateComponent *rotate;
    PhysicsComponent *physics;
};

class Joint
{
protected: // should this be private ??
    RigidBody *poles[2];
};
} // namespace tnt

#endif //!TNT_RIGID_BODY_HPP