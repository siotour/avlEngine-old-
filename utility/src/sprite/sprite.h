#ifndef __AVL_UTILITY_SPRITE__
#define __AVL_UTILITY_SPRITE__
/**********
 * Author: Sheldon Bachstein
 * Date: Jan 21, 2011
 * Description: Encapsulates a basic textured quadrilateral in 3-D space, oriented to be parallel to
 * the XY plane. Includes accessors and mutators for the spatial coordinates as well as the texture
 * coordinates. Additionally, defines and a TextureHandle type which is used to represent textures.
 * Contains a TextureHandle member which can be modified and accessed as needed by a rendering system.
 **********/


#include"..\vertex 2d\vertex 2d.h"
#include<list>

namespace avl
{
namespace utility
{
	// See the beginning of the file for details.
	class Sprite
	{
	public:
		// Defines a type used to represent a handle to a texture used to cover this sprite.
		typedef unsigned int TextureHandle;
		// Defines a type for storing multiple Sprites in a light, sortable container.
		typedef std::list<Sprite* const> SpriteList;


		// Constructors:
		// Initializes each vertice to the origin with texture coordinates of (0, 0).
		Sprite();
		// Initializes each of the vertices to those supplied by the user. Winding order is clockwise.
		// p1-p4 are the spatial coordinates, and q1-q4 are the texture coordinates. z is the z-depth
		// of the sprite. texture_handle is the texture handle for the sprite.
		Sprite(const Vertex2D& p1, const Vertex2D& p2, const Vertex2D& p3, const Vertex2D& p4,
			   const Vertex2D& q1, const Vertex2D& q2, const Vertex2D& q3, const Vertex2D& q4,
			   const float& z, const TextureHandle texture_handle);
		// Creates an axis-aligned bounding box with texture coordinates ranging from (0, 0) in the
		// bottom-left to (1, 1) in the top-right and with z-depth z. texture_handle is the texture
		// handle of the sprite.
		Sprite(const float& left, const float& top, const float& right, const float& bottom,
			   const float& z, const TextureHandle texture_handle);
		// Copy constructor.
		Sprite(const Sprite& original);
		// Destructor.
		~Sprite();

		// Accessors:
		// Returns the visibility of the sprite.
		const bool IsVisible() const;

		// Returns the texture handle for this sprite.
		const TextureHandle GetTextureHandle() const;

		// Getters for each of the untransformed spatial vertices.
		const Vertex2D& GetUntransformedP1() const;
		const Vertex2D& GetUntransformedP2() const;
		const Vertex2D& GetUntransformedP3() const;
		const Vertex2D& GetUntransformedP4() const;

		// Getters for each of the spatial vertices.
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
		// Sets the sprite's visibility.
		void SetVisibility(const bool new_visibility);

		// Sets the texture handle for this sprite.
		void SetTextureHandle(const TextureHandle new_texture_handle);

		// Translates the sprite such that its center is at the specified vertex.
		void SetCenter(const Vertex2D& new_center);
		// Moves the spatial coordinates along the vector delta.
		void Move(const Vertex2D& delta);
		// Sets the rotation to the rotation specified. Rotation is counter-clockwise.
		void SetRotation(const float& new_rotation);
		// Rotates the quad counter-clockwise by the degrees specified.
		void Rotate(const float& delta_theta);
		// Sets the scale to the specified scale.
		void SetScale(float new_scale);
		// Scales the quad by the scaling factor specified.
		void Scale(const float& delta_scale);

		// Resets rotation and scale, and then sets the Sprite's vertices and z-depth
		// to chose specified.
		void ResetPosition(const Vertex2D& new_p1, const Vertex2D& new_p2, const Vertex2D& new_p3, const Vertex2D& new_p4, const float& new_z);
		// Resets the rotation and scale, and then aligns the Sprite's positions to
		// the boundaries and z-depth specified.
		void ResetPosition(const float& left, const float& top, const float& right, const float& bottom, const float& new_z);

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
		// Utility functions:
		// Finds the center of the sprite and assigns those coordinates to the member center.
		void FindCenter();
		// Rotates untransformed_point about the center_point by theta degrees counter-clockwise.
		Vertex2D RotateVertice(const Vertex2D& center_point, const Vertex2D& untransformed_point, float theta);
		// Scales untransformed_point in relation to center_point by scaling_factor.
		Vertex2D ScaleVertice(const Vertex2D& center_point, const Vertex2D& untransformed_point, const float& scale_factor);

		// Is this sprite currently visible or invisible?
		bool is_visible;

		// The handle of the texture for this sprite.
		TextureHandle texture_handle;

		// The untransformed spatial vertices.
		Vertex2D untransformed_p1, untransformed_p2, untransformed_p3, untransformed_p4;
		// The transformed spatial vertices
		Vertex2D p1, p2, p3, p4;

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