// Copyright (c) 2010-15 Bifrost Entertainment AS and Tommy Nguyen
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)

#ifndef INPUT_INPUT_H_
#define INPUT_INPUT_H_

#include "Common/NonCopyable.h"
#include "Input/Acceleration.h"
#include "Input/InputListener.h"

/// <summary>
///   Handles accelerometer/gyroscope and pointer events independent of
///   platform.
/// <summary>
/// <remarks>
///   <para>
///     Accelerometer data changes do not follow the observer pattern because
///     they happen every frame. Polling is preferred in this case.
///   </para>
///   References
///   <list type="bullet">
///     <item>http://developer.apple.com/library/ios/#documentation/UIKit/Reference/UIAcceleration_Class/Reference/UIAcceleration.html</item>
///   </list>
/// <remarks>
class Input : private InputListener, NonCopyable<Input>
{
public:
	Input() : last_listener_(this) {}

	/// <summary>Clears all input listeners.</summary>
	void reset()
	{
		pop();
		last_listener_ = this;
	}

	/// <summary>Subscribes an object to input events.</summary>
	/// <param name="i">The object to subscribe.</param>
	void subscribe(InputListener *const i);

	/// <summary>Unsubscribes an object from input events.</summary>
	/// <param name="i">The object to unsubscribe.</param>
	void unsubscribe(InputListener *const i);

	/// <summary>Acceleration event.</summary>
	/// <param name="x">Acceleration data (x-value).</param>
	/// <param name="y">Acceleration data (y-value).</param>
	/// <param name="z">Acceleration data (z-value).</param>
	/// <param name="t">
	///   The relative time at which the acceleration occurred.
	/// </param>
	void accelerated(const double x,
	                 const double y,
	                 const double z,
	                 const double t);

	void on_controller_axis_motion(const ControllerAxisMotion &);
	void on_controller_button_down(const ControllerButton &);
	void on_controller_button_up(const ControllerButton &);
	void on_controller_connected(const unsigned int id);
	void on_controller_disconnected(const unsigned int id);

	void on_key_down(const Key &k);
	void on_key_up(const Key &k);

	void on_pointer_began(const unsigned int count, Pointer *pointers);
	void on_pointer_canceled();
	void on_pointer_ended(const unsigned int count, Pointer *pointers);
	void on_pointer_moved(const unsigned int count, Pointer *pointers);

private:
	Acceleration acceleration_;  ///< Accelerometer data
	InputListener *last_listener_;

	void on_end_link_removed(Link *node) override;
};

#endif
