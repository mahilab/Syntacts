#include <iostream>
#include <vector>
#include <string>
#include <memory>

using namespace std;

template <typename T>
void draw(const T& x, ostream& out, size_t position) {
    out << string(position, ' ') << x << endl;
}

class object_t {
public:
    template<typename T>
    object_t(T x) : self_(make_shared<model<T>>(move(x)))
    { }

    friend void draw(const object_t& x, ostream& out, size_t position) {
        x.self_->draw_(out, position); 
    }

private:
    struct concept_t {
        virtual ~concept_t() = default;
        virtual void draw_(ostream&, size_t) const = 0;
    };

    template <typename T>
    struct model final : concept_t {
        model(T x) : data_(move(x)) { }
        void draw_(ostream& out, size_t position) const override
        {
            draw(data_, out, position);
        }
        T data_;
    };

    shared_ptr<const concept_t> self_;
};

using document_t = vector<object_t>;

void draw(const document_t& x, ostream& out, size_t position) {
    out << string(position, ' ') << "<document>" << endl;
    for (const auto& e : x)
        draw(e, out, position + 2);
    out << string(position, ' ') << "</document>" << std::endl;
}



int main()
{
    document_t document;
    document.push_back(0);
    document.emplace_back(1);
    document.emplace_back(2);
    document.push_back("hello, world");
    document.emplace_back(3);
    draw(document, cout, 0);
    return 0;
}