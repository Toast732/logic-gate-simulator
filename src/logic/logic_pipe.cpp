#include "vector"
#include "logic_gate_io.cpp"
#include "..\rendering\objects\mesh_data.cpp"
#include "iostream"
#include "raylib-cpp.hpp"
#include "..\math\rand.cpp"
#include "..\rendering\objects\instanced_mesh_factory.cpp"

#ifndef LOGIC_PIPE_CPP
#define LOGIC_PIPE_CPP

#define LOGIC_PIPE_THICKNESS 0.15
#define LOGIC_PIPE_SIDES 14
#define PIPE_PARTICLE_SIDES 15
#define PIPE_PARTICLE_RADIUS 0.025

inline float const HALF_PI = PI * 0.5;

inline void setup_logic_pipe() {

	// create pipe particle mesh
	CreateInstancedMesh(
		"pipe_particle",
		GenMeshSphere(PIPE_PARTICLE_RADIUS, PIPE_PARTICLE_SIDES, PIPE_PARTICLE_SIDES),
		{255, 255, 0, 255}
	);

	// create the pipe mesh
	CreateInstancedMesh(
		"logic_pipe",
		GenMeshCylinder(LOGIC_PIPE_THICKNESS, 1.0f, LOGIC_PIPE_SIDES),
		{ 200, 200, 200, 150 }
	);
}

class PipeParticle {
	public:
		SMatrix start_pos;
		SMatrix end_pos;
		bool is_trash = false;
		float progress = 0.0f;
		PipeParticle(Matrix start_pos, Matrix end_pos) {

			float RAND_POS_MULT = 0.5f;

			float x_rand_start = randf(-RAND_POS_MULT, RAND_POS_MULT, 5);
			float y_rand_start = randf(-RAND_POS_MULT, RAND_POS_MULT, 5);

			this->start_offset = MatrixFromVector3({ (float)(x_rand_start * LOGIC_PIPE_THICKNESS), (float)(y_rand_start * LOGIC_PIPE_THICKNESS), 0.0f });

			float x_rand_end = randf(-RAND_POS_MULT, RAND_POS_MULT, 5);
			float y_rand_end = randf(-RAND_POS_MULT, RAND_POS_MULT, 5);

			this->end_offset = MatrixFromVector3({ (float)(x_rand_end * LOGIC_PIPE_THICKNESS), (float)(y_rand_end * LOGIC_PIPE_THICKNESS), 0.0f });

			this->start_pos = std::make_shared<Matrix>(start_pos);
			this->end_pos = std::make_shared<Matrix>(end_pos);
			this->matrix = std::make_shared<Matrix>(MatrixAddPosition(start_pos, this->start_offset));

			this->instance_id = MeshInstances::Get("pipe_particle")->addInstance(this->matrix);

			this->speed = randf(1.1f, 1.4f, 5);
		}
		void update() {

			Vector3 _start_pos = Vector3FromMatrix(MatrixAddPosition(*this->start_pos, this->start_offset));
			Vector3 _end_pos = Vector3FromMatrix(MatrixAddPosition(*this->end_pos, this->end_offset));

			// get progress increment
			// get length of line
			float len = Vector3Distance(_start_pos, _end_pos);

			// get the progress increment
			float progress_inc = this->speed / len;

			// increment progress
			this->progress += progress_inc/144;

			// check if the progress is max.
			if (this->progress >= 1.0f) {
				// remove the object
				MeshInstances::Get("pipe_particle")->removeInstance(this->instance_id);
				// trash it
				this->is_trash = true;
				return;
			}

			// set new position
			Vector3 new_pos = Vector3Lerp(_start_pos, _end_pos, this->progress);

			this->matrix->m12 = new_pos.x;
			this->matrix->m13 = new_pos.y;
			this->matrix->m14 = new_pos.z;

			MeshInstances::Get("pipe_particle")->moveInstancePos(this->instance_id, this->matrix);
		}
	private:
		float speed = 0.0f;
		SMatrix matrix;
		int instance_id;
		Matrix start_offset;
		Matrix end_offset;
};

class LogicPipe {
	private:
		LogicIO* source;
		LogicIO* target;
		std::vector<int> instance_ids;
		std::vector<PipeParticle> particles;
		int tick_counter = 0;

