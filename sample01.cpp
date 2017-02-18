
extern "C" {
#include <fpp.h>
}

#include <vector>
#include <fstream>
#include <string>
#include <streambuf>
#include <iostream>

std::vector<char> g_buffer;
size_t g_bufferPos;

char* fppInput(char* _buffer, int _size, void* _userData)
{
	auto begin = g_buffer.begin() + g_bufferPos;
	auto end = g_buffer.end();
	auto it = begin;

	if (begin == end)
	{
		return nullptr;
	}

	int i = 0;
	while (it != end)
	{
		_buffer[i] = (*it);
		it++;
		i++;

		if (i == _size)
		{
			_buffer[i] = '\0';
			break;
		}
	}
	if (i != _size)
	{
		_buffer[i] = '\0';
	}

	g_bufferPos = it - g_buffer.begin();
	return _buffer;

}

void fppOutput(int _ch, void* _userData)
{
	std::cout << (char)_ch;
}

void fppError(void* /*_userData*/, char* _format, va_list _vargs)
{
}

std::vector<char> MakeStringBuffer(const std::string& str)
{
	std::vector<char> buffer;
	buffer.reserve(str.size() + 1);
	buffer.assign(str.begin(), str.end());
	buffer.push_back('\0');
	return buffer;
}

int main()
{
	std::ifstream ifs;
	ifs.open("./sample/test.glsl");
	g_buffer.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());

	std::vector<fppTag> tags;
	fppTag tag;
	tag.tag = FPPTAG_USERDATA;
	tag.data = nullptr;
	tags.push_back(tag);

	tag.tag = FPPTAG_INPUT;
	tag.data = (void*)fppInput;
	tags.push_back(tag);

	tag.tag = FPPTAG_OUTPUT;
	tag.data = (void*)fppOutput;
	tags.push_back(tag);

	tag.tag = FPPTAG_ERROR;
	tag.data = (void*)fppError;
	tags.push_back(tag);

	tag.tag = FPPTAG_IGNOREVERSION;
	tag.data = (void*)0;
	tags.push_back(tag);

	tag.tag = FPPTAG_LINE;
	tag.data = (void*)0;
	tags.push_back(tag);

	auto incDir = MakeStringBuffer("./sample");
	tag.tag = FPPTAG_INCLUDE_DIR;
	tag.data = incDir.data();
	tags.push_back(tag);

	const char* filename = "test.glsl";

	tag.tag = FPPTAG_INPUT_NAME;
	tag.data = (void*)filename;
	tags.push_back(tag);

	auto def1 = MakeStringBuffer("DATA=5678");
	tag.tag = FPPTAG_DEFINE;
	tag.data = (void*)def1.data();
	tags.push_back(tag);

	auto def2 = MakeStringBuffer("DATA2=999");
	tag.tag = FPPTAG_DEFINE;
	tag.data = (void*)def2.data();
	tags.push_back(tag);

	tag.tag = FPPTAG_END;
	tag.data = nullptr;
	tags.push_back(tag);

	fppPreProcess((fppTag*)tags.data());

	return 0;
}
