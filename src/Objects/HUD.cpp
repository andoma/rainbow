/*
 *  HUD.cpp
 *  OnWire
 *
 *  Created by Tommy Nguyen on 7/9/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "HUD.h"

const unsigned int HUD::label_margin	= 10;
const unsigned int HUD::padding			= 8;
const unsigned int HUD::total_margin	= 14;

const float HUD::label_size				= 0.4f;
const float HUD::progress_size			= 0.85f;

const char HUD::distance_format[]		= "%um";
const char HUD::init_time[]				= "0:00";
const char HUD::label_font[]			= "ArialBold.fnt";
const char HUD::progress_font[]			= "ArialBlack.fnt";
const char HUD::time_format[]			= "%u:%.2u";

HUD::HUD()
{
	this->distance = new Label("9000m", progress_font);
	this->distance_label = new Label("Distance", label_font);
	this->time = new Label(init_time, progress_font);
	this->time_label = new Label("Time", label_font);

	// Set vertical alignment to bottom
	this->distance->set_valign(0.0f);
	this->distance_label->set_valign(0.0f);
	this->time->set_valign(0.0f);
	this->time_label->set_valign(0.0f);

	// Set horizontal alignment
	this->distance->align_left();
	this->distance_label->align_left();
	this->time->align_right();
	this->time_label->align_right();

	// Set font size
	this->distance->scale(progress_size);
	this->distance_label->scale(label_size);
	this->time->scale(progress_size);
	this->time_label->scale(label_size);

	// Calculate label position on screen
	Screen *s = Screen::Instance();
	const unsigned int
		w = s->get_width(),
		top_pos = s->get_height() - this->distance->get_height() + padding,
		top_label_pos = s->get_height() - this->distance_label->get_height() + padding;

	// Set position of labels
	this->distance->set_position((this->distance_label->get_width() * label_size) + total_margin, top_pos);
	this->distance_label->set_position(label_margin, top_label_pos);
	this->time->set_position(w - (this->time_label->get_width() * label_size) - total_margin, top_pos);
	this->time_label->set_position(w - label_margin, top_label_pos);
}

HUD::~HUD()
{
	delete this->distance;
	delete this->distance_label;
	delete this->time;
	delete this->time_label;
}

RealLabel **HUD::get_elements()
{
	RealLabel **labels = new RealLabel *[HUD_COUNT];
	labels[0] = this->distance->get_label();
	labels[1] = this->distance_label->get_label();
	labels[2] = this->time->get_label();
	labels[3] = this->time_label->get_label();
	return labels;
}

void HUD::reset(const unsigned int target)
{
	this->time->set_text(init_time);
	this->update_distance(target);
}

void HUD::update_distance(const unsigned int d)
{
	sprintf(this->buffer, this->distance_format, d);
	this->distance->set_text(this->buffer);
}

void HUD::update_time(const unsigned int time)
{
	sprintf(this->buffer, this->time_format, time / 60, time % 60);
	this->time->set_text(this->buffer);
}
