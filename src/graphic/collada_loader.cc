
#include <set>
#include <map>
#include <string>
#include <sstream>
#include <memory>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <util/xerces_lcp_transcoder.h>
#include <util/logger.h>
#include <math/matrix.h>
#include "generic_vertex.h"
#include "collada_loader.h"

#if defined(max)
#undef max
#endif

namespace collada_elements
{

using namespace util;
using namespace graphic;

class mesh_e;
class library_visual_scenes_e;
class library_geometries_e;
class library_controllers_e;
class library_materials_e;
class library_effects_e;
class library_images_e;

class error {
public:
	error(std::wstring const &message):
		message_(message)
	{
	}

	std::wstring const &what() const { return message_; }

private:
	std::wstring message_;
};

class root_element {
public:
	virtual Model *get_model() const = 0;
	virtual library_visual_scenes_e *get_visual_scenes() const = 0;
	virtual library_geometries_e *get_geometries() const = 0;
	virtual library_controllers_e *get_controllers() const = 0;
	virtual library_materials_e *get_materials() const = 0;
	virtual library_effects_e *get_effects() const = 0;
	virtual library_images_e *get_images() const = 0;
	virtual boost::filesystem::wpath const &get_basepath() const = 0;
};

class element {
private:
	root_element *root_;

public:
	element():
		root_(0)
	{
	}

	void go(xercesc::DOMElement const *element_ptr) {
		for (xercesc::DOMNode const *child_ptr = element_ptr->getFirstChild();
			 child_ptr != 0; child_ptr = child_ptr->getNextSibling())
		{
			if (child_ptr->getNodeType() == xercesc::DOMNode::ELEMENT_NODE) {
				process_child(child_ptr->getLocalName(), xerces_get_attribute<std::wstring>(child_ptr, L"id"),
					(xercesc::DOMElement *) child_ptr);
			}
		}
	}

	root_element *get_root() const { return root_; }
	Model *get_model() const { return root_->get_model(); }
	void set_root(root_element *root) { root_ = root; }

	void init(xercesc::DOMElement const *element_ptr) {}
	virtual void process_child(std::wstring const &child_name, std::wstring const &child_id, xercesc::DOMElement const *child_ptr) {}
	void check() {}

	template <class T> std::auto_ptr<T>
	construct(xercesc::DOMElement const *element_ptr)
	{
		std::auto_ptr<T> holder(new T());
		holder->set_root(get_root());
		holder->init(element_ptr);
		holder->go(element_ptr);
		holder->check();
		return holder;
	}

	template<class T> static void
	parse_list(xercesc::DOMElement const *element_ptr, std::vector<T> &v)
	{
		std::wstringstream ss(xerces_get_text(element_ptr));
		T dummy;
		while (ss >> dummy) v.push_back(dummy);
		if (ss.bad()) throw error(L"can't parse list");
	}

	template<class T> static unsigned
	parse_list(xercesc::DOMElement const *element_ptr, T *list_ptr, unsigned size)
	{
		std::wstringstream ss(xerces_get_text(element_ptr));
		unsigned i = 0;
		while (i < size && ss >> *list_ptr++) i++;
		return i;
	}

	void parse_matrix(xercesc::DOMElement const *element_ptr, math::matrix<4, 4> &tf)
	{
		if (parse_list(element_ptr, &tf.ij[0][0], 16) != 16) throw error(L"invalid <matrix>");
		tf.transpose_inplace();
	}
};

class technique_common_e: public element {
private:
	struct param {
		std::wstring name;
		std::wstring type;

		param(std::wstring const &n, std::wstring const &t):
			name(n),
			type(t)
		{
		}
	};

	unsigned count_, stride_;
	std::vector<param> params_;
	std::map<std::wstring, std::wstring> instance_materials_;

	void process_child(std::wstring const &child_name, std::wstring const &child_id, xercesc::DOMElement const *child_ptr)
	{
		if (child_name == L"accessor")
		{
			count_ = xerces_get_attribute<unsigned>(child_ptr, L"count");
			stride_ = xerces_get_attribute<unsigned>(child_ptr, L"stride", 3);

			xercesc::DOMNodeList const *params = child_ptr->getElementsByTagName(L"param");

			for (unsigned i = 0, n = params->getLength(); i < n; i++)
			{
				xercesc::DOMNode const *node_e = params->item(i);
				params_.push_back(param(xerces_get_attribute<std::wstring>(node_e, L"name"), xerces_get_attribute<std::wstring>(node_e, L"type")));
			}
		}
		else if (child_name == L"instance_material")
		{
			std::wstring const &symbol = xerces_get_attribute<std::wstring>(child_ptr, L"symbol");
			std::wstring const &target = xerces_get_attribute<std::wstring>(child_ptr, L"target");
			instance_materials_.insert(std::make_pair(symbol, target.substr(1)));
		}
	}

public:
	unsigned get_stride() const { return stride_; }
	unsigned get_count() const { return count_; }

	std::wstring const &get_material_by_symbol(std::wstring const &symbol) const
	{
		std::map<std::wstring, std::wstring>::const_iterator it = instance_materials_.find(symbol);
		if (it != instance_materials_.end()) return it->second;

		throw error(L"<instance_material> with symbol = " + symbol + L" not found");
	}
};

class source_e: public element {
private:
	std::vector<float> float_array_;
	std::vector<std::wstring> name_array_;
	std::auto_ptr<technique_common_e> technique_;

	void process_child(std::wstring const &child_name, std::wstring const &child_id, xercesc::DOMElement const *child_ptr)
	{
		if (child_name == L"float_array")
		{
			parse_list(child_ptr, float_array_);
		}
		else if (child_name == L"Name_array")
		{
			parse_list(child_ptr, name_array_);
		}
		else if (child_name == L"technique_common")
		{
			technique_ = construct<technique_common_e>(child_ptr);
		}
	}

public:
	technique_common_e const &get_technique() const { return *technique_; }

	void check() {
		if (float_array_.empty() && name_array_.empty()) throw error(L"malformed or unknown <source>");
		if (!technique_.get()) throw error(L"no <technique_common> in <source>");

		unsigned array_size = std::max(float_array_.size(), name_array_.size());
		unsigned expected_size = technique_->get_stride() * technique_->get_count();

		if (array_size != expected_size)
		{
			throw error(L"array has " + boost::lexical_cast<std::wstring>(expected_size) 
				+ L" elements, parsed " + boost::lexical_cast<std::wstring>(array_size) + L" elements");
		}

	}

	unsigned get_size() const {
		return std::max(name_array_.size(), float_array_.size());
	}

	void get(unsigned index, std::wstring &str) const {
		str = name_array_.at(index);
	}

	void get(unsigned index, float &f) const {
		f = float_array_.at(index);
	}

	template <int N>
	void get(unsigned index, math::vec<N> &v) const {
		unsigned array_index = index * technique_->get_stride();

		for (unsigned i = 0; i < N; i++) {
			v.i[i] = float_array_.at(array_index++);
		}
	}

	void get(unsigned index, math::matrix<4, 4> &tf) const {
		unsigned array_index = index * technique_->get_stride();

		for (unsigned i = 0; i < 4; i++) {
			for (unsigned j = 0; j < 4; j++) {
				tf.ij[j][i] = float_array_.at(array_index++);
			}
		}
	}
};

class sources_list_e: public element {
protected:
	boost::ptr_map<std::wstring, source_e> sources_;

