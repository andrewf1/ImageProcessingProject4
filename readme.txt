U09026779, Andrew Fernandez

Parameters
--------------------------
There is one file that is read by the program. It has very strict options for each 
line in the file. It includes the information for each ROI, parameters for the 
various functions for each ROI, and the images the appropriate functions should be run on. 
There are cycles for the lines. For every image/function that you want to have operated on,
the first line will contain the name of the source image, what you wish to name the target image,
the function that you want to operate over the image on, and the number of ROIs (n) used for the
function. The following n lines will be information given for the parameters for each ROI for each 
function. Each of these n lines will contain the following information in the following order: x, y, sx, sy. 
If the function is hist_stretch, then following the ROI parameters will be two values: a and b used for thresholding.
If the function is sobel_edge or comb_ops_sobel, then following the ROI parameters would be one value 
that represents a threshold value. If the function is canny_edge or comb_ops_canny, then following the
ROI parameters would be two values that represent two threshold values. Everything is separated by spaces. 
The first lines will be in this form: 
input_image output_image function. Where the function can be hist_stretch (for histogram stretching), 
hist_eq (for histogram equalization), canny_edge (for Canny edge detection), sobel edge (for Sobel 
edge detection), comb_ops_sobel (for combining histogram equalization and Sobel edge detection), 
comb_ops_canny (for combining, histogram equalization and Canny edge detection), and qr_decode (for 
QR detection and decoding). This qr_decode function is unique int that is the only function that takes 0
in for the number of ROIs. The output image's name should also be the same as the input image's name as 
the function does not generate an image, just prints the QR code's message. All these functions are 
implemented using OpenCV's C++ library. A side-note is that the parameters file should be in the following path: 
/Project2/project/bin/. The following is an example of 
a properly constructed parameters file called parameters.txt:
floor.pgm floor_hist_stretch.pgm hist_stretch 2
0 0 100 250 100 200
400 0 100 300 50 100
lena.pgm lena_hist_stretch.pgm hist_stretch 2
0 0 200 200 100 200
0 300 200 150 150 200
floor.pgm floor_hist_eq.pgm hist_eq 2
0 0 100 250 100 200
400 0 100 300 50 100
lena.pgm lena_hist_eq.pgm hist_eq 2
0 0 200 200 100 200
0 300 200 150 150 200
tree.pgm tree_canny.pgm canny_edge 1
0 0 500 300 200 300
picasso.pgm picasso_canny.pgm canny_edge 2
0 0 200 200 100 200
600 100 300 200 150 200
tree.pgm tree_sobel.pgm sobel_edge 1
0 0 500 300 15
picasso.pgm picasso_sobel.pgm sobel_edge 2
0 0 200 200 15
600 100 300 100 20
tree.pgm tree_comb_ops_sobel.pgm comb_ops_sobel 1
0 0 500 300 15
picasso.pgm picasso_comb_ops_sobel.pgm comb_ops_sobel 2
0 0 200 200 15
600 100 300 100 20
tree.pgm tree_comb_ops_canny.pgm comb_ops_canny 1
0 0 500 300 200 300
picasso.pgm picasso_comb_ops_canny.pgm comb_ops_canny 2
0 0 200 200 100 200
600 100 300 200 150 200
qr_alphabet.jpg qr_alphabet.jpg qr_decode 0
qr_cap4401.png qr_cap4401.png qr_decode 0
qr_stuff_paper.jpg qr_stuff_paper.jpg qr_decode 0
qr_stuff_monitor.jpg qr_stuff_monitor.jpg qr_decode 0
qr_wikipedia.jpg qr_wikipedia.jpg qr_decode 0

Compilation and Execution
--------------------------
In order to compile this program, make needs to be run twice. Once inside the Project4 directory, 
run make inside of /Project4/iptools to compile both image.cpp and utility.cpp. Then travel to 
/Project4/project and run make to compile iptool.cpp. This will create the executable in 
/Project4/project/bin of the name iptool. To run the program, travel to /Project4/project/bin/ 
and execute ./iptool <file_name> where <file_name> is the name of your parameters file located 
inside /Project4/project/bin/.