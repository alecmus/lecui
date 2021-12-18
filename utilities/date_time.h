//
// date_time.h - date/time interface
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

#pragma once

#if defined(LECUI_EXPORTS)
#include "../form.h"
#else
#include <liblec/lecui/form.h>
#endif

namespace liblec {
	namespace lecui {
		/// <summary>Interact with date/time.</summary>
		class lecui_api date_time {
		public:
			/// <summary>
			/// Make timestamp from current local time in the form 20190802 20:48:06.
			/// </summary>
			static std::string time_stamp();

			/// <summary>Get week day in the form of a string.</summary>
			/// <param name="d">The day of the week, 1 to 7 inclusive.</param>
			/// <returns>The week day in the form "Tuesday".</returns>
			static std::string weekday_to_string(unsigned short d);

			/// <summary>Get month in the form of a string.</summary>
			/// <param name="m">The month number, from 1 to 12 inclusive.</param>
			/// <returns>Returns the month in the form "Jan".</returns>
			static std::string month_to_string(unsigned short m);

			/// <summary>Get month from string.</summary>
			/// <param name="m">The month string, in the form "Jan".</param>
			/// <returns>Returns the month as a number from 1 to 12 inclusive.</returns>
			static unsigned short month_from_string(std::string m);

			/// <summary>Get date in the form of a string.</summary>
			/// <param name="dt">The date.</param>
			/// <returns>The date in the form "01-Jan-2020".</returns>
			static std::string to_string(date dt);

			/// <summary>Parse date from a date string.</summary>
			/// <param name="dt">The date string.</param>
			/// <returns>The date.</returns>
			static date from_string(const std::string& dt);

			/// <summary>Get the current local date.</summary>
			/// <returns>The local date.</returns>
			static date today();

			/// <summary>Get the last day of the month for a given date.</summary>
			/// <param name="dt">The date.</param>
			/// <returns>The last day of the month.</returns>
			static date last_day_of_month(const date& dt);

			/// <summary>Get the day of the week for a given date.</summary>
			/// <param name="dt">The date.</param>
			/// <returns>The day of the week, from 1 to 7 inclusive (with Sunday as 1).</returns>
			static unsigned short day_of_week(date dt);

			/// <summary>Add days to a given date.</summary>
			/// <param name="dt">The date.</param>
			/// <param name="n">The number of days to add.</param>
			/// <returns>A date that is n days away from the given date.</returns>
			static date add_days(date dt, int n);

			/// <summary>Get the start and end days of the week for a given date.</summary>
			/// <param name="dt">The date.</param>
			/// <param name="start">The week's start day (Sunday).</param>
			/// <param name="end">The week's end day (Saturday).</param>
			static void get_week(const date dt, date& start, date& end);

			/// <summary>Get the start and end days of the month for a given date.</summary>
			/// <param name="dt">The date.</param>
			/// <param name="start">The month's start day.</param>
			/// <param name="end">The month's end day.</param>
			static void get_month(const date dt, date& start, date& end);

			/// <summary>Get the start and end days of the year for a given date.</summary>
			/// <param name="dt">The date.</param>
			/// <param name="start">The year's start day.</param>
			/// <param name="end">The year's end day.</param>
			static void get_year(const date dt, date& start, date& end);

		private:
			class impl;
			impl& _d;

			// Copying an object of this class are not allowed
			date_time(const date_time&) = delete;
			date_time& operator=(const date_time&) = delete;
		};
	}
}