	void process_child(std::wstring const &child_name, std::wstring const &child_id, xercesc::DOMElement const *child_ptr) {
		if (child_name == L"source") {
			sources_.insert(const_cast<std::wstring &>(child_id), construct<source_e>(child_ptr));
		}
	}

public:
	source_e const *get_source(std::wstring const &id) const {
		boost::ptr_map<std::wstring, source_e>::const_iterator it = sources_.find(id);
		if (it == sources_.end()) throw error(L"source id = " + id + L" not found");
		return it->second;
	}
};

class transform_e: public element {
protected:
	std::wstring sid_;

public:
	void init(xercesc::DOMElement const *element_ptr)
	{
		sid_ = xerces_get_attribute<std::wstring>(element_ptr, L"sid");
	}

	std::wstring const &get_sid() const { return sid_; }

	virtual math::matrix<4,4> get_baked_matrix() const = 0;
	virtual math::matrix<4,4> get_baked_sample(source_e const *source_ptr, int index, std::wstring const &param) const = 0;
};

class identity_transform_e: public transform_e {
public:
	math::matrix<4,4> get_baked_matrix() const
	{
		math::matrix<4,4> m;
		m.identity();
		return m;
	}

	math::matrix<4,4> get_baked_sample(source_e const *source_ptr, int index, std::wstring const &param) const
	{
		throw error(L"can't read identity transform sample from source");
	}
};


class matrix_transform_e: public transform_e {
private:
	math::matrix<4,4> matrix_;

public:
	void init(xercesc::DOMElement const *element_ptr)
	{
		transform_e::init(element_ptr);
		parse_matrix(element_ptr, matrix_);
	}

	math::matrix<4,4> get_baked_matrix() const
	{
		return matrix_;
	}

	math::matrix<4,4> get_baked_sample(source_e const *source_ptr, int index, std::wstring const &param) const
	{
		math::matrix<4,4> result;
		source_ptr->get(index, result);
		return result;
	}
};

class translation_transform_e: public transform_e {
private:
	math::vec<3> translate_;

public:
	void init(xercesc::DOMElement const *element_ptr)
	{
		transform_e::init(element_ptr);
		parse_list(element_ptr, translate_.i, 3);
	}

	math::matrix<4,4> get_baked_matrix() const
	{
		math::matrix<4,4> result;
		result.translation(translate_);
		return result;
	}

	math::matrix<4,4> get_baked_sample(source_e const *source_ptr, int index, std::wstring const &param) const
	{
		math::vec<3> translate;
		source_ptr->get(index, translate);

		math::matrix<4,4> result;
		result.translation(translate);

		return result;
	}
};

class rotation_transform_e: public transform_e {
private:
	math::vec<3> axis_;
	math::scalar angle_;

public:
	void init(xercesc::DOMElement const *element_ptr)
	{
		transform_e::init(element_ptr);

		math::scalar s[4];
		parse_list(element_ptr, s, 4);

		axis_.set(s[0], s[1], s[2]);
		angle_ = s[3];
	}

	math::matrix<4,4> get_baked_matrix() const
	{
		math::matrix<4,4> result;
		result.rotation(axis_, angle_ * math::PI / 180.0f);
		return result;
	}

	math::matrix<4,4> get_baked_sample(source_e const *source_ptr, int index, std::wstring const &param) const
	{
		if (param != L"ANGLE") throw error(L"unexpected rotation transform param type " + param);

		math::scalar angle;
		source_ptr->get(index, angle);

		math::matrix<4,4> result;
		result.rotation(axis_, angle * math::PI / 180.0f);

		return result;
	}
};

class scale_transform_e: public transform_e {
private:
	math::vec<3> scale_;

public:
	void init(xercesc::DOMElement const *element_ptr)
	{
		transform_e::init(element_ptr);
		parse_list(element_ptr, scale_.i, 3);
	}

	math::matrix<4,4> get_baked_matrix() const
	{
		math::matrix<4,4> result;
		result.scaling(scale_);
		return result;
	}

	math::matrix<4,4> get_baked_sample(source_e const *source_ptr, int index, std::wstring const &param) const
	{
		math::vec<3> scale;
		source_ptr->get(index, scale);

		math::matrix<4,4> result;
		result.scaling(scale);

		return result;
	}
};

class bind_material_e: public element {
private:
	std::auto_ptr<technique_common_e> technique_common_;

	void process_child(std::wstring const &child_name, std::wstring const &child_id, xercesc::DOMElement const *child_ptr)
	{
		if (child_name == L"technique_common")
		{
			technique_common_ = construct<technique_common_e>(child_ptr);
		}
	}

public:
	void bind_mesh_materials(mesh_e const *, MeshPtr mesh_ptr) const;

	technique_common_e const *get_technique_common() const
	{
		return technique_common_.get();
	}
};

class instance_controller_e: public element {
private:
	std::wstring url_;
	std::wstring skeleton_;
	std::auto_ptr<bind_material_e> bind_material_;

	void process_child(std::wstring const &child_name, std::wstring const &child_id, xercesc::DOMElement const *child_ptr)
	{
		if (child_name == L"skeleton")
		{
			skeleton_ = xerces_get_text(child_ptr).substr(1);
		}
		else if (child_name == L"bind_material")
		{
			bind_material_ = construct<bind_material_e>(child_ptr);
		}
	}

public:
	std::wstring const &get_url() const { return url_; }
	std::wstring const &get_skeleton() const { return skeleton_; }

	bind_material_e const *get_bind_material() const
	{
		return bind_material_.get();
	}

	void init(xercesc::DOMElement const *element_ptr)
	{
		url_ = xerces_get_attribute<std::wstring>(element_ptr, L"url").substr(1);
	}
};

class instance_geometry_e: public element {
private:
	std::wstring url_;
	std::auto_ptr<bind_material_e> bind_material_;

	void process_child(std::wstring const &child_name, std::wstring const &child_id, xercesc::DOMElement const *child_ptr)
	{
		if (child_name == L"bind_material")
		{
			bind_material_ = construct<bind_material_e>(child_ptr);
		}
	}

public:
	std::wstring const &get_url() const
	{
		return url_;
	}

	bind_material_e const *get_bind_material() const
	{
		return bind_material_.get();
	}

	void init(xercesc::DOMElement const *element_ptr)
	{
		url_ = xerces_get_attribute<std::wstring>(element_ptr, L"url").substr(1);
	}
};

class node_e: public element {
protected:
	std::wstring id_, sid_, name_, type_;
	node_e *parent_;
	boost::ptr_vector<node_e> childs_;
	boost::ptr_vector<transform_e> transforms_;
	std::auto_ptr<instance_controller_e> instance_controller_;
	std::auto_ptr<instance_geometry_e> instance_geometry_;

	void process_child(std::wstring const &child_name, std::wstring const &child_id, xercesc::DOMElement const *child_ptr)
	{
		if (child_name == L"node")
		{
			childs_.push_back(construct<node_e>(child_ptr));
			childs_.back().parent_ = this;
		}
		else if (child_name == L"matrix")
		{
			transforms_.push_back(construct<matrix_transform_e>(child_ptr));
		}
		else if (child_name == L"translate")
		{
			transforms_.push_back(construct<translation_transform_e>(child_ptr));
		}
		else if (child_name == L"rotate")
		{
			transforms_.push_back(construct<rotation_transform_e>(child_ptr));
		}
		else if (child_name == L"scale")
		{
			transforms_.push_back(construct<scale_transform_e>(child_ptr));
		}
		else if (child_name == L"instance_geometry")
		{
			instance_geometry_ = construct<instance_geometry_e>(child_ptr);
		}
		else if (child_name == L"instance_controller")
		{
			instance_controller_ = construct<instance_controller_e>(child_ptr);
		}
	}

public:
	void init(xercesc::DOMElement const *element_ptr) {
		id_ = xerces_get_attribute<std::wstring>(element_ptr, L"id");
		sid_ = xerces_get_attribute<std::wstring>(element_ptr, L"sid");
		name_ = xerces_get_attribute<std::wstring>(element_ptr, L"name");
		type_ = xerces_get_attribute<std::wstring>(element_ptr, L"type");
	}

