
#include <boost/test/unit_test.hpp>
#include <rpc/buffer.h>

#if defined(max)
#undef max
#endif

BOOST_AUTO_TEST_SUITE(test_buffer)

BOOST_AUTO_TEST_CASE (test_buffer_copy)
{
	rpc::Buffer a, b;
	b = a;
}

BOOST_AUTO_TEST_CASE (test_buffer_enlarge)
{
	rpc::Buffer a;
	a.enlarge();
	BOOST_CHECK_THROW(a.enlarge(), std::bad_alloc);
}

BOOST_AUTO_TEST_CASE (test_buffer_read_write)
{
	static size_t const TEXT_SIZE[] = {
		1,
		2,
		41,
		2650,
		4091,
		4092,
		4093,
		4095,
		4096,
		4097,
		20123,
		30132,
		40213,
		50312,
		65535
	};

	static size_t const SINGLE_READ_WRITE_SIZE[] = {
		1,
		2,
		32,
		64,
		1024,
		4096,
		8192,
		65536
	};

	for (size_t i = 0; i < sizeof(TEXT_SIZE) / sizeof(TEXT_SIZE[0]); i++)
	{
		for (size_t j = 0; j < sizeof(SINGLE_READ_WRITE_SIZE) / sizeof(SINGLE_READ_WRITE_SIZE[0]); j++)
		{
			std::vector<char> etalon(TEXT_SIZE[i]), readed(TEXT_SIZE[i]);
			for (size_t k = 0; k < etalon.size(); k++) etalon[k] = (char) k;

			rpc::Buffer buf;

			size_t delta = SINGLE_READ_WRITE_SIZE[j];

			for (size_t k = 0; k < etalon.size(); k += delta)
			{
				buf.write(&etalon[k], std::min(etalon.size() - k, delta));
			}

			for (size_t k = 0; k < etalon.size(); k += delta)
			{
				buf.read(&readed[k], std::min(readed.size() - k, delta));
			}

			BOOST_REQUIRE (etalon == readed);
		}
	}
}

BOOST_AUTO_TEST_SUITE_END()
