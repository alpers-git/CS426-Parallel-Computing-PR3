#include <stdio.h>
#include "util.h"
#include <math.h>
#include <string.h>
#include <limits.h>
#include <bits/types/clock_t.h>
#include <time.h>

void create_histogram(int * hist, int ** img, int num_rows, int num_cols)
{
    //hist = calloc(256, sizeof(int));
    int neighbour_offsets[16] = {-1,-1, 0,-1, 1,-1, 1,0, 1,1, 0,1, -1,1, -1,0};

    for (int i = 1; i < num_rows-1; ++i)
    {
        for (int j = 1; j < num_cols-1; ++j)
        {
            int histogram_ind=0;
            for (int k = 0; k < 8; ++k)
            {
                //printf("%d,%d\n", 1 + neighbour_offsets[2*k], 1+ neighbour_offsets[2*k + 1]);

                //convert from binary number to decimal
                histogram_ind += img[i][j] < img[i+ neighbour_offsets[2 * k]][j+neighbour_offsets[2 * k + 1]] ? (int)pow(2.0, (7 - k)) : 0;
            }
            //printf("%d\n", histogram_ind);
            hist[histogram_ind]++;
        }
    }
}

double distance(int * a, int *b, int size)
{
    double distance = 0;
    for (int i = 0; i < size; ++i)
    {
        distance += a[i] + b[i] != 0 ? (pow((a[i] - b[i]), 2) / (double)(a[i] + b[i]) ): 0.0;
    }

    return distance/2;
}

int find_closest(int*** training_set, int num_persons, int num_training, int size, int* test_image)// since parameter is named test_image and not test_test histogram int* is made to int**
{
    int closest_person_id = -1;
    //int closest_photo_id = -1;
    double min_distance = INFINITY;

    for (int i = 1; i <= num_persons; ++i)
    {
        for (int j = 1; j <= num_training; ++j)
        {
            double cur_dist = distance(training_set[i-1][j-1], test_image, size);
            if (cur_dist < min_distance)
            {
                closest_person_id = i;//added 1 since ids start from 1
                //closest_photo_id = j;
                min_distance = cur_dist;
            }
        }
    }

    //free(test_histogram);

    return closest_person_id;
}

int main(int argc, char *argv[])
{
    clock_t start, end;
    start = clock();
    int k=1;
    if (argc >=1)
        k = atoi(argv[1]);
    else
        return -1;

    int** image;
    int*** training_set = calloc(18, sizeof(int**));
    char sperson_id[12];
    char sphoto_id[12];

    //Training
    for (int person_id = 1; person_id <= 18; ++person_id)
    {
        training_set[person_id-1] = calloc(k, sizeof(int*));
        for (int photo_id = 1; photo_id <= k; ++photo_id)
        {
            char train_file[256] = "images/";

            sprintf(sperson_id, "%d", person_id);
            sprintf(sphoto_id, "%d", photo_id);

            strcat(train_file, sperson_id);
            strcat(train_file, ".");
            strcat(train_file, sphoto_id);
            strcat(train_file, ".txt");

            //printf("%s\n", file_name);

            int* histogram = calloc(256, sizeof(int));
            image = read_pgm_file(train_file, 180, 200);
            create_histogram(histogram, image, 180, 200);

            training_set[person_id-1][photo_id-1] = histogram;
            dealloc_2d_matrix(image, 180, 200);
        }
    }

    int wrong_predictions = 0;

    //Testing
    for (int j = 1; j <= 18; ++j)
    {
        for (int i = k+1; i <= 20; ++i)
        {
            char test_file[256] = "images/";
            char test_img[256] = "";

            sprintf(sperson_id, "%d", j);
            sprintf(sphoto_id, "%d", i);

            strcat(test_img, sperson_id);
            strcat(test_img, ".");
            strcat(test_img, sphoto_id);
            strcat(test_img, ".txt");
            strcat(test_file,test_img);

            int** img = read_pgm_file(test_file, 180, 200);
            int* hist = calloc(256, sizeof(int));
            create_histogram(hist,img, 180, 200);

            int prediction = find_closest(training_set, 18, k, 256, hist);

            printf("%s %d %d\n", test_img, prediction, j);

            wrong_predictions = j == prediction ? wrong_predictions : wrong_predictions + 1;

            free(hist);
            dealloc_2d_matrix(img, 180, 200);
        }
    }

    printf("Accuracy: %d correct answers for %d tests\n", (20-k) * 18 - wrong_predictions, (20-k)*18);
    //char filename[256] = "images/12.16.txt";
    end = clock();

    printf("Sequential time: %.2fms\n",((double) (end - start)) / CLOCKS_PER_SEC);

    //dealloc_2d_matrix(image, 180, 200);
    for (int i = 0; i < 18; ++i)
        dealloc_2d_matrix(training_set[i], k, 256);

    free(training_set);

    return 0;
}