
#include <boost/noncopyable.hpp>
#include "vertex.h"
#include "vertex_buffer.h"
#include "vertex_array.h"
#include "index_buffer.h"
#include "index_array.h"
#include "material.h"
#include "texture.h"
#include "draw_call.h"
#include "uber_draw_call.h"
#include "joint.h"
#include "mesh.h"
#include "memory_mesh.h"
#include "model.h"
#include "static_model.h"
#include "animated_model.h"
#include "memory_model.h"
#include "collada_loader.h"
#include "camera.h"
#include "font.h"
#include "renderer.h"
#include "scheduler.h"
#include "bind.h"

namespace graphic
{

void bind(lua_State *L)
{
	bind_vertex(L);
	VertexBuffer::bind(L);
	bind_vertex_array(L);
	IndexBuffer::bind(L);
	bind_index_array(L);
	Material::bind(L);
	Texture::bind(L);
	DrawCall::bind(L);
	UberDrawCall::bind(L);
	Joint::bind(L);
    Mesh::bind(L);
	MemoryMesh::bind(L);
    Model::bind(L);
    bind_static_model(L);
	bind_animated_model(L);
	MemoryModel::bind(L);
	ColladaLoader::bind(L);
	Camera::bind(L);
	Font::bind(L);
    Renderer::bind(L);
	Scheduler::bind(L);
}

}
