#include "common/recognizer.hpp"

namespace comp {

	bool Recognizer::SourceHandler::code(const string& s) {
		if (coded && s == "%}") {
			coded = false;
			return true;
		} else if (s == "%{") {
			coded = true;
			return true;
		} else if (coded || section == 2) {
			code_content.append(s).push_back('\n');
			return true;
		}
		return false;
	}
} // namespace comp