		SMatrix calculateMatrix() {
			// get the logic IO's positions
			Vector3 source_pos = Vector3FromMatrix(*this->source->getPosition());
			Vector3 target_pos = Vector3FromMatrix(*this->target->getPosition());

			// get the distance between the source and target for scaling.
			float dist = Vector3Distance(source_pos, target_pos);

			// get the angle from the source and the target
			//Vector2 angle = Vector3AngleBetween(source_pos, target_pos);

			//// get the position matrix
			//Matrix pipe_matrix = MatrixFromVector3(source_pos);

			//// get the rotation matrix
			//Matrix rotation_matrix = MatrixRotateZYX({ angle.y, 0.0f, angle.x });

			//rotation_matrix = MatrixRotateZYX({ angle.x, 0.0f, angle.y });


			/*Matrix rotation_matrices[3] = {
				MatrixRotateX(-angle.x),
				MatrixRotateY(-angle.y),
				MatrixRotateZ(PI * -0.5f)
			};

			Matrix rotation_matrix = MatrixAdd(MatrixAdd(rotation_matrices[0], rotation_matrices[1]), rotation_matrices[2]);*/

			// scale the matrix
			/*rotation_matrix = MatrixMultiply(MatrixScale(1.0f, 1.0f, dist), QuaternionToMatrix(QuaternionFromVector3ToVector3(target_pos, source_pos)));

			rotation_matrix = QuaternionToMatrix(QuaternionFromVector3ToVector3(target_pos, source_pos));

			// add the position
			pipe_matrix = MatrixAddPosition(rotation_matrix, pipe_matrix);

			//pipe_matrix = QuaternionToMatrix(QuaternionFromVector3ToVector3(target_pos, source_pos));

			//Matrix pipe_matrix = MatrixLookAt(source_pos, target_pos, { 0.0f, 1.0f, 0.0f });

			pipe_matrix = MatrixMultiply(MatrixInvert(MatrixLookAt(source_pos, target_pos, { 0.0f, 1.0f, 0.0f })), MatrixFromVector3(source_pos));*/

			Matrix matrix = MatrixMultiply(MatrixScale(1.0f, dist, 1.0f), MatrixMultiply(MatrixRotateX(-HALF_PI), MatrixInvert(MatrixLookAt(source_pos, target_pos, { 0.0f, 1.0f, 0.0f }))));

			return std::make_shared<Matrix>(matrix);
		}
	public:
		LogicPipe(LogicIO* source, LogicIO* target) {
			this->source = source;
			this->target = target;

			this->instance_ids.push_back(MeshInstances::Get("logic_pipe")->addInstance(this->calculateMatrix()));
		}
		void update() {
			//this->target->setValue(this->source->getValue());

			MeshInstances::Get("logic_pipe")->moveInstancePos(this->instance_ids[0], this->calculateMatrix());

			SMatrix source_pos = this->source->getPosition();
			SMatrix target_pos = this->target->getPosition();

			bool output_is_on = this->target->getValue() == 1.0f;

			bool change_state = this->tick_counter % 10 == 1.0f;

			// update particles
			for (int particle_index = this->particles.size() - 1; particle_index >= 0; particle_index--) {

				PipeParticle* particle = &this->particles[particle_index];

				particle->start_pos = source_pos;
				particle->end_pos = target_pos;

				particle->update();

				// if this particle should be deleted
				if (particle->is_trash) {
					this->particles.erase(this->particles.begin() + particle_index);
					continue;
				}

				// if we might need to change state
				if (change_state) {
					// if the current output value is on
					if (output_is_on) {
						// check if this particle hit a progress threshold
						if (particle->progress >= 0.9f) {
							// we dont need to switch states
							change_state = false;
						}
					}
					else {
						// if the current output value is off
						if (particle->progress >= 0.95f) {
							// switch state to on, and set change_state to off
							this->target->setValue(1.0f);
							change_state = false;
						}
					}
				}
			}

			if (output_is_on && change_state) {
				this->target->setValue(0.0f);
			}

			// every 0.5 seconds
			if (this->tick_counter % 2 == 0) {
				// if the pipe input is on.
				if (this->source->getValue() == 1.0f) {
					// create a particle
					this->particles.push_back(PipeParticle(*source_pos, *target_pos));
				}
			}

			// increment tick counter
			this->tick_counter++;
		}
};

#endif