	void check() {
		if (transforms_.empty()) {
			transforms_.push_back(new identity_transform_e());
		}
	}

	void build_hierarchy(JointPtr parent_ptr);

	node_e const *get_node_by_id(std::wstring const &id) const
	{
		if (id == id_) return this;

		for (boost::ptr_vector<node_e>::const_iterator it = childs_.begin(); it != childs_.end(); it++)
		{
			if (node_e const *node_ptr = it->get_node_by_id(id)) return node_ptr;
		}

		return 0;
	}

	transform_e const *get_transform_by_sid(std::wstring const &sid) const
	{
		for (boost::ptr_vector<transform_e>::const_iterator it = transforms_.begin();
			 it != transforms_.end(); it ++)
		{
			if (it->get_sid() == sid) return &*it;
		}

		throw error(L"transform with sid = " + sid + L" not found in node id = " + id_);
	}
};

class visual_scene_e: public node_e {
public:
	void check() {
	}

	void build_hierarchy() {
		for (boost::ptr_vector<node_e>::iterator it = childs_.begin(); it != childs_.end(); it++) {
			it->build_hierarchy(JointPtr());
		}
	}
};

class library_visual_scenes_e: public element {
protected:
	boost::ptr_map<std::wstring, visual_scene_e> visual_scenes_;

	void process_child(std::wstring const &child_name, std::wstring const &child_id, xercesc::DOMElement const *child_ptr) {
		if (child_name == L"visual_scene") {
			visual_scenes_.insert(const_cast<std::wstring &>(child_id), construct<visual_scene_e>(child_ptr));
		}
	}

public:
	node_e const *get_node_by_id(std::wstring const &id) const
	{
		for (boost::ptr_map<std::wstring, visual_scene_e>::const_iterator it = visual_scenes_.begin();
			 it != visual_scenes_.end(); it++)
		{
			if (node_e const *node_ptr = it->second->get_node_by_id(id)) return node_ptr;
		}

		throw error(L"node with id = " + id + L" not found");
	}

	void build_hierarchy() {
		if (visual_scenes_.size() != 1) throw error(L"expecting one <visual_scene> in <library_visual_scenes>");
		visual_scenes_.begin()->second->build_hierarchy();
	}
};

class input_e: public element {
private:
	std::wstring source_;
	unsigned offset_;

public:
	void init(xercesc::DOMElement const *element_ptr) {
		source_ = xerces_get_attribute<std::wstring>(element_ptr, L"source");
		offset_ = xerces_get_attribute<unsigned>(element_ptr, L"offset");
	}

	std::wstring const &get_source() const { return source_; }
	unsigned get_offset() const { return offset_; }
};

class inputs_list_e: public element {
protected:
	typedef boost::ptr_map<std::wstring, input_e> inputs_type;
	inputs_type inputs_;

	void process_child(std::wstring const &child_name, std::wstring const &child_id, xercesc::DOMElement const *child_ptr) {
		if (child_name == L"input") {
			inputs_.insert(xerces_get_attribute<std::wstring>(child_ptr, L"semantic"), construct<input_e>(child_ptr));
		}
	}

public:
	unsigned get_stride() const
	{
		if (inputs_.empty()) throw error(L"trying to calculate stride of empty input list");

		unsigned max_offset = 0;

		for (inputs_type::const_iterator it = inputs_.begin(); it != inputs_.end(); it++)
		{
			if (it->second->get_offset() > max_offset) max_offset = it->second->get_offset(); 
		}

		return 1 + max_offset;
	}

	input_e const *get_input_nothrow(std::wstring const &semantic) const
	{
		inputs_type::const_iterator it = inputs_.find(semantic);
		if (it == inputs_.end()) return 0;
		return it->second;
	}

	input_e const *get_input(std::wstring const &semantic) const
	{
		input_e const *input_ptr = get_input_nothrow(semantic);
		if (!input_ptr) throw error(L"input with semantic = " + semantic + L" not found");
		return input_ptr;
	}
};

class vertices_e: public inputs_list_e {
};

class triangles_e: public inputs_list_e {
private:
	unsigned count_;
	std::vector<unsigned> indices_;
	std::wstring material_;

	void process_child(std::wstring const &child_name, std::wstring const &child_id, xercesc::DOMElement const *child_ptr)
	{
		if (child_name == L"p")
		{
			parse_list(child_ptr, indices_);
		}

		inputs_list_e::process_child(child_name, child_id, child_ptr);
	}

public:
	void init(xercesc::DOMElement const *element_ptr)
	{
		count_ = xerces_get_attribute<unsigned>(element_ptr, L"count");
		material_ = xerces_get_attribute<std::wstring>(element_ptr, L"material");

		LOG_DEBUG("triangles count = " << count_ << ", material = " << xerces_lcp_transcode(material_));
	}

	unsigned get_count() const { return count_; }
	std::wstring const &get_material() const { return material_; }
	std::vector<unsigned> const &get_indices() const { return indices_; }
};

class vertex_indexer {
public:
	typedef GenericVertex vertex_type;

	std::pair<unsigned, bool> choose_index(vertex_type const &v)
	{
		vertex_x_index_type::const_iterator it = vertex_x_index_.find(v);
		if (it != vertex_x_index_.end()) return std::pair<unsigned, bool>(it->second, false);

		unsigned index = vertex_x_index_.size();
		vertex_x_index_.insert(std::make_pair(v, index));
		return std::pair<unsigned, bool>(index, true);
	}

private:
	typedef std::map<vertex_type, unsigned> vertex_x_index_type;
	vertex_x_index_type vertex_x_index_;
};

class mesh_e: public sources_list_e {
public:
	typedef std::set<std::pair<unsigned, unsigned> > position_index_to_vertex_index_type;

	mesh_e():
		vertex_format_(0)
	{
	}

	void check() {
		if (!vertices_.get()) throw error(L"no <vertices> found");
		if (triangles_.empty()) throw error(L"no <triangles> found");
	}

	void build_mesh(MeshPtr mesh_ptr);

	boost::ptr_vector<triangles_e> const &get_triangles() const
	{
		return triangles_;
	}

	position_index_to_vertex_index_type const &get_position_index_to_vertex_index_mapping()
	{
		return position_index_to_vertex_index_;
	}

	unsigned get_vertices_count() const { return vertex_array_.size(); }

private:
	std::auto_ptr<vertices_e> vertices_;
	boost::ptr_vector<triangles_e> triangles_;

	vertex_indexer vertex_indexer_;
	position_index_to_vertex_index_type position_index_to_vertex_index_;

	GenericVertex::format_type vertex_format_;
	std::vector<GenericVertex> vertex_array_;
	std::vector<unsigned> index_array_;

	void process_child(std::wstring const &child_name, std::wstring const &child_id, xercesc::DOMElement const *child_ptr) {
		sources_list_e::process_child(child_name, child_id, child_ptr);

		if (child_name == L"vertices")
		{
			vertices_ = construct<vertices_e>(child_ptr);
		}
		else if (child_name == L"triangles" || child_name == L"polygons")
		{
			triangles_.push_back(construct<triangles_e>(child_ptr));
		}
	}
};

class geometry_e: public element {
private:
	std::wstring id_, name_;
	std::auto_ptr<mesh_e> mesh_;

