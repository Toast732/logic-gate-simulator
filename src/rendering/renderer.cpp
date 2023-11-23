#include "renderer.hpp"

#ifndef RENDERER_CPP
#define RENDERER_CPP

typedef ResourceManager<Shader> Shaders;
typedef std::shared_ptr<Shader> SShader;

class Renderer {

	private:

		int width;
		int height;
		std::string name;

		raylib::Window window;
		bool window_created = false;
		int camera_mode = 0;
		float surface_size = 50.0f;
		float grid_spacing = 0.75f;
		//bool msaa = false;

		float step = 0;

	public:

		ObjectRenderer object_renderer;

		Renderer(
			int width,
			int height,
			const std::string name) {

			//setMSAA(true);
			SetConfigFlags(FLAG_MSAA_4X_HINT);

			new (&this->window) raylib::Window(width, height, name);
			window_created = true;
			this->width = width;
			this->height = height;
			this->name = name;

			// Define the object renderer
			ObjectRenderer object_renderer = ObjectRenderer();

			//setMSAA(false);
			//setMSAA(true);
		}

		void reloadWindow() {
			if (window_created) {
				// close the window
				window.Close();

				// create a new window
				window.Init(width, height, name);
			}
		}

		raylib::Window getWindow() const {
			return window;
		}

		//void setMSAA(bool enable) {
			// if msaa needs to be updated
			//SetConfigFlags(FLAG_MSAA_4X_HINT);
			//if (enable != msaa) {
				// if the window has already been created
				/*if (window_created) {
					
				}
				else {
					glfwWindowHint(GLFW_SAMPLES, 4);
				}*/
				/*if (enable) {
					//SetWindowState(FLAG_MSAA_4X_HINT);
					glfwWindowHint(GLFW_SAMPLES, 4);
				}
				else {
					//ClearWindowState(FLAG_MSAA_4X_HINT);
					glfwWindowHint(GLFW_SAMPLES, 0);
				}
				msaa = enable;*/
			//}
		//}

		void setCameraMode(int mode) {
			camera_mode = mode;
		}

