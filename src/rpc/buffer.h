#pragma once

#include <malloc.h>
#include <string.h>
#include <new>
#include <exception>
#include <boost/shared_ptr.hpp>

namespace rpc
{

typedef boost::shared_ptr<class Buffer> BufferPtr;

class Buffer
{
public:
	struct end_of_data: public std::exception
	{
		char const *what() const throw()
		{
			return "end of data";
		}
	};

	Buffer():
		data_((char *) malloc(size_)),
		size_(SIZE_1),
		read_(data_),
		write_(data_)
	{
		if (!data_) throw std::bad_alloc();
	}

	Buffer(Buffer const &rhs):
		data_((char *) malloc(rhs.size_)),
		size_(rhs.size_),
		read_(rhs.read_offset() + data_),
		write_(rhs.write_offset() + data_)
	{
		if (!data_) throw std::bad_alloc();
		memcpy(data_, rhs.data_, rhs.write_offset());
	}

	void swap(Buffer &rhs)
	{
		std::swap(data_, rhs.data_);
		std::swap(size_, rhs.size_);
		std::swap(read_, rhs.read_);
		std::swap(write_, rhs.write_);
	}

	Buffer &operator =(Buffer const &rhs)
	{
		if (&rhs != this)
		{
			Buffer(rhs).swap(*this);
		}

		return *this;
	}

	~Buffer()
	{
		if (data_)
		{
			free(data_);
		}
	}

	void enlarge()
	{
		if (size_ != SIZE_1) throw std::bad_alloc();

		char *data = (char *) realloc(data_, SIZE_2);
		if (!data) throw std::bad_alloc();

		read_ = (read_ - data_) + data;
		write_ = (write_ - data_) + data;
		data_ = data;
		size_ = SIZE_2;
	}

	void *begin_ptr() const { return data_; }
	void *read_ptr() const { return read_; }
	size_t read_offset() const { return read_ - data_; }
	size_t read_bytes_available() const { return write_ - read_; }
	void seek_read(ptrdiff_t delta) { read_ += delta; }
	void set_read(size_t offset) { read_ = data_ + offset; }
	void *write_ptr() const { return write_; }
	size_t write_offset() const { return write_ - data_; }
	size_t write_bytes_available() const { return size_ - write_offset(); }
	void seek_write(ptrdiff_t delta) { write_ += delta; }
	void set_write(size_t offset) { write_ = data_ + offset; }

	void read(void *buffer, size_t size)
	{
		if (read_ + size > write_) throw end_of_data();

		memcpy(buffer, read_, size);
		read_ += size;
	}

	void write(void const *buffer, size_t size)
	{
		if (write_offset() + size >= size_) enlarge();

		memcpy(write_, buffer, size);
		write_ += size;
	}

	void consume(size_t size)
	{
		memcpy(data_, data_ + size, size_ - size);

		read_ -= size;
		write_ -= size;
	}

private:
	static size_t const SIZE_1 = 4096;
	static size_t const SIZE_2 = SIZE_1 + 65536;

	size_t size_;
	char *data_;
	char *read_, *write_;
};

}