	void process_child(std::wstring const &child_name, std::wstring const &child_id, xercesc::DOMElement const *child_ptr) {
		if (child_name == L"mesh")
		{
			mesh_ = construct<mesh_e>(child_ptr);
		}
	}

public:
	std::wstring const &get_id() const { return id_; }
	std::wstring const &get_name() const { return name_; }

	void init(xercesc::DOMElement const *element_ptr)
	{
		id_ = xerces_get_attribute<std::wstring>(element_ptr, L"id");
		name_ = xerces_get_attribute<std::wstring>(element_ptr, L"name");
	}

	void check()
	{
		if (!mesh_.get()) throw error(L"<mesh> not found in <geometry>");
	}

	mesh_e *get_mesh() const
	{
		return mesh_.get();
	}
};

class library_geometries_e: public element {
protected:
	boost::ptr_vector<geometry_e> geometries_;

	void process_child(std::wstring const &child_name, std::wstring const &child_id, xercesc::DOMElement const *child_ptr) {
		if (child_name == L"geometry")
		{
			geometries_.push_back(construct<geometry_e>(child_ptr));
		}
	}

public:
	void build_meshes()
	{
		for (boost::ptr_vector<geometry_e>::iterator it = geometries_.begin(); it != geometries_.end(); it++)
		{
			MeshPtr mesh_ptr = get_model()->add_mesh();

			mesh_ptr->id = xerces_lcp_transcode(it->get_id());
			mesh_ptr->name = xerces_lcp_transcode(it->get_name());

			LOG_DEBUG("building mesh id = " << mesh_ptr->id << ", name = " << mesh_ptr->name);

			it->get_mesh()->build_mesh(mesh_ptr);
		}
	}

	geometry_e const *get_geometry_by_id(std::wstring const &id)
	{
		for (boost::ptr_vector<geometry_e>::iterator it = geometries_.begin(); it != geometries_.end(); it++)
		{
			if (it->get_id() == id) return &*it;
		}

		throw error(L"geometry id = " + id + L" not found");
	}
};

class joints_e: public inputs_list_e {
};

class vertex_weights_e: public inputs_list_e {
private:
	std::vector<unsigned> vcount_;
	std::vector<unsigned> v_;

	void process_child(std::wstring const &child_name, std::wstring const &child_id, xercesc::DOMElement const *child_ptr) {
		inputs_list_e::process_child(child_name, child_id, child_ptr);

		if (child_name == L"vcount")
		{
			parse_list(child_ptr, vcount_);
		}
		else if (child_name == L"v")
		{
			parse_list(child_ptr, v_);
		}
	}

public:
	std::vector<unsigned> const &get_vcount() const { return vcount_; }
	std::vector<unsigned> const &get_v() const { return v_; }
};

class skin_e: public sources_list_e {
private:
	math::matrix<4,4> bind_shape_matrix_;
	std::auto_ptr<joints_e> joints_;
	std::auto_ptr<vertex_weights_e> vertex_weights_;

	void process_child(std::wstring const &child_name, std::wstring const &child_id, xercesc::DOMElement const *child_ptr)
	{
		sources_list_e::process_child(child_name, child_id, child_ptr);

		if (child_name == L"bind_shape_matrix")
		{
			parse_matrix(child_ptr, bind_shape_matrix_);
		}
		else if (child_name == L"joints")
		{
			joints_ = construct<joints_e>(child_ptr);
		}
		else if (child_name == L"vertex_weights")
		{
			vertex_weights_ = construct<vertex_weights_e>(child_ptr);
		}
	}

public:
	void build_inverted_bind_matrices(MeshPtr mesh_ptr)
	{
		mesh_ptr->bind_shape_matrix = bind_shape_matrix_;

		{
			math::matrix<4,4> t;
			t.identity();
			mesh_ptr->inverted_bind_matrices.resize(get_model()->joints.size(), t);
		}

		source_e const *joints_source = get_source(joints_->get_input(L"JOINT")->get_source().substr(1));
		source_e const *inverted_bind_matrices_source = get_source(joints_->get_input(L"INV_BIND_MATRIX")->get_source().substr(1));

		for (unsigned i = 0, n = joints_source->get_size(); i < n; i++)
		{
			std::wstring joint_name;
			math::matrix<4,4> inv_bind_matrix;

			joints_source->get(i, joint_name);
			inverted_bind_matrices_source->get(i, inv_bind_matrix);

			unsigned joint_index = mesh_ptr->get_model()->get_joint_by_name(xerces_lcp_transcode(joint_name))->index;
			mesh_ptr->inverted_bind_matrices[joint_index] = inv_bind_matrix;
		}
	}

	void build_joint_weights(MeshPtr mesh_ptr)
	{
		mesh_e *mesh_element_ptr = get_root()->get_geometries()->get_geometry_by_id(xerces_lcp_transcode(mesh_ptr->id))->get_mesh();
		std::vector<GenericVertex> vertices(mesh_element_ptr->get_vertices_count());

		mesh_e::position_index_to_vertex_index_type const &position_index_to_vertex_index = mesh_element_ptr->get_position_index_to_vertex_index_mapping();

		input_e const *joints_input = vertex_weights_->get_input(L"JOINT");
		input_e const *weights_input = vertex_weights_->get_input(L"WEIGHT");

		source_e const *joints_source = get_source(joints_input->get_source().substr(1));
		source_e const *weights_source = get_source(weights_input->get_source().substr(1));

		unsigned stride = 1 + std::max(joints_input->get_offset(), weights_input->get_offset());

		std::vector<unsigned> const &vcount = vertex_weights_->get_vcount();
		std::vector<unsigned> const &v = vertex_weights_->get_v();

		std::vector<unsigned>::const_iterator v_get = v.begin();

		unsigned position_index = 0;

		mesh_ptr->joint_index_transform.resize(joints_source->get_size());

		for (unsigned i = 0; i < joints_source->get_size(); i++)
		{
			std::wstring joint_name;
			joints_source->get(i, joint_name);
			mesh_ptr->joint_index_transform[i] = mesh_ptr->get_model()->get_joint_by_name(xerces_lcp_transcode(joint_name))->index;
		}

		for (std::vector<unsigned>::const_iterator it = vcount.begin(); it != vcount.end(); it++)
		{
			math::vec<8, unsigned> joint_indices;
			math::vec<8> joint_weights;

			joint_indices.set_all(0);
			joint_weights.set_all(0);

			for (unsigned i = 0, j = 0; i < *it; i++)
			{
				unsigned index = 0;
				math::scalar weight = 0;

				for (unsigned offset = 0; offset < stride; offset++)
				{
					if (offset == joints_input->get_offset())
					{
						index = *v_get;
					}

					if (offset == weights_input->get_offset())
					{
						weights_source->get(*v_get, weight);
					}

					v_get++;
				}

				if (weight > math::EPSILON)
				{
					if (j == 8)
					{
						unsigned ligtest_index = unsigned(-1);
						float ligtest_weight = weight;

						for (unsigned k = 0; k < 8; k++)
						{
							if (joint_weights.i[k] < ligtest_weight)
							{
								ligtest_index = k;
								ligtest_weight = joint_weights.i[k];
							}
						}

						if (ligtest_index == unsigned(-1))
						{
							LOG_WARN("vertex with more than 8 joint weights found in mesh " << mesh_ptr->id
								<< " and last one is lightest with weight = " << weight
								<< ", index = " << index << " -- ignoring it");
						}
						else
						{
							LOG_WARN("vertex with more than 8 joint weights found in mesh " << mesh_ptr->id
								<< " and lightest weight index = " << ligtest_index
								<< ", weight = " << ligtest_weight
								<< ", joint = " << joint_indices.i[ligtest_index]
								<< " will be replaced with joint = " << index
								<< ", weight = " << weight);

							joint_indices.i[ligtest_index] = index;
							joint_weights.i[ligtest_index] = weight;
						}
					}
					else
					{
						joint_indices.i[j] = index;
						joint_weights.i[j] = weight;

						j++;
					}
				}
			}

			// for all vertices coresponding to this position
			mesh_e::position_index_to_vertex_index_type::const_iterator it =
				position_index_to_vertex_index.lower_bound(std::make_pair(position_index, 0));

			while (it != position_index_to_vertex_index.end() && it->first == position_index)
			{
				vertices.at(it->second).joints = joint_indices;
				vertices.at(it->second).weights = joint_weights;

				it++;
			}

			position_index++;
		}

		mesh_ptr->set_vertices(&vertices[0], GenericVertex::FORMAT_JOINT_INDICES | GenericVertex::FORMAT_JOINT_WEIGHTS,
			vertices.size());
	}
};

class controller_e: public element {
private:
	boost::ptr_map<std::wstring, skin_e> skins_;

