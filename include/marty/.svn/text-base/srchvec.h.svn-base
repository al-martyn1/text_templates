/*
   Copyright (c) 2007-2009 Alex Martynov, amart at mail.ru
   This code can be used without any limitations.
   This code provided as is, use it on your own risk.
*/

#ifndef MARTY_SRCHVEC_H
#define MARTY_SRCHVEC_H


#if !defined(_MAP_) && !defined(_STLP_MAP) && !defined(__STD_MAP__) && !defined(_CPP_MAP) && !defined(_GLIBCXX_MAP)
    #include <map>
#endif

#if !defined(_VECTOR_) && !defined(_STLP_VECTOR) && !defined(__STD_VECTOR__) && !defined(_CPP_VECTOR) && !defined(_GLIBCXX_VECTOR)
    #include <vector>
#endif

#if !defined(_UTILITY_) && !defined(_STLP_UTILITY) && !defined(__STD_UTILITY__) && !defined(_CPP_UTILITY) && !defined(_GLIBCXX_UTILITY)
    #include <utility>
#endif

#if !defined(_EXCEPTION_) && !defined(__EXCEPTION__) && !defined(_STLP_EXCEPTION) && !defined(__STD_EXCEPTION)
    #include <exception>
#endif

#if !defined(_STDEXCEPT_) && !defined(_STLP_STDEXCEPT) && !defined(__STD_STDEXCEPT) && !defined(_CPP_STDEXCEPT) && !defined(_GLIBCXX_STDEXCEPT)
    #include <stdexcept>
#endif


namespace marty{


template < class Key
         , class Type
         , class KeyCmp = ::std::less<Key>
         , class VectorType = ::std::vector< Type, ::std::allocator<Type> >
         , class SearchMapType = ::std::map< Key
                                           , typename VectorType :: size_type
                                           , KeyCmp
                                           >
         , class IndexSearchMapType = ::std::map< typename VectorType :: size_type
                                                , Key
                                                , ::std::less< typename VectorType :: size_type >
                                                >
>
class fast_search_vector
{

    public:
        typedef VectorType container_type;
        typedef Key        key_type;        
    
        typedef typename container_type :: allocator_type          allocator_type        ;   /*!< A type that represents the allocator class for the vector object. */

        typedef typename container_type :: const_iterator          const_iterator        ;   /*!< A type that provides a random-access iterator that can read a const element in a vector. */
        typedef typename container_type :: const_pointer           const_pointer         ;   /*!< A type that provides a pointer to a const element in a vector. */
        typedef typename container_type :: const_reference         const_reference       ;   /*!< A type that provides a reference to a const element stored in a vector for reading and performing const operations. */
        typedef typename container_type :: const_reverse_iterator  const_reverse_iterator;   /*!< A type that provides a random-access iterator that can read any const element in the vector. */

        typedef typename container_type :: difference_type         difference_type       ;   /*!< A type that provides the difference between the addresses of two elements in a vector. */
        typedef typename container_type :: iterator                iterator              ;   /*!< A type that provides a random-access iterator that can read or modify any element in a vector. */
        typedef typename container_type :: pointer                 pointer               ;   /*!< A type that provides a pointer to an element in a vector. */
        typedef typename container_type :: reference               reference             ;   /*!< A type that provides a reference to an element stored in a vector. */
        typedef typename container_type :: reverse_iterator        reverse_iterator      ;   /*!< A type that provides a random-access iterator that can read or modify any element in a reversed vector. */
        typedef typename container_type :: size_type               size_type             ;   /*!< A type that counts the number of elements in a vector. */
        typedef typename container_type :: value_type              value_type            ;   /*!< */
    //    typedef typename container_type :: ;                        /*!< */

    protected:
 
        container_type      vec;
        SearchMapType       key2index;
        IndexSearchMapType  index2key;

    public:

        void swap(fast_search_vector &v)
           {
            vec.swap(v.vec);      
            key2index.swap(v.key2index);
            index2key.swap(v.index2key);
           }

        fast_search_vector() : vec(), key2index(), index2key() {}

        fast_search_vector(const fast_search_vector &v) : vec(v.vec), key2index(v.key2index), index2key(v.index2key) {}

        fast_search_vector& operator=(const fast_search_vector &v)
            {
             if (&v==this) return *this;
             fast_search_vector tmp(v);
             swap(tmp);
             return *this;
            }

        void clear() { vec.clear(); key2index.erase(key2index.begin(), key2index.end()); index2key.erase(index2key.begin(), index2key.end()); }

        //void assign(size_type _Count, const Type& _Val) { clear(); }

        iterator push_back(const Key& _Key, const Type& _Val)
           {
            size_type n = vec.size();
            key2index[_Key] = n;
            index2key[n] = _Key;
            vec.push_back(_Val);
            return vec.begin() + n;
           }

        iterator push_back(const ::std::pair<Key, Type> &p)
           {
            size_type n = vec.size();
            key2index[p.first] = n;
            index2key[n] = p.first;
            vec.push_back(p.second);
            return vec.begin() + n;
           }