		void draw(raylib::Camera3D* camera) {

			// Update Shaders
			SShader primary_instanced_shader = Shaders::Get("primary_instanced");
			SShader primary_shader = Shaders::Get("primary");

			float cameraPos[3] = { camera->position.x, camera->position.y, camera->position.z };
			SetShaderValue(*primary_instanced_shader, primary_instanced_shader->locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
			SetShaderValue(*primary_shader, primary_shader->locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);

			// Update Camera
			UpdateCamera(camera, this->camera_mode);
			
			BeginDrawing();

			// erase previous frame
			ClearBackground({ 70, 70, 70, 255 });

			BeginMode3D(*camera);

			if (camera_mode == CAMERA_ORBITAL) {
				DrawSphere(camera->target, 0.1f, { 100, 45, 5, 225 });
			}

			// Draw the "surface" plane
			/*DrawPlane({0.0f, -0.005f, 0.0f}, {surface_size, surface_size}, {80, 80, 80, 200});
			// Add a grid ontop of the "surface" plane
			DrawGrid(int(surface_size/grid_spacing), grid_spacing);

			// if camera mode is orbit, then draw the camera target
			

			float surface_size_h = surface_size/2.0f;
			float axis_line_thickness = 0.2f;
			float margin = 1.0f;
			// Draw X axis line.
			rlPushMatrix();
			rlTranslatef(-surface_size_h, 0.0f, surface_size_h);
			rlRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
			DrawCylinder({ 0.0f, 0.0f, 0.0f }, axis_line_thickness, axis_line_thickness, surface_size - margin, 10, { 200, 25, 25, 200 });
			rlPopMatrix();

			// Draw X axis arrow head.
			rlPushMatrix();
			rlTranslatef(surface_size_h + axis_line_thickness*4.0f - margin, 0.0f, surface_size_h);
			rlRotatef(90.0f, 0.0f, 0.0f, 1.0f);
			DrawCylinder({ 0.0f, 0.0f, 0.0f }, axis_line_thickness * 2.0f, axis_line_thickness * 0.05f, axis_line_thickness * 4.0f, 15, { 200, 25, 25, 200 });
			rlPopMatrix();

			// Draw Y axis line.
			rlPushMatrix();
			rlTranslatef(surface_size_h, 0.0f, surface_size_h);
			DrawCylinder({ 0.0f, 0.0f, 0.0f }, axis_line_thickness, axis_line_thickness, surface_size_h - margin, 10, { 25, 200, 25, 200 });
			rlPopMatrix();

			// Draw Y axis arrow head.
			rlPushMatrix();
			rlTranslatef(surface_size_h, surface_size_h + axis_line_thickness*4.0f - margin, surface_size_h);
			rlRotatef(180.0f, 1.0f, 0.0f, 0.0f);
			DrawCylinder({ 0.0f, 0.0f, 0.0f }, axis_line_thickness * 2.0f, axis_line_thickness * 0.05f, axis_line_thickness * 4.0f, 15, { 25, 200, 25, 200 });
			rlPopMatrix();

			// Draw Z axis line.
			rlPushMatrix();
			rlTranslatef(surface_size_h, 0.0f, -surface_size_h);
			rlRotatef(90.0f, 1.0f, 0.0f, 0.0f);
			DrawCylinder({ 0.0f, 0.0f, 0.0f }, axis_line_thickness, axis_line_thickness, surface_size - margin, 10, { 25, 25, 200, 200 });
			rlPopMatrix();

			// Draw Z axis arrow head.
			rlPushMatrix();
			rlTranslatef(surface_size_h, 0.0f, surface_size_h + axis_line_thickness*4.0f - margin);
			rlRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
			DrawCylinder({ 0.0f, 0.0f, 0.0f }, axis_line_thickness * 2.0f, 0.0f, axis_line_thickness * 4.0f, 350, { 25, 25, 200, 255 });
			rlPopMatrix();*/

			// Moving Object Debug

			// X Axis
			/*rlPushMatrix();
			rlTranslatef(step, 0.0f, 0.0f);
			DrawCube({ 0.0f, 0.5f, 0.0f }, 1.0f, 1.0f, 1.0f, { 200, 25, 25, 200 });
			rlPopMatrix();

			// Y Axis
			rlPushMatrix();
			rlTranslatef(0.0f, step, 0.0f);
			DrawCube({ 0.0f, 0.5f, 0.0f }, 1.0f, 1.0f, 1.0f, { 25, 200, 25, 200 });
			rlPopMatrix();

			// Z Axis
			rlPushMatrix();
			rlTranslatef(0.0f, 0.0f, step);
			DrawCube({ 0.0f, 0.5f, 0.0f }, 1.0f, 1.0f, 1.0f, { 25, 25, 200, 200 });
			rlPopMatrix();*/

			step += 0.005f;

			// Draw the "surface" plane
			DrawPlane({ 0.0f, -0.005f, 0.0f }, { surface_size, surface_size }, { 80, 80, 80, 200 });
			// Add a grid ontop of the "surface" plane
			DrawGrid(int(surface_size / grid_spacing), grid_spacing);

			this->object_renderer.draw();

			EndMode3D();

			DrawFPS(10, 20);

			DrawText(("Mesh Count: " + std::to_string(this->object_renderer.object_count)).c_str(), 10, 80, 20, {25, 255, 25, 255});

			DrawText(("Instanced Mesh Count: " + std::to_string(this->object_renderer.instanced_mesh_count)).c_str(), 10, 100, 20, { 25, 255, 25, 255 });

			if (IsKeyDown(KEY_G)) {
				DrawText("G Pressed", 10, 40, 20, YELLOW);
			}

			if (IsKeyDown(KEY_H)) {
				DrawText("H Pressed", 10, 60, 20, YELLOW);
			}

			

			EndDrawing();
		}
};

#endif