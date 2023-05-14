#include <vector>

using std::vector;

class Page {
public:
    Page(int x, int y, int width, int height, int id, int content);

    int get_x() ;
    int get_y() ;
    int get_width() const;
    int get_height() const;
    int get_id() ;
    char get_content() ;
    vector<Page> get_vec_top();
    void add_vec_top(Page add);
    void clear_vec_top();
    void delete_vec_top(int idx);

private:
    int x, y;
    int width, height;
    int id;
    char content;
    vector<Page> vec_top = {};
};

Page::Page(int x, int y, int width, int height, int id, int content) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->id = id;
    this->content = content;
}

int Page::get_x()  {
    return x;
}

int Page::get_y()  {
    return y;
}

int Page::get_width() const {
    return width;
}

int Page::get_height() const {
    return height;
}

int Page::get_id() {
    return id;
}

char Page::get_content()  {
    return content;
}

vector<Page> Page::get_vec_top() {
    return vec_top;
}

void Page::add_vec_top(Page add) {
    vec_top.push_back(add);
}

void Page::clear_vec_top() {
    vec_top = {};
}

void Page::delete_vec_top(int idx) {
    vec_top.erase(vec_top.begin()+idx);
}