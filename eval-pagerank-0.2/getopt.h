#ifndef __IRLAB_GETOPT__
#define __IRLAB_GETOPT__

#include "perly.h"

using std::string;
using std::vector;
using std::map;

namespace irlab {
    struct Option {
	std::string name;
	std::string full;
	std::string abbr;
	std::string type;
	std::string desc;
	std::string defaultValue;
	bool isDummy;

	Option(const std::string& n, const std::string& f, 
		const std::string& a, const std::string& t, 
		const std::string& d, const std::string& dv, 
		bool b): 
	    name(n), full(f), abbr(a), type(t), desc(d), defaultValue(dv), isDummy(b) {}

	int size() const {
	    return full.length() + (full.length() && !isDummy? 2: 0) + 
		(full.length() && abbr.length()? 2: 0) +
		 abbr.length() + (abbr.length()? 1: 0) +
		(type.length()? 1 + type.length(): 0);
	}
    };

    struct OptionSize: public std::unary_function<Option, int> {
	int operator() (const Option& o) const { return o.size(); }
    };

    struct OptionMatchFull: public std::binary_function<Option, std::string, bool> {
	bool operator() (const Option& o, const std::string& s) const { return o.full.compare(s) == 0; }
    };

    struct OptionMatchAbbr: public std::binary_function<Option, std::string, bool> {
	bool operator() (const Option& o, const std::string& s) const { return o.abbr.compare(s) == 0; }
    };

    class Getopt {
	int _argc;
	char** _argv;
	std::vector<Option> _info;
	std::string _summary;
	std::string _usage;
	bool _autohelp;
	bool _gnustyle;
	bool _prettyprint;

	std::string _getCmd() {
	    // Get basename for argv[0]
	    std::string cmd(_argv[0]);
	    std::string::size_type m = cmd.rfind('/');
	    if (m > 0) cmd.erase(0, m + 1);
	    return cmd;
	}

	std::string _toupper(const std::string& src) {
	    std::string dest(src);
	    perly::map(dest.begin(), dest.end(), dest.begin(), ::toupper);
	    return dest;
	}

    public:
	Getopt(int c, char** v): 
	    _argc(c), 
	    _argv(v), 
	    _autohelp(true),
	    _gnustyle(true),
	   _prettyprint(false) {}

	~Getopt() {}

	void addOption(const char* name, const char* full = "", const char* abbr = "", 
		const char* type = "", const char* desc = "", const char* defaultValue = "") 
	{
	    _info.push_back(Option(name, full, abbr, type, desc, defaultValue, false));
	}

	void addDummyOption(const char* full, const char* desc = "") {
	    _info.push_back(Option("", full, "", "", desc, "", true));
	}

	void setSummary(const char* summary) { _summary = summary; }
	void setUsage(const char* usage) { _usage = usage; }
	void setAutoHelp(bool b) { _autohelp = b; }
	void setGNUStyle(bool b) { _gnustyle = b; }
	void setPrettyPrint(bool b) { _prettyprint = b; }

	void showHelp() {
	    std::string cmd = _getCmd();

	    if (_usage.length()) std::cerr << "Usage: " << cmd << " " << _usage << std::endl;
	    else std::cerr << "Usage: " << cmd << " [options..]" << std::endl;

	    if (_summary.length()) std::cerr << _summary << std::endl << std::endl;

	    if (_info.size()) {
		std::vector<int> optsize;
		perly::map(_info.begin(), _info.end(), std::back_inserter<std::vector<int> >(optsize), OptionSize());
		int maxsize = *(perly::max(optsize.begin(), optsize.end()));

		std::cerr << "Options:" << std::endl;
		for (std::vector<Option>::const_iterator i = _info.begin(); i != _info.end(); i++) {
		    std::cerr << "  ";
		    if (i->full.length() && i->abbr.length()) 
			std::cerr << "-" << i->abbr << ", --" << i->full;
		    else {
			if (i->full.length() && !i->isDummy) std::cerr << "--" << i->full;
			if (i->full.length() && i->isDummy) std::cerr << i->full;
			if (i->abbr.length()) std::cerr << "-" << i->abbr;
		    }

		    if (i->type.length()) std::cerr << " " << _toupper(i->type);

		    std::cerr << "  ";
		    if (i->size() < maxsize) std::cerr << std::string(maxsize - i->size(), ' ');

		    for (std::string::const_iterator j = i->desc.begin(); j != i->desc.end(); j++) {
			std::cerr << *j;
			if (*j == '\n') std::cerr << std::string(maxsize + 4, ' ');
		    }
		    std::cerr << std::endl;
		}
	    }
	}

	bool getOptions(std::map<std::string,std::string>& opt,
		std::vector<std::string>& cmdline) 
	{
	    for (std::vector<Option>::const_iterator j = _info.begin(); j != _info.end(); j++) {
		if (j->name.length()) opt[j->name] = j->defaultValue;
	    }

	    if (_argc == 1 && _autohelp) {
		showHelp();
		return 0;
	    }

	    for (int i = 1; i < _argc; i++) {
		std::string arg(_argv[i]);

		if (arg[0] != '-' || arg.length() <= 1) {
		    cmdline.push_back(arg);
		    continue;
		}

		std::vector<Option>::iterator matched;

		//--------------------------------------------------
		// Dirty!
		//-------------------------------------------------- 
		if (arg.compare("-help") == 0 || arg.compare("--help") == 0) {
		    showHelp();
		    return 0;
		}

		if (arg[1] == '-') {
		    arg.erase(0, 2);
		    matched = perly::first(_info.begin(), _info.end(), std::bind2nd(OptionMatchFull(), arg));
		}
		else {
		    arg.erase(0, 1);
		    matched = perly::first(_info.begin(), _info.end(), std::bind2nd(OptionMatchAbbr(), arg));
		}

		if (matched == _info.end()) 
		    std::cerr << "Warning: unknown option '" << arg << "'" << std::endl;
		else if (i + 1 == _argc)
		    std::cerr << "Warning: option '" << arg << "' has no argument" << std::endl;
		else 
		    opt[matched->name] = _argv[++i];
	    }

	    return 1;
	}
    };
}

#endif