	void process_child(std::wstring const &child_name, std::wstring const &child_id, xercesc::DOMElement const *child_ptr)
	{
		skins_.insert(xerces_get_attribute<std::wstring>(child_ptr, L"source"), construct<skin_e>(child_ptr));
	}

public:
	std::wstring get_skin_source_id() const
	{
		if (skins_.size() != 1) throw error(L"zero or more than one <skin> in <controller>");
		return skins_.begin()->first.substr(1);
	}

	void build_joint_influences() {
		for (boost::ptr_map<std::wstring, skin_e>::iterator it = skins_.begin(); it != skins_.end(); it++)
		{
			MeshPtr mesh_ptr = get_model()->get_mesh_by_id(xerces_lcp_transcode(it->first.substr(1)));
			it->second->build_inverted_bind_matrices(mesh_ptr);
			it->second->build_joint_weights(mesh_ptr);
		}
	}
};

class library_controllers_e: public element {
private:
	boost::ptr_map<std::wstring, controller_e> controllers_;

	void process_child(std::wstring const &child_name, std::wstring const &child_id, xercesc::DOMElement const *child_ptr)
	{
		if (child_name == L"controller")
		{
			controllers_.insert(child_id, construct<controller_e>(child_ptr));
		}
	}

public:
	controller_e const *get_controller_by_id(std::wstring const &id) const
	{
		boost::ptr_map<std::wstring, controller_e>::const_iterator it = controllers_.find(id);
		if (it != controllers_.end()) return it->second;

		throw error(L"controller with id = " + id + L" not found");
	}

	void build_joint_influences()
	{
		for (boost::ptr_map<std::wstring, controller_e>::iterator it = controllers_.begin(); it != controllers_.end(); it++)
		{
			it->second->build_joint_influences();
		}
	}
};

class sampler_e: public inputs_list_e {
private:
	void process_child(std::wstring const &child_name, std::wstring const &child_id, xercesc::DOMElement const *child_ptr) {
		inputs_list_e::process_child(child_name, child_id, child_ptr);
	}
};

class animation_e: public sources_list_e {
private:
	struct channel {
		std::wstring source_url;
		std::wstring target_url;

		channel(std::wstring const &s, std::wstring const &t):
			source_url(s),
			target_url(t)
		{
		}
	};

	std::vector<channel> channels_;
	boost::ptr_map<std::wstring, sampler_e> samplers_;

	void process_child(std::wstring const &child_name, std::wstring const &child_id, xercesc::DOMElement const *child_ptr)
	{
		sources_list_e::process_child(child_name, child_id, child_ptr);

		if (child_name == L"sampler")
		{
			samplers_.insert(child_id, construct<sampler_e>(child_ptr));
		}
		else if (child_name == L"channel")
		{
			channels_.push_back(channel(xerces_get_attribute<std::wstring>(child_ptr, L"source"), xerces_get_attribute<std::wstring>(child_ptr, L"target")));
		}
	}

public:
	Animation build_animation(Model *model_ptr)
	{
		Animation result;

		for (std::vector<channel>::const_iterator it = channels_.begin(); it != channels_.end(); it++) {
			boost::ptr_map<std::wstring, sampler_e>::iterator jt = samplers_.find(it->source_url.substr(1));
			if (jt == samplers_.end()) throw error(L"sampler for url " + it->source_url + L" not found");

			sampler_e *sampler_ptr = jt->second;

			source_e const *time_input = get_source(sampler_ptr->get_input(L"INPUT")->get_source().substr(1));
			source_e const *transform_output = get_source(sampler_ptr->get_input(L"OUTPUT")->get_source().substr(1));

			Sampler dummy;
			transform_e const *transform_ptr = 0;

			// target is in form <node>/<transform>.<param>, <param> may be omitted
			std::vector<std::wstring> parsed_target_components;

			{
				using namespace boost::lambda;
				using boost::lambda::_1;
				boost::split(parsed_target_components, it->target_url, _1 == '/' || _1 == '.', boost::algorithm::token_compress_on);
			}

			if (parsed_target_components.size() == 2) parsed_target_components.push_back(std::wstring());
			if (parsed_target_components.size() != 3) throw error(L"invalid url " + it->target_url);

			JointPtr joint_ptr = model_ptr->get_joint_by_id(xerces_lcp_transcode(parsed_target_components[0]));
			dummy.set_target(joint_ptr, xerces_lcp_transcode(parsed_target_components[1]));

			transform_ptr = get_root()->get_visual_scenes()->get_node_by_id(parsed_target_components[0])->get_transform_by_sid(parsed_target_components[1]);

			for (unsigned i = 0, n = time_input->get_size(); i < n; i++) {
				float t;
				time_input->get(i, t);

				math::matrix<4,4> m = transform_ptr->get_baked_sample(transform_output, i, parsed_target_components.back());

				dummy.add_sample(t, m);
			}

			result.samplers.push_back(dummy);
		}

		return result;
	}
};

class library_animations_e: public element {
private:
	boost::ptr_map<std::wstring, animation_e> animations_;

	void process_child(std::wstring const &child_name, std::wstring const &child_id, xercesc::DOMElement const *child_ptr)
	{
		if (child_name == L"animation")
		{
			animations_.insert(child_id, construct<animation_e>(child_ptr));
		}
	}

public:
	void build_animations() {
		for (boost::ptr_map<std::wstring, animation_e>::iterator it = animations_.begin(); it != animations_.end(); it++)
		{
			get_model()->animations.push_back(AnimationPtr(new Animation(it->second->build_animation(get_model()))));
			get_model()->animations.back()->name = xerces_lcp_transcode(it->first);
		}
	}
};

class material_e: public element {
private:
	std::wstring id_, name_;
	std::wstring instance_effect_url_;

	void process_child(std::wstring const &child_name, std::wstring const &child_id, xercesc::DOMElement const *child_ptr)
	{
		if (child_name == L"instance_effect")
		{
			instance_effect_url_ = xerces_get_attribute<std::wstring>(child_ptr, L"url");
		}
	}

public:
	void init(xercesc::DOMElement const *element_ptr)
	{
		id_ = xerces_get_attribute<std::wstring>(element_ptr, L"id");
		name_ = xerces_get_attribute<std::wstring>(element_ptr, L"name");
	}

	void check()
	{
		if (instance_effect_url_.empty()) throw error(L"no <instance_effect> url specified for material");
	}

	MaterialPtr build_material() const;
};

class library_materials_e: public element {
private:
	boost::ptr_map<std::wstring, material_e> materials_;

