#include <stdexcept>

namespace comp {
	class syntax_error: public std::logic_error {
	public:
		using std::logic_error::logic_error;
		syntax_error& operator=(const syntax_error&) = default;
		syntax_error& operator=(syntax_error&&) = default;
		~syntax_error() noexcept = default;
	};
}