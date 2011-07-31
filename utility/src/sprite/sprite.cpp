/**********
 * Author: Sheldon Bachstein
 * Date: Jan 21, 2011
 * Description: See textured quad.h for details.
 **********/

#include"textured quad.h"
#include"..\textured vertex\textured vertex.h"
#include"..\assert\assert.h"
#include<cmath>



namespace avl
{
namespace utility
{


	// Basic constructor.
	TexturedQuad::TexturedQuad()
		: p1(0, 0, 0, 0, 0), p2(0, 0, 0, 0, 0), p3(0, 0, 0, 0, 0), p4(0, 0, 0, 0, 0)
	{
	}




	// Takes four vertices which will form the resultant quad. Vertices are copied in a clockwise direction.
	TexturedQuad::TexturedQuad(const TexturedVertex& p1, const TexturedVertex& p2, const TexturedVertex& p3, const TexturedVertex& p4)
		: p1(p1), p2(p2), p3(p3), p4(p4)
	{
	}




	// Copy constructor.
	TexturedQuad::TexturedQuad(const TexturedQuad& original)
		: p1(original.GetP1()), p2(original.GetP2()), p3(original.GetP3()), p4(original.GetP4())
	{
	}




	// Basic desctructor.
	TexturedQuad::~TexturedQuad()
	{
	}




	// Returns vertex p1.
	const TexturedVertex& TexturedQuad::GetP1() const
	{
		return p1;
	}




	// Returns vertex p2.
	const TexturedVertex& TexturedQuad::GetP2() const
	{
		return p2;
	}




	// Returns vertex p3.
	const TexturedVertex& TexturedQuad::GetP3() const
	{
		return p3;
	}




	// Returns vertex p4.
	const TexturedVertex& TexturedQuad::GetP4() const
	{
		return p4;
	}




	// Assignment operator.
	const TexturedQuad& TexturedQuad::operator=(const TexturedQuad& rhs)
	{
		p1 = rhs.GetP1();
		p2 = rhs.GetP2();
		p3 = rhs.GetP3();
		p4 = rhs.GetP4();
		return *this;
	}




	// Moves each vertex along each axis by the amounts specified.
	void TexturedQuad::Move(const float& delta_x, const float& delta_y, const float& delta_z)
	{
		ASSERT(p1 != p2 && p2 != p3 && p3 != p4 && p4 != p1);
		// Move each vertex along each of the three axes by the amounts specified.
		p1.SetX(p1.GetX() + delta_x);
		p1.SetY(p1.GetY() + delta_y);
		p1.SetZ(p1.GetZ() + delta_z);

		p2.SetX(p2.GetX() + delta_x);
		p2.SetY(p2.GetY() + delta_y);
		p2.SetZ(p2.GetZ() + delta_z);

		p3.SetX(p3.GetX() + delta_x);
		p3.SetY(p3.GetY() + delta_y);
		p3.SetZ(p3.GetZ() + delta_z);

		p4.SetX(p4.GetX() + delta_x);
		p4.SetY(p4.GetY() + delta_y);
		p4.SetZ(p4.GetZ() + delta_z);
	}




