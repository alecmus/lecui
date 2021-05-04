/*
** date_time.cpp - date/time implementation
**
** lecui user interface library
** Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
**
*******************************************************************************
** This file is part of the liblec library which is released under the Creative
** Commons Attribution Non-Commercial 2.0 license (CC-BY-NC 2.0). See the file
** LICENSE.txt or go to https://github.com/alecmus/liblec/blob/master/LICENSE.md
** for full license details.
*/

#include "../date_time.h"
#include <ctime>
#include <sstream>

namespace liblec {
	namespace lecui {
		std::string date_time::time_stamp() {
			std::time_t time_temp = ::time(0);
			std::tm time_out = { };
			localtime_s(&time_out, &time_temp);

			std::string year = std::to_string(time_out.tm_year + 1900);
			std::string month = std::to_string(time_out.tm_mon + 1);
			std::string day = std::to_string(time_out.tm_mday);
			std::string hour = std::to_string(time_out.tm_hour);
			std::string minute = std::to_string(time_out.tm_min);
			std::string second = std::to_string(time_out.tm_sec);

			for (size_t i = year.length(); i < 4; i++) year = "0" + year;
			for (size_t i = month.length(); i < 2; i++) month = "0" + month;
			for (size_t i = day.length(); i < 2; i++) day = "0" + day;
			for (size_t i = hour.length(); i < 2; i++) hour = "0" + hour;
			for (size_t i = minute.length(); i < 2; i++) minute = "0" + minute;
			for (size_t i = second.length(); i < 2; i++) second = "0" + second;

			return std::string(year + month + day + " " + hour + ":" + minute + ":" + second);
		}

		std::string date_time::weekday_to_string(unsigned short d) {
			std::string weekday;
			switch (d) {
			case 1: weekday = "Sunday"; break;
			case 2: weekday = "Monday"; break;
			case 3: weekday = "Tuesday"; break;
			case 4: weekday = "Wednesday"; break;
			case 5: weekday = "Thursday"; break;
			case 6: weekday = "Friday"; break;
			case 7: weekday = "Saturday"; break;
			default: break;
			}
			return weekday;
		}

		std::string date_time::month_to_string(unsigned short m) {
			std::string month_string;
			switch (m) {
			case 1: month_string = "Jan"; break;
			case 2: month_string = "Feb"; break;
			case 3: month_string = "Mar"; break;
			case 4: month_string = "Apr"; break;
			case 5: month_string = "May"; break;
			case 6: month_string = "Jun"; break;
			case 7: month_string = "Jul"; break;
			case 8: month_string = "Aug"; break;
			case 9: month_string = "Sep"; break;
			case 10: month_string = "Oct"; break;
			case 11: month_string = "Nov"; break;
			case 12:
			default: month_string = "Dec"; break;
			}
			return month_string;
		}

		unsigned short date_time::month_from_string(std::string m) {
			unsigned short month;

			std::string month_string(m);
			if (month_string == "Jan")
				month = 1;
			else
				if (month_string == "Feb")
					month = 2;
				else
					if (month_string == "Mar")
						month = 3;
					else
						if (month_string == "Apr")
							month = 4;
						else
							if (month_string == "May")
								month = 5;
							else
								if (month_string == "Jun")
									month = 6;
								else
									if (month_string == "Jul")
										month = 7;
									else
										if (month_string == "Aug")
											month = 8;
										else
											if (month_string == "Sep")
												month = 9;
											else
												if (month_string == "Oct")
													month = 10;
												else
													if (month_string == "Nov")
														month = 11;
													else
														month = 12;

			return month;
		}

		std::string date_time::to_string(date dt) {
			std::string day = std::to_string(dt.day);
			std::string month = month_to_string(dt.month);
			std::string year = std::to_string(dt.year);

			for (size_t i = day.length(); i < 2; i++) day = "0" + day;
			for (size_t i = year.length(); i < 4; i++) year = "0" + year;

			return std::string(day + "-" + month + "-" + year);
		}

		date date_time::from_string(const std::string& dt) {
			date date_ = today();
			try {
				std::stringstream ss;
				ss << dt.substr(0, 2);	// parse day
				ss >> date_.day;		// write day
				ss.clear();

				std::string month_string;
				ss << dt.substr(3, 3);	// parse month
				ss >> month_string;		// write month
				date_.month = month_from_string(month_string);
				ss.clear();
				ss << dt.substr(7, 4);	// parse year
				ss >> date_.year;		// write year
			}
			catch (const std::exception&) {}
			return date_;
		}

		date date_time::today() {
			std::time_t time_temp = ::time(0);
			std::tm time_out = { };
			localtime_s(&time_out, &time_temp);

			date date_;
			date_.day = (unsigned short)time_out.tm_mday;
			date_.month = (unsigned short)time_out.tm_mon + 1;
			date_.year = (unsigned short)time_out.tm_year + 1900;
			return date_;
		}

		date date_time::last_day_of_month(const date& dt) {
			struct tm when = { 0, 0, 0, 1 };

			if (dt.month == 12) {
				when.tm_mon = 0;
				when.tm_year = dt.year - 1900 + 1;
			}
			else {
				when.tm_mon = dt.month;
				when.tm_year = dt.year - 1900;
			}

			// get the first day of the next month and subtract one day
			const time_t lastday = mktime(&when) - 86400;
			localtime_s(&when, &lastday);

			date date_;
			date_.day = (unsigned short)(when.tm_mday);
			date_.month = (unsigned short)(when.tm_mon + 1);
			date_.year = (unsigned short)(when.tm_year + 1900);
			return date_;
		}

		unsigned short date_time::day_of_week(date dt) {
			std::tm time_in = { 0, 0, 0, dt.day, dt.month - 1, dt.year - 1900 };
			std::time_t time_temp = std::mktime(&time_in);
			std::tm time_out = { };
			localtime_s(&time_out, &time_temp);
			return (unsigned short)(time_out.tm_wday + 1);
		}

		date date_time::add_days(date dt, int n) {
			std::tm time_in = { 0, 0, 0, dt.day, dt.month - 1, dt.year - 1900 };
			time_in.tm_mday += n;

			std::time_t time_temp = std::mktime(&time_in);
			std::tm time_out = { };
			localtime_s(&time_out, &time_temp);

			date date_;
			date_.day = (unsigned short)(time_out.tm_mday);
			date_.month = (unsigned short)(time_out.tm_mon + 1);
			date_.year = (unsigned short)(time_out.tm_year + 1900);
			return date_;
		}

		void date_time::get_week(const date dt, date& start, date& end) {
			start = end = dt;
			const auto d = day_of_week(dt);
			add_days(start, 0 - d);
			add_days(end, 6 - d);
		}

		void date_time::get_month(const date dt, date& start, date& end) {
			start = end = dt;
			const auto d = dt.day;
			add_days(start, 1 - d);
			end = last_day_of_month(dt);
		}

		void date_time::get_year(const date dt, date& start, date& end) {
			start = { 1, 1, dt.year };
			end = { 31, 12, dt.year };
		}
	}
}
