extern "C" const char lbl_8078B394[];

namespace std {

// inferred: std::bad_cast derives from std::exception; the base destructor is inline and empty.
class exception {
public:
    virtual ~exception() {}
    virtual const char* what() const { return ""; }
};

class bad_cast : public exception {
public:
    virtual ~bad_cast();
    virtual const char* what() const;
};

bad_cast::~bad_cast() {}
const char* bad_cast::what() const { return lbl_8078B394; }

} // namespace std