	// TODO: This function inadvertantly scales the quad down in addition to rotating it! The quad continually
	// shrinks in on itself until the first assertion fails.
	// Rotates the quad clockwise by the degrees specified.
	void TexturedQuad::Rotate(const float& delta_theta)
	{
		ASSERT(p1 != p2 && p2 != p3 && p3 != p4 && p4 != p1);
		// If delta_theta is either 0 or a multiple of 360, then simply return.
		if(fmod(delta_theta, 360.0f) == 0.0f)
		{
			return;
		}


		// First of all, the centroid of the quad lies in the same plane as the
		// midpoints along each edge of the quad. That is, the centroid lies in
		// the same plane as the midpoint between p1 and p2, p2 and p3, and p3
		// p4. From there, find the vector perpendicular to that plane by taking
		// the cross product of two vectors within the plane (namely, vectors
		// the centroid to one of the midpoints along the quad edges). And then
		// use a rotation matrix on each vertex.

		// Calculate the centroid for the quad (which is basically the average of the
		// of each dimensions).
		float centroid_x = (p1.GetX() + p2.GetX() + p3.GetX() + p4.GetX())/4.0f;
		float centroid_y = (p1.GetY() + p2.GetY() + p3.GetY() + p4.GetY())/4.0f;
		float centroid_z = (p1.GetZ() + p2.GetZ() + p3.GetZ() + p4.GetZ())/4.0f;

		// Calculate the midpoint between p1 and p2.
		float m12_x = (p1.GetX() + p2.GetX())/2.0f;
		float m12_y = (p1.GetY() + p2.GetY())/2.0f;
		float m12_z = (p1.GetZ() + p2.GetZ())/2.0f;
		// Calculate the midpoint between p2 and p3.
		float m23_x = (p2.GetX() + p3.GetX())/2.0f;
		float m23_y = (p2.GetY() + p3.GetY())/2.0f;
		float m23_z = (p2.GetZ() + p3.GetZ())/2.0f;

		// Find the vector from the centroid to the midpoint between p1 and p2.
		float c_to_m12_x = m12_x - centroid_x;
		float c_to_m12_y = m12_y - centroid_y;
		float c_to_m12_z = m12_z - centroid_z;

		// Find the vector from the centroid to the midpoint between p2 and p3.
		float c_to_m23_x = m23_x - centroid_x;
		float c_to_m23_y = m23_y - centroid_y;
		float c_to_m23_z = m23_z - centroid_z;


		// Now that we have two vectors in the center plane, find a vector
		// perpendicular to both. This vector will be used to rotate each
		// vertex around.
		float rot_vector_x = (c_to_m12_y * c_to_m23_z) - (c_to_m12_z * c_to_m23_y);
		float rot_vector_y = (c_to_m12_z * c_to_m23_x) - (c_to_m12_x * c_to_m23_z);
		float rot_vector_z = (c_to_m12_x * c_to_m23_y) - (c_to_m12_y * c_to_m23_x);


		// Now construct a rotation matrix from the rotation vector.
		float rot_matrix[3][3];
		const float PI = 3.14159265f;
		float c = cos(delta_theta * PI / 180.0f);
		float s = sin(delta_theta * PI / 180.0f);
		float t = 1.0f - c;
		rot_matrix[0][0] = 1 + t * (rot_vector_x * rot_vector_x - 1);
		rot_matrix[0][1] = (-rot_vector_z * s) + (t * rot_vector_x * rot_vector_y);
		rot_matrix[0][2] = (rot_vector_y * s) + (t * rot_vector_x * rot_vector_z);
		rot_matrix[1][0] = (rot_vector_z * s) + (t * rot_vector_x * rot_vector_y);
		rot_matrix[1][1] = 1 + t * (rot_vector_y * rot_vector_y - 1);
		rot_matrix[1][2] = (-rot_vector_x * s) + (t * rot_vector_y * rot_vector_z);
		rot_matrix[2][0] = (-rot_vector_y * s) + (t * rot_vector_x * rot_vector_z);
		rot_matrix[2][1] = (rot_vector_x * s) + (t * rot_vector_y * rot_vector_z);
		rot_matrix[2][2] = 1 + t * (rot_vector_z * rot_vector_z - 1);


		// Now, for each vertex of the quad, rotate it by multiplaying it by the
		// rotation matrix.
		float old_x, old_y, old_z, new_x, new_y, new_z;

		// Rotate p1.
		old_x = p1.GetX() - centroid_x;
		old_y = p1.GetY() - centroid_y;
		old_z = p1.GetZ() - centroid_z;
		new_x = (old_x * rot_matrix[0][0]) + (old_y * rot_matrix[1][0]) + (old_z * rot_matrix[2][0]) + centroid_x;
		new_y = (old_x * rot_matrix[0][1]) + (old_y * rot_matrix[1][1]) + (old_z * rot_matrix[2][1]) + centroid_y;
		new_z = (old_x * rot_matrix[0][2]) + (old_y * rot_matrix[1][2]) + (old_z * rot_matrix[2][2]) + centroid_z;
		p1.SetX(new_x);
		p1.SetY(new_y);
		p1.SetZ(new_z);

		// Rotate p2.
		old_x = p2.GetX() - centroid_x;
		old_y = p2.GetY() - centroid_y;
		old_z = p2.GetZ() - centroid_z;
		new_x = (old_x * rot_matrix[0][0]) + (old_y * rot_matrix[1][0]) + (old_z * rot_matrix[2][0]) + centroid_x;
		new_y = (old_x * rot_matrix[0][1]) + (old_y * rot_matrix[1][1]) + (old_z * rot_matrix[2][1]) + centroid_y;
		new_z = (old_x * rot_matrix[0][2]) + (old_y * rot_matrix[1][2]) + (old_z * rot_matrix[2][2]) + centroid_z;
		p2.SetX(new_x);
		p2.SetY(new_y);
		p2.SetZ(new_z);

		// Rotate p3.
		old_x = p3.GetX() - centroid_x;
		old_y = p3.GetY() - centroid_y;
		old_z = p3.GetZ() - centroid_z;
		new_x = (old_x * rot_matrix[0][0]) + (old_y * rot_matrix[1][0]) + (old_z * rot_matrix[2][0]) + centroid_x;
		new_y = (old_x * rot_matrix[0][1]) + (old_y * rot_matrix[1][1]) + (old_z * rot_matrix[2][1]) + centroid_y;
		new_z = (old_x * rot_matrix[0][2]) + (old_y * rot_matrix[1][2]) + (old_z * rot_matrix[2][2]) + centroid_z;
		p3.SetX(new_x);
		p3.SetY(new_y);
		p3.SetZ(new_z);

		// Rotate p4.
		old_x = p4.GetX() - centroid_x;
		old_y = p4.GetY() - centroid_y;
		old_z = p4.GetZ() - centroid_z;
		new_x = (old_x * rot_matrix[0][0]) + (old_y * rot_matrix[1][0]) + (old_z * rot_matrix[2][0]) + centroid_x;
		new_y = (old_x * rot_matrix[0][1]) + (old_y * rot_matrix[1][1]) + (old_z * rot_matrix[2][1]) + centroid_y;
		new_z = (old_x * rot_matrix[0][2]) + (old_y * rot_matrix[1][2]) + (old_z * rot_matrix[2][2]) + centroid_z;
		p4.SetX(new_x);
		p4.SetY(new_y);
		p4.SetZ(new_z);
	}




