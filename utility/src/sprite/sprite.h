#ifndef __AVL_UTILITY_SPRITE__
#define __AVL_UTILITY_SPRITE__
/**********
 * Author: Sheldon Bachstein
 * Date: Jan 21, 2011
 * Description: Encapsulates a basic textured quadrilateral in 3-D space, oriented to be parallel to
 * the XY plane. Includes accessors and mutators for the spatial coordinates as well as the texture
 * coordinates.
 **********/


#include"..\vertex 2d\vertex 2d.h"


namespace avl
{
namespace utility
{
	// See the beginning of the file for details.
	class Sprite
	{
	public:
		// Constructors:
		// Initializes each vertice to the origin with texture coordinates of (0, 0).
		Sprite();
		// Initializes each of the vertices to those supplied by the user. Winding order is clockwise.
		// p1-p4 are the spatial coordinates, and q1-q4 are the texture coordinates. z is the z-depth
		// of the sprite.
		Sprite(const Vertex2D& p1, const Vertex2D& p2, const Vertex2D& p3, const Vertex2D& p4,
			   const Vertex2D& q1, const Vertex2D& q2, const Vertex2D& q3, const Vertex2D& q4 const float& z);
		// Copy constructor.
		Sprite(const Sprite& original);
		// Destructor.
		~Sprite();

		// Accessors:
		// Getters for each of the spatial coordinates.
		const Vertex2D& GetP1() const;
		const Vertex2D& GetP2() const;
		const Vertex2D& GetP3() const;
		const Vertex2D& GetP4() const;

		// Returns the center of the sprite.
		const Vertex2D& GetCenter() const;
		// Returns the current rotation of the sprite.
		const float& GetRotation() const;
		// Returns the current scale of the sprite.
		const float& GetScale() const;

		// Getters for each of the texture coordinates.
		const Vertex2D& GetQ1() const;
		const Vertex2D& GetQ2() const;
		const Vertex2D& GetQ3() const;
		const Vertex2D& GetQ4() const;

		// Returns the z-depth of the sprite.
		const float& GetZ() const;


		// Mutators:
		// Setters for each of the spatial coordinates.
		void SetP1(const Vertex2D& p);
		void SetP2(const Vertex2D& p);
		void SetP3(const Vertex2D& p);
		void SetP4(const Vertex2D& p);

		// Sets the center of the sprite to the new position.
		void SetCenter(const Vertex2D& new_center);
		// Moves the spatial coordinates along the vector delta.
		void Move(const Vertex2D& delta);
		// Sets the rotation to the rotation specified.
		void SetRotation(const float& theta);
		// Rotates the quad clockwise by the degrees specified.
		void Rotate(const float& delta_theta);
		// Sets the scale to the specified scale.
		void SetScale(const float& new_scale);
		// Scales the quad by the scaling factor specified.
		void Scale(const float& delta_scale);

		// Setters for each of the texture coordinates.
		void SetQ1(const Vertex2D& q);
		void SetQ2(const Vertex2D& q);
		void SetQ3(const Vertex2D& q);
		void SetQ4(const Vertex2D& q);

		// Moves the texture coordinates along the vector delta.
		void MoveTexture(const Vertex2D& delta);

		// Sets the z-depth of this sprite.
		void SetZ(const float& new_z);

		// Assignment operator.
		const Sprite& operator=(const Sprite& rhs);

	private:
		// The original four spatial vertices
		Vertex2D p1, p2, p3, p4;
		// The transformed spatial coordinates.
		Vertex2D transformed_p1, transformed_p2, transformed_p3, transformed_p4;

		// The center of the sprite.
		Vertex2D center;
		// The current rotation.
		float rotation;
		// The current scale.
		float scale;

		// The z-depth of the sprite.
		float z;

		// The texture coordinates.
		Vertex2D q1, q2, q3, q4;
	};




} //avl
} //utility
#endif // __AVL_UTILITY_SPRITE__