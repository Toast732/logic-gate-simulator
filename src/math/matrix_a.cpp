#include "matrix_a.hpp"

#ifndef MATRIX_A_CPP
#define MATRIX_A_CPP

typedef std::shared_ptr<Matrix> SMatrix;

// Clones a matrix.
inline Matrix MatrixClone(Matrix matrix) {
	Matrix cloned = { 0 };
	
	// clone the matrix
	cloned.m0 = matrix.m0;
	cloned.m1 = matrix.m1;
	cloned.m2 = matrix.m2;
	cloned.m3 = matrix.m3;
	cloned.m4 = matrix.m4;
	cloned.m5 = matrix.m5;
	cloned.m6 = matrix.m6;
	cloned.m7 = matrix.m7;
	cloned.m8 = matrix.m8;
	cloned.m9 = matrix.m9;
	cloned.m10 = matrix.m10;
	cloned.m11 = matrix.m11;
	cloned.m12 = matrix.m12;
	cloned.m13 = matrix.m13;
	cloned.m14 = matrix.m14;
	cloned.m15 = matrix.m15;

	return cloned;
}

// Clones a matrix, gives back as a smart pointer.
inline SMatrix MatrixClonePTR(Matrix matrix) {
	SMatrix cloned = std::make_shared<Matrix>(Matrix(matrix));
	//Matrix cloned = { 0 };

	// clone the matrix
	cloned.get()->m0 = matrix.m0;
	cloned.get()->m1 = matrix.m1;
	cloned.get()->m2 = matrix.m2;
	cloned.get()->m3 = matrix.m3;
	cloned.get()->m4 = matrix.m4;
	cloned.get()->m5 = matrix.m5;
	cloned.get()->m6 = matrix.m6;
	cloned.get()->m7 = matrix.m7;
	cloned.get()->m8 = matrix.m8;
	cloned.get()->m9 = matrix.m9;
	cloned.get()->m10 = matrix.m10;
	cloned.get()->m11 = matrix.m11;
	cloned.get()->m12 = matrix.m12;
	cloned.get()->m13 = matrix.m13;
	cloned.get()->m14 = matrix.m14;
	cloned.get()->m15 = matrix.m15;

	// return the cloned matrix.
	return cloned;
}

// Turns a smart pointer matrix in a normal matrix.
inline Matrix MatrixFromSMatrix(SMatrix smatrix) {
	return *smatrix.get();
}


// Adds just the positions of two matrices.
inline Matrix MatrixAddPosition(Matrix matrix1, Matrix matrix2) {
	Matrix result = MatrixClone(matrix1);

	result.m12 += matrix2.m12;
	result.m13 += matrix2.m13;
	result.m14 += matrix2.m14;

	return result;
}

// Subtracts just the positions of two matrices. (matrix1 - matrix2)
inline Matrix MatrixSubtractPosition(Matrix matrix1, Matrix matrix2) {
	Matrix result = MatrixClone(matrix1);

	result.m12 -= matrix2.m12;
	result.m13 -= matrix2.m13;
	result.m14 -= matrix2.m14;

	return result;
}

// Prints a matrix.
inline void MatrixPrint(Matrix matrix) {
	// print all fields from m0 to m15, seperate each with "\n> "
	std::cout << "Matrix:\n> m0: " << matrix.m0 << "\n> m1: " << matrix.m1 << "\n> m2: " << matrix.m2 << "\n> m3: " << matrix.m3 << "\n> m4: " << matrix.m4 << "\n> m5: " << matrix.m5 << "\n> m6: " << matrix.m6 << "\n> m7: " << matrix.m7 << "\n> m8: " << matrix.m8 << "\n> m9: " << matrix.m9 << "\n> m10: " << matrix.m10 << "\n> m11: " << matrix.m11 << "\n> m12: " << matrix.m12 << "\n> m13: " << matrix.m13 << "\n> m14: " << matrix.m14 << "\n> m15: " << matrix.m15 << "\n";	
}

