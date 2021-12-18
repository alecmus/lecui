//
// command_line_parser.h - win32 commandline parser
//
// lecui user interface library, part of the liblec library
// Copyright (c) 2019 Alec Musasa (alecmus at live dot com)
//
// Released under the MIT license. For full details see the
// file LICENSE.txt
//

/*
** win32 commandline parser
**
** source adapted from
** http://www.codeguru.com/cpp/w-p/win32/article.php/c1427/A-Simple-Win32-CommandLine-Parser.htm
*/

#pragma once

#include <windows.h>
#include <vector>

/// <summary>
/// Command line parsing class.
/// </summary>
/// <remarks>
/// Simply declare an object. The object inherits from a vector.
/// It will be automatically populated with the commandline arguments.
/// </remarks>
class command_line_parser : public std::vector<char*> {
public:
	command_line_parser() {
		// Save local copy of the command line string, because
		// ParseCmdLine() modifies this string while parsing it.
		PSZ cmdline = GetCommandLineA();
		m_cmdline = new char[strlen(cmdline) + 1];
		if (m_cmdline) {
			/*
			** customization by Alec Musasa on 26 Feb 2014
			**
			** replace strcpy with strcpy_s
			*/
			strcpy_s(m_cmdline, strlen(cmdline) + 1, cmdline);
			ParseCmdLine();
		}
	}

	command_line_parser(const char* cc) {
		// Save local copy of the command line string, because
		// ParseCmdLine() modifies this string while parsing it.
		PSZ cmdline = (PSZ)cc;
		m_cmdline = new char[strlen(cmdline) + 1];
		if (m_cmdline) {
			/*
			** customization by Alec Musasa on 26 Feb 2014
			**
			** replace strcpy with strcpy_s
			*/
			strcpy_s(m_cmdline, strlen(cmdline) + 1, cmdline);
			ParseCmdLine();
		}
	}

	~command_line_parser() {
		delete m_cmdline;
	}

private:
	PSZ m_cmdline; // the command line string

				   ////////////////////////////////////////////////////////////////////////////////
				   // Parse m_cmdline into individual tokens, which are delimited by spaces. If a
				   // token begins with a quote, then that token is terminated by the next quote
				   // followed immediately by a space or terminator.  This allows tokens to contain
				   // spaces.
				   // This input string:     This "is" a ""test"" "of the parsing" alg"o"rithm.
				   // Produces these tokens: This, is, a, "test", of the parsing, alg"o"rithm
				   ////////////////////////////////////////////////////////////////////////////////
	void ParseCmdLine() {
		enum {
			TERM = '\0',
			QUOTE = '\"'
		};

		bool bInQuotes = false;
		PSZ pargs = m_cmdline;

		while (*pargs) {
			while (isspace(*pargs))        // skip leading whitespace
				pargs++;

			bInQuotes = (*pargs == QUOTE);  // see if this token is quoted

			if (bInQuotes)                  // skip leading quote
				pargs++;

			push_back(pargs);              // store position of current token

										   // Find next token.
										   // NOTE: Args are normally terminated by whitespace, unless the
										   // arg is quoted.  That's why we handle the two cases separately,
										   // even though they are very similar.
			if (bInQuotes) {
				// find next quote followed by a space or terminator
				while (*pargs &&
					!(*pargs == QUOTE && (isspace(pargs[1]) || pargs[1] == TERM)))
					pargs++;
				if (*pargs) {
					*pargs = TERM;  // terminate token
					if (pargs[1])   // if quoted token not followed by a terminator
						pargs += 2; // advance to next token
				}
			}
			else {
				// skip to next non-whitespace character
				while (*pargs && !isspace(*pargs))
					pargs++;
				if (*pargs && isspace(*pargs)) { // end of token
					*pargs = TERM;    // terminate token
					pargs++;         // advance to next token or terminator
				}
			}
		}
	}
};
