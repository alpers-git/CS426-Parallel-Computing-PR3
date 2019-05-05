# CS426-Parallel-Computing-PR3
Implementation of a parallel face recognition algorithm using idea of Local Binary Patterns using omp. Application runs in 2 steps:

## Training Step:

• For each pixel in an image, it compares the pixel with its 8 neighbors (on its left-top, leftmiddle,
left-bottom, right-top, etc.).It follows the pixels along a circle, i.e. clockwise.
• Where the center pixel's value is less than the neighbor's value, it writes "1". Otherwise, write "0".
This gives an 8-digit binary number.
• It converts this value into a decimal value.
• Finally, it computes the histogram, over the pixels, of the frequency of each "number" occurring (i.e., each
combination of which pixels are smaller and which are greater than the center).

## Test Step:

• It creates a histogram for the test image as described previously.
• It finds the distance values between test image's histogram and training histograms.
• It selects the closest training histogram to determine the person.
• It uses this distance function: 
![distance funct](/res/formula.PNG)

## To compile:
cd into the directory of downloaded file unzip the file
```
chmod +x ./compile.sh
./compile.sh
```

## To run:
cd into the directory
  for sequential code:
```
 ./lbp_seq k
```

for OMP parallelized code:
```
 ./lbp_omp k
```
where k is the number of training pictures. Higher the k more accurate the test results. There are 20 different pictures of 18 people.Thus k cannot be greater than 19. Pictures are in grayscale format in the images folder as txt files.

**OR**
cd into the directory
```
chmod +x ./run.sh
./run.sh
```