// Multiplies a matrix around a rotation matrix.
inline Matrix MatrixMultiplyRotation(Matrix base_matrix, Matrix rotation_matrix) {
	Matrix result = MatrixClone(base_matrix);

	//MatrixRotateXYZ(rotation_matrix);

	// Multiply the matrices in column major order.
	result.m0 = base_matrix.m0 * rotation_matrix.m0 + base_matrix.m4 * rotation_matrix.m1 + base_matrix.m8 * rotation_matrix.m2 + base_matrix.m12 * rotation_matrix.m3;
	result.m1 = base_matrix.m1 * rotation_matrix.m0 + base_matrix.m5 * rotation_matrix.m1 + base_matrix.m9 * rotation_matrix.m2 + base_matrix.m13 * rotation_matrix.m3;
	result.m2 = base_matrix.m2 * rotation_matrix.m0 + base_matrix.m6 * rotation_matrix.m1 + base_matrix.m10 * rotation_matrix.m2 + base_matrix.m14 * rotation_matrix.m3;
	result.m3 = base_matrix.m3 * rotation_matrix.m0 + base_matrix.m7 * rotation_matrix.m1 + base_matrix.m11 * rotation_matrix.m2 + base_matrix.m15 * rotation_matrix.m3;

	result.m4 = base_matrix.m0 * rotation_matrix.m4 + base_matrix.m4 * rotation_matrix.m5 + base_matrix.m8 * rotation_matrix.m6 + base_matrix.m12 * rotation_matrix.m7;
	result.m5 = base_matrix.m1 * rotation_matrix.m4 + base_matrix.m5 * rotation_matrix.m5 + base_matrix.m9 * rotation_matrix.m6 + base_matrix.m13 * rotation_matrix.m7;
	result.m6 = base_matrix.m2 * rotation_matrix.m4 + base_matrix.m6 * rotation_matrix.m5 + base_matrix.m10 * rotation_matrix.m6 + base_matrix.m14 * rotation_matrix.m7;
	result.m7 = base_matrix.m3 * rotation_matrix.m4 + base_matrix.m7 * rotation_matrix.m5 + base_matrix.m11 * rotation_matrix.m6 + base_matrix.m15 * rotation_matrix.m7;

	result.m8 = base_matrix.m0 * rotation_matrix.m8 + base_matrix.m4 * rotation_matrix.m9 + base_matrix.m8 * rotation_matrix.m10 + base_matrix.m12 * rotation_matrix.m11;
	result.m9 = base_matrix.m1 * rotation_matrix.m8 + base_matrix.m5 * rotation_matrix.m9 + base_matrix.m9 * rotation_matrix.m10 + base_matrix.m13 * rotation_matrix.m11;
	result.m10 = base_matrix.m2 * rotation_matrix.m8 + base_matrix.m6 * rotation_matrix.m9 + base_matrix.m10 * rotation_matrix.m10 + base_matrix.m14 * rotation_matrix.m11;
	result.m11 = base_matrix.m3 * rotation_matrix.m8 + base_matrix.m7 * rotation_matrix.m9 + base_matrix.m11 * rotation_matrix.m10 + base_matrix.m15 * rotation_matrix.m11;

	result.m12 = base_matrix.m0 * rotation_matrix.m12 + base_matrix.m4 * rotation_matrix.m13 + base_matrix.m8 * rotation_matrix.m14 + base_matrix.m12 * rotation_matrix.m15;
	result.m13 = base_matrix.m1 * rotation_matrix.m12 + base_matrix.m5 * rotation_matrix.m13 + base_matrix.m9 * rotation_matrix.m14 + base_matrix.m13 * rotation_matrix.m15;
	result.m14 = base_matrix.m2 * rotation_matrix.m12 + base_matrix.m6 * rotation_matrix.m13 + base_matrix.m10 * rotation_matrix.m14 + base_matrix.m14 * rotation_matrix.m15;
	result.m15 = base_matrix.m3 * rotation_matrix.m12 + base_matrix.m7 * rotation_matrix.m13 + base_matrix.m11 * rotation_matrix.m14 + base_matrix.m15 * rotation_matrix.m15;

	return result;
}

// returns a matrix from a given vector3.
inline Matrix MatrixFromVector3(Vector3 v) {
	return MatrixTranslate(v.x, v.y, v.z);
}

inline float VectorMagnitude(Vector3 v) {
	// x squared
	float xs = v.x * v.x;
	// y squared
	float ys = v.y * v.y;
	// z squared
	float zs = v.z * v.z;
	// return all added together square rooted.
	return sqrt(xs + ys + zs);
}

