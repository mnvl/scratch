#pragma once

#include <windows.h>
#include <exception>

namespace graphic {

class directx_error: public std::exception {
private:
	HRESULT hresult_;

public:
	directx_error(HRESULT hr) throw(): hresult_(hr) {}
	~directx_error() throw() {}
	char const *what() const throw() { return ""/* DXGetErrorDescription(hresult_)*/; }
};

}
