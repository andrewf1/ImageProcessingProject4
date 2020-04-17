#include "../iptools/core.h"
#include <string.h>
#include "roi.hpp"
#include <strings.h>
#include <chrono>
#include <iostream>
#include <vector>
using namespace std;

#define MAXLEN 256

int main (int argc, char** argv)
{
    FILE *fp;
    char str[MAXLEN];
    char outfile[MAXLEN];
    char *pch;
    if ((fp = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "Can't open file: %s\n", argv[1]);
		exit(1);        
    }

    while (fgets(str, MAXLEN, fp) != NULL) {
        pch = strtok(str, " ");
        string src_name = pch;
        cv::Mat I = cv::imread(src_name);
        cv::Mat I2;

        if (I.empty()) {
            cout << "Could not open or find the image: " << src_name << endl;
            exit(1);
        }

        pch = strtok(NULL, " ");
        strcpy(outfile, pch);

        pch = strtok(NULL, " ");
        string func_name = pch;

        pch = strtok(NULL, " ");
        int num_regions = atoi(pch);

        if (num_regions > 3) {
            cout << "Only 3 ROI allowed" << endl;
            exit(1);
        }

        vector<roi> regions;
        for (int i = 0; i < num_regions; i++) {
            if (fgets(str, MAXLEN, fp) != NULL) {
                pch = strtok(str, " ");
                int x = atoi(pch);
                pch = strtok(NULL, " ");
                int y = atoi(pch);
                pch = strtok(NULL, " ");
                int sx = atoi(pch);
                pch = strtok(NULL, " ");
                int sy = atoi(pch);

                if (func_name == "hist_stretch") {
                    pch = strtok(NULL, " ");
                    int a = atoi(pch);
                    pch = strtok(NULL, " ");
                    int b = atoi(pch);
                    roi new_region = roi(x, y, sx, sy);
                    new_region.a = a;
                    new_region.b = b;
                    regions.push_back(new_region);
                }
                else if (func_name == "hist_eq") {
                    roi new_region = roi(x, y, sx, sy);
                    regions.push_back(new_region);
                }
                else if (func_name == "canny_edge") {
                    pch = strtok(NULL, " ");
                    int T1 = atoi(pch);
                    pch = strtok(NULL, " ");
                    int T2 = atoi(pch);
                    roi new_region = roi(x, y, sx, sy);
                    new_region.canny_T1 = T1;
                    new_region.canny_T2 = T2;
                    regions.push_back(new_region);            
                }
                else if (func_name == "comb_ops") {
                    // pch = strtok(NULL, " ");
                    // int color_thresh = atoi(pch);
                    // pch = strtok(NULL, " ");
                    // int color_direction = atoi(pch);
                    // roi new_region = roi(x, y, sx, sy);
                    // new_region.color_threshold = color_thresh;
                    // new_region.color_direction = color_direction;
                    // regions.push_back(new_region);                        
                }
                else {
                    cout << "ERROR: Function DNE" << endl;
                    exit(1);
                }
            }
        }

        // actually calling all the functions
        if (func_name == "hist_stretch") {
            // auto start = chrono::high_resolution_clock::now();
            utility::cv_hist_stretch(I, I2, regions);
            // auto end = chrono::high_resolution_clock::now();
            // cout << "Gray Edge time for " << src_name << " = " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;
        }
        else if (func_name == "hist_eq") {
            utility::cv_hist_eq(I, I2, regions);
            // auto start = chrono::high_resolution_clock::now();
            // utility::RGBEdgeDetection(src, tgt, regions, outfile);
            // auto end = chrono::high_resolution_clock::now();
            // cout << "RGB Edge time for " << src_name << " = " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;
        }
        else if (func_name == "canny_edge") {
            utility::cv_canny_edge(I, I2, regions);
        }
        else if (func_name == "comb_ops") {
            // auto start = chrono::high_resolution_clock::now();
            // utility::HSIEdgeDetection(src, tgt, regions, outfile);
            // auto end = chrono::high_resolution_clock::now();
            // cout << "HSI Edge time for " << src_name << " = " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;
        }
        else {
            cout << "ERROR: Function DNE" << endl;
            exit(1);
        }

        cout << "bout to write to " << outfile << endl;
        cv::imwrite(outfile, I2);
        cout << "just wrote there" << endl;
    }

    fclose(fp);
    return 0;
}
