//
// limit_single_instance.h - limit_single_instance interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the CC-BY-NC 2.0 license. For full details see the file
// LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
//

#include <string>

/// <summary>
/// Limit app to a single instance.
/// </summary>
class limit_single_instance {
public:
	/// <summary>
	/// Constructor.
	/// </summary>
	/// 
	/// <param name="mutex_name">
	/// Unique guid used to prevent multiple instances of the app,
	/// e.g. "Global\\{14434072-3695-4BD7-8CD5-ECFCDD571FA4}"
	/// </param>
	limit_single_instance(const std::string& mutex_name);
	~limit_single_instance();

	/// <summary>
	/// Check if another instance is running.
	/// </summary>
	/// 
	/// <returns>
	/// Returns true if another instance is running, else false.
	/// </returns>
	bool another_instance_running();

	/// <summary>
	/// Wait for the other instance. This function will block until the other instance is closed.
	/// </summary>
	void wait_for_other_instance();
private:
	limit_single_instance();

	class impl;
	impl* d_;
};