	// Scales the quad by the scaling factor specified.
	void TexturedQuad::Scale(const float& scaling_factor)
	{
		ASSERT(p1 != p2 && p2 != p3 && p3 != p4 && p4 != p1);
		ASSERT(scaling_factor != 0.0f);
		// If the scaling factor is 1.0f, then simply return.
		if(scaling_factor == 1.0f)
		{
			return;
		}

		// Calculate the centroid for the quad (which is basically the average
		// of each dimension).
		float centroid_x = (p1.GetX() + p2.GetX() + p3.GetX() + p4.GetX())/4.0f;
		float centroid_y = (p1.GetY() + p2.GetY() + p3.GetY() + p4.GetY())/4.0f;
		float centroid_z = (p1.GetZ() + p2.GetZ() + p3.GetZ() + p4.GetZ())/4.0f;

		// Scale each vertice's disance from the centroid by the scaling factor.
		p1.SetX(scaling_factor * (p1.GetX() - centroid_x) + centroid_x);
		p1.SetY(scaling_factor * (p1.GetY() - centroid_y) + centroid_y);
		p1.SetZ(scaling_factor * (p1.GetZ() - centroid_z) + centroid_z);

		p2.SetX(scaling_factor * (p2.GetX() - centroid_x) + centroid_x);
		p2.SetY(scaling_factor * (p2.GetY() - centroid_y) + centroid_y);
		p2.SetZ(scaling_factor * (p2.GetZ() - centroid_z) + centroid_z);

		p3.SetX(scaling_factor * (p3.GetX() - centroid_x) + centroid_x);
		p3.SetY(scaling_factor * (p3.GetY() - centroid_y) + centroid_y);
		p3.SetZ(scaling_factor * (p3.GetZ() - centroid_z) + centroid_z);

		p4.SetX(scaling_factor * (p4.GetX() - centroid_x) + centroid_x);
		p4.SetY(scaling_factor * (p4.GetY() - centroid_y) + centroid_y);
		p4.SetZ(scaling_factor * (p4.GetZ() - centroid_z) + centroid_z);
	}



}
}