	void process_child(std::wstring const &child_name, std::wstring const &child_id, xercesc::DOMElement const *child_ptr)
	{
		if (child_name == L"material")
		{
			materials_.insert(child_id, construct<material_e>(child_ptr));
		}
	}

public:
	material_e const *get_material_by_name(std::wstring const &name) const
	{
		boost::ptr_map<std::wstring, material_e>::const_iterator it = materials_.find(name);
		if (it != materials_.end()) return it->second;

		throw error(L"material " + name + L" not found");
	}

	void build_materials() const
	{
		LOG_DEBUG("building materials...");

		for (boost::ptr_map<std::wstring, material_e>::const_iterator it = materials_.begin();
			 it != materials_.end(); it++)
		{
			get_model()->materials.push_back(it->second->build_material());
		}
	}
};

class surface_e: public element {
private:
	std::wstring init_from_;

	void process_child(std::wstring const &child_name, std::wstring const &child_id, xercesc::DOMElement const *child_ptr)
	{
		if (child_name == L"init_from")
		{
			init_from_ = xerces_get_text(child_ptr);
		}
	}

public:
	std::wstring const &get_init_from() const
	{
		return init_from_;
	}
};

class sampler2d_e: public element {
private:
	std::wstring source_;

	void process_child(std::wstring const &child_name, std::wstring const &child_id, xercesc::DOMElement const *child_ptr)
	{
		if (child_name == L"source")
		{
			source_ = xerces_get_text(child_ptr);
		}
	}

public:
	std::wstring const &get_source() const
	{
		return source_;
	}
};

class newparam_e: public element {
private:
	std::auto_ptr<surface_e> surface_;
	std::auto_ptr<sampler2d_e> sampler2d_;

	void process_child(std::wstring const &child_name, std::wstring const &child_id, xercesc::DOMElement const *child_ptr)
	{
		if (child_name == L"surface")
		{
			surface_ = construct<surface_e>(child_ptr);
		}
		else if (child_name == L"sampler2D")
		{
			sampler2d_ = construct<sampler2d_e>(child_ptr);
		}
	}

public:
	surface_e const *get_surface() const
	{
		return surface_.get();
	}

	sampler2d_e const *get_sampler() const
	{
		return sampler2d_.get();
	}

	void check()
	{
		if (surface_.get() && sampler2d_.get())
		{
			throw error(L"only one of <surface> or <sampler2D> may be defined in <newparam>");
		}
	}
};

class phong_lambert_blinn_e: public element {
private:
	std::map<std::wstring, std::wstring> textures_;

	void process_child(std::wstring const &child_name, std::wstring const &child_id, xercesc::DOMElement const *child_ptr)
	{
		xercesc::DOMNodeList *textures_list_ptr = child_ptr->getElementsByTagName(L"texture");
		if (textures_list_ptr->getLength() == 0) return;
		if (textures_list_ptr->getLength() > 1) throw error(L"more than one <texture> elements in effect");
		textures_.insert(std::make_pair(child_name, xerces_get_attribute<std::wstring>(textures_list_ptr->item(0), L"texture")));
	}

public:
	std::wstring get_sampler_name(std::wstring const &component_name) const
	{
		std::map<std::wstring, std::wstring>::const_iterator it = textures_.find(component_name);
		if (it != textures_.end()) return it->second;

		return std::wstring();
	}
};

class technique_e: public element {
private:
	std::auto_ptr<phong_lambert_blinn_e> shader_;

	void process_child(std::wstring const &child_name, std::wstring const &child_id, xercesc::DOMElement const *child_ptr)
	{
		if (child_name == L"phong" || child_name == L"lambert" || child_name == L"blinn")
		{
			if (shader_.get()) throw error(L"shader for <technique> defined more than one time");

			shader_ = construct<phong_lambert_blinn_e>(child_ptr);
		}
	}

public:
	void check()
	{
		if (!shader_.get()) throw error(L"material not found in <technique> or unknown material type");
	}

	phong_lambert_blinn_e const *get_shader() const
	{
		return shader_.get();
	}
};

class profile_common_e: public element {
private:
	boost::ptr_map<std::wstring, newparam_e> newparams_;
	std::auto_ptr<technique_e> technique_;

	void process_child(std::wstring const &child_name, std::wstring const &child_id, xercesc::DOMElement const *child_ptr)
	{
		if (child_name == L"newparam")
		{
			std::wstring sid = xerces_get_attribute<std::wstring>(child_ptr, L"sid");
			newparams_.insert(sid, construct<newparam_e>(child_ptr));
		}
		else if (child_name == L"technique")
		{
			if (technique_.get()) throw error(L"<technique> in <profile_common> defined more than one time");

			technique_ = construct<technique_e>(child_ptr);
		}
	}

	newparam_e const *get_newparam(std::wstring const &name, bool throw_on_error = true) const
	{
		boost::ptr_map<std::wstring, newparam_e>::const_iterator it = newparams_.find(name);
		if (it == newparams_.end()) {
			if (throw_on_error) throw error(L"<newparam> with name " + name + L" not found");
			return 0;
		}
		return it->second;
	}

	std::string get_surface_filename(std::wstring const &component_name) const;

public:
	MaterialPtr build_material() const
	{
		MaterialPtr material_ptr;

		if (get_model()->get_renderer())
		{
			material_ptr = get_model()->get_renderer()->create_material();
		}
		else
		{
			LOG_WARN("material without renderer");
			material_ptr.reset(new Material());
		}

		{
			std::string const &diffuse_texture = get_surface_filename(L"diffuse");

			if (diffuse_texture.size())
			{
				std::string filename = (get_root()->get_basepath() / diffuse_texture).string();

				LOG_DEBUG("diffuse texture = " << filename);
				material_ptr->set_diffuse_texture(filename);
			}
			else
			{
				std::string const &transparent_texture = get_surface_filename(L"transparent");

				if (transparent_texture.size())
				{
					std::string filename = (get_root()->get_basepath() / transparent_texture).string();

					LOG_DEBUG("transparent diffuse texture = " << filename);
					material_ptr->set_diffuse_texture(filename);
				}
				else
				{
					LOG_WARN("diffuse texture = null");
				}
			}
		}

		material_ptr->set_cull_mode(Material::CULL_CCW);

		return material_ptr;
	}
};

class effect_e: public element {
private:
	std::auto_ptr<profile_common_e> profile_common_;

	void process_child(std::wstring const &child_name, std::wstring const &child_id, xercesc::DOMElement const *child_ptr)
	{
		if (child_name == L"profile_COMMON")
		{
			profile_common_ = construct<profile_common_e>(child_ptr);
		}
	}

public:
	MaterialPtr build_material() const
	{
		return profile_common_->build_material();
	}
};

class library_effects_e: public element {
private:
	boost::ptr_map<std::wstring, effect_e> effects_;

	void process_child(std::wstring const &child_name, std::wstring const &child_id, xercesc::DOMElement const *child_ptr)
	{
		if (child_name == L"effect")
		{
			effects_.insert(child_id, construct<effect_e>(child_ptr));
		}
	}

public:
	effect_e const *get_effect_by_name(std::wstring const &name) const
	{
		boost::ptr_map<std::wstring, effect_e>::const_iterator it = effects_.find(name);
		if (it != effects_.end()) return it->second;

		throw error(L"effect " + name + L" not found");
	}
};

class image_e: public element {
private:
	std::wstring init_from_;

	void process_child(std::wstring const &child_name, std::wstring const &child_id, xercesc::DOMElement const *child_ptr)
	{
		if (child_name == L"init_from")
		{
			init_from_ = xerces_get_text(child_ptr);
		}
	}

public:
	std::wstring const &get_init_from() const
	{
		return init_from_;
	}
};

class library_images_e: public element {
private:
	boost::ptr_map<std::wstring, image_e> images_;

