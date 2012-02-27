#ifndef RAINBOW_CANVAS_H_
#define RAINBOW_CANVAS_H_

#include "Common/SpriteVertex.h"
#include "Common/Vector.h"
#include "Graphics/Drawable.h"
#include "Graphics/Pipeline.h"
#include "Input/Touchable.h"

class  Shader;
struct Texture;

/// Canvas is a painting application.
///
/// Copyright 2012 Bifrost Entertainment. All rights reserved.
/// \author Tommy Nguyen
class Canvas : public Drawable, public Touchable
{
public:
	Canvas(const int width, const int height);
	~Canvas();

	/// Clear the canvas.
	void clear();

	/// Set background colour.
	void set_background(const unsigned int color);

	/// Set background texture.
	void set_background(const Texture &texture);

	inline void set_brush(const Texture &brush);

	/// Set brush diameter size.
	inline void set_brush_size(const unsigned int size);

	/// Set foreground colour.
	void set_foreground(const unsigned int colour);

	/// Set position.
	void set_position(const int x, const int y);

	/// Returns \c true if the canvas was successfully created, otherwise \c false.
	inline operator bool() const;

	// Implement Drawable

	virtual void draw();
	virtual void update();

	// Implement Touchable

	virtual void touch_began(const Touch *const touches, const unsigned int count);
	virtual void touch_canceled();
	virtual void touch_ended(const Touch *const touches, const unsigned int count);
	virtual void touch_moved(const Touch *const touches, const unsigned int count);

private:
	struct BrushVertex
	{
		Vec2f texcoord;
		Vec2f position;
	};

	bool changed;             ///< Whether an update is needed.
	bool down;                ///< Whether the brush is down.
	Colorb foreground_color;  ///< Brush colour.
	unsigned int brush_size;  ///< Size of the brush in diameter.

	unsigned int background_tex;  ///< Background texture.
	unsigned int brush_attr;      ///< Brush shader attribute.
	unsigned int canvas_fb;       ///< Canvas frame buffer.
	unsigned int canvas_tex;      ///< Canvas texture.

	int x, y;           ///< Position of canvas, centered.
	int width, height;  ///< Width and height of canvas.

	const Shader *vsh;       ///< Canvas vertex shader.
	const Shader *fsh;       ///< Canvas fragment shader.
	const Texture *brush;    ///< Brush texture.

	Vec2f prev_point;        ///< Previous touch point.
	Pipeline pipeline;       ///< Canvas pipeline.
	SpriteVertex sprite[4];  ///< Canvas sprite.
	Touch touch;             ///< Current touch point.

	/// Intentionally left undefined.
	Canvas(const Canvas &c);

	/// Create a sprite from a point.
	void create_point(BrushVertex *vertex, const int x, const int y);

	/// Release all resources.
	void release();

	/// Intentionally left undefined.
	Canvas& operator=(const Canvas &c);
};

void Canvas::set_brush(const Texture &brush)
{
	this->brush = &brush;
}

void Canvas::set_brush_size(const unsigned int size)
{
	this->brush_size = size;
}

Canvas::operator bool() const
{
	return this->canvas_fb;
}

#endif