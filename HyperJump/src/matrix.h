#pragma once

#include <meteor/core/non_copyable.h>

#include <vector>

template<typename T>
class Matrix : mtr::NonCopyable
{
public:
	Matrix() = default;
	Matrix(size_t width, size_t height, const T& value = T());
	Matrix(Matrix&& other);
	Matrix(const Matrix& other) = delete;

	Matrix& operator=(Matrix&& other);
	Matrix& operator=(const Matrix& other) = delete;

	T& operator[](const std::tuple<size_t, size_t>& position);
	const T& operator[](const std::tuple<size_t, size_t>& position) const;

	size_t width() const;
	size_t height() const;

private:
	std::vector<T> m_buffer;
	size_t m_width = 0;
	size_t m_height = 0;
};

template<typename T>
Matrix<T>::Matrix(size_t width, size_t height, const T& value) :
	m_buffer(width * height, value),
	m_width{ width },
	m_height{ height }
{}

template<typename T>
Matrix<T>::Matrix(Matrix&& other) :
	m_buffer{ std::move(other.m_buffer) },
	m_width{ other.m_width },
	m_height{ other.m_height }
{
	other.m_width = 0;
	other.m_height = 0;
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(Matrix&& other)
{
	m_buffer = std::move(other.m_buffer);
	m_width = other.m_width;
	m_height = other.m_height;

	other.m_width = 0;
	other.m_height = 0;

	return *this;
}

template<typename T>
T& Matrix<T>::operator[](const std::tuple<size_t, size_t>& position)
{
	return m_buffer[std::get<0>(position) * m_width + std::get<1>(position)];
}

template<typename T>
const T& Matrix<T>::operator[](const std::tuple<size_t, size_t>& position) const
{
	return m_buffer[std::get<0>(position) * m_width + std::get<1>(position)];
}

template<typename T>
size_t Matrix<T>::width() const
{
	return m_width;
}

template<typename T>
size_t Matrix<T>::height() const
{
	return m_height;
}
