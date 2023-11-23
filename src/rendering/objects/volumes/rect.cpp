#include "rect.hpp"

#ifndef RECT_CPP
#define RECT_CPP

class Rect {
private:
	
public:
	SMatrix matrix;
	MeshData mesh;
	std::vector<MeshData> meshes;
	RenderObject render_object;
	Color colour;

	Rect() {}

	// generic creating rectangle.
	Rect(
		Vector3 position,
		Vector3 rotation,
		float length,
		float width,
		float height,
		Color colour)
	{
		this->updatePosition(position, rotation);

		this->mesh=MeshData(
			GenMeshCube(width, height, length),
			this->matrix,
			colour
		);

		new (&this->render_object) RenderObject(this->meshes);
	}

	// creating rectangle with a matrix.
	Rect(
		Matrix matrix,
		float length,
		float width,
		float height,
		Color colour)
	{
		// the given matrices' memory address.
		//std::cout << "Given Matrix Memory: " << &matrix << "\n";

		// update position
		this->updatePosition(matrix);

		// generate mesh data
		this->mesh=MeshData(
			GenMeshCube(width, height, length),
			this->matrix,
			colour
		);

		// print stored matrices' memory address.
		//std::cout << "Matrix Memory: " << &this->matrix << "\n";

		// add the generated mesh data to the meshes vector.
		this->meshes.push_back(this->mesh);

		// create the render object.
		new (&this->render_object) RenderObject(this->meshes);
	}

	// creating rectangle with a material.
	Rect(
		Matrix matrix,
		float length,
		float width,
		float height,
		SMaterial material)
	{
		// the given matrices' memory address.
		//std::cout << "Given Matrix Memory: " << &matrix << "\n";

		// update position
		this->updatePosition(matrix);

		// generate mesh data
		this->mesh = MeshData(
			GenMeshCube(width, height, length),
			this->matrix,
			material
		);

		// print stored matrices' memory address.
		//std::cout << "Matrix Memory: " << &this->matrix << "\n";

		// add the generated mesh data to the meshes vector.
		this->meshes.push_back(this->mesh);

		// create the render object.
		new (&this->render_object) RenderObject(this->meshes);
	}

	// updating the position with a matrix.
	void updatePosition(Matrix matrix) {
		this->matrix = MatrixClonePTR(matrix);
	}

	// updating the position with position and rotation.
	void updatePosition(Vector3 position, Vector3 rotation) {
		this->matrix = MatrixClonePTR(MatrixMultiply(MatrixRotateXYZ(rotation), MatrixTranslate(position.x, position.y, position.z)));
	}
};
#endif