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

        // creating ROI objects
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
                else if (func_name == "sobel_edge") {
                    pch = strtok(NULL, " ");
                    int T = atoi(pch);
                    roi new_region = roi(x, y, sx, sy);
                    new_region.sobel_T = T;
                    regions.push_back(new_region);
                }
                else if (func_name == "comb_ops_sobel") {
                    pch = strtok(NULL, " ");
                    int T = atoi(pch);
                    roi new_region = roi(x, y, sx, sy);
                    new_region.sobel_T = T;
                    regions.push_back(new_region);                    
                }
                else if (func_name == "comb_ops_canny") {
                    pch = strtok(NULL, " ");
                    int T1 = atoi(pch);
                    pch = strtok(NULL, " ");
                    int T2 = atoi(pch);
                    roi new_region = roi(x, y, sx, sy);
                    new_region.canny_T1 = T1;
                    new_region.canny_T2 = T2;
                    regions.push_back(new_region);                     
                }
                else if (func_name == "qr_decode") {
                    continue;
                }
                else {
                    cout << "ERROR: Function DNE" << endl;
                    exit(1);
                }
            }
        }

        // actually calling all the functions
        if (func_name == "hist_stretch") {
            auto start = chrono::high_resolution_clock::now();
            utility::cv_hist_stretch(I, I2, regions);
            auto end = chrono::high_resolution_clock::now();
            cout << "Hist Stretch time for " << src_name << " = " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;
        }
        else if (func_name == "hist_eq") {
            auto start = chrono::high_resolution_clock::now();
            utility::cv_hist_eq(I, I2, regions);
            auto end = chrono::high_resolution_clock::now();
            cout << "Hist Eq time for " << src_name << " = " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;
        }
        else if (func_name == "canny_edge") {
            auto start = chrono::high_resolution_clock::now();
            utility::cv_canny_edge(I, I2, regions);
            auto end = chrono::high_resolution_clock::now();
            cout << "Canny Edge time for " << src_name << " = " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;
        }
        else if (func_name == "sobel_edge") {
            auto start = chrono::high_resolution_clock::now();
            utility::cv_sobel_edge(I, I2, regions);
            auto end = chrono::high_resolution_clock::now();
            cout << "Sobel Edge time for " << src_name << " = " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;
        }
        else if (func_name == "comb_ops_sobel") {
            // auto start = chrono::high_resolution_clock::now();
            utility::cv_comb_ops_sobel(I, I2, regions, outfile);
            // auto end = chrono::high_resolution_clock::now();
            // cout << "Combine Ops for HE and Sobel ED time for " << src_name << " = " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;
        }
        else if (func_name == "comb_ops_canny") {
            // auto start = chrono::high_resolution_clock::now();
            // utility::cv_comb_ops_canny(I, I2, regions, outfile);
            // auto end = chrono::high_resolution_clock::now();
            // cout << "Combine Ops for HE and Canny ED time for " << src_name << " = " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;
        }
        else if (func_name == "qr_decode") {
            cout << "calling cv_qr_decode" << endl;
            utility::cv_qr_decode(I, I2);
            continue;
            cout << "if u see me, what u think is happening is not happening" << endl;
        }
        else {
            cout << "ERROR: Function DNE" << endl;
            exit(1);
        }

        cv::imwrite(outfile, I2);
    }

    fclose(fp);
    return 0;
}
