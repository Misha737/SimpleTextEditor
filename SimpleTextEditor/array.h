#pragma once

#include <cmath>
#include <stdexcept>

template <typename T>
class Array {
private:
	static const size_t SEGMENT_SIZE = 64;
	T* items;
	size_t number_of_items;
	size_t segments;
	void init_array(const T* new_items, size_t new_length);
	void resize(size_t new_segments);

public:
	Array();
	Array(const T* new_items, size_t new_length);
	~Array();
	void add(const T& new_item);
	T pop();
	T operator[](size_t index);
	const T operator[](size_t index) const;
	size_t length();
	const T& last() const;
	Array<T>* copy() const;
	void clean();
	void print();
};

template<typename T>
inline void Array<T>::init_array(const T* new_items, size_t new_length)
{
	items = nullptr;
	number_of_items = new_length;
	segments = static_cast<size_t>(std::ceil((float)number_of_items / (float)SEGMENT_SIZE));
	if (segments == 0)
		segments++;
	items = new T[segments * SEGMENT_SIZE];
	for (size_t i = 0; i < number_of_items; i++) {
		items[i] = new_items[i];
	}
	for (size_t i = number_of_items; i < segments * SEGMENT_SIZE; i++) {
		items[i] = (T)0b0;
	}
}

template<typename T>
inline void Array<T>::resize(size_t new_segments)
{
	T* new_items = new T[new_segments * SEGMENT_SIZE];
	for (size_t i = 0; i < number_of_items; i++) {
		new_items[i] = items[i];
	}
	for (size_t i = number_of_items; i < new_segments * SEGMENT_SIZE; i++) {
		new_items[i] = (T)0b0;
	}
	segments = new_segments;
	delete[] items;
	items = new_items;
}

template<typename T>
inline Array<T>::Array()
{
	init_array(nullptr, 0);
}

template<typename T>
inline Array<T>::Array(const T* new_items, size_t new_length)
{
	init_array(new_items, new_length);
}

template<typename T>
inline Array<T>::~Array()
{
	delete[] items;
}

template<typename T>
inline void Array<T>::add(const T& new_item)
{
	if (number_of_items == segments * SEGMENT_SIZE) {
		resize(++segments);
	}
	items[number_of_items++] = new_item;
}

template<typename T>
inline T Array<T>::pop()
{
	if (number_of_items == 0)
		throw std::out_of_range("Array is empty");
	T pop_item = items[--number_of_items];
	if (number_of_items == (segments - 1) * SEGMENT_SIZE) {
		resize(--segments);
	}

	return pop_item;
}

template<typename T>
inline T Array<T>::operator[](size_t index)
{
	if (index < 0 || index >= number_of_items) {
		throw std::out_of_range("Index out of bounds");
	}
	return items[index];
}

template<typename T>
inline const T Array<T>::operator[](size_t index) const
{
	if (index < 0 || index >= number_of_items) {
		throw std::out_of_range("Index out of bounds");
	}
	return items[index];
}

template<typename T>
inline size_t Array<T>::length()
{
	return number_of_items;
}

template<typename T>
inline const T& Array<T>::last() const
{
	if (number_of_items == 0)
		throw std::out_of_range("Array is empty");
	return items[number_of_items - 1];
}

template<typename T>
inline Array<T>* Array<T>::copy() const
{
	return new Array(items, number_of_items);
}

template<typename T>
inline void Array<T>::clean()
{
	resize(1);
	number_of_items = 0;
	for (size_t i = 0; i < SEGMENT_SIZE; i++) {
		items[i] = (T)0b0;
	}
}

template<typename T>
inline void Array<T>::print()
{
	for (size_t i = 0; i < length(); i++)
		std::cout << items[i] << std::endl;
}
