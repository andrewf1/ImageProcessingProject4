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
    image src, tgt;
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
        src.read(pch); // creating the source image
        string src_name = pch;

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

                if (func_name == "hist_mod") {
                    pch = strtok(NULL, " ");
                    int a_i = atoi(pch);
                    pch = strtok(NULL, " ");
                    int b_i = atoi(pch);
                    roi new_region = roi(x, y, sx, sy);
                    new_region.a = a_i;
                    new_region.b = b_i;
                    regions.push_back(new_region);
                }
                else if (func_name == "edge_detect") {
                    pch = strtok(NULL, " ");
                    int color_thresh = atoi(pch);
                    pch = strtok(NULL, " ");
                    int color_direction = atoi(pch);
                    roi new_region = roi(x, y, sx, sy);
                    new_region.color_threshold = color_thresh;
                    new_region.color_direction = color_direction;
                    regions.push_back(new_region);                   
                }
                else if (func_name == "comb_ops") {
                    pch = strtok(NULL, " ");
                    int color_thresh = atoi(pch);
                    pch = strtok(NULL, " ");
                    int color_direction = atoi(pch);
                    roi new_region = roi(x, y, sx, sy);
                    new_region.color_threshold = color_thresh;
                    new_region.color_direction = color_direction;
                    regions.push_back(new_region);                        
                }
                else {
                    cout << "ERROR: Function DNE" << endl;
                    exit(1);
                }
            }
        }

        // actually calling all the functions
        if (func_name == "hist_mod") {
            // auto start = chrono::high_resolution_clock::now();
            utility::cv_hist_mod(src, tgt, regions, outfile);
            // auto end = chrono::high_resolution_clock::now();
            // cout << "Gray Edge time for " << src_name << " = " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;
        }
        else if (func_name == "edge_detect") {
            // auto start = chrono::high_resolution_clock::now();
            // utility::RGBEdgeDetection(src, tgt, regions, outfile);
            // auto end = chrono::high_resolution_clock::now();
            // cout << "RGB Edge time for " << src_name << " = " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;
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

        tgt.save(outfile);
    }

    fclose(fp);
    return 0;
}
