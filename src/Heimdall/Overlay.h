// Copyright (c) 2010-15 Bifrost Entertainment AS and Tommy Nguyen
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#ifndef HEIMDALL_OVERLAY_H_
#define HEIMDALL_OVERLAY_H_

#include "Graphics/Buffer.h"
#include "Graphics/Drawable.h"
#include "Graphics/SceneGraph.h"
#include "Graphics/SpriteVertex.h"
#include "Graphics/VertexArray.h"

namespace heimdall
{
	class Overlay final : public Drawable
	{
	public:
		Overlay();
		~Overlay() override;

		unsigned int count() const { return 6; }
		bool is_visible() const { return node_->is_enabled(); }
		rainbow::SceneNode* node() const { return node_; }
		const VertexArray& vertex_array() const { return array_; }

		void init(rainbow::SceneNode &parent, const Vec2i &screen);

		void hide() { node_->set_enabled(false); }
		void show() { node_->set_enabled(true); }

		template<typename T>
		void add_child(T *component) const { node_->add_child(component); }

		/// <summary>Binds all used textures.</summary>
		void bind_textures() const;

	private:
		rainbow::SceneNode *node_;
		VertexArray array_;
		unsigned int texture_;
		Buffer vertex_buffer_;

		/* Implement Drawable. */

		void draw_impl() override;
		void update_impl(const unsigned long dt) override;
	};
}

#endif
