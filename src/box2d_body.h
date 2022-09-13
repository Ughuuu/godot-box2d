#ifndef BOX2D_BODY_H
#define BOX2D_BODY_H

#include <godot_cpp/classes/physics_server2d.hpp>

#include "box2d_collision_object.h"
#include "box2d_space.h"

using namespace godot;

class Box2DDirectBodyState;

class Box2DBody: public Box2DCollisionObject {
	PhysicsServer2D::BodyMode mode = PhysicsServer2D::BODY_MODE_RIGID;

	SelfList<Box2DBody> active_list;

	bool active = true;

	Transform2D new_transform;

	typedef void (*BodyStateCallback)(void *p_instance, PhysicsDirectBodyState2D *p_state);

	void *body_state_callback_instance = nullptr;
	BodyStateCallback body_state_callback = nullptr;

	Box2DDirectBodyState *direct_state = nullptr;
	friend class Box2DDirectBodyState; // i give up, too many functions to expose
public:
	void set_state_sync_callback(void *p_instance, BodyStateCallback p_callback);

	Box2DDirectBodyState *get_direct_state();

	void set_active(bool p_active);
	_FORCE_INLINE_ bool is_active() const { return active; }

	_FORCE_INLINE_ void wakeup() {
		if ((!get_space()) || mode == PhysicsServer2D::BODY_MODE_STATIC || mode == PhysicsServer2D::BODY_MODE_KINEMATIC) {
			return;
		}
		set_active(true);
	}

	void set_mode(PhysicsServer2D::BodyMode p_mode);
	PhysicsServer2D::BodyMode get_mode() const;

	void set_state(PhysicsServer2D::BodyState p_state, const Variant &p_variant);
	Variant get_state(PhysicsServer2D::BodyState p_state) const;

	void set_space(Box2DSpace* p_space) override;

	Box2DBody();
	~Box2DBody();
};

#endif // BOX2D_BODY_H