	void process_child(std::wstring const &child_name, std::wstring const &child_id, xercesc::DOMElement const *child_ptr)
	{
		if (child_name == L"image")
		{
			images_.insert(child_id, construct<image_e>(child_ptr));
		}
	}

public:
	image_e const *get_image_by_name(std::wstring const &name)
	{
		boost::ptr_map<std::wstring, image_e>::const_iterator it = images_.find(name);
		if (it != images_.end()) return it->second;

		throw error(L"image " + name + L" not found");
	}
};

void bind_material_e::bind_mesh_materials(mesh_e const *mesh_element_ptr, MeshPtr mesh_ptr) const
{
	boost::ptr_vector<triangles_e> const &triangles = mesh_element_ptr->get_triangles();

	Mesh::triangles_list_type::iterator mesh_triangles_it = mesh_ptr->triangles_list.begin();
	boost::ptr_vector<triangles_e>::const_iterator triangles_it = triangles.begin();

	LOG_DEBUG("*** mesh id = " << mesh_ptr->id << ", name = " << mesh_ptr->name);

	while (mesh_triangles_it != mesh_ptr->triangles_list.end() && triangles_it != triangles.end())
	{
		std::wstring const &material_symbol = triangles_it->get_material();
		std::wstring const &material_name = get_technique_common()->get_material_by_symbol(material_symbol);

		LOG_DEBUG("material symbol = " << xerces_lcp_transcode(material_symbol)
			<< ", name = " << xerces_lcp_transcode(material_name)
			<< ", triangles_count = " << mesh_triangles_it->triangles_count);

		mesh_triangles_it->material_ptr = mesh_ptr->get_model()->get_material_by_name(
			xerces_lcp_transcode(material_name));

		mesh_triangles_it++;
		triangles_it++;
	}

	if (mesh_triangles_it != mesh_ptr->triangles_list.end() || triangles_it != triangles.end())
	{
		throw error(L"mismatching number of <triangles> and triangles_lists in mesh with id = " + xerces_lcp_transcode(mesh_ptr->id));
	}
}

void mesh_e::build_mesh(MeshPtr mesh_ptr)
{
	input_e const *position_input = vertices_->get_input(L"POSITION");
	source_e const *position_source = get_source(position_input->get_source().substr(1));

	vertex_format_ |= GenericVertex::FORMAT_POSITION;
	vertex_format_ |= GenericVertex::FORMAT_NORMAL;
	vertex_format_ |= GenericVertex::FORMAT_TEXCOORDS;

	unsigned first_index_offset = 0;

	for (boost::ptr_vector<triangles_e>::const_iterator it = triangles_.begin();
		 it != triangles_.end(); it++)
	{
		triangles_e const *triangles_ptr = &*it;

		input_e const *tangents_input = vertices_->get_input_nothrow(L"TANGENT");
		if (!tangents_input) tangents_input = triangles_ptr->get_input_nothrow(L"TANGENT");
		if (tangents_input) vertex_format_ |= GenericVertex::FORMAT_TANGENT;

		input_e const *binormals_input = vertices_->get_input_nothrow(L"BINORMAL");
		if (!binormals_input) binormals_input = triangles_ptr->get_input_nothrow(L"BINORMAL");
		if (binormals_input) vertex_format_ |= GenericVertex::FORMAT_BINORMAL;

		input_e const *normals_input = vertices_->get_input_nothrow(L"NORMAL");
		if (!normals_input) normals_input = triangles_ptr->get_input(L"NORMAL");

		source_e const *tangents_source = tangents_input ? get_source(tangents_input->get_source().substr(1)) : 0;
		source_e const *binormals_source = binormals_input ? get_source(binormals_input->get_source().substr(1)) : 0;
		source_e const *normals_source = get_source(normals_input->get_source().substr(1));

		input_e const *texcoords_input = triangles_ptr->get_input(L"TEXCOORD");
		source_e const *texcoords_source = get_source(texcoords_input->get_source().substr(1));

		std::vector<unsigned> const &indices = triangles_ptr->get_indices();
		std::vector<unsigned>::const_iterator indices_get = indices.begin();

		unsigned stride = triangles_ptr->get_stride();

		Mesh::Triangles dummy;
		dummy.triangles_count = triangles_ptr->get_count();
		dummy.first_index_offset = first_index_offset;
		dummy.minimal_vertex_index = unsigned(-1);
		dummy.vertices_count = 0;

		for (unsigned i = 0, n = triangles_ptr->get_count() * 3; i < n; i++) {
			unsigned position_index;

			GenericVertex v;
			v.tangent.set_all(0);
			v.binormal.set_all(0);

			for (unsigned j = 0; j < stride; j++) {
				if (j == position_input->get_offset()) {
					position_index = *indices_get;
					position_source->get(position_index, v.position);
				}

				if (tangents_input && j == tangents_input->get_offset()) {
					tangents_source->get(*indices_get, v.tangent);
				}

				if (binormals_input && j == binormals_input->get_offset()) {
					binormals_source->get(*indices_get, v.binormal);
				}

				if (j == normals_input->get_offset()) {
					normals_source->get(*indices_get, v.normal);
				}

				if (j == texcoords_input->get_offset()) {
					texcoords_source->get(*indices_get, v.texcoords);

					// For texture coordinates, COLLADA's right-handed coordinate system applies; therefore, an ST texture
					// coordinate of [0,0] maps to the lower-left texel of a texture image, when loaded in a professional 2-D
					// texture viewer/editor. [COLLADA 1.4 Specification http://www.khronos.org/files/collada_spec_1_4.pdf]
					v.texcoords.y = 1 - v.texcoords.y;
				}

				indices_get++;
			}

			std::pair<unsigned, bool> vertex_index = vertex_indexer_.choose_index(v);

			if (vertex_index.second)
			{
				position_index_to_vertex_index_.insert(std::make_pair(position_index, vertex_index.first));
				vertex_array_.push_back(v);
				dummy.vertices_count++;
			}

			if (vertex_index.first < dummy.minimal_vertex_index)
			{
				dummy.minimal_vertex_index = vertex_index.first;
			}

			index_array_.push_back(vertex_index.first);
		}

		mesh_ptr->triangles_list.push_back(dummy);

		first_index_offset += triangles_ptr->get_count() * 3;
	}

	mesh_ptr->set_vertices(&vertex_array_[0], vertex_format_, vertex_array_.size());
	mesh_ptr->set_indices(&index_array_[0], index_array_.size());
}

void node_e::build_hierarchy(JointPtr parent_ptr)
{
	JointPtr joint_ptr = get_model()->add_joint();
	joint_ptr->id = xerces_lcp_transcode(id_);
	joint_ptr->name = xerces_lcp_transcode(sid_);
	joint_ptr->parent = parent_ptr;

	for (boost::ptr_vector<transform_e>::const_iterator it = transforms_.begin(); it != transforms_.end(); it++)
	{
		joint_ptr->transforms.push_back(Joint::transform(xerces_lcp_transcode(it->get_sid()), it->get_baked_matrix()));
	}

	if (parent_ptr) parent_ptr->childs.push_back(joint_ptr);

	if (instance_geometry_.get())
	{
		MeshPtr mesh_ptr = get_model()->get_mesh_by_id(xerces_lcp_transcode(instance_geometry_->get_url()));

		joint_ptr->mesh_ptr = mesh_ptr;
		mesh_e const *mesh_element_ptr = get_root()->get_geometries()->get_geometry_by_id(instance_geometry_->get_url())->get_mesh();

		instance_geometry_->get_bind_material()->bind_mesh_materials(mesh_element_ptr, mesh_ptr);
	}

	if (instance_controller_.get())
	{
		controller_e const *controller_ptr = get_root()->get_controllers()->get_controller_by_id(instance_controller_->get_url());

		joint_ptr->mesh_ptr = get_model()->get_mesh_by_id(xerces_lcp_transcode(controller_ptr->get_skin_source_id()));
		mesh_e const *mesh_element_ptr = get_root()->get_geometries()->get_geometry_by_id(controller_ptr->get_skin_source_id())->get_mesh();

		instance_controller_->get_bind_material()->bind_mesh_materials(mesh_element_ptr, joint_ptr->mesh_ptr.lock());
	}

	for (boost::ptr_vector<node_e>::iterator it = childs_.begin(); it != childs_.end(); it++)
	{
		it->build_hierarchy(joint_ptr);
	}
}

MaterialPtr material_e::build_material() const
{
	LOG_DEBUG("material " << xerces_lcp_transcode(name_)
		<< ", instance effect " << xerces_lcp_transcode(instance_effect_url_));

	MaterialPtr material_ptr(get_root()->
		get_effects()->
		get_effect_by_name(instance_effect_url_.substr(1))->
		build_material());

	material_ptr->set_name(xerces_lcp_transcode(name_));

	return material_ptr;
}

std::string profile_common_e::get_surface_filename(std::wstring const &component_name) const
{
	std::wstring const &sampler_name = technique_->get_shader()->get_sampler_name(component_name);
	if (sampler_name.empty()) return std::string();

	if (newparam_e const *sampler_ptr = get_newparam(sampler_name, false)) {
		std::wstring const &surface_name = sampler_ptr->get_sampler()->get_source();
		newparam_e const *surface_ptr = get_newparam(surface_name);
		std::wstring const &image_name = surface_ptr->get_surface()->get_init_from();
		std::wstring const &filepath = get_root()->get_images()->get_image_by_name(image_name)->get_init_from();
		LOG_INFO("sampler " << xerces_lcp_transcode(sampler_name) << " has texture "
			<< xerces_lcp_transcode(filepath) << " (via newparams)");
		return boost::filesystem::wpath(filepath).filename().string();
	}

	std::wstring const &filepath = get_root()->get_images()->get_image_by_name(sampler_name)->get_init_from();
	LOG_INFO("sampler " << xerces_lcp_transcode(sampler_name) << " has texture " << xerces_lcp_transcode(filepath));
	return boost::filesystem::wpath(filepath).filename().string();
}

class collada_e: public element, public root_element {
private:
	Model *current_model_;
	std::auto_ptr<library_visual_scenes_e> visual_scenes_;
	std::auto_ptr<library_geometries_e> geometries_;
	std::auto_ptr<library_controllers_e> controllers_;
	std::auto_ptr<library_animations_e> animations_;
	std::auto_ptr<library_materials_e> materials_;
	std::auto_ptr<library_effects_e> effects_;
	std::auto_ptr<library_images_e> images_;
	boost::filesystem::wpath basepath_;

