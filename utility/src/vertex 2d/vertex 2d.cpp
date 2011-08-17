/**********
 * Author: Sheldon Bachstein
 * Date: Jan 17, 2011
 * Description: See vertex 2d.h for details.
 **********/

#include"vertex 2d.h"


namespace avl
{
namespace utility
{


	// Basic constructor; initializes coordinates to 0.0f.
	Vertex2D::Vertex2D()
	: x(0.0f), y(0.0f)
	{
	}




	// Takes the values of the 6 coordinates and initializes the respective coordinates.
	Vertex2D::Vertex2D(const float& x, const float& y)
	: x(x), y(y)
	{
	}




	// Basic destructor.
	Vertex2D::~Vertex2D()
	{
	}




	// Copy constructor.
	Vertex2D::Vertex2D(const Vertex2D& original)
	: x(original.GetX()), y(original.GetY())
	{
	}




	// Returns x.
	const float& Vertex2D::GetX() const
	{
		return x;
	}




	// Returns y.
	const float& Vertex2D::GetY() const
	{
		return y;
	}




	// Sets x.
	void Vertex2D::SetX(const float& new_x)
	{
		x = new_x;
	}




	// Sets y.
	void Vertex2D::SetY(const float& new_y)
	{
		y = new_y;
	}




	// Adds two vertices together. Simply combines their respective components and returns the result.
	Vertex2D Vertex2D::operator+(const Vertex2D& rhs) const
	{
		return Vertex2D(x + rhs.GetX(), y + rhs.GetY());
	}




	// Subtracts one vertex from another. Simply subtracts their respective components and returns the result.
	Vertex2D Vertex2D::operator-(const Vertex2D& rhs) const
	{
		return Vertex2D(x - rhs.GetX(), y - rhs.GetY());
	}




	// Compound += operator.
	Vertex2D& Vertex2D::operator+=(const Vertex2D& rhs)
	{
		x += rhs.GetX();
		y += rhs.GetY();

		return *this;
	}




	// Compound -= operator.
	Vertex2D& Vertex2D::operator-=(const Vertex2D& rhs)
	{
		x -= rhs.GetX();
		y -= rhs.GetY();

		return *this;
	}
	
	
	
	
	// Compares two vertices for equality.
	const bool Vertex2D::operator==(const Vertex2D& rhs) const
	{
		return (x == rhs.GetX() && y == rhs.GetY());
	}




	// Compares two vertices for unequality.
	const bool Vertex2D::operator!=(const Vertex2D& rhs) const
	{
		return !(this->operator==(rhs));
	}




	// Assignment operator.
	const Vertex2D& Vertex2D::operator=(const Vertex2D& rhs)
	{
		x = rhs.GetX();
		y = rhs.GetY();

		return *this;
	}



}
}