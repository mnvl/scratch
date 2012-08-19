
#include "archive.h"

namespace util
{

Archive::Archive()
{
	file_.exceptions(std::ios::failbit | std::ios::badbit);
}

void Archive::read_directory()
{
	chunks_.clear();

	ARCHIVE_HEADER header;
	file_.seekg(0, std::ios_base::beg);
	file_.read((char *) &header, sizeof(ARCHIVE_HEADER));

	if (header.version != VERSION)
	{
		throw std::runtime_error("archive with invalid version");
	}

	if (header.directory_layout.length == 0)
	{
		return;
	}

	std::vector<char> buffer(header.directory_layout.length);
	file_.seekg(header.directory_layout.offset, std::ios_base::beg);
	file_.read((char *) &buffer[0], buffer.size());

	char *cursor = &buffer[0];
	size_t bytes_left = buffer.size();

	while (bytes_left > 0)
	{
		std::string name;

		while (bytes_left > 0 && *cursor)
		{
			name += *cursor++;
			--bytes_left;
		}

		if (bytes_left > 0 && *cursor == 0)
		{
			--bytes_left;
			++cursor;
		}
		else
		{
			throw std::runtime_error("invalid directory record");
		}

		CHUNK_LAYOUT chunk_layout;

		if (bytes_left >= sizeof(CHUNK_LAYOUT))
		{
			std::copy(cursor, cursor + sizeof(CHUNK_LAYOUT), (char *) &chunk_layout);

			cursor += sizeof(CHUNK_LAYOUT);
			bytes_left -= sizeof(CHUNK_LAYOUT);
		}
		else
		{
			throw std::runtime_error("invalid directory record");
		}

		chunks_.insert(std::make_pair(name, chunk_layout));
	}
}

void ArchiveReader::open(std::string const &filename)
{
	file_.open(filename.c_str(), std::ios_base::in | std::ios_base::binary);
	read_directory();
}

void ArchiveReader::read_data(std::string const &key, std::vector<char> &buffer)
{
	chunks_t::const_iterator it = chunks_.find(key);
	if (it == chunks_.end()) throw std::runtime_error("can't find chunk " + key);

	buffer.resize(it->second.length);
	file_.seekg(it->second.offset, std::ios_base::beg);
	file_.read(&buffer[0], buffer.size());
}

void ArchiveReader::read_data(std::string const &key, std::string &buffer)
{
	std::vector<char> temp;
	read_data(key, temp);

	buffer.assign(temp.begin(), temp.end());
}

luabind::object ArchiveReader::read_string(lua_State *L, std::string const &key)
{
	std::vector<char> buffer;
	read_data(key, buffer);

	lua_pushlstring(L, &buffer[0], buffer.size());

	return luabind::object(luabind::from_stack(L, -1));
}

void ArchiveReader::close()
{
	file_.close();
}

void ArchiveWriter::create(std::string const &filename)
{
	file_.open(filename.c_str(), std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);

	ARCHIVE_HEADER header;
	header.version = VERSION;
	header.directory_layout.offset = 0;
	header.directory_layout.length = 0;
	file_.write((char const *) &header, sizeof(ARCHIVE_HEADER));
}

void ArchiveWriter::write_data(std::string const &key, void const *data, size_t len)
{
	CHUNK_LAYOUT chunk_layout;
	chunk_layout.offset = file_.tellp();
	chunk_layout.length = len;

	file_.write((char const *) data, len);

	if (!chunks_.insert(std::make_pair(key, chunk_layout)).second)
	{
		throw std::runtime_error("archive already contains file with key " + key);
	}
}

void ArchiveWriter::write_data(std::string const &key, std::string const &data)
{
	write_data(key, data.c_str(), data.size());
}

void ArchiveWriter::write_file(std::string const &key, std::fstream &file)
{
	file.seekg(0, std::ios_base::end);
	size_t size = file.tellg();

	std::vector<char> buffer(size);

	if (size)
	{
		file.seekg(0, std::ios_base::beg);
		file.read(&buffer[0], size);
	}

	write_data(key, &buffer[0], buffer.size());
}

void ArchiveWriter::write_file(std::string const &key, std::string const &path)
{
	std::fstream fs(path.c_str(), std::ios_base::in | std::ios_base::binary);
	write_file(key, fs);
}

void ArchiveWriter::close()
{
	write_directory();
	file_.close();
}

void ArchiveWriter::write_directory()
{
	ARCHIVE_HEADER header;
	header.version = VERSION;
	header.directory_layout.offset = file_.tellp();
	header.directory_layout.length = 0;

	for (chunks_t::const_iterator it = chunks_.begin(); it != chunks_.end(); it++)
	{
		file_.write(it->first.c_str(), it->first.size() + 1);
		file_.write((char const *) &it->second, sizeof(CHUNK_LAYOUT));
	}

	header.directory_layout.length = size_t(file_.tellp()) - header.directory_layout.offset;
	file_.seekp(0, std::ios_base::beg);
	file_.write((char const *) &header, sizeof(ARCHIVE_HEADER));
}

}
