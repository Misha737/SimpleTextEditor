#pragma once

#include <cmath>
#include <stdexcept>
#include <type_traits>
#include <iostream>

template <typename T>
class Array {
protected:
	//static const size_t SEGMENT_SIZE = 64;
	static const size_t SEGMENT_SIZE = 4;
	T* items;
	size_t number_of_items;
	size_t segments;
	void init_array(const T* new_items, size_t new_length);
	void resize(size_t new_segments);

public:
	Array();
	Array(const Array<T>& copy);
	Array(const T* new_items, size_t new_length);
	~Array();
	void add(T new_item);
	void insertRange(const Array<T>& range, size_t index);
	void insertRange(const T* range, size_t range_len, size_t index);
	void deleteRange(size_t index, size_t length);
	T pop();
	T& operator[](size_t index);
	const T& operator[](size_t index) const;
	size_t length() const;
	const T& last() const;
	Array<T>* copy() const;
	void clean();
	void print();
	const T* getArray() const;
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
	for (size_t i = 0; i < new_segments * SEGMENT_SIZE; i++) {
		if (i < number_of_items)
			new_items[i] = items[i];
		else
			new_items[i] = (T)0b0;
	}
	segments = new_segments;
	if constexpr (std::is_pointer<T>::value)
		for (size_t i = new_segments * SEGMENT_SIZE; i < number_of_items; i++) {
			delete items[i];
		}
	delete[] items;
	items = new_items;
}

template<typename T>
inline Array<T>::Array()
{
	init_array(nullptr, 0);
}

template<typename T>
inline Array<T>::Array(const Array<T>& copy)
{
	init_array(copy.items, copy.number_of_items);
}

template<typename T>
inline Array<T>::Array(const T* new_items, size_t new_length)
{
	init_array(new_items, new_length);
}

template<typename T>
inline Array<T>::~Array()
{
	if constexpr (std::is_pointer<T>::value) {
		for (size_t i = 0; i < number_of_items; i++) {
			delete items[i];
		}
	}
	delete[] items;
}

template<typename T>
inline void Array<T>::add(T new_item)
{
	if (number_of_items == segments * SEGMENT_SIZE) {
		resize(++segments);
	}
	items[number_of_items++] = new_item;
}

template<typename T>
inline void Array<T>::insertRange(const Array<T>& range, size_t insert_index)
{	
	size_t new_segment_size = std::ceil((float)(number_of_items + range.length()) / (float)SEGMENT_SIZE);
	if (new_segment_size != segments)
		resize(new_segment_size);
	//if (append_segments > 0)
		//resize(segments + append_segments);
	for (int i = number_of_items + range.length() - 1; i >= (int)insert_index; i--) {
		items[i] = items[i - range.length()];
	}
	for (size_t i = 0; i < range.length(); i++) {
		items[i + insert_index] = range[i];
	}
	number_of_items += range.length();
}

template<typename T>
inline void Array<T>::insertRange(const T* range, size_t range_len, size_t insert_index)
{
	size_t new_segment_size = std::ceil((float)(number_of_items + range_len) / (float)SEGMENT_SIZE);
	if (new_segment_size != segments)
		resize(new_segment_size);

	for (int i = number_of_items + range_len - 1; i >= (int)insert_index; i--) {
		items[i] = items[i - range_len];
	}
	for (size_t i = 0; i < range_len; i++) {
		items[i + insert_index] = range[i];
	}
	number_of_items += range_len;
}

template<typename T>
inline void Array<T>::deleteRange(size_t index, size_t length)
{
	if (index + length > number_of_items)
		length = number_of_items - index;

	if constexpr (std::is_pointer<T>::value)
		for (size_t i = index; i < index + length; i++) {
			delete items[i];
		}
	for (size_t i = index + length; i < number_of_items; i++) {
		items[i - length] = items[i];
	}
	number_of_items -= length;
	size_t new_segment_number = std::ceil((float)number_of_items / (float)(SEGMENT_SIZE));
	resize(new_segment_number);
}

template<typename T>
inline T Array<T>::pop()
{
	if (number_of_items == 0)
		throw std::out_of_range("Array is empty");
	T pop_item = items[--number_of_items];
	if constexpr (std::is_pointer<T>::value)
		delete items[number_of_items];
	if (number_of_items == (segments - 1) * SEGMENT_SIZE) {
		resize(--segments);
	}

	return pop_item;
}

template<typename T>
inline T& Array<T>::operator[](size_t index)
{
	if (index < 0 || index >= number_of_items) {
		throw std::out_of_range("Index out of bounds");
	}
	return items[index];
}

template<typename T>
inline const T& Array<T>::operator[](size_t index) const
{
	if (index < 0 || index >= number_of_items) {
		throw std::out_of_range("Index out of bounds");
	}
	return items[index];
}

template<typename T>
inline size_t Array<T>::length() const
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
	if (std::is_pointer<T>::value) {
		for (size_t i = 0; i < number_of_items; i++) {
			delete items[i];
		}
	}
	resize(1);
	number_of_items = 0;
	for (size_t i = 0; i < SEGMENT_SIZE; i++) {
		items[i] = (T)0b0;
	}
}

template<typename T>
inline void Array<T>::print()
{
	for (size_t i = 0; i < length(); i++) {
		if (i % SEGMENT_SIZE == 0)
			std::cout << "---" << std::endl;
		std::cout << items[i] << std::endl;
	}
}

template<typename T>
inline const T* Array<T>::getArray() const
{
	return items;
}
