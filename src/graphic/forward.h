#pragma once

#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>

namespace graphic
{

class Camera;
class D3D9Renderer;
class DrawCall;
class Font;
class IndexBuffer;
class Joint;
class Material;
class Mesh;
class MeshShader;
class MeshShader;
class Model;
class Renderer;
class Scheduler;
class Shader;
class Texture;
class UberDrawCall;
class UberShader;
class VertexBuffer;
struct Animation;

typedef boost::shared_ptr<Animation> AnimationPtr;
typedef boost::shared_ptr<Camera> CameraPtr;
typedef boost::shared_ptr<D3D9Renderer> D3D9RendererPtr;
typedef boost::shared_ptr<DrawCall> DrawCallPtr;
typedef boost::shared_ptr<Font> FontPtr;
typedef boost::shared_ptr<IndexBuffer> IndexBufferPtr;
typedef boost::shared_ptr<Joint> JointPtr;
typedef boost::shared_ptr<Material> MaterialPtr;
typedef boost::shared_ptr<Mesh> MeshPtr;
typedef boost::shared_ptr<MeshShader> MeshShaderPtr;
typedef boost::shared_ptr<Model> ModelPtr;
typedef boost::shared_ptr<Renderer> RendererPtr;
typedef boost::shared_ptr<Scheduler> SchedulerPtr;
typedef boost::shared_ptr<Shader> ShaderPtr;
typedef boost::shared_ptr<Texture> TexturePtr;
typedef boost::shared_ptr<UberDrawCall> UberDrawCallPtr;
typedef boost::shared_ptr<UberShader> UberShaderPtr;
typedef boost::shared_ptr<VertexBuffer> VertexBufferPtr;
typedef boost::weak_ptr<D3D9Renderer> D3D9RendererWeakPtr;
typedef boost::weak_ptr<Joint> JointWeakPtr;
typedef boost::weak_ptr<Mesh> MeshWeakPtr;
typedef boost::weak_ptr<Renderer> RendererWeakPtr;

}

