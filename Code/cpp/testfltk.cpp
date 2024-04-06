#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
//mkdir build
//   cd build
//    cmake ..
//    make
//    sudo make install (optional) <- faire cette ligne
//compilation les bougs : g++ testfltk.cpp -o test -I/../fltk/build/FL -L/../fltk/build/lib -lfltk -lX11 -ldl -lm -lpthread -lXext -lXinerama -lXcursor -lXfixes -lXrender -lfontconfig -lXft

int main(int argc, char **argv) {
    Fl_Window *window = new Fl_Window(340, 180);
    Fl_Box *box = new Fl_Box(20, 40, 300, 100, "Hello, World!");
    box->box(FL_UP_BOX);
    box->labelfont(FL_BOLD + FL_ITALIC);
    box->labelsize(36);
    box->labeltype(FL_SHADOW_LABEL);
    window->end();
    window->show(argc, argv);
    return Fl::run();
}
