#pragma once
#ifndef AVL_UTILITY_POLYMORPHIC_QUEUE__
#define AVL_UTILITY_POLYMORPHIC_QUEUE__
/* Copyright 2012 Sheldon Bachstein
This file is part of the avl Library.

The avl Library is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

The avl Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the avl Library.  If not, see <http://www.gnu.org/licenses/>.
*/
/**
@file
Defines the PolymorphicQueue generic container class.
@author Sheldon Bachstein
@date Aug 21, 2012
*/

#include<queue>


namespace avl
{
namespace utility
{

	/**
	A thin wrapper class for keeping a queue of polymorphic
	obects with automatic garbage collection.
	*/
	template<class Type>
	class PolymorphicQueue
	{
	public:
		PolymorphicQueue();
		PolymorphicQueue(PolymorphicQueue&& original);
		~PolymorphicQueue();

		const bool empty() const;
		/** @attention Calling this function gives ownership of
		\a object to this queue. Do not attempt to delete \a
		object after calling this function.
		@post \a object will be deleted when it is popped from this
		queue or when this queue is destroyed.
		*/
		void push(Type* const object);
		const Type& front() const;
		Type& front();
		void pop();

		PolymorphicQueue& operator=(PolymorphicQueue&& rhs);

	private:
		std::queue<Type* const> objects;

		/// NOT IMPLEMENTED.
		PolymorphicQueue(const PolymorphicQueue&);
		/// NOT IMPLEMENTED.
		PolymorphicQueue& operator=(const PolymorphicQueue&);
	};


	// See method declaration for details.
	template<class Type>
	PolymorphicQueue<Type>::PolymorphicQueue()
	{
	}

	// See method declaration for details.
	template<class Type>
	PolymorphicQueue<Type>::PolymorphicQueue(PolymorphicQueue<Type>&& original)
	{
		try
		{
			objects.swap(original.objects);
		}
		catch(const std::bad_alloc&)
		{
			throw OutOfMemoryError();
		}
	}

	// See method declaration for details.
	template<class Type>
	PolymorphicQueue<Type>::~PolymorphicQueue()
	{
		while(objects.empty() == false)
		{
			delete objects.front();
			objects.pop();
		}
	}

	// See method declaration for details.
	template<class Type>
	const bool PolymorphicQueue<Type>::empty() const
	{
		return objects.empty();
	}

	// See method declaration for details.
	template<class Type>
	void PolymorphicQueue<Type>::push(Type* const object)
	{
		try
		{
			objects.push(object);
		}
		catch(const std::bad_alloc&)
		{
			delete object;
			throw OutOfMemoryError();
		}
		catch(...)
		{
			delete object;
			throw;
		}
	}

	// See method declaration for details.
	template<class Type>
	Type& PolymorphicQueue<Type>::front()
	{
		if(objects.empty() == true)
		{
			throw InvalidCallException("avl::utility::input_objects::PolymorphicQueue<Type>::front()", "The queue is empty.");
		}
		return *objects.front();
	}

	// See method declaration for details.
	template<class Type>
	const Type& PolymorphicQueue<Type>::front() const
	{
		if(objects.empty() == true)
		{
			throw InvalidCallException("avl::utility::input_objects::PolymorphicQueue<Type>::front()", "The queue is empty.");
		}
		return *objects.front();
	}

	// See method declaration for details.
	template<class Type>
	void PolymorphicQueue<Type>::pop()
	{
		if(objects.empty() == true)
		{
			throw InvalidCallException("avl::utility::input_objects::PolymorphicQueue<Type>::pop()", "The queue is empty.");
		}
		delete objects.front();
		objects.pop();
	}

	// See method declaration for details.
	template<class Type>
	PolymorphicQueue<Type>& PolymorphicQueue<Type>::operator=(PolymorphicQueue<Type>&& rhs)
	{
		try
		{
			objects.swap(rhs.objects);
		}
		catch(const std::bad_alloc&)
		{
			throw OutOfMemoryError();
		}
		return *this;
	}


} // utility
} // avl
#endif // AVL_UTILITY_POLYMORPHIC_QUEUE__
