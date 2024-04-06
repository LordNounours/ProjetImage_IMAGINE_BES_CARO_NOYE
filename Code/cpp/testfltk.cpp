#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Pixmap.H>
#include <FL/Fl_PNM_Image.H>
#include <FL/Fl_Button.H>
#include <algorithm>
#include <iostream>
using namespace std;

//mkdir build
//   cd build
//    cmake ..
//    make
//    sudo make install (optional) <- faire cette ligne
//compilation les bougs : g++ testfltk.cpp -o test -I/../FL/build/FL -L/../FL/build/lib -lfltk -lX11 -ldl -lm -lpthread -lXext -lXinerama -lXcursor -lXfixes -lXrender -lfontconfig -lXft
//g++ testfltk.cpp -o test -I/../FL/build/FL -L/../FL/build/lib -lfltk -lfltk_images -lX11 -ldl -lm -lpthread -lXext -lXinerama -lXcursor -lXfixes -lXrender -lfontconfig -lXft <- faire cette compil

void slic_callback(Fl_Widget *widget, void *data) {
    std::cout << "Bouton SLIC cliqué !" << std::endl;
}

void felzen_callback(Fl_Widget *widget, void *data) {
    std::cout << "Bouton FELZEN cliqué !" << std::endl;
}

void quickshift_callback(Fl_Widget *widget, void *data) {
    std::cout << "Bouton QUICKSHIFT cliqué !" << std::endl;
}
int main(int argc, char **argv) {
    Fl_PNM_Image *image1 = new Fl_PNM_Image("../images/cote.ppm");
    Fl_PNM_Image *image2 = new Fl_PNM_Image("../resultats/slic5400_2000.ppm");

    if (image1 && image2) {
        int image1_width = image1->w();
        int image1_height = image1->h();
        int image2_width = image2->w();
        int image2_height = image2->h();

        Fl_Window *window = new Fl_Window(image1_width + image2_width + 60, std::max(image1_height, image2_height) + 120);

        Fl_Box *box1 = new Fl_Box(20, 40, image1_width, image1_height);
        Fl_Box *box2 = new Fl_Box(image1_width + 40, 40, image2_width, image2_height);
       
        box1->image(image1);
        box2->image(image2);
        Fl_Button *button1 = new Fl_Button(image1_width - image1_width/3-20, image1_width+40, 120, 40, "SLIC");
        Fl_Button *button2 = new Fl_Button(image1_width -20 , image1_width+40, 120, 40, "FELZEN");
        Fl_Button *button3 = new Fl_Button(image1_width + image1_width/3 -20, image1_width+40, 120, 40, "QUICKSHIFT");
        button1->callback(slic_callback);
        button2->callback(felzen_callback);
        button3->callback(quickshift_callback);
        window->end();
        window->show(argc, argv);
        return Fl::run();
    } else {
        Fl_Box *errorBox = new Fl_Box(20, 20, 640, 40, "Erreur: Impossible de charger les images.");
        errorBox->labelsize(16);
        errorBox->labelcolor(FL_RED);
    }
}
