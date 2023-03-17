#include "common/recognizer.hpp"
#include <fstream>

namespace comp {

	bool Recognizer::SourceHandler::code(const std::string& s) {
		if (coded && s == "%}") {
			coded = false;
			return true;
		} else if (s == "%{") {
			coded = true;
			return true;
		} else if (coded || section == 2) {
			output_file << s << '\n';
			return true;
		}
		return false;
	}
}