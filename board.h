#include <fstream>
#include <vector>

#include "page.h"

using std::vector;
using std::endl;
using std::ofstream;


class Board {
    public:
        Board(int num_jobs, int width, int height, ofstream& output_stream);
        ~Board();

        void print_board();
        void print_job(int job_idx, char job_type, int id);

        //job functions
        void insert_page(int x, int y, int width, int height, int id, int content);
        void delete_page(int id);
        void modify_content(int id, char content);
        void modify_position(int id, int x, int y);
        
        void only_show_state(vector<Page> vec);
        int check_existing_vec (vector<Page> vec_top, int id);
        void delete_page_recursive_part(int id);

    private:
        int num_jobs, width, height; 
        ofstream& output; 
        char* board; 
        int* board_page;
        vector<Page> vec;
        vector<Page> deleted_page;
};

Board::Board(int num_jobs, int width, int height, ofstream& output_stream): output(output_stream) {
    this->width = width;
    this->height = height;
    this->num_jobs = num_jobs;

    board = new char[width*height];
    board_page = new int[width*height];

    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            board[h*width + w] = ' ';
            board_page[h*width + w] = 0;
        }
    }

}

Board::~Board() {
    delete board;
    
}


void Board::print_board() {
    int h, w;
    for (w = 0; w < width+2; w++) output << "- ";
    output << endl;
    
    for (h = 0; h < height; h++) {
        output << "| ";
        for (w = 0; w < width; w++) {
            output << board[h*width + w] << " ";
        }
        output << "| " << endl;
    }

    for (w = 0; w < width+2; w++) output << "- ";
    output << endl;
}

void Board::print_job(int job_idx, char job_type, int id) {
    output << ">> (" << job_idx <<") ";
    switch(job_type) {
        
        case 'i':
            output << "Insert ";
            break;
        case 'd':
            output << "Delete ";
            break;
        case 'm':
            output << "Modify ";
            break;
    }

    output << id << endl;
}

 
void Board::insert_page(int x, int y, int width, int height, int id, int content) {
    Page new_page = Page(x, y, width, height, id, content);
    vec.push_back(new_page);
    bool on[vec.size()];
    for (int i=0; i<vec.size(); i++) {
        on[i] = false;
    }

    for (int h = y; h < y + height; h++) {
        for (int w = x; w < x + width; w++) {
            for (int i=0; i<vec.size(); i++) {
                if (board_page[h * this->width + w] == vec[i].get_id()) {
                    on[i] = true;
                }
            }
            board[h * this->width + w] = content;
            board_page[h * this->width + w] = id;
        }
    }
    for (int i=0; i<vec.size(); i++) {
        if (on[i] == true) {
            vec[i].add_vec_top(new_page);
        }
    }

    print_board();
}

void Board::delete_page(int id) {
    delete_page_recursive_part(id);
    deleted_page.erase(deleted_page.begin() + deleted_page.size() -1);
    for (int i=0; i <deleted_page.size(); i++) {
        int idx = deleted_page.size() -i -1;
        insert_page(deleted_page[idx].get_x(), deleted_page[idx].get_y(), deleted_page[idx].get_width(), deleted_page[idx].get_height(), deleted_page[idx].get_id(), deleted_page[idx].get_content());
    }
    deleted_page = {};
}

void Board::modify_content(int id, char content) {
    delete_page_recursive_part(id);
    int change_idx = deleted_page.size()-1;
    Page new_page = Page(deleted_page[change_idx].get_x(), deleted_page[change_idx].get_y(), deleted_page[change_idx].get_width(), deleted_page[change_idx].get_height(), deleted_page[change_idx].get_id(), content);
    deleted_page.erase(deleted_page.begin() + deleted_page.size() -1);
    deleted_page.push_back(new_page);
    for (int i=0; i <deleted_page.size(); i++) {
        int idx = deleted_page.size() -i -1;
        insert_page(deleted_page[idx].get_x(), deleted_page[idx].get_y(), deleted_page[idx].get_width(), deleted_page[idx].get_height(), deleted_page[idx].get_id(), deleted_page[idx].get_content());
    }
    deleted_page = {};
}
void Board::modify_position(int id, int x, int y) {
    delete_page_recursive_part(id);
    int change_idx = deleted_page.size()-1;
    Page new_page = Page(x, y, deleted_page[change_idx].get_width(), deleted_page[change_idx].get_height(), deleted_page[change_idx].get_id(), deleted_page[change_idx].get_content());
    deleted_page.erase(deleted_page.begin() + deleted_page.size() -1);
    deleted_page.push_back(new_page);
    for (int i=0; i <deleted_page.size(); i++) {
        int idx = deleted_page.size() -i -1;
        insert_page(deleted_page[idx].get_x(), deleted_page[idx].get_y(), deleted_page[idx].get_width(), deleted_page[idx].get_height(), deleted_page[idx].get_id(), deleted_page[idx].get_content());
    }
    deleted_page = {};
}

void Board::only_show_state(vector<Page> vec) {
    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            board[h*width + w] = ' ';
            board_page[h*width + w] = 0;
        }
    }
    for (int i=0; i<vec.size(); i++) {
        for (int h = vec[i].get_y(); h < vec[i].get_y() + vec[i].get_height(); h++) {
            for (int w = vec[i].get_x(); w < vec[i].get_x() + vec[i].get_width(); w++) {
                board[h * this->width + w] = vec[i].get_content();
                board_page[h * this->width + w] = vec[i].get_id();
            }
        }
    }
}

int Board::check_existing_vec (vector<Page> vec_top, int id) {
    for (int i=0; i<vec_top.size(); i++) {
        if (vec_top[i].get_id() == id) {
            return 1;
        }
        else {
            continue;
        }
    }
    return 0;
}

void Board::delete_page_recursive_part(int id) {
    int tidx;
    for (int i=0; i<vec.size(); i++) {
        if (vec[i].get_id() == id) {
            tidx = i;
        }
    }
    int id_min;
    int id_min_idx;
    while (vec[tidx].get_vec_top().size() > 0) {
        id_min = vec[tidx].get_vec_top()[0].get_id();
        id_min_idx = 0;
        for (int i=0; i<vec[tidx].get_vec_top().size(); i++) {
            if (vec[tidx].get_vec_top()[i].get_id() < id_min) {
                id_min = vec[tidx].get_vec_top()[i].get_id();
                id_min_idx = i;
            }
        }
        delete_page_recursive_part(id_min);
    }
    //삭제 부분
    deleted_page.push_back(vec[tidx]);
    vec.erase(vec.begin() + tidx);
    for (int i=0; i<vec.size(); i++){
        if (check_existing_vec(vec[i].get_vec_top(), id) == 1) {
            for (int j=0; j<vec[i].get_vec_top().size(); j++) {
                if (vec[i].get_vec_top()[j].get_id() == id) {
                    vec[i].delete_vec_top(j);
                }
            }
        }
    }
    only_show_state(vec);
    print_board();
}