        reference front()             { return vec.front(); }
        const_reference front() const { return vec.front(); }
        reference back()              { return vec.back(); }
        const_reference back() const  { return vec.back(); }

        bool empty() const { return vec.empty(); }

        size_type size() const { return vec.size(); }

        const_reference operator[](size_type pos) const
           {
            return vec[pos];
           }

        reference operator[](size_type pos)
           {
            return vec[pos];
           }

        key_type key(size_type pos) const
           {
            typename IndexSearchMapType::const_iterator it = index2key.find(pos);
            if (it==index2key.end())
               throw ::std::out_of_range("fast_search_vector::key - pos out of range");
            return it->second;
           }

        key_type key(iterator it) const
           {
            const_iterator cit = it;
            return key(size_type(cit - vec.begin()));
           }

        key_type key(const_iterator it) const
           {
            const_iterator cit = it;
            return key(size_type(cit - vec.begin()));
           }

        key_type key(reverse_iterator it) const
           {
            const_reverse_iterator cit = it;
            return key(size_type(cit - vec.rbegin()));
           }

        key_type key(const_reverse_iterator it) const
           {
            const_reverse_iterator cit = it;
            return key(size_type(cit - vec.rbegin()));
           }

        const_iterator find(const key_type &key) const
           {
            typename SearchMapType::const_iterator it = key2index.find(key);
            if (it==key2index.end()) return vec.end();
            return vec.begin() + it->second;
           }

        iterator find(const key_type &key)
           {
            typename SearchMapType::const_iterator it = key2index.find(key);
            if (it==key2index.end()) return vec.end();
            return vec.begin() + it->second;
           }

        const_iterator         begin()  const { return vec.begin(); }
        const_reverse_iterator rbegin() const { return vec.rbegin(); }
        iterator               begin()        { return vec.begin(); }
        reverse_iterator       rbegin()       { return vec.rbegin(); }

        const_iterator         end()    const { return vec.end(); }
        const_reverse_iterator rend()   const { return vec.rend(); }
        iterator               end()          { return vec.end(); }
        reverse_iterator       rend()         { return vec.rend(); }

}; // fast_search_vector



}; // namespace marty
/*

template <
   class Key, 
   class Type, 
   class Traits = less<Key>, 
      class Allocator=allocator<pair <const Key, Type> > 
>


}; // namespace marty


template <
   class Type, 
   class Allocator = allocator<Type> 
>
class vector

 
template <
   class Key, 
   class Type, 
   class Traits = less<Key>, 
      class Allocator=allocator<pair <const Key, Type> > 
>
class map
 


Constructors 
vector
 Constructs a vector of a specific size or with elements of a specific value or with a specific allocator or as a copy of some other vector.
 
Typedefs 
allocator_type
 A type that represents the allocator class for the vector object.
 
const_iterator
 A type that provides a random-access iterator that can read a const element in a vector.
 
const_pointer
 A type that provides a pointer to a const element in a vector.
 
const_reference
 A type that provides a reference to a const element stored in a vector for reading and performing const operations.
 
const_reverse_iterator
 A type that provides a random-access iterator that can read any const element in the vector.
 
difference_type
 A type that provides the difference between the addresses of two elements in a vector.
 
iterator
 A type that provides a random-access iterator that can read or modify any element in a vector.
 
pointer
 A type that provides a pointer to an element in a vector.
 
reference
 A type that provides a reference to an element stored in a vector.
 
reverse_iterator
 A type that provides a random-access iterator that can read or modify any element in a reversed vector.
 
size_type
 A type that counts the number of elements in a vector.
 
value_type
 A type that represents the data type stored in a vector.
 
Member Functions 
assign
 Erases a vector and copies the specified elements to the empty vector.
 
at
 Returns a reference to the element at a specified location in the vector.
 
back
 Returns a reference to the last element of the vector.
 
begin
 Returns a random-access iterator to the first element in the container.
 
capacity
 Returns the number of elements that the vector could contain without allocating more storage.
 
clear
 Erases the elements of the vector.
 
empty
 Tests if the vector container is empty.
 
end
 Returns a random-access iterator that points just beyond the end of the vector.
 
erase
 Removes an element or a range of elements in a vector from specified positions.
 
front
 Returns a reference to the first element in a vector.
 
get_allocator
 Returns an object to the allocator class used by a vector.
 
insert
 Inserts an element or a number of elements into the vector at a specified position.
 
max_size
 Returns the maximum length of the vector.
 
pop_back
 Deletes the element at the end of the vector.
 
push_back
 Add an element to the end of the vector.
 
rbegin
 Returns an iterator to the first element in a reversed vector.
 
rend
 Returns an iterator to the end of a reversed vector.
 
resize
 Specifies a new size for a vector.
 
reserve
 Reserves a minimum length of storage for a vector object.
 
size
 Returns the number of elements in the vector.
 
swap
 Exchanges the elements of two vectors.
 
Operators 
operator[]
 
*/


#endif /* SRCHVEC_H */

