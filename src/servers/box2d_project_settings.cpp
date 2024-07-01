#include "box2d_project_settings.h"

#include <godot_cpp/classes/project_settings.hpp>

using namespace godot;

constexpr char RUN_ON_SEPARATE_THREAD[] = "physics/2d/run_on_separate_thread";
constexpr char MAX_THREADS[] = "threading/worker_pool/max_threads";
constexpr char SUB_STEP_COUNT[] = "physics/box_2d/sub_step_count";
constexpr char LOGGING_ENABLED[] = "physics/box_2d/logging";

void register_setting(
		const String &p_name,
		const Variant &p_value,
		bool p_needs_restart,
		PropertyHint p_hint,
		const String &p_hint_string) {
	ProjectSettings *project_settings = ProjectSettings::get_singleton();

	if (!project_settings->has_setting(p_name)) {
		project_settings->set(p_name, p_value);
	}

	Dictionary property_info;
	property_info["name"] = p_name;
	property_info["type"] = p_value.get_type();
	property_info["hint"] = p_hint;
	property_info["hint_string"] = p_hint_string;

	project_settings->add_property_info(property_info);
	project_settings->set_initial_value(p_name, p_value);
	project_settings->set_restart_if_changed(p_name, p_needs_restart);

	// HACK(mihe): We want our settings to appear in the order we register them in, but if we start
	// the order at 0 we end up moving the entire `physics/` group to the top of the tree view, so
	// instead we give it a hefty starting order and increment from there, which seems to give us
	// the desired effect.
	static int32_t order = 1000000;

	project_settings->set_order(p_name, order++);
}

void register_setting_plain(
		const String &p_name,
		const Variant &p_value,
		bool p_needs_restart = false) {
	register_setting(p_name, p_value, p_needs_restart, PROPERTY_HINT_NONE, {});
}

void register_setting_hinted(
		const String &p_name,
		const Variant &p_value,
		const String &p_hint_string,
		bool p_needs_restart = false) {
	register_setting(p_name, p_value, p_needs_restart, PROPERTY_HINT_NONE, p_hint_string);
}

void register_setting_ranged(
		const String &p_name,
		const Variant &p_value,
		const String &p_hint_string,
		bool p_needs_restart = false) {
	register_setting(p_name, p_value, p_needs_restart, PROPERTY_HINT_RANGE, p_hint_string);
}

void Box2DProjectSettings::register_settings() {
	register_setting_ranged(SUB_STEP_COUNT, 4, U"1,16,or_greater");
	register_setting_plain(LOGGING_ENABLED, false, true);
}

template <typename TType>
TType get_setting(const char *p_setting) {
	const ProjectSettings *project_settings = ProjectSettings::get_singleton();
	const Variant setting_value = project_settings->get_setting_with_override(p_setting);
	const Variant::Type setting_type = setting_value.get_type();
	const Variant::Type expected_type = Variant(TType()).get_type();

	ERR_FAIL_COND_V(setting_type != expected_type, Variant());

	return setting_value;
}

bool Box2DProjectSettings::should_run_on_separate_thread() {
	return get_setting<bool>(RUN_ON_SEPARATE_THREAD);
}

int Box2DProjectSettings::get_max_threads() {
	return get_setting<int>(MAX_THREADS);
}

int Box2DProjectSettings::get_sub_step_count() {
	return get_setting<int>(SUB_STEP_COUNT);
}

bool Box2DProjectSettings::get_logging_enabled() {
	return get_setting<bool>(LOGGING_ENABLED);
}