	void process_child(std::wstring const &child_name, std::wstring const &child_id, xercesc::DOMElement const *child_ptr)
	{
		if (child_name == L"library_visual_scenes")
		{
			visual_scenes_ = construct<library_visual_scenes_e>(child_ptr);
		}
		else if (child_name == L"library_geometries")
		{
			geometries_ = construct<library_geometries_e>(child_ptr);
		}
		else if (child_name == L"library_controllers")
		{
			controllers_ = construct<library_controllers_e>(child_ptr);
		}
		else if (child_name == L"library_animations")
		{
			animations_ = construct<library_animations_e>(child_ptr);
		}
		else if (child_name == L"library_materials")
		{
			materials_ = construct<library_materials_e>(child_ptr);
		}
		else if (child_name == L"library_effects")
		{
			effects_ = construct<library_effects_e>(child_ptr);
		}
		else if (child_name == L"library_images")
		{
			images_ = construct<library_images_e>(child_ptr);
		}
	}

public:
	collada_e():
		current_model_(0)
	{
		set_root(this);
	}

	Model *get_model() const
	{
		return current_model_;
	}

	library_visual_scenes_e *get_visual_scenes() const
	{
		return visual_scenes_.get();
	}

	library_geometries_e *get_geometries() const
	{
		return geometries_.get();
	}

	library_controllers_e *get_controllers() const
	{
		return controllers_.get();
	}

	library_materials_e *get_materials() const
	{
		return materials_.get();
	}

	library_effects_e *get_effects() const
	{
		return effects_.get();
	}

	library_images_e *get_images() const
	{
		return images_.get();
	}

	boost::filesystem::wpath const &get_basepath() const
	{
		return basepath_;
	}

	void check()
	{
		if (!visual_scenes_.get()) throw error(L"<library_visual_scenes> not found");
		if (!geometries_.get()) throw error(L"<library_geometries> not found");
	}

	void set_filename(std::wstring const &filename)
	{
		basepath_ = boost::filesystem::wpath(filename).parent_path();
	}

	void build_model(Model *model_ptr)
	{
		current_model_ = model_ptr;

		try
		{
			geometries_->build_meshes();
			if (materials_.get()) materials_->build_materials();
			visual_scenes_->build_hierarchy();
			if (controllers_.get()) controllers_->build_joint_influences();
			if (animations_.get()) animations_->build_animations();
		}
		catch(...)
		{
			current_model_ = 0;
			throw;
		}

		current_model_ = 0;
	}
};

}

namespace graphic
{

class ColladaLoader::Impl
{
public:
	collada_elements::collada_e *get_root()
	{
		return &root_;
	}

private:
	collada_elements::collada_e root_;
};

ColladaLoader::ColladaLoader(Model *model_ptr):
model_(model_ptr),
impl_(new Impl())
{
}

ColladaLoader::~ColladaLoader()
{
}

void ColladaLoader::load(std::string const &filename)
{
	try
	{
		LOG_INFO("parsing collada file " << filename);

		std::auto_ptr<xercesc::XercesDOMParser> parser(new xercesc::XercesDOMParser());
		parser->setValidationScheme(xercesc::XercesDOMParser::Val_Always);
		parser->setDoNamespaces(true);

		std::auto_ptr<xercesc::ErrorHandler> error_handler(new xercesc::HandlerBase());
		parser->setErrorHandler(error_handler.get());

		parser->parse(filename.c_str());

		xercesc::DOMDocument *document = parser->getDocument();

		xercesc::DOMNodeList *node_list = document->getChildNodes();
		assert (node_list->getLength() == 1);

		xercesc::DOMNode *root_node = node_list->item(0);
		if (xercesc::XMLString::compareString(root_node->getLocalName(), L"COLLADA") != 0)
			throw load_error("root element is not <COLLADA />");

		impl_->get_root()->set_filename(util::xerces_lcp_transcode(filename));
		impl_->get_root()->go((xercesc::DOMElement *) root_node);

		LOG_INFO("finished parsing " << filename);
	}
	catch (collada_elements::error const &error)
	{
		throw load_error("error loading collada '" + filename + "': " +
			util::xerces_lcp_transcode(error.what()));
	}
	catch (xercesc::XMLException const &error)
	{
		throw load_error("xml error loading collada '" + filename + "': " +
			util::xerces_lcp_transcode(error.getMessage()));
	}
	catch (xercesc::SAXException const &error)
	{
		throw load_error("sax error loading collada '" + filename + "': " +
			util::xerces_lcp_transcode(error.getMessage()));
	}
}

void ColladaLoader::build_model()
{
	impl_->get_root()->build_model(model_);

	LOG_INFO("starting model self-check...");
	model_->check();
}

}
