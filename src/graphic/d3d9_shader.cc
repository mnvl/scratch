
#include "directx_error.h"
#include <util/logger.h>
#include "d3d9_texture.h"
#include "d3d9_renderer.h"
#include "d3d9_shader.h"

namespace graphic
{

#if defined(NDEBUG)
static DWORD const SCRATCH_SHADER_FLAGS = D3DXSHADER_OPTIMIZATION_LEVEL3;
#else
static DWORD const SCRATCH_SHADER_FLAGS = D3DXSHADER_DEBUG;
#endif

class D3D9Shader::D3D9Parameter: public Parameter {
public:
	D3D9Parameter(boost::shared_ptr<D3D9Shader> const &shader, std::string const &name):
		shader_(shader),
		name_(name),
		handle_(0)
	{
		handle_ = shader_->get()->GetParameterByName(0, name_.c_str());
		if (!handle_) throw std::logic_error("can't get " + name_ + " parameter handle");
	}

	virtual ~D3D9Parameter()
	{
	}

	template <class T>
	inline void set_i(T const &value)
	{
		set(&value, sizeof(value));
	}

	virtual void set(math::vec<2> const &v) { set_i(v); }
	virtual void set(math::vec<3> const &v) { set_i(v); }
	virtual void set(math::vec<4> const &v) { set_i(v); }

	virtual void set(math::matrix<4,4> const &value)
	{
		HRESULT hr = shader_->get()->SetMatrix(handle_, (D3DXMATRIX const *) value.ij);
		if (FAILED(hr)) throw directx_error(hr);
	}

	virtual void set(std::vector<math::matrix<4,4> > const &value)
	{
		HRESULT hr = shader_->get()->SetMatrixArray(handle_, (D3DXMATRIX const *) &value[0], value.size());
		if (FAILED(hr)) throw directx_error(hr);
	}

	virtual void set(TexturePtr const &texture)
	{
		HRESULT hr = shader_->get()->SetTexture(handle_, texture ? static_cast<D3D9Texture *>(texture.get())->get() : 0);
		if (FAILED(hr)) throw directx_error(hr);
	}

	virtual void set(void const *ptr, size_t size)
	{
		HRESULT hr = shader_->get()->SetValue(handle_, ptr, size);
		if (FAILED(hr)) throw directx_error(hr);
	}

private:
	boost::shared_ptr<D3D9Shader> const shader_;
	std::string const name_;
	D3DXHANDLE handle_;
};

D3D9Shader::~D3D9Shader()
{
}

void D3D9Shader::add_preprocessor_macro(std::string const &key, std::string const &value)
{
	macros_.push_back(std::make_pair(key, value));
}

void D3D9Shader::load_from_file(std::string const &filename)
{
	D3D9RendererPtr renderer(renderer_);

	HRESULT hr;

	LOG_INFO("*** building preprocessor macros for shader " << filename);
	std::vector<D3DXMACRO> macros;

	for (std::vector<std::pair<std::string, std::string> >::const_iterator it = macros_.begin();
		 it != macros_.end(); ++it)
	{
		D3DXMACRO macro;
		macro.Name = it->first.c_str();
		macro.Definition = it->second.c_str();

		if (it->second.empty()) LOG_INFO(it->first);
		else LOG_INFO(it->first << " = " << it->second);

		macros.push_back(macro);
	}

	{
		D3DXMACRO empty;
		empty.Name = empty.Definition = 0;
		macros.push_back(empty);
	}

	LOG_INFO("*** compiling shader from file " << filename);

	ID3DXEffect *effect_ptr;
	ID3DXBuffer *compilation_errors_ptr;
	hr = D3DXCreateEffectFromFile(renderer->get_device(), filename.c_str(), &macros[0], 0, SCRATCH_SHADER_FLAGS, 0,
		&effect_ptr, &compilation_errors_ptr);

	log_compilation_errors(compilation_errors_ptr);

	if (FAILED(hr)) throw directx_error(hr);

	effect_.reset(effect_ptr);
}

void D3D9Shader::load_from_buffer(char const *buffer, size_t len)
{
	D3D9RendererPtr renderer(renderer_);

	HRESULT hr;

	LOG_INFO("loading shader from buffer");

	ID3DXEffect *effect_ptr;
	ID3DXBuffer *compilation_errors_ptr;
	hr = D3DXCreateEffect(renderer->get_device(), buffer, len, 0, 0, SCRATCH_SHADER_FLAGS, 0,
		&effect_ptr, &compilation_errors_ptr);

	log_compilation_errors(compilation_errors_ptr);

	if (FAILED(hr)) throw directx_error(hr);

	effect_.reset(effect_ptr);
}

void D3D9Shader::log_compilation_errors(ID3DXBuffer *compilation_errors_ptr)
{
	if (compilation_errors_ptr && compilation_errors_ptr->GetBufferSize() > 0)
	{
		LOG_WARN("*** compilation messages:" << std::string((char const *) compilation_errors_ptr->GetBufferPointer(),
				compilation_errors_ptr->GetBufferSize()));
		compilation_errors_ptr->Release();
	}
}

void D3D9Shader::handle_lost_device()
{
	HRESULT hr = effect_->OnLostDevice();
	if (FAILED(hr)) throw directx_error(hr);
}

void D3D9Shader::handle_reset_device()
{
	HRESULT hr = effect_->OnResetDevice();
	if (FAILED(hr)) throw directx_error(hr);
}

Shader::ParameterPtr D3D9Shader::get_parameter(std::string const &name)
{
	return Shader::ParameterPtr(new D3D9Parameter(shared_from_this(), name));
}

D3D9Shader::D3D9Shader(D3D9RendererPtr const &renderer_ptr):
	renderer_(renderer_ptr)
{
}

}

