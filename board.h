#include <fstream>
#include <vector>
#include <algorithm>

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
        bool on_the_top(Page top, Page bottom);

    private:
        int num_jobs, width, height; 
        ofstream& output; 
        char* board;
        vector<Page> vec;
        vector<Page> deleted_page;
        vector<Page> vec_top;
};

Board::Board(int num_jobs, int width, int height, ofstream& output_stream): output(output_stream) {
    this->width = width;
    this->height = height;
    this->num_jobs = num_jobs;

    board = new char[width*height];

    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            board[h*width + w] = ' ';
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


bool Board::on_the_top(Page top, Page bottom) {
    auto it = find(vec.begin(), vec.end(), bottom);
    int idx = distance(vec.begin(), it);
    for (int i=(idx+1); i<vec.size(); i++) {
        if ((vec[i].get_x() > (vec[idx].get_x()+vec[idx].get_width())) || (vec[idx].get_x() > (vec[i].get_x()+vec[i].get_width())) 
            || (vec[i].get_y() > (vec[idx].get_y()+vec[idx].get_height())) || (vec[idx].get_y() > (vec[i].get_y()+vec[i].get_height())))
            {

            }
        else 
        {
            vec_top.push_back(vec[i]);
        }
    }
    return true;
}

void Board::insert_page(int x, int y, int width, int height, int id, int content) {
    Page new_page = Page(x, y, width, height, id, content);
    vec.push_back(new_page);
    int idx = vec.size();
    bool on[idx];
    for (int i=0; i<idx; i++) {
        on[i] = false;
    }
    // 보드에 페이지 추가
    for (int h = y; h < y + height; h++) {
        for (int w = x; w < x + width; w++) {
            for (int i=0; i<idx; i++) {
                if (board[h * this->width + w] == vec[i].get_content()) {
                    
                    on[i] = true;
                }
            }
            board[h * this->width + w] = content;
        }
    }
    for (int i=0; i<idx; i++) {
        if (on[i] == true) {
            vec[i].add_vec_top(new_page);
        }
    }

    print_board();
}

void Board::delete_page(int id) {
    int tidx;
    for (int i=0; i<vec.size(); i++) {
        if (vec[i].get_id() == id) {
            tidx = i;
        }
    }
    Page target = vec[tidx];
    int id_min;
    int id_min_idx;
    Page first_to_delete = target;
    if (target.get_vec_top().size() != 0) {
        id_min = target.get_vec_top()[0].get_id();
        id_min_idx = 0;
        for (int i=0; i<target.get_vec_top().size(); i++) {
            if (target.get_vec_top()[i].get_id() < id_min) {
                id_min = target.get_vec_top()[i].get_id();
                id_min_idx = i;
            }
        }
        first_to_delete = target.get_vec_top()[id_min_idx];
        delete_page(id_min);
    }
    //삭제 부분
    int h, w;
    int vec_idx;
    for (int i=0; i<vec.size(); i++) {
        if (vec[i].get_id() == id) {
            vec_idx = i;
        }
    }
    for (int i=0; i<vec_idx; i++) {
        for (int j=0; j<vec[i].get_vec_top().size(); j++) {
            if (vec[i].get_vec_top()[j].get_id() == id) {
                //vec[i].get_vec_top().erase(vec[i].get_vec_top().begin()+j);
            }
        }
    }
    
    vec.erase(vec.begin() + vec_idx);
    for (int i=0; i<vec.size(); i++){
            for (int j=0; j<vec[i].get_vec_top().size(); j++) {
                if (vec[i].get_vec_top()[j].get_id() == id) {
                    vec[i].get_vec_top().erase(vec[i].get_vec_top().begin()+j);
                }
            }
        
    }
    print_board();
    
    deleted_page.erase(deleted_page.begin() + deleted_page.size() -1);
    for (int i=0; i <deleted_page.size(); i++) {
        int idx = deleted_page.size() -i -1;
        insert_page(deleted_page[idx].get_x(), deleted_page[idx].get_y(), deleted_page[idx].get_width(), deleted_page[idx].get_height(), deleted_page[idx].get_id(), deleted_page[idx].get_content());
    }
    deleted_page = {};

}

void Board::modify_content(int id, char content) {
    
}
void Board::modify_position(int id, int x, int y) {
    
}