#include "vector"
#include "logic_gate_io.cpp"
#include "../rendering/objects/mesh_data.cpp"
#include "iostream"
#include "raylib-cpp.hpp"
#include "../math/rand.cpp"
#include "../rendering/objects/instanced_mesh_factory.cpp"
#include "../simulation/ticker.cpp"
#include "../storage/object_storage.cpp"

#pragma once

class PipeParticle {
	private:
		float speed = 0.0f;
		SMatrix matrix;
		int instance_id;
		Matrix start_offset;
		Matrix end_offset;
	public:
		SMatrix start_pos;
		SMatrix end_pos;
		bool is_trash = false;
		float progress = 0.0f;

		PipeParticle(Matrix start_pos, Matrix end_pos);
		void update();
};

class LogicPipe {
	private:
		LogicIO* source;
		LogicIO* target;
		std::vector<int> instance_ids;
		std::vector<PipeParticle> particles;
		int tick_counter = 0;

		SMatrix calculateMatrix();
	public:
		LogicPipe(LogicIO* source, LogicIO* target);
		void update();

};