#ifndef __AVL_UTILITY_VERTEX_2D__
#define __AVL_UTILITY_VERTEX_2D__
/**********
 * Author: Sheldon Bachstein
 * Date: Jan 17, 2011
 * Description: Encapsulates a single 2-dimensional vertex.
 **********/



namespace avl
{
namespace utility
{
	// See the beginning of the file for details.
	class Vertex2D
	{
	public:
		// Constructors:
		Vertex2D();
		Vertex2D(const float& x, const float& y);
		~Vertex2D();
		Vertex2D(const Vertex2D& original);

		// Accessors:
		// Getters.
		const float& GetX() const;
		const float& GetY() const;
		// Comparison operators.
		const bool operator==(const Vertex2D& rhs) const;
		const bool operator!=(const Vertex2D& rhs) const;

		// Mutators:
		// Setters.
		void SetX(const float& new_x);
		void SetY(const float& new_y);
		// Assignment operator.
		const Vertex2D& operator=(const Vertex2D& rhs);

	private:
		// Coordinates.
		float x;
		float y;
	};




} //avl
} //utility
#endif // __AVL_UTILITY_VERTEX_2D__