inline Vector3 RotateAtDistance(Vector3 pivot_pos, Vector3 pos, Vector3 rotation) {
	// Pivot around the pivot with the specified distance, put in desired rotation.
	//std::cout << "Normal Pivot:\nx: " << normal_pivot.x << "\ny: " << normal_pivot.y << "\nz: " << normal_pivot.z << "\nm: " << VectorMagnitude(normal_pivot) << "\n";
	
	/*Vector3 pivot = {1.0f, 1.0f, 1.0f};
	Vector3 result = Vector3RotateByAxisAngle(pivot, { 1.0f, 1.0f, 0.0f }, rotation.x);
	result = Vector3RotateByAxisAngle(result, { 0.0f, 1.0f, 1.0f }, rotation.y);
	result = Vector3RotateByAxisAngle(result, { 1.0f, 0.0f, 1.0f }, rotation.z);

	float x = sin(rotation.x)

	// Add the distance to the result.
	result = Vector3Scale(result, dist);

	return result;*/

	// Alias
	float pitch = rotation.y;
	float yaw = rotation.z;
	float roll = rotation.x;

	// Translate to the pivot position
	float translatedX = pivot_pos.x - pos.x;
	float translatedY = pivot_pos.y - pos.y;
	float translatedZ = pivot_pos.z - pos.z;

	// precalculate some trig values
	float sr = sin(roll);
	float sy = sin(yaw);
	float sp = sin(pitch);

	float cr = cos(roll);
	float cy = cos(yaw);
	float cp = cos(pitch);

	// Apply rotation
	float rotatedX = (cy * (cp * translatedX + sp * sr * translatedY - sp * cr * translatedZ) - sy * (cr * translatedY + sr * translatedZ));
	float rotatedY = (sy * (cp * translatedX + sp * sr * translatedY - sp * cr * translatedZ) + cy * (cr * translatedY + sr * translatedZ));
	float rotatedZ = (sp * translatedX - cp * sr * translatedY + cp * cr * translatedZ);

	// Translate back to the original position
	float finalX = rotatedX + pos.x;
	float finalY = rotatedY + pos.y;
	float finalZ = rotatedZ + pos.z;

	return { finalX, finalY, finalZ };
}

// gets the XZ distance between the vectors.
inline float Vector3Distance2D(Vector3 v1, Vector3 v2) {
	float rx = v1.x - v2.x;
	float rz = v1.z - v2.z;

	return sqrtf(rx * rx + rz * rz);
}

// gets the XZ length of the vector.
inline float Vector3Length2D(Vector3 v) {
	return sqrtf(v.x * v.x + v.z * v.z);
}

inline Vector3 Vector3FromMatrix(Matrix matrix) {
	return { matrix.m12, matrix.m13, matrix.m14 };
}

inline static float wrap(float x, float min, float max) {
	return fmod((x - min) / (max - min), 1) * (max - min) + min;
}

// gets the yaw and pitch between the two vectors, x = yaw, y = pitch
inline Vector2 Vector3AngleBetween(Vector3 v1, Vector3 v2) {

	// XZ distance between the vectors.
	float xz_distance = Vector3Distance2D(v1, v2);

	// Return angles
	return {
		atan2f(v2.x - v1.x, v2.z - v1.z),
		(v2.y - v1.y) / xz_distance
	};

	//// get the direction by subracting v2 from v1
	//Vector3 direction = Vector3Subtract(v1, v2);

	//// get the length of the vector, used to calculate pitch.
	//float len = Vector3Length2D(direction);

	//return {
	//	atan2f(direction.z, direction.x),
	//	(float)(atan2f(direction.y, len))
	//};

	// normalise the vectors
	//Vector3 nv1 = Vector3Normalize(v1);
	//Vector3 nv2 = Vector3Normalize(v2);

	//float angle = acos(Vector3DotProduct(nv1, nv2));
	//Vector3 axis = Vector3Normalize(Vector3CrossProduct(nv1, nv2));

	////float yaw = Vector2Angle({ 0.0f, 0.0f, }, { axis.x, axis.z });

	//return {
	//	angle * axis.x,
	//	angle * axis.z
	//};
}

inline static float Vector3MagnitudeAngleBetween(Vector3 v1, Vector3 v2) {
	Vector2 angle_between = Vector3AngleBetween(v1, v2);

	return (angle_between.x + angle_between.y) * 0.5f;

	return wrap((angle_between.x + angle_between.y) * 0.5f, PI * -0.5, PI * 0.5);
}

#endif