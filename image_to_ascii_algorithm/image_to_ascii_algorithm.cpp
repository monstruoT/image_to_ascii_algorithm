#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <Windows.h>

using namespace std;
using namespace cv;

string ascii_char = " .,!1?08%$#";

string image_path;
string selection;
Mat image;
string name;
string file_route;
int background_color[3];
int font_color[3];

float compressing_factor = 0.5;


Mat gris_ascii() {
    Mat in_image = imread(image_path, IMREAD_GRAYSCALE);

    int original_width = in_image.rows;
    int original_height = in_image.cols;

    float factor_height = compressing_factor - (original_height / 3120.000);
    if (original_height> 1871) {
        factor_height = 0.05;
    }
    float factor_width = compressing_factor - (original_width / 3120.000);
    if (original_width> 1871) {
        factor_width = 0.05;
    }
    float factor = (factor_height + factor_width) / 2;

    int new_width = static_cast < int> (original_width * factor);
    int new_height = static_cast < int> (original_height * factor);

    Mat out_image;

    resize(in_image, out_image, Size(new_height, new_width));
    namedWindow("Ventana", WINDOW_NORMAL);

    int thickness = 2;
    int font_scale = 1;
    int cell_size = 29;

    int ascii_width = out_image.cols * cell_size;
    int ascii_height = out_image.rows * cell_size;
    Mat ascii_img(ascii_height, ascii_width, CV_8UC3, Scalar(background_color[0], background_color[1], background_color[2]));

    for (int i = 0; i < out_image.rows; i++) {
        for (int j = 0; j < out_image.cols; j++) {

            uchar intensity = out_image.at<uchar>(i, j);

            int index = (intensity * (ascii_char.size() - 1)) / 255.000;
            string c(1, ascii_char[index]);

            int x = j * cell_size;
            int y = i * cell_size + cell_size;

            putText(ascii_img, c, Point(x, y), FONT_HERSHEY_SIMPLEX,
                font_scale, Scalar(font_color[0], font_color[1], font_color[2]), thickness, LINE_AA);
        }
    }

    int compressed_width = ascii_img.cols / 2.5;
    int compressed_height = ascii_img.rows / 2.5;

    Mat compressed_ascii_img;
    resize(ascii_img, compressed_ascii_img, Size(compressed_width, compressed_height));

    return compressed_ascii_img;
}

Mat background_image() {
    Mat in_image = imread(image_path, IMREAD_COLOR);

    int original_width = in_image.rows;
    int original_height = in_image.cols;

    float factor_height = compressing_factor - (original_height / 3120.000);
    if (original_height> 1871) {
        factor_height = 0.05;
    }
    float factor_width = compressing_factor - (original_width / 3120.000);
    if (original_width> 1871) {
        factor_width = 0.05;
    }
    float factor = (factor_height + factor_width) / 2;

    int new_width = static_cast<int>(original_width * factor);
    int new_height = static_cast<int>(original_height * factor);

    Mat background_image;
    Mat bright_bg;
    float alpha = 0.5;

    resize(in_image, background_image, Size(new_height, new_width));
    background_image.convertTo(bright_bg, -1, alpha, 0);

    return bright_bg;
}

Mat color_ascii() {
    Mat in_image = imread(image_path, IMREAD_COLOR);

    int original_width = in_image.rows;
    int original_height = in_image.cols;

    float factor_height = compressing_factor - (original_height / 3120.000);
    if (original_height> 1871) {
        factor_height = 0.05;
    }
    float factor_width = compressing_factor - (original_width / 3120.000);
    if (original_width> 1871) {
        factor_width = 0.05;
    }
    float factor = (factor_height + factor_width) / 2;

    int new_width = static_cast<int>(original_width * factor);
    int new_height = static_cast<int>(original_height * factor);

    Mat out_image;

    resize(in_image, out_image, Size(new_height, new_width));
    namedWindow("Ventana", WINDOW_NORMAL);

    int font_scale = 1;
    int thickness = 3;
    int cell_size = 29;

    int ascii_width = out_image.cols * cell_size;
    int ascii_height = out_image.rows * cell_size;

    Mat background_color = background_image();
    resize(background_color, background_color, Size(ascii_width, ascii_height));

    Mat ascii_img = background_color.clone();

    for (int i = 0; i < out_image.rows; i++) {
        for (int j = 0; j < out_image.cols; j++) {
            Vec3b color = out_image.at<Vec3b>(i, j);
            uchar blue = color[0];
            uchar green = color[1];
            uchar red = color[2];

            uchar intensity = static_cast<uchar>(0.299 * red + 0.587 * green + 0.114 * blue);
            int index = intensity * (ascii_char.size() - 1) / 255;
            string c(1, ascii_char[index]);

            int x = j * cell_size;
            int y = i * cell_size + cell_size;

            float alpha = 1.1;
            int beta = 15;
            blue = saturate_cast<uchar>(blue * alpha + beta);
            green = saturate_cast<uchar>(green * alpha + beta);
            red = saturate_cast<uchar>(red * alpha + beta);


            putText(ascii_img, c, Point(x, y), FONT_HERSHEY_SIMPLEX,
                font_scale, Scalar(blue, green, red), thickness, LINE_AA);
        }
    }

    int compressed_width = ascii_img.cols / 3;
    int compressed_height = ascii_img.rows / 3;

    Mat compressed_color_ascii_img;
    resize(ascii_img, compressed_color_ascii_img, Size(compressed_width, compressed_height));

    return compressed_color_ascii_img;
}

int main() {
    int bb, gb, rb;
    int bf, gf, rf;

    utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);

    cout << "Paste here the image route (only png, jpg, jpeg, gif)" << "\n"
         << "[WARNING]: If the route has '' '' in betwen, delete those: ";
    cin >> image_path;
    cout << "\n";

    cout << "Write here the name you want the image to have: ";
    cin >> name;
    cout << "\n";

    cout << "Paste here the file route you want the generated image to be in" << "\n"
         << "(follow the same warning as before): ";
    cin >> file_route;
    cout << "\n";

    cout << "Do you want the image to be in color [C] or grey [G], [C,G]: ";
    cin >> selection;
    cout << "\n";

    if (tolower(selection[0]) == 'g') {
        cout << "Put here the BGR values for the background"
             << "(Blue, Green, Red (0 0 0 to black)): ";
        cin >> bb >> gb >> rb;
        background_color[0] = bb;
        background_color[1] = gb;
        background_color[2] = rb;
        
        cout << "\n";
        cout << "Now, put here the BGR values for the characters"
             << "(255 255 255 to white): ";
        cin >> bf >> gf >> rf;
        font_color[0] = bf;
        font_color[1] = gf;
        font_color[2] = rf;
        

        image = gris_ascii();
    }
    else if (tolower(selection[0]) == 'c') {
        image = color_ascii();
    }

    vector<int>params;
    params.push_back(IMWRITE_PNG_COMPRESSION);
    params.push_back(9);


    string example_window = "Low_resolution_example";
    namedWindow(example_window, WINDOW_KEEPRATIO);
    resizeWindow(example_window, image.cols / 2.5, image.rows / 2.5);
    imshow(example_window, image);

    imwrite(file_route + "/" + name + ".png", image, params);

    waitKey(0);
    return 0;
}
