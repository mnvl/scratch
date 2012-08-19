
#include <io.h>
#include <boost/test/unit_test.hpp>
#include "logger.h"
#include "archive.h"

BOOST_AUTO_TEST_SUITE(archive)

BOOST_AUTO_TEST_CASE(writer_reader)
{
	char archive_file_name[MAX_PATH] = "test_archive.XXXXXX";

	static char const KEY1[] = "Brian Kernighan & Dennis Ritchie";
	static char const DATA1[] = "hello world";

	static char const KEY2[] = "Neil Armstrong";
	static char const DATA2[] = "That's one small step for man; one giant leap for mankind.";

	static char const KEY3[] = "Shakespeare";
	static char const DATA3[] = "To be, or not to be: that is the question.";

	mktemp(archive_file_name);

	{
		util::ArchiveWriter writer;
		writer.create(archive_file_name);
		writer.write_data(KEY1, DATA1, sizeof(DATA1));
		writer.write_data(KEY2, DATA2, sizeof(DATA2));
		writer.close();
	}

	{
		util::ArchiveReader reader;
		reader.open(archive_file_name);

		std::vector<char> data1;
		reader.read_data(KEY1, data1);
		BOOST_REQUIRE(std::equal(data1.begin(), data1.end(), DATA1));

		std::vector<char> data2;
		reader.read_data(KEY2, data2);
		BOOST_REQUIRE(std::equal(data2.begin(), data2.end(), DATA2));
	}

	/*
	{
		util::ArchiveWriter writer;
		writer.open(archive_file_name);
		writer.write_data(KEY3, DATA3, sizeof(DATA3));
		writer.close();
	}

	{
		util::ArchiveReader reader;
		reader.open(archive_file_name);

		std::vector<char> data1;
		reader.read_data(KEY1, data1);
		BOOST_REQUIRE(std::equal(data1.begin(), data1.end(), DATA1));

		std::vector<char> data2;
		reader.read_data(KEY2, data2);
		BOOST_REQUIRE(std::equal(data2.begin(), data2.end(), DATA2));

		std::vector<char> data3;
		reader.read_data(KEY3, data3);
		BOOST_REQUIRE(std::equal(data3.begin(), data3.end(), DATA3));
	}
	*/

	unlink(archive_file_name);
}

BOOST_AUTO_TEST_SUITE_END()
