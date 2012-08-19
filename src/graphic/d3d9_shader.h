#pragma once

#include <string>
#include <d3d9.h>
#include <d3dx9.h>
#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <util/boost_com_intrusive_ptr.h>
#include "forward.h"
#include "shader.h"

namespace graphic
{

class D3D9Shader: public Shader, public boost::enable_shared_from_this<D3D9Shader> {
	friend class D3D9Renderer;

public:
	virtual ~D3D9Shader();

	virtual void add_preprocessor_macro(std::string const &key, std::string const &value = std::string());

    virtual void load_from_file(std::string const &filename);
    virtual void load_from_buffer(char const *buffer, size_t len);

    ID3DXEffect *get() const
    {
        return effect_.get();
    }

	void handle_lost_device();
	void handle_reset_device();

	virtual ParameterPtr get_parameter(std::string const &name);

protected:
	class D3D9Parameter;

    D3D9Shader(D3D9RendererPtr const &renderer_ptr);

	D3D9RendererWeakPtr const renderer_;
    boost::intrusive_ptr<ID3DXEffect> effect_;
	D3DXHANDLE tech_;
	std::vector<std::pair<std::string, std::string> > macros_;

	static void log_compilation_errors(ID3DXBuffer *compilation_errors_ptr);
};

}
