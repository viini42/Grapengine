#ifndef GRAPENGINE_HPP
#define GRAPENGINE_HPP

// Core
#include "core/ge_application.hpp"
#include "core/ge_assert.hpp"
#include "core/ge_memory.hpp"
#include "core/ge_time_step.hpp"
#include "core/ge_type_aliases.hpp"
#include "core/ge_window.hpp"

// Input
#include "input/ge_input.hpp"
#include "input/ge_key_codes.hpp"

// Layers
#include "layer/ge_layer.hpp"

// Drawables
#include "drawables/ge_cube.hpp"
#include "drawables/ge_cylinder.hpp"
#include "drawables/ge_mesh.hpp"

// Utilities
#include "utils/ge_dimension.hpp"
#include "utils/ge_random.hpp"
#include "utils/ge_type_utils.hpp"

// Math
#include "math/ge_arithmetic.hpp"
#include "math/ge_transformations.hpp"

// Renderer
#include "renderer/ge_editor_camera.hpp"
#include "renderer/ge_framebuffer.hpp"
#include "renderer/ge_renderer.hpp"
#include "renderer/shader_programs/ge_material_shader.hpp"

// Scene
#include "scene/ge_components.hpp"
#include "scene/ge_scene.hpp"
#include "scene/ge_scriptable_entity.hpp"

// Logger
#include "log/ge_logger.hpp"

// Controllers
#include "controllers/ge_app_controller.hpp"

// Serializer
#include "serializer/ge_scene_serializer.hpp"

// ImGui
#include <imgui.h>

#endif // GRAPENGINE_HPP
