#ifndef __AVL_UTILITY_VERTEX_2D__
#define __AVL_UTILITY_VERTEX_2D__
/**
@file
Defines the utility::Vertex2D class which encapsulates a single 2-dimensional vertex.
@author Sheldon Bachstein
@date Jan 17, 2011
*/



namespace avl
{
namespace utility
{
	/** Encapsulates a 2 dimensional vertex. */
	class Vertex2D
	{
	public:
		/** Default destructor.
		@post \ref x and \ref y will be initialized to 0.
		*/
		Vertex2D();
		/** Full-spec constructor.
		@param x The coordinate along the x axis.
		@param y The coordinate along the y axis.
		*/
		Vertex2D(const float& x, const float& y);
		/** Basic destructor.*/
		~Vertex2D();
		/** Copy constructor.
		@param original The vertice being copied.
		*/
		Vertex2D(const Vertex2D& original);

		/** Returns this vertex's x coordinate.
		@return The x coordinate of this vertex.
		*/
		const float& GetX() const;
		/** Returns this vertex's y coordinate.
		@return The y coordinate of this vertex.
		*/
		const float& GetY() const;
		
		/** Compares for equality.
		@param rhs The vertex against which to compare this one.
		*/
		const bool operator==(const Vertex2D& rhs) const;
		/** Compares for inequality.
		@param rhs The vertex against which to compare this one.
		*/
		const bool operator!=(const Vertex2D& rhs) const;

		/** Sets this vertex's x coordinate.
		@param new_x The new x coordinate.
		*/
		void SetX(const float& new_x);
		/** Sets this vertex's y coordinate.
		@param new_y The new y coordinate.
		*/
		void SetY(const float& new_y);
		
		/** Addition operator.
		@param rhs The other vertex with which to sum this one.
		@return The vertex which is the sum of this vertex and \a rhs.
		*/
		Vertex2D operator+(const Vertex2D& rhs) const;
		/** Subtraction operator.
		@param rhs The other vertex with which to form the different with this one.
		@return The vertex which is the difference of this vertex and \a rhs.
		*/
		Vertex2D operator-(const Vertex2D& rhs) const;
		
		/** Adds \a rhs to this vertex.
		@param rhs The vertex to add to this one.
		@return This vertex after it has had \a rhs added to it.
		*/
		Vertex2D& operator+=(const Vertex2D& rhs);
		/** Subtracts \a rhs from this vertex.
		@param rhs The vertex to sutract from this one.
		@return This vertex after it has had \a rhs subtracted from it.
		*/
		Vertex2D& operator-=(const Vertex2D& rhs);
		
		/** Assignment operator.
		@param rhs The vertex to set this one equal to.
		@return This vertex after it has been assigned to \a rhs.
		*/
		const Vertex2D& operator=(const Vertex2D& rhs);

	private:
		/// The x coordinate.
		float x;
		/// The y coordinate.
		float y;
	};




} //avl
} //utility
#endif // __AVL_UTILITY_VERTEX_2D__