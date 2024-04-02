#include <stdio.h>

// function for averaging arrays
double mean(double x[], int n){
    int sum = 0;
    for (int i = 0; i < n; i++){
        sum += x[i];
        }
    return (double)sum/n;
    }

// function for standard deviation in arrays
double sd_sample(double x[], int n){
   double average = mean(x, n);
   
    double sum = 0;
    for (int i = 0; i < n; i++){
        sum += (x[i]-average)*(x[i]-average);
    }

    double sd = sqrt(sum/(n-1));
    return sd;
}

// function for standard deviation of slope
double Isq(double x[], double y[], double *m, double *b, int n){
    double sum_x = 0, sum_y = 0, sum_xsq = 0, sum_xy = 0;

    for (int i=0; i < n; i++){
        sum_x += x[i];
        sum_y += y[i];
        sum_xsq += x[i] * x[i];
        sum_xy += x[i] * y[i];
    }

    *m = (n * sum_xy - sum_x * sum_y) / (n * sum_xsq - sum_x * sum_x);
    *b = (sum_y - (*m) * sum_x) / n;

    double sum_squared_diff_y = 0, sum_squared_diff_x = 0;
    for (int i = 0; i < n; i++) {
        double predicted_y = (*m) * x[i] + (*b);
        sum_squared_diff_y += (y[i] - predicted_y) * (y[i] - predicted_y);
        sum_squared_diff_x += (x[i] - mean(x, n)) * (x[i] - mean(x, n));
    }
    double sd_m = sqrt(sum_squared_diff_y / (n - 2)) / sqrt(sum_squared_diff_x);

    return sd_m;
}

int main(void){

    // replace x with the name of the array and y with the size of the array
    double avg = mean(x, y);
    return